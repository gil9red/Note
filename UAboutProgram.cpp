#include "UAboutProgram.h"

/// PUBLIC
UAboutProgram::UAboutProgram(QWidget *parent):
    QDialog(parent)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName(textCodec.toUtf8()));

    setWindowFlags(Qt::Widget
                   | Qt::WindowCloseButtonHint);

    QString absFilePath = QApplication::applicationDirPath()
            + QDir::separator()
            + fileNameChangeHistory;

    // получаем путь к файлу истории, хранящийся в ехе
    QString oldName = ":/resource/" + fileNameChangeHistory;

    // абсолютный путь до файла истории, которое хранится
    // в той же директории, где и ехе
    QString newName = absFilePath;

    // убираем аттрибут "только для чтения"
    // если он был
    QFile file(absFilePath);
    file.setPermissions(file.permissions()
                        | QFile::WriteOwner
                        | QFile::WriteUser
                        | QFile::WriteGroup
                        | QFile::WriteOther);

    QFile::remove(absFilePath);
    QFile::copy(oldName,
                newName);

///----

    absFilePath = QApplication::applicationDirPath()
            + QDir::separator()
            + fileNameLicense;

    // if file license is not exist
    if(!QFileInfo(absFilePath).isFile())
    {
        QString oldName = ":/resource/" + fileNameLicense;
        QString newName = absFilePath;
        QFile::copy(oldName,
                    newName);
    }

    createGUI();

    setFixedSize(minSizeAbout);
}

/// PRIVATE
void UAboutProgram::createGUI()
{
    QHBoxLayout *hBoxLayoutMain = new QHBoxLayout();

    pButtonOpenFileHistory = new QPushButton();
    connect(pButtonOpenFileHistory, SIGNAL(clicked()),
            this, SLOT(clickPButtonOpenFileHistory()));

    pButtonOpenFileLicense = new QPushButton();
    connect(pButtonOpenFileLicense, SIGNAL(clicked()),
            this, SLOT(clickPButtonOpenFileLicense()));


    pButtonShowHideDevelopers = new QPushButton();
    pButtonShowHideDevelopers->setCheckable(true);
    pButtonShowHideDevelopers->setChecked(false);
    connect(pButtonShowHideDevelopers, SIGNAL(clicked()),
            this, SLOT(clickPButtonShowHideDevelopers()));

    pButtonClose = new QPushButton();
    connect(pButtonClose, SIGNAL(clicked()),
            this, SLOT(close()));

    vBoxLayoutMain = new QVBoxLayout();        


    labelImageLogo = new QLabel();
    labelImageLogo->setScaledContents(true);
    labelImageLogo->setFixedSize(150, 150);

    QPixmap logo(IconLogo);
    labelImageLogo->setPixmap(logo);

    // LOGO
    addLogo(hBoxLayoutMain, labelImageLogo);
    // LOGO

    QVBoxLayout *vBoxLayout1 = new QVBoxLayout();
    hBoxLayoutMain->addLayout(vBoxLayout1);

    labelAllRightsReserved = new QLabel();
    labelWordFromTheOwner = new QLabel();

    labelSectionAuthors     = new QLabel();
    labelSectionProgrammers = new QLabel();
    labelSectionDesigners   = new QLabel();
    labelSectionTesters     = new QLabel();
    labelSectionTranslations = new QLabel();

    labelAuthor1     = new QLabel();
    labelProgrammer1 = new QLabel();
    labelDesigner1   = new QLabel();
    labelTester1     = new QLabel();

    labelContaktAuthor1   = new QLabel();
    labelContaktDesigner1 = new QLabel();
    labelContaktTester1   = new QLabel();

    labelContaktAuthor1->setOpenExternalLinks(true);
    labelContaktDesigner1->setOpenExternalLinks(true);
    labelContaktTester1->setOpenExternalLinks(true);

    // VERSION
    QLabel *labelVersion = new QLabel();
    labelVersion->setText(tr("<html> <font size=6 face=Times> <b>%1 %2 </font>"
                             " <font size=5 face=Times> [Build %3]</b> </font> </html>")
                          .arg(nameProject)
                          .arg(versionProject)
                          .arg(dataBuildProject));
    vBoxLayout1->addWidget(labelVersion);
    // VERSION

    // DESCRIPTION
    labelDescription = new QLabel();
    vBoxLayout1->addWidget(labelDescription);
    // DESCRIPTION

    // COPYRIGHT
    QLabel *yearOfCreation = new QLabel();
    yearOfCreation->setText(QString("<html> &copy; %1 %2.</html>")
                            .arg(rangeOfDates)
                            .arg(ownerProgram));
    vBoxLayout1->addWidget(yearOfCreation);
    // COPYRIGHT

    // AllRightsReserved
    vBoxLayout1->addWidget(labelAllRightsReserved);
    // AllRightsReserved

    // WordFromTheOwner
    vBoxLayout1->addWidget(labelWordFromTheOwner);
    addPass(hBoxLayoutMain);
    // WordFromTheOwner

// Button Developers
    QHBoxLayout *hBoxLayoutDevelopers = new QHBoxLayout();
    hBoxLayoutDevelopers->addWidget(pButtonShowHideDevelopers);
    hBoxLayoutDevelopers->addStretch();
    vBoxLayout1->addLayout(hBoxLayoutDevelopers);
// Button Developers

    addPass(hBoxLayoutMain);

// SCROLL AREA
    QVBoxLayout *vBoxLayoutScrollArea = new QVBoxLayout();

    // AUTHORS
    addSectionAuthors(vBoxLayoutScrollArea);
    addAuthors(labelSectionAuthors);
    addAuthors(labelAuthor1, labelContaktAuthor1);
    addPass(vBoxLayoutScrollArea);

    // PROGRAMMERS
    addSectionProgrammers(vBoxLayoutScrollArea);
    addProgrammers(labelSectionProgrammers);
    addProgrammers(labelProgrammer1);
    addPass(vBoxLayoutScrollArea);

    // DESIGNERS
    addSectionDesigners(vBoxLayoutScrollArea);
    addDesigners(labelSectionDesigners);
    addDesigners(labelDesigner1, labelContaktDesigner1);
    addPass(vBoxLayoutScrollArea);

    // TESTERS
    addSectionTesters(vBoxLayoutScrollArea);
    addTesters(labelSectionTesters);
    addTesters(labelTester1, labelContaktTester1);
    addPass(vBoxLayoutScrollArea);

    // TRANSLATIONS
    addSectionTranslations(vBoxLayoutScrollArea);
    addPass(vBoxLayoutScrollArea);

    mainWidgetToScrollArea = new QWidget(this);
    mainWidgetToScrollArea->setLayout(vBoxLayoutScrollArea);

    scrollAreaWithAutoscroll = new QScrollArea();
    scrollAreaWithAutoscroll->setVisible(pButtonShowHideDevelopers->isChecked());
    scrollAreaWithAutoscroll->setWidget(mainWidgetToScrollArea);

    scrollAreaWithAutoscroll->setWidgetResizable(true);

    vBoxLayout1->addWidget(scrollAreaWithAutoscroll);
// SCROLL AREA

    addPass(vBoxLayoutMain);

    vBoxLayoutMain->addLayout(hBoxLayoutMain);

    QHBoxLayout *hBoxLayoutButtons = new QHBoxLayout();
    hBoxLayoutButtons->addWidget(pButtonOpenFileHistory);
    hBoxLayoutButtons->addWidget(pButtonOpenFileLicense);
    hBoxLayoutButtons->addStretch();
    hBoxLayoutButtons->addWidget(pButtonClose);

    vBoxLayoutMain->addLayout(hBoxLayoutButtons);    

    setLayout(vBoxLayoutMain);
}

void UAboutProgram::addSectionAuthors(QBoxLayout *layout)
{
    vBoxMainLayoutAuthors = new QVBoxLayout();
    layout->addLayout(vBoxMainLayoutAuthors);
}
void UAboutProgram::addAuthors(QWidget *name)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(name);
    hBoxLayout->addStretch();
    vBoxMainLayoutAuthors->addLayout(hBoxLayout);
}
void UAboutProgram::addAuthors(QWidget *name, QWidget *contact)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(name);
    hBoxLayout->addStretch();

    hBoxLayout->addWidget(contact,
                          0,
                          Qt::AlignRight);
    vBoxMainLayoutAuthors->addLayout(hBoxLayout);
}
void UAboutProgram::addAuthors(QList <QWidget*> *list)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout();

    while(!list->isEmpty())
    {
        hBoxLayout->addWidget(list->takeFirst());
        hBoxLayout->addStretch();
    }

    vBoxMainLayoutAuthors->addLayout(hBoxLayout);
}

void UAboutProgram::addSectionProgrammers(QBoxLayout *layout)
{
    vBoxMainLayoutProgrammers = new QVBoxLayout();
    layout->addLayout(vBoxMainLayoutProgrammers);
}
void UAboutProgram::addProgrammers(QWidget *name)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(name);
    hBoxLayout->addStretch();
    vBoxMainLayoutProgrammers->addLayout(hBoxLayout);
}
void UAboutProgram::addProgrammers(QWidget *name, QWidget *contact)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(name);
    hBoxLayout->addStretch();

    hBoxLayout->addWidget(contact,
                          0,
                          Qt::AlignRight);
    vBoxMainLayoutProgrammers->addLayout(hBoxLayout);
}
void UAboutProgram::addProgrammers(QList <QWidget*> *list)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout();

    while(!list->isEmpty())
    {
        hBoxLayout->addWidget(list->takeFirst());
        hBoxLayout->addStretch();
    }

    vBoxMainLayoutProgrammers->addLayout(hBoxLayout);
}

void UAboutProgram::addSectionDesigners(QBoxLayout *layout)
{
    vBoxMainLayoutDesigners = new QVBoxLayout();
    layout->addLayout(vBoxMainLayoutDesigners);
}
void UAboutProgram::addDesigners(QWidget *name)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(name);
    hBoxLayout->addStretch();
    vBoxMainLayoutDesigners->addLayout(hBoxLayout);
}
void UAboutProgram::addDesigners(QWidget *name, QWidget *contact)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(name);
    hBoxLayout->addStretch();

    hBoxLayout->addWidget(contact,
                          0,
                          Qt::AlignRight);
    vBoxMainLayoutDesigners->addLayout(hBoxLayout);
}
void UAboutProgram::addDesigners(QList <QWidget*> *list)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout();

    while(!list->isEmpty())
    {
        hBoxLayout->addWidget(list->takeFirst());
        hBoxLayout->addStretch();
    }

    vBoxMainLayoutDesigners->addLayout(hBoxLayout);
}

void UAboutProgram::addSectionTesters(QBoxLayout *layout)
{
    vBoxMainLayoutTesters = new QVBoxLayout();
    layout->addLayout(vBoxMainLayoutTesters);
}
void UAboutProgram::addTesters(QWidget *name)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(name);
    hBoxLayout->addStretch();
    vBoxMainLayoutTesters->addLayout(hBoxLayout);
}
void UAboutProgram::addTesters(QWidget *name, QWidget *contact)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(name);
    hBoxLayout->addStretch();

    hBoxLayout->addWidget(contact,
                          0,
                          Qt::AlignRight);
    vBoxMainLayoutTesters->addLayout(hBoxLayout);
}
void UAboutProgram::addTesters(QList <QWidget*> *list)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout();

    while(!list->isEmpty())
    {
        hBoxLayout->addWidget(list->takeFirst());
        hBoxLayout->addStretch();
    }

    vBoxMainLayoutTesters->addLayout(hBoxLayout);
}

void UAboutProgram::addSectionTranslations(QBoxLayout *layout)
{
    vBoxMainLayoutTranslations = new QVBoxLayout();
    layout->addLayout(vBoxMainLayoutTranslations);

    vBoxMainLayoutTranslations->addWidget(labelSectionTranslations);
}

void UAboutProgram::addTranslations(QList<QString> *list)
{
    for(int i = 0; i < list->size(); i++)
    {
        QString strTranslaction = QString("<html> %1 </html>")
                .arg(list->at(i));

        QLabel *labelTranslaction = new QLabel();
        labelTranslaction->setOpenExternalLinks(true);
        labelTranslaction->setText(strTranslaction);

        vBoxMainLayoutTranslations->addWidget(labelTranslaction);
    }
}

void UAboutProgram::addPass(QBoxLayout *layout)
{
    layout->addSpacing(10);
}
void UAboutProgram::addLogo(QBoxLayout *layout, QWidget *logo)
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(logo,
                          0,
                          Qt::AlignTop);
    layout->addLayout(hBoxLayout);
}

/// PUBLIC SLOTS
void UAboutProgram::setLanguage(QString pathToTheFileLanguage)
{
    QSettings ini(pathToTheFileLanguage,
                       QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("AboutProgram");

    setWindowTitle(ini.value("Title").toString());

    pButtonClose->setText(ini.value("Close").toString());
    pButtonOpenFileHistory->setText(ini.value("RevisionHistory").toString());
    pButtonOpenFileLicense->setText(ini.value("License").toString());
    pButtonShowHideDevelopers->setText(ini.value("Developers").toString());

    labelSectionAuthors    ->setText(ini.value("SectionAuthors").toString());
    labelSectionProgrammers->setText(ini.value("SectionProgrammers").toString());
    labelSectionDesigners  ->setText(ini.value("SectionDesigners").toString());
    labelSectionTesters    ->setText(ini.value("SectionTesters").toString());

    labelSectionTranslations->setText(ini.value("SectionTranslations").toString());

    labelAuthor1    ->setText(ini.value("Author1").toString());
    labelProgrammer1->setText(ini.value("Programmer1").toString());
    labelDesigner1  ->setText(ini.value("Designer1").toString());
    labelTester1    ->setText(ini.value("Tester1").toString());

    labelContaktAuthor1  ->setText(ini.value("ContaktAuthor1").toString());
    labelContaktDesigner1->setText(ini.value("ContaktDesigner1").toString());
    labelContaktTester1  ->setText(ini.value("ContaktTester1").toString());

    labelAllRightsReserved->setText(ini.value("AllRightsReserved").toString());
    labelWordFromTheOwner->setText(ini.value("WordFromTheOwner").toString());

    ini.endGroup();

    labelDescription->setText(ini.value("File/Description").toString());
}

void UAboutProgram::clickPButtonOpenFileHistory()
{
    QString path = QApplication::applicationDirPath()
            + QDir::separator()
            + fileNameChangeHistory;

    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void UAboutProgram::clickPButtonOpenFileLicense()
{
    QString path = QApplication::applicationDirPath()
            + QDir::separator()
            + fileNameLicense;

    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void UAboutProgram::clickPButtonShowHideDevelopers()
{
    bool checked = pButtonShowHideDevelopers->isChecked();

    if(checked)
    {
        scrollAreaWithAutoscroll->setVisible(checked);
        setFixedSize(maxSizeAbout);
    }else
    {
        scrollAreaWithAutoscroll->setVisible(checked);
        setFixedSize(minSizeAbout);
    }
}

/// PROTECTED
void UAboutProgram::closeEvent(QCloseEvent *)
{
    hide();
    emit iClose();
}
