#include "UFileAssociationManager.h"

/// PUBLIC
bool UFileAssociationManager::isApplicationDefaultForExtension(const QString &extension)
{
    bool isDefault = false;
    QString extensionWithDot = extensionWithDotFromExtension(extension);

    if(QSysInfo::windowsVersion() < QSysInfo::WV_VISTA)
    {
        QSettings hklmSoftwareClasses("HKEY_LOCAL_MACHINE\\Software\\Classes", QSettings::NativeFormat);
        isDefault = hklmSoftwareClasses.value(kOpenWithFormat.arg(progIdForExtension(extensionWithDot))).toString() == executablePathWithParam();
        if(isDefault)
        {
            if(isRegisteredApplicationOverridenInFileExts(extensionWithDot, false))
                isDefault = false;
        }
    }
#ifdef WIN_VISTA_OR_LATER
    else
    {
        // don't try to register in case of error
        isDefault = true;

        IApplicationAssociationRegistration *pAAR;
        HRESULT hr = CoCreateInstance(CLSID_ApplicationAssociationRegistration, NULL, CLSCTX_INPROC, IID_PPV_ARGS(&pAAR));
        if(SUCCEEDED(hr))
        {
            BOOL isDefaultBOOL;
            hr = pAAR->QueryAppIsDefault(extensionWithDot.utf16(), AT_FILEEXTENSION, AL_EFFECTIVE, qApp->applicationName().utf16(), &isDefaultBOOL);
            if(HRESULT_CODE(hr) == ERROR_FILE_NOT_FOUND)
            {
                qDebug("app is not in the registry");
                hr = S_OK;
            }else if(FAILED(hr))
                ERROR_BOX_NO_PARENT(QString("Error calling QueryAppIsDefault(): %1").arg(HRESULT_CODE(hr)));

            if(SUCCEEDED(hr))
                isDefault = static_cast<bool>(isDefaultBOOL);

            pAAR->Release();
        }else
            ERROR_BOX_NO_PARENT(QString("Error calling CoCreateInstance(CLSID_ApplicationAssociationRegistration): %1").arg(HRESULT_CODE(hr)));
    }
#endif
    return isDefault;
}

void UFileAssociationManager::makeApplicationDefaultForExtension(const QString &extension)
{
    QString extensionWithDot = extensionWithDotFromExtension(extension);
    registerProgID(extensionWithDot);
    registerApplication(extensionWithDot);

    QSettings hkcr("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    QString executableName = QFileInfo(executablePath()).fileName();
    QString nameExe = QFileInfo(executableName).baseName();

    hkcr.setValue(QString("/.%1/.").arg(extension),
                  QString("%1.%2")
                  .arg(nameExe)
                  .arg(extension));
    hkcr.setValue(QString("/%1.%2/DefaultIcon/.")
                  .arg(nameExe)
                  .arg(extension),
                  '"' + executablePath() + '"' + "," + "1");
    hkcr.setValue(QString("/%1%2/shell/open/command/.").arg(nameExe).arg(extension),
                  '"' + executablePath() + "\" \"%1\"");

    if(QSysInfo::windowsVersion() < QSysInfo::WV_VISTA)
        isRegisteredApplicationOverridenInFileExts(extensionWithDot, true);
#ifdef WIN_VISTA_OR_LATER
    else
    {
        IApplicationAssociationRegistration *pAAR;
        HRESULT hr = CoCreateInstance(CLSID_ApplicationAssociationRegistration, NULL, CLSCTX_INPROC, IID_PPV_ARGS(&pAAR));
        if(SUCCEEDED(hr))
        {
            hr = pAAR->SetAppAsDefault(qApp->applicationName().utf16(), extensionWithDot.utf16(), AT_FILEEXTENSION);
            if(SUCCEEDED(hr))
                qDebug("app is default now");
            else
                ERROR_BOX_NO_PARENT(QString("Error calling SetAppAsDefault(): %1").arg(HRESULT_CODE(hr)));

            pAAR->Release();
        }else
            ERROR_BOX_NO_PARENT(QString("Error calling CoCreateInstance(CLSID_ApplicationAssociationRegistration): %1").arg(HRESULT_CODE(hr)));
    }
#endif

    ::SHChangeNotify(SHCNE_ASSOCCHANGED, 0, NULL, NULL);
}

QString UFileAssociationManager::progIdForExtension(const QString &extension)
{
    return qApp->applicationName().remove(' ')
            + extensionWithDotFromExtension(extension);
}

/// PRIVATE
// helpers
QString UFileAssociationManager::extensionWithDotFromExtension(const QString &extension)
{
    return extension.startsWith('.') ? extension : '.' + extension;
}
QString UFileAssociationManager::executablePath()
{
    return QDir::toNativeSeparators(qApp->applicationFilePath());
}
QString UFileAssociationManager::executablePathWithParam()
{
    // "path\to\exe" "%1"
    return QString("\"%1\" \"%2\"").arg(executablePath(), "%1");
}

// editing registry
void UFileAssociationManager::registerProgID(const QString &extensionWithDot)
{
    QString progId = UFileAssociationManager::progIdForExtension(extensionWithDot);
    QSettings hklmSoftwareClasses("HKEY_LOCAL_MACHINE\\Software\\Classes", QSettings::NativeFormat);
    hklmSoftwareClasses.setValue(QString("%1/.").arg(extensionWithDot), progId);
    hklmSoftwareClasses.setValue(QString("%1/.").arg(progId), descriptionFilesNote);
    hklmSoftwareClasses.setValue(kOpenWithFormat.arg(progId), executablePathWithParam());
}
void UFileAssociationManager::registerApplication(const QString &extensionWithDot)
{
    QString executableName = QFileInfo(executablePath()).fileName();

    QSettings hklmSoftware("HKEY_LOCAL_MACHINE\\Software", QSettings::NativeFormat);
    hklmSoftware.beginGroup("Microsoft/Windows/CurrentVersion/App Paths");
    hklmSoftware.setValue(QString("%1/.").arg(executableName), executablePath());
    hklmSoftware.setValue(QString("%1/Path").arg(executableName),
                          QDir::toNativeSeparators(qApp->applicationDirPath()));
    hklmSoftware.endGroup();

    QString registryApplications = QString("Applications/%1").arg(executableName);
    hklmSoftware.beginGroup("Classes");
    hklmSoftware.setValue(kOpenWithFormat.arg(registryApplications), executablePathWithParam());
    // empty string is intended
    hklmSoftware.setValue(QString("%1/SupportedTypes/%2").arg(registryApplications, extensionWithDot),
                          QString(""));
    hklmSoftware.endGroup();
}
bool UFileAssociationManager::isRegisteredApplicationOverridenInFileExts(const QString &extensionWithDot, bool removeIfExists)
{
    QSettings hkcuFileExts("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\" + extensionWithDot,
                           QSettings::NativeFormat);
    bool result = hkcuFileExts.contains("Application")
            && hkcuFileExts.value("Application").toString() != QFileInfo(executablePath()).fileName();
    if(result && removeIfExists)
        hkcuFileExts.remove("Application");
    return result;
}
