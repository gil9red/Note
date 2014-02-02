#include "UPasswordProtect.h"

/// PUBLIC
UPasswordProtect::UPasswordProtect(QWidget *parent):
    QWidget(parent)
{
    labelInputPassword = new QLabel(this);


    QPixmap pixmap_enter(IconClearTextEnter);
    QPixmap pixmap_leave(IconClearTextLeave);

    int widthIcon = 20;
    int heightIcon = 20;
    pixmap_enter = pixmap_enter.scaled(widthIcon, heightIcon, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pixmap_leave = pixmap_leave.scaled(widthIcon, heightIcon, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    lineEditInputPassword = new ULineEditWithClearButton(this);
    lineEditInputPassword->setIconButton(pixmap_leave);
    lineEditInputPassword->setIconButton(ULineEditWithClearButton::EnterEvent,
                            pixmap_enter);
    lineEditInputPassword->setIconButton(ULineEditWithClearButton::LeaveEvent,
                            pixmap_leave);

    lineEditInputPassword->setEchoMode(QLineEdit::Password);
    connect(lineEditInputPassword, SIGNAL(returnPressed()),
            this, SLOT(clickPButtonOk()));

    pButtonOk = new QPushButton(this);
    connect(pButtonOk, SIGNAL(clicked()),
            this, SLOT(clickPButtonOk()));

    QVBoxLayout *vBoxLayoutMain = new QVBoxLayout();
    vBoxLayoutMain->addStretch();
    vBoxLayoutMain->addWidget(labelInputPassword);
    vBoxLayoutMain->addWidget(lineEditInputPassword);
    vBoxLayoutMain->addWidget(pButtonOk,
                              0,
                              Qt::AlignRight);

    setLayout(vBoxLayoutMain);
}

/// PRIVATE
bool UPasswordProtect::testInputData()
{
    if(lineEditInputPassword->text() == password
            && !lineEditInputPassword->text().isEmpty())
        return true;
    else
        return false;
}


/// PUBLIC SLOTS
void UPasswordProtect::setLanguage(QString pathToTheFileLanguage)
{    
    QSettings ini(pathToTheFileLanguage,
                       QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("PasswordProtect");

    pButtonOk->setText(ini.value("Ok").toString());
    labelInputPassword->setText(ini.value("InputPassword").toString());
    strTitle     = ini.value("Title").toString();
    strTextError = ini.value("Text").toString();

    // TOOL TIP
    pButtonOk->setToolTip(ini.value("ToolTipOk").toString());
    labelInputPassword->setToolTip(ini.value("ToolTipInputPassword").toString());
    lineEditInputPassword->setToolTip(ini.value("ToolTipInputPassword").toString());

    ini.endGroup();
}

void UPasswordProtect::clickPButtonOk()
{
    if(testInputData())
        emit enteredThePasswordRight();
    else
        QMessageBox::warning(this,
                             strTitle,
                             strTextError);

    lineEditInputPassword->clear();
}
