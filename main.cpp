#include <QtGui>
#include <QtCore>

#include "ManagerNote.h"
#include "qtsingleapplication/qtsingleapplication.h"
#include "UAnimatedSplashScreen.h"
#include "UFileAssociationManager/UFileAssociationManager.h"

int main(int argc, char *argv[])
{
    QApplication::setColorSpec(QApplication::ManyColor);

    QtSingleApplication *application = new QtSingleApplication(argc, argv);

    qApp->setApplicationName(nameOrganization);
    qApp->setApplicationVersion(versionProject);
    qApp->setOrganizationName(nameOrganization);

    // ������� ��� ������ �����
    for(int i = 0; i < listFolders().count(); i++)
    {
        QString name = listFolders().at(i);
        makeFolder(name);
    }

    QLibrary libraryIcons;
    libraryIcons.setFileName(qApp->applicationDirPath()
                             + QDir::separator()
                             + "icons.dll");
    libraryIcons.load();

    QDir *dirFonts = new QDir(absolutePathToTheFolderFonts());
    dirFonts->setFilter(QDir::Files);

#ifdef Q_OS_WIN
    extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
#endif

#ifdef Q_OS_WIN
    qt_ntfs_permission_lookup++;
#endif

    QFileInfoList listFonts = dirFonts->entryInfoList();

#ifdef Q_OS_WIN
    qt_ntfs_permission_lookup--;
#endif

    for(int i = 0; i < listFonts.count(); i++)
        QFontDatabase::addApplicationFont(listFonts.at(i).absoluteFilePath());

    delete dirFonts;

    UFileAssociationManager::makeApplicationDefaultForExtension(suffixFilesNote);

    QTextCodec::setCodecForTr(QTextCodec::codecForName(textCodec.toUtf8()));

    QString flatListArguments = "";
    QStringList listArguments;

    // ������� ������ ���������� (����������) ���
    // ������ �������� (������ 0) - ���� � ���,
    // ������� ������� � ������� ���������
    for(int i = 1; i < argc; i++)
    {
        QString argument = QObject::tr(argv[i]);
        QString suffix = QFileInfo(argument).suffix();

        // ���� �������� - ���� � ����� � ��� ������ - ������ �������
        if(QFileInfo(argument).isFile()
                && suffix == suffixFilesNote)
        {
            flatListArguments += argument;
            listArguments.append(argument);

            // ���� ������� �������� �� ���������
            if(i < argc - 1)
                flatListArguments += separatorListFilesNote;
        }
    }

    // ���� ����� ��� ��������
    if(application->isRunning())
    {
        application->sendMessage(messageFromQtSingleApplication_Show);

        if(!flatListArguments.isEmpty())
            application->sendMessage(flatListArguments);

        return 0;
    }

    // �������������� ��������� ��������������� �����
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    /// _
    QSettings *ini = new QSettings(absolutePathToTheConfigurationProgram(),
                                   QSettings::IniFormat);
    ini->setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    QString language = ini->value("Language").toString();

    // ���� ����� � ������� �����
    if(QDir(absolutePathToTheFolderLanguages())
            .entryInfoList(QDir::Files).isEmpty())
    {
        QFile::copy(strDefaultLanguageFile,
                    absolutePathToTheFolderLanguages()
                    + QDir::separator()
                    + fileNameRussianLanguage);

        language = fileNameRussianLanguage;
    }

    QString pathToLanguage = absolutePathToTheFolderLanguages()
            + QDir::separator()
            + language;

    // ���� ��� ����� �����, ������������ � ��������� ���
    // ��� ������� ����
    if(!QFileInfo(pathToLanguage).isFile())
    {
        // �� ����� ������� ����� ������ �� ������ ������
        QDir dir = QDir(absolutePathToTheFolderLanguages());
        dir.setFilter(QDir::Files);
        dir.setSorting(QDir::Name);

        QStringList filters;
        filters << QString("*." + suffixLanguageFiles);
        dir.setNameFilters(filters);

        QStringList list = dir.entryList();

        if(!list.isEmpty())
            language = list.first();

        pathToLanguage = absolutePathToTheFolderLanguages()
                + QDir::separator()
                + language;
    }

    delete ini;

    ini = new QSettings(pathToLanguage, QSettings::IniFormat);
    ini->setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    QStringList list = ini->value("File/LoadingInfo").toStringList();
    delete ini;
    /// _

    //---
    QFont font;
    font.setFamily("Arial");
    font.setPointSize(8);
    font.setWeight(QFont::Bold);

    QPixmap *pixmap = new QPixmap(IconSplashIcon);

    UAnimatedSplashScreen *animatedSplash = new UAnimatedSplashScreen();
    animatedSplash->setWindowFlags(animatedSplash->windowFlags()
                                   | Qt::WindowStaysOnTopHint);
    animatedSplash->setPixmap(*pixmap);
    animatedSplash->show();

    ManagerNote *managerNote = new ManagerNote();

    const int numberPhase = 8;
    for(int i = 0; i < numberPhase + 1; i++)
    {
        if(!list.isEmpty())
            animatedSplash->setMessage(list.takeFirst(), font);

        application->processEvents();
        managerNote->startThePreparatoryPhase(i + 1);
    }

    animatedSplash->finish(managerNote);

    // ������� � ������� ����� �������, ������� ���� ������� �
    // ���������� ���
    for(int i = 0; i < listArguments.count(); i++)
        managerNote->openFileNote(listArguments.at(i));

    delete pixmap;
    delete animatedSplash;

    application->setQuitOnLastWindowClosed(false);

    QObject::connect(application, SIGNAL(messageReceived(QString)),
                     managerNote, SLOT(messageToApp(QString)));

    return application->exec();
}
