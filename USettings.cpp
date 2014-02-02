#include "USettings.h"

/// PUBLIC
USettings::USettings(QWidget *parent):
    QDialog(parent)
{
    setWindowFlags(Qt::Dialog
                   | Qt::WindowCloseButtonHint);

    setWindowIcon(QIcon(IconSettingsProgram));

    QTextCodec::setCodecForTr(QTextCodec::codecForName(textCodec.toUtf8()));

    createGUI();
}

void USettings::setAutorun(bool bValue)
{
    if(bValue)
    {
#ifdef Q_WS_WIN
        QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                      QSettings::NativeFormat);
        reg.setValue(nameExe, QString("\"%1\"").arg(QDir::toNativeSeparators(qApp->applicationFilePath())));
#endif

    }else
    {
#ifdef Q_WS_WIN
        QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                      QSettings::NativeFormat);
        reg.remove(nameExe);
#endif
    }
}

bool USettings::isAutorun()
{
    bool autoRunStat = false;

#ifdef Q_WS_WIN
    QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    autoRunStat = (reg.value(nameExe) == QString("\"%1\"")
                   .arg(QDir::toNativeSeparators(qApp->applicationFilePath())));
#endif

    return autoRunStat;
}

/// PRIVATE
void USettings::createGUI()
{
    checkBoxRunWithWindows = new QCheckBox();
    connect(checkBoxRunWithWindows, SIGNAL(clicked()),
            this, SLOT(clickCheckBoxRunWithWindows()));

    checkBoxShowTheManagerWhenTheProgramStarts = new QCheckBox();
    connect(checkBoxShowTheManagerWhenTheProgramStarts, SIGNAL(clicked(bool)),
            this, SLOT(setCheckedShowManagerAtStartProgram(bool)));

    vBoxLayoutGroupBoxProgram = new QVBoxLayout();
    vBoxLayoutGroupBoxProgram->addWidget(checkBoxRunWithWindows);
    vBoxLayoutGroupBoxProgram->addWidget(checkBoxShowTheManagerWhenTheProgramStarts);

    groupBoxProgram = new QGroupBox();
    groupBoxProgram->setLayout(vBoxLayoutGroupBoxProgram);

    // GROUP BOX WhenClosingManager
    rButtonMinimizeToTrayWhenClosingManager = new QRadioButton();
    rButtonMinimizeToTrayWhenClosingManager->setCheckable(true);

    connect(rButtonMinimizeToTrayWhenClosingManager, SIGNAL(clicked()),
            this, SLOT(minimizeToTrayWhenClosingManager()));

    rButtonCloseTheProgramWhenClosingManager = new QRadioButton();
    rButtonCloseTheProgramWhenClosingManager->setCheckable(true);

    connect(rButtonCloseTheProgramWhenClosingManager, SIGNAL(clicked()),
            this, SLOT(minimizeToTrayWhenClosingManager()));

    vBoxLayoutGroupBoxWhenClosingManager = new QVBoxLayout();
    vBoxLayoutGroupBoxWhenClosingManager->addWidget(rButtonMinimizeToTrayWhenClosingManager);
    vBoxLayoutGroupBoxWhenClosingManager->addWidget(rButtonCloseTheProgramWhenClosingManager);

    groupBoxWhenClosingManager = new QGroupBox();
    groupBoxWhenClosingManager->setLayout(vBoxLayoutGroupBoxWhenClosingManager);

    // GROUP BOX WhenMinimizeManager
    rButtonMinimizeToTrayWhenMinimizeManager = new QRadioButton();
    rButtonMinimizeToTaskbarMinimizeManager = new QRadioButton();

    connect(rButtonMinimizeToTrayWhenMinimizeManager, SIGNAL(clicked()),
            this, SLOT(minimizeToTrayWhenMinimizeManager()));

    connect(rButtonMinimizeToTaskbarMinimizeManager, SIGNAL(clicked()),
            this, SLOT(minimizeToTrayWhenMinimizeManager()));

    vBoxLayoutGroupBoxWhenMinimizeManager = new QVBoxLayout();
    vBoxLayoutGroupBoxWhenMinimizeManager->addWidget(rButtonMinimizeToTrayWhenMinimizeManager);
    vBoxLayoutGroupBoxWhenMinimizeManager->addWidget(rButtonMinimizeToTaskbarMinimizeManager);

    groupBoxWhenMinimizeManager = new QGroupBox();
    groupBoxWhenMinimizeManager->setLayout(vBoxLayoutGroupBoxWhenMinimizeManager);


    comboBoxSelectLanguage = new QComboBox();
    comboBoxSelectLanguage->setEditable(false);

    loadLanguageToComboBox();

    connect(comboBoxSelectLanguage, SIGNAL(activated(int)),
            this, SLOT(comboBoxSelectLanguageItemIsActivated(int)));

    vBoxLayoutGroupBoxSelectLanguage = new QVBoxLayout();
    vBoxLayoutGroupBoxSelectLanguage->addWidget(comboBoxSelectLanguage);

    groupBoxSelectLanguage = new QGroupBox();
    groupBoxSelectLanguage->setLayout(vBoxLayoutGroupBoxSelectLanguage);

    QVBoxLayout *layoutMain = new QVBoxLayout();
    layoutMain->addWidget(groupBoxProgram);
    layoutMain->addWidget(groupBoxWhenClosingManager);
    layoutMain->addWidget(groupBoxWhenMinimizeManager);
    layoutMain->addWidget(groupBoxSelectLanguage);
    layoutMain->addStretch();

    setLayout(layoutMain);

    checkBoxRunWithWindows->setChecked(isAutorun());

    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    bool showManagerAtStartProgram = ini.value("ShowManagerAtStartup",
                                               true).toBool();

    bool minimizeToTrayManagerAtClosing = ini.value("Manager/MinimizeToTrayManagerAtClosing",
                                                    isMinimizeToTrayManagerAtClosing).toBool();

    bool minimizeToTrayManagerAtMinimize = ini.value("Manager/MinimizeToTrayManagerAtMinimize",
                                                     false).toBool();

    setCheckedShowManagerAtStartProgram(showManagerAtStartProgram);
    rButtonMinimizeToTrayWhenClosingManager->setChecked(minimizeToTrayManagerAtClosing);
    rButtonCloseTheProgramWhenClosingManager->setChecked(!minimizeToTrayManagerAtClosing);

    rButtonMinimizeToTrayWhenMinimizeManager->setChecked(minimizeToTrayManagerAtMinimize);
    rButtonMinimizeToTaskbarMinimizeManager->setChecked(!minimizeToTrayManagerAtMinimize);
}

void USettings::loadLanguageToComboBox()
{
    QDir dir = QDir(absolutePathToTheFolderLanguages());

    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);

    QStringList filters;
    filters << QString("*." + suffixLanguageFiles);
    dir.setNameFilters(filters);

    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); i++)
    {
        QFileInfo fileInfo = list.at(i);

        QSettings ini(fileInfo.absoluteFilePath(),
                      QSettings::IniFormat);
        ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

        ini.beginGroup("FILE");

        QString nameLanguage = ini.value("Name").toString();
        QString nameLangFile = fileInfo.absoluteFilePath();

        QPixmap pixmap;

        QByteArray arrayHexIcon = ini.value("Icon").toByteArray();
        pixmap.loadFromData(arrayHexIcon.fromHex(arrayHexIcon));

        comboBoxSelectLanguage->addItem(QIcon(pixmap),
                                        nameLanguage,
                                        nameLangFile);

        ini.endGroup();
    }
}

/// PUBLIC SLOTS
void USettings::setLanguage(QString pathToTheFileLanguage)
{
    emit languageIsChange(pathToTheFileLanguage);

    QSettings ini(pathToTheFileLanguage,
                       QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("SettingsProgram");

    setWindowTitle(ini.value("Title").toString());

    groupBoxProgram->setTitle(ini.value("GroupBoxProgramTitle").toString());
    checkBoxRunWithWindows->setText(ini.value("RunWithWindows").toString());
    checkBoxShowTheManagerWhenTheProgramStarts->setText(ini.value("ShowTheManagerWhenTheProgramStarts").toString());
    groupBoxSelectLanguage->setTitle(ini.value("GroupBoxSelectLanguage").toString());

    groupBoxWhenClosingManager->setTitle(ini.value("WhenClosingManager").toString());
    rButtonMinimizeToTrayWhenClosingManager->setText(ini.value("MinimizeToTray").toString());
    rButtonCloseTheProgramWhenClosingManager->setText(ini.value("CloseTheProgram").toString());

    groupBoxWhenMinimizeManager->setTitle(ini.value("WhenMinimizeManager").toString());

    rButtonMinimizeToTrayWhenMinimizeManager->setText(ini.value("MinimizeToTray").toString());
    rButtonMinimizeToTaskbarMinimizeManager->setText(ini.value("MinimizeToTaskbar").toString());


    // TOOL TIP
    checkBoxRunWithWindows->setToolTip(ini.value("ToolTipRunWithWindows").toString());
    checkBoxShowTheManagerWhenTheProgramStarts->setToolTip(ini.value("ToolTipShowTheManagerWhenTheProgramStarts").toString());
    comboBoxSelectLanguage->setToolTip(ini.value("ToolTipSelectLanguage").toString());

    rButtonMinimizeToTrayWhenClosingManager->setToolTip(ini.value("ToolTipMinimizeToTray").toString());
    rButtonCloseTheProgramWhenClosingManager->setToolTip(ini.value("ToolTipCloseTheProgram").toString());

    rButtonMinimizeToTrayWhenMinimizeManager->setToolTip(ini.value("ToolTipMinimizeToTray").toString());
    rButtonMinimizeToTaskbarMinimizeManager->setToolTip(ini.value("ToolTipMinimizeToTaskbar").toString());

    ini.endGroup();

    // выбранный язык будет показан в line edit combo box
    comboBoxSelectLanguage->setCurrentIndex(comboBoxSelectLanguage->findText(ini.value("FILE/Name").toString()));


    QSettings ini2(absolutePathToTheConfigurationProgram(),
                   QSettings::IniFormat);
    ini2.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini2.setValue("Language", QFileInfo(pathToTheFileLanguage).fileName());
    ini2.sync();
}

void USettings::minimizeToTrayWhenClosingManager()
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    bool bValue = rButtonMinimizeToTrayWhenClosingManager->isChecked();
    ini.setValue("Manager/MinimizeToTrayManagerAtClosing", bValue);

    ini.sync();
}
void USettings::minimizeToTrayWhenMinimizeManager()
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    bool bValue = rButtonMinimizeToTrayWhenMinimizeManager->isChecked();
    ini.setValue("Manager/MinimizeToTrayManagerAtMinimize", bValue);

    ini.sync();
}

/// PRIVATE SLOTS
void USettings::comboBoxSelectLanguageItemIsActivated(int indexItem)
{
    QString pathToTheFileLanguage = comboBoxSelectLanguage->itemData(indexItem).toString();
    setLanguage(pathToTheFileLanguage);

    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));    
    ini.setValue("Language", QFileInfo(pathToTheFileLanguage).fileName());
    ini.sync();
}

void USettings::setCheckedShowManagerAtStartProgram(bool bValue)
{
    checkBoxShowTheManagerWhenTheProgramStarts->setChecked(bValue);

    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("ShowManagerAtStartup", bValue);
    ini.sync();
}
