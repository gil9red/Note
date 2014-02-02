#include "DialogSettingsNote.h"

/// PUBLIC
DialogSettingsNote::DialogSettingsNote(QWidget *parent):
    QWidget(parent)
{
    setWindowFlags(Qt::Tool
                   | Qt::WindowCloseButtonHint);

    setWindowIcon(QIcon(IconSettingsNote));

    pButtonGeneralSettings  = new QPushButton();
    pButtonGeneralSettings->setCheckable(true);
    pButtonGeneralSettings->setChecked(true);

    pButtonSettingsPassword = new QPushButton();
    pButtonSettingsPassword->setCheckable(true);

    pButtonGeneralSettings->setIcon(QIcon(IconSettingsNote));
    pButtonSettingsPassword->setIcon(QIcon(IconPassword));

    connect(pButtonGeneralSettings, SIGNAL(clicked()),
            this, SLOT(clickPButtonGeneralSettings()));
    connect(pButtonSettingsPassword, SIGNAL(clicked()),
            this, SLOT(clickPButtonSettingsPassword()));

    QVBoxLayout *vBoxLayoutSelectPage = new QVBoxLayout();
    vBoxLayoutSelectPage->addWidget(pButtonGeneralSettings);
    vBoxLayoutSelectPage->addWidget(pButtonSettingsPassword);
    vBoxLayoutSelectPage->addStretch();

    widgetSelectPage = new QWidget();
    widgetSelectPage->setLayout(vBoxLayoutSelectPage);

    QVBoxLayout *vBoxLayoutGeneral = new QVBoxLayout();

    addSelectTitle(vBoxLayoutGeneral);
    addSelectColor(vBoxLayoutGeneral);
    addSelectZPosition(vBoxLayoutGeneral);
    addSelectOpacity(vBoxLayoutGeneral);

    widgetGeneral = new QWidget();
    widgetGeneral->setLayout(vBoxLayoutGeneral);

    settingsPassword = new USettingsPassword();
    settingsPassword->hide();
    connect(settingsPassword, SIGNAL(newPasswordIsSelected()),
            this, SLOT(newPasswordIsSelected()));
    connect(settingsPassword->checkBoxPasswordProtect, SIGNAL(clicked()),
            this, SLOT(changeProtectionEnable()));

    QVBoxLayout *vBoxLayoutPages = new QVBoxLayout();
    vBoxLayoutPages->addWidget(widgetGeneral);
    vBoxLayoutPages->addWidget(settingsPassword);

    widgetPages = new QWidget();
    widgetPages->setLayout(vBoxLayoutPages);

    mainSplitter = new QSplitter();
    mainSplitter->addWidget(widgetSelectPage);
    mainSplitter->addWidget(widgetPages);

    QVBoxLayout *vBoxLayoutMain = new QVBoxLayout();
    vBoxLayoutMain->addWidget(mainSplitter);

    addCommandButtons(vBoxLayoutMain);

    setLayout(vBoxLayoutMain);
}

/// PRIVATE
void DialogSettingsNote::addSelectTitle(QBoxLayout *layout)
{    
    labelSelectTitle = new QLabel(this);

    QPixmap pixmap_enter(IconClearTextEnter);
    QPixmap pixmap_leave(IconClearTextLeave);

    int widthIcon = 20;
    int heightIcon = 20;
    pixmap_enter = pixmap_enter.scaled(widthIcon, heightIcon, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pixmap_leave = pixmap_leave.scaled(widthIcon, heightIcon, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    lineEditSelectTitle = new ULineEditWithClearButton(this);
    lineEditSelectTitle->setIconButton(pixmap_leave);
    lineEditSelectTitle->setIconButton(ULineEditWithClearButton::EnterEvent,
                            pixmap_enter);
    lineEditSelectTitle->setIconButton(ULineEditWithClearButton::LeaveEvent,
                            pixmap_leave);


    QVBoxLayout *vBoxLayoutGroupBoxSelectTitle = new QVBoxLayout();
    vBoxLayoutGroupBoxSelectTitle->addWidget(labelSelectTitle);
    vBoxLayoutGroupBoxSelectTitle->addWidget(lineEditSelectTitle);

    groupBoxSelectTitle = new QGroupBox(this);
    groupBoxSelectTitle->setLayout(vBoxLayoutGroupBoxSelectTitle);

    layout->addWidget(groupBoxSelectTitle);

    connect(lineEditSelectTitle, SIGNAL(textEdited(QString)),
            this, SLOT(lineEditTitleChangeText(QString)));
}
void DialogSettingsNote::addSelectOpacity(QBoxLayout *layout)
{
    sliderSelectOpacity = new QSlider(this);
    sliderSelectOpacity->setOrientation(Qt::Horizontal);
    sliderSelectOpacity->setRange(minimalOpacity, maximalOpacity);
    sliderSelectOpacity->setSliderPosition(maximalOpacity);

    labelSelectOpacity = new QLabel(this);

    QHBoxLayout *hBoxLayoutSelectOpacity = new QHBoxLayout();
    hBoxLayoutSelectOpacity->addWidget(new QLabel(QString::number(minimalOpacity) + "%"));
    hBoxLayoutSelectOpacity->addWidget(sliderSelectOpacity);
    hBoxLayoutSelectOpacity->addWidget(new QLabel(QString::number(maximalOpacity) + "%"));

    QVBoxLayout *vBoxLayoutGroupBoxSelectOpacity = new QVBoxLayout();
    vBoxLayoutGroupBoxSelectOpacity->addWidget(labelSelectOpacity);    
    vBoxLayoutGroupBoxSelectOpacity->addLayout(hBoxLayoutSelectOpacity);

    groupBoxSelectOpacity = new QGroupBox(this);
    groupBoxSelectOpacity->setLayout(vBoxLayoutGroupBoxSelectOpacity);

    layout->addWidget(groupBoxSelectOpacity);

    connect(sliderSelectOpacity, SIGNAL(valueChanged(int)),
            this, SLOT(sliderChangePosition(int)));
}
void DialogSettingsNote::addSelectColor(QBoxLayout *layout)
{   
    labelSelectColor = new QLabel(this);
    pButtonSelectColor = new UPushButtonSelectColor(this);

    QVBoxLayout *vBoxLayoutGroupBoxSelectColor = new QVBoxLayout();
    vBoxLayoutGroupBoxSelectColor->addWidget(labelSelectColor);
    vBoxLayoutGroupBoxSelectColor->addWidget(pButtonSelectColor);

    groupBoxSelectColor = new QGroupBox(this);
    groupBoxSelectColor->setLayout(vBoxLayoutGroupBoxSelectColor);

    layout->addWidget(groupBoxSelectColor);

    connect(pButtonSelectColor, SIGNAL(selectedColor(QColor)),
            this, SLOT(clickButtonSelectColor(QColor)));
}
void DialogSettingsNote::addSelectZPosition(QBoxLayout *layout)
{
    radioButtonWindowStaysOnBottomHint = new QRadioButton();
    radioButtonWindowStaysOnTopHint = new QRadioButton();

    QHBoxLayout *hBoxLayoutGroupBoxSelectZPosition = new QHBoxLayout();
    hBoxLayoutGroupBoxSelectZPosition->addWidget(radioButtonWindowStaysOnBottomHint);
    hBoxLayoutGroupBoxSelectZPosition->addWidget(radioButtonWindowStaysOnTopHint);

    labelSelectZPosition = new QLabel(this);

    QVBoxLayout *vBoxLayoutMainGroupBox = new QVBoxLayout();
    vBoxLayoutMainGroupBox->addWidget(labelSelectZPosition);
    vBoxLayoutMainGroupBox->addLayout(hBoxLayoutGroupBoxSelectZPosition);

    groupBoxSelectZPosition = new QGroupBox(this);
    groupBoxSelectZPosition->setLayout(vBoxLayoutMainGroupBox);

    layout->addWidget(groupBoxSelectZPosition);

    connect(radioButtonWindowStaysOnBottomHint, SIGNAL(clicked()),
            this, SLOT(clickRadioButtonChangeZPosition()));
    connect(radioButtonWindowStaysOnTopHint, SIGNAL(clicked()),
            this, SLOT(clickRadioButtonChangeZPosition()));
}
void DialogSettingsNote::addCommandButtons(QBoxLayout *layout)
{    
    pButtonOk = new QPushButton(this);
    pButtonOk->setDefault(true);

    QHBoxLayout *hBoxLayoutCommandButton = new QHBoxLayout();
    hBoxLayoutCommandButton->addStretch();
    hBoxLayoutCommandButton->addWidget(pButtonOk);

    layout->addLayout(hBoxLayoutCommandButton);

    connect(pButtonOk, SIGNAL(clicked()),
            this, SLOT(clickButtonOk()));
}

void DialogSettingsNote::setDataNote(DataNote *data)
{
    lineEditSelectTitle->setText(data->title);

    QString color = getStringHexColorForStyleSheet(data->backgroundColor);
    pButtonSelectColor->setStyleSheet(QString("background-color: #%1")
                                      .arg(color));
    pButtonSelectColor->setColor(data->backgroundColor);

    if(data->zPosition == UzPosition::AlwaysOnBottom)
        radioButtonWindowStaysOnBottomHint->setChecked(true);
    else
        radioButtonWindowStaysOnTopHint->setChecked(true);

    sliderSelectOpacity->setSliderPosition(data->opacity * 100);

    settingsPassword->setPassword(data->password);
    settingsPassword->setProtectionEnabled(data->protectionActive);
}

/// PUBLIC SLOTS
void DialogSettingsNote::showDialog(DataNote *data)
{
    setDataNote(data);

    settingsPassword->hide();
    widgetGeneral->show();

    pButtonSettingsPassword->setChecked(false);
    pButtonGeneralSettings->setChecked(true);

    show();
}

void DialogSettingsNote::showDialog(DataNote *data,
                                    QPoint pos)
{
    setDataNote(data);

    settingsPassword->hide();
    widgetGeneral->show();

    pButtonSettingsPassword->setChecked(false);
    pButtonGeneralSettings->setChecked(true);

    move(pos);

    show();
}

void DialogSettingsNote::setLanguage(QString pathToTheFileLanguage)
{
    settingsPassword->setLanguage(pathToTheFileLanguage);
    pButtonSelectColor->setLanguage(pathToTheFileLanguage);

    QSettings ini(pathToTheFileLanguage,
                       QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("SettingsNote");

    setWindowTitle(ini.value("Title").toString());
    labelSelectTitle->setText(ini.value("SelectTitle").toString());
    labelSelectOpacity->setText(ini.value("SelectOpacity").toString());
    labelSelectColor->setText(ini.value("SelectColor").toString());
    pButtonOk->setText(ini.value("Ok").toString());
    pButtonGeneralSettings->setText(ini.value("GeneralSettings").toString());
    pButtonSettingsPassword->setText(ini.value("SettingsPassword").toString());
    labelSelectZPosition->setText(ini.value("SelectZPosition").toString());
    radioButtonWindowStaysOnBottomHint->setText(ini.value("WindowStaysOnBottomHint").toString());
    radioButtonWindowStaysOnTopHint->setText(ini.value("WindowStaysOnTopHint").toString());

    // TOOL TIP
    lineEditSelectTitle->setToolTip(ini.value("ToolTipSelectTitle").toString());
    labelSelectTitle->setToolTip(ini.value("ToolTipSelectTitle").toString());
    labelSelectColor->setToolTip(ini.value("ToolTipSelectColor").toString());

    pButtonOk->setToolTip(ini.value("ToolTipOk").toString());
    pButtonGeneralSettings->setToolTip(ini.value("ToolTipGeneralSettings").toString());
    pButtonSettingsPassword->setToolTip(ini.value("ToolTipSettingsPassword").toString());

    labelSelectZPosition->setToolTip(ini.value("ToolTipSelectZPosition").toString());
    radioButtonWindowStaysOnBottomHint->setToolTip(ini.value("ToolTipWindowStaysOnBottomHint").toString());
    radioButtonWindowStaysOnTopHint->setToolTip(ini.value("ToolTipWindowStaysOnTopHint").toString());

    ini.endGroup();
}

/// PRIVATE SLOTS
void DialogSettingsNote::clickButtonSelectColor(QColor color)
{
    QString hexColor = getStringHexColorForStyleSheet(color);

    pButtonSelectColor->setStyleSheet(QString("background-color: #%1")
                                      .arg(hexColor));

    emit changeColor(color);
}

void DialogSettingsNote::setColorToButtonSelectColor(QColor color)
{
    QString hexColor = getStringHexColorForStyleSheet(color);

    pButtonSelectColor->setStyleSheet(QString("background-color: #%1")
                                      .arg(hexColor));
}

void DialogSettingsNote::clickPButtonGeneralSettings()
{
    settingsPassword->hide();
    widgetGeneral->show();

    pButtonSettingsPassword->setChecked(false);
}
void DialogSettingsNote::clickPButtonSettingsPassword()
{
    settingsPassword->show();
    widgetGeneral->hide();

    pButtonGeneralSettings->setChecked(false);
}

void DialogSettingsNote::clickRadioButtonChangeZPosition()
{
    QString zPosition;

    if(radioButtonWindowStaysOnTopHint->isChecked())
        zPosition = UzPosition::AlwaysOnTop;
    else
        zPosition = UzPosition::AlwaysOnBottom;

    emit changeZPosition(zPosition);
}
void DialogSettingsNote::newPasswordIsSelected()
{
    QString password = settingsPassword->getPassword();
    bool active = settingsPassword->protectionIsEnabled();

    emit changePassword(password,
                        active);
}
void DialogSettingsNote::changeProtectionEnable()
{
    QString password = settingsPassword->getPassword();

    if(!password.isEmpty())
    {
        bool active = settingsPassword->protectionIsEnabled();
        emit changePassword(password,
                            active);
    }
}

/// PROTECTED
void DialogSettingsNote::closeEvent(QCloseEvent *event)
{
    event->ignore();
    hide();

    emit iClose();
}
