#include "USettingsPassword.h"

/// PUBLIC
USettingsPassword::USettingsPassword(QWidget *parent):
    QWidget(parent),
    password("")
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName(textCodec.toUtf8()));

    checkBoxPasswordProtect    = new QCheckBox();

    checkBoxHideTheInputData   = new QCheckBox();
    checkBoxHideTheInputData->setChecked(true);
    connect(checkBoxHideTheInputData, SIGNAL(toggled(bool)),
            this, SLOT(setHideTheInputData(bool)));

    QPixmap pixmap_enter(IconClearTextEnter);
    QPixmap pixmap_leave(IconClearTextLeave);

    int widthIcon = 20;
    int heightIcon = 20;
    pixmap_enter = pixmap_enter.scaled(widthIcon, heightIcon, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pixmap_leave = pixmap_leave.scaled(widthIcon, heightIcon, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    labelNewPassword           = new QLabel();
    lineEditNewPassword        = new ULineEditWithClearButton();
    lineEditNewPassword->setIconButton(pixmap_leave);
    lineEditNewPassword->setIconButton(ULineEditWithClearButton::EnterEvent,
                            pixmap_enter);
    lineEditNewPassword->setIconButton(ULineEditWithClearButton::LeaveEvent,
                            pixmap_leave);

    labelConfirmNewPassword    = new QLabel();
    lineEditConfirmNewPassword = new ULineEditWithClearButton();
    lineEditConfirmNewPassword->setIconButton(pixmap_leave);
    lineEditConfirmNewPassword->setIconButton(ULineEditWithClearButton::EnterEvent,
                            pixmap_enter);
    lineEditConfirmNewPassword->setIconButton(ULineEditWithClearButton::LeaveEvent,
                            pixmap_leave);

    labelOldPassword    = new QLabel();
    lineEditOldPassword = new ULineEditWithClearButton();
    lineEditOldPassword->setIconButton(pixmap_leave);
    lineEditOldPassword->setIconButton(ULineEditWithClearButton::EnterEvent,
                            pixmap_enter);
    lineEditOldPassword->setIconButton(ULineEditWithClearButton::LeaveEvent,
                            pixmap_leave);

    vBoxLayoutGroupBoxNewPassword = new QVBoxLayout();
    vBoxLayoutGroupBoxNewPassword->addWidget(labelOldPassword);
    vBoxLayoutGroupBoxNewPassword->addWidget(lineEditOldPassword);
    vBoxLayoutGroupBoxNewPassword->addWidget(labelNewPassword);
    vBoxLayoutGroupBoxNewPassword->addWidget(lineEditNewPassword);
    vBoxLayoutGroupBoxNewPassword->addWidget(labelConfirmNewPassword);
    vBoxLayoutGroupBoxNewPassword->addWidget(lineEditConfirmNewPassword);

    groupBoxNewPassword = new QGroupBox();
    groupBoxNewPassword->setLayout(vBoxLayoutGroupBoxNewPassword);

    pButtonSaveChange   = new QPushButton();
    connect(pButtonSaveChange, SIGNAL(clicked()),
            this, SLOT(clickPButtonSaveChange()));

    vBoxLayoutGroupBoxMain = new QVBoxLayout();
    vBoxLayoutGroupBoxMain->addWidget(checkBoxHideTheInputData);
    vBoxLayoutGroupBoxMain->addWidget(groupBoxNewPassword);
    vBoxLayoutGroupBoxMain->addWidget(pButtonSaveChange);
    vBoxLayoutGroupBoxMain->addStretch();

    groupBoxMain = new QGroupBox();
    groupBoxMain->setLayout(vBoxLayoutGroupBoxMain);

    vBoxLayoutMain = new QVBoxLayout();
    vBoxLayoutMain->addWidget(checkBoxPasswordProtect);
    vBoxLayoutMain->addWidget(groupBoxMain);
    vBoxLayoutMain->addStretch();

    setLayout(vBoxLayoutMain);

    setHideTheInputData(checkBoxHideTheInputData->isChecked());

    if(getPassword().isEmpty())
    {
        labelOldPassword->hide();
        lineEditOldPassword->hide();
    }else
    {
        labelOldPassword->show();
        lineEditOldPassword->show();
    }    
}

void USettingsPassword::setPassword(QString str)
{
    password = str;
    if(getPassword().isEmpty())
    {
        labelOldPassword->hide();
        lineEditOldPassword->hide();
    }else
    {
        labelOldPassword->show();
        lineEditOldPassword->show();
    }
}

/// PRIVATE
bool USettingsPassword::testInputData()
{
    if(lineEditOldPassword->text().isEmpty()
            // new password = confirm new passoword
            && lineEditNewPassword->text() == lineEditConfirmNewPassword->text()
            // new password no empty
            && !lineEditNewPassword->text().isEmpty())
    {
        labelOldPassword->show();
        lineEditOldPassword->show();
        return true;

    }else
        // old password is true
        if(lineEditOldPassword->text() == password
                // new password = confirm new passoword
                && lineEditNewPassword->text() == lineEditConfirmNewPassword->text()
                // new password no empty
                && !lineEditNewPassword->text().isEmpty())
            return true;
        else
            return false;        
}

void USettingsPassword::clearLineEdit()
{
    lineEditNewPassword->clear();
    lineEditConfirmNewPassword->clear();
    lineEditOldPassword->clear();
}

/// PUBLIC SLOTS
void USettingsPassword::setLanguage(QString pathToTheFileLanguage)
{
    QSettings ini(pathToTheFileLanguage,
                       QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("PasswordSettings");

    checkBoxPasswordProtect->setText(ini.value("PasswordProtect").toString());
    checkBoxHideTheInputData->setText(ini.value("HideTheInputData").toString());
    labelNewPassword->setText(ini.value("NewPassword").toString());
    labelConfirmNewPassword->setText(ini.value("ConfirmNewPassword").toString());
    labelOldPassword->setText(ini.value("OldPassword").toString());
    pButtonSaveChange->setText(ini.value("SaveChange").toString());
    strInformation                = ini.value("Information").toString();
    strWarning                    = ini.value("Warning").toString();
    strAcceptedNewPassword        = ini.value("AcceptedNewPassword").toString();
    strErrorWhenOldPasswordChange = ini.value("ErrorWhenOldPasswordChange").toString();

    // TOOL TIP
    checkBoxHideTheInputData->setToolTip(ini.value("ToolTipHideTheInputData").toString());
    checkBoxPasswordProtect->setToolTip(ini.value("ToolTipBoxPasswordProtect").toString());
    labelConfirmNewPassword->setToolTip(ini.value("ToolTipConfirmNewPassword").toString());
    labelNewPassword->setToolTip(ini.value("ToolTipNewPassword").toString());
    labelOldPassword->setToolTip(ini.value("ToolTipOldPassword").toString());
    lineEditConfirmNewPassword->setToolTip(ini.value("ToolTipConfirmNewPassword").toString());
    lineEditNewPassword->setToolTip(ini.value("ToolTipNewPassword").toString());
    lineEditOldPassword->setToolTip(ini.value("ToolTipOldPassword").toString());
    pButtonSaveChange->setToolTip(ini.value("ToolTipSaveChange").toString());

    ini.endGroup();
}

void USettingsPassword::setHideTheInputData(bool hide)
{
    QLineEdit::EchoMode echo;

    if(hide)
        echo = QLineEdit::Password;
    else
        echo = QLineEdit::Normal;

    lineEditNewPassword->setEchoMode(echo);
    lineEditConfirmNewPassword->setEchoMode(echo);
    lineEditOldPassword->setEchoMode(echo);
}

void USettingsPassword::clickPButtonSaveChange()
{
    if(testInputData())
    {
        setPassword(lineEditNewPassword->text());
        QMessageBox::information(this,
                                 strInformation,
                                 strAcceptedNewPassword);
        emit newPasswordIsSelected();

    }else    
        QMessageBox::warning(this,
                             strWarning,
                             strErrorWhenOldPasswordChange);

    clearLineEdit();
}
