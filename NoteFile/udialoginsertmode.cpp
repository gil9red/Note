#include "udialoginsertmode.h"

/// PUBLIC
UDialogInsertMode::UDialogInsertMode(QWidget *parent):
    QDialog(parent)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName(textCodec.toUtf8()));

    setWindowFlags(Qt::Dialog
                   | Qt::WindowCloseButtonHint);

    radioButtonTop            = new QRadioButton();
    radioButtonEnd            = new QRadioButton();
    radioButtonMiddle         = new QRadioButton();
    radioButtonPositionCursor = new QRadioButton();

    radioButtonTop->setChecked(true);


    vBoxLayoutGroupBox = new QVBoxLayout();
    vBoxLayoutGroupBox->addWidget(radioButtonTop);
    vBoxLayoutGroupBox->addWidget(radioButtonEnd);
    vBoxLayoutGroupBox->addWidget(radioButtonMiddle);
    vBoxLayoutGroupBox->addWidget(radioButtonPositionCursor);

    groupBoxRadioButtonSelectInsertMode = new QGroupBox();
    groupBoxRadioButtonSelectInsertMode->setLayout(vBoxLayoutGroupBox);

    pButtonOk = new QPushButton();
    pButtonOk->setDefault(true);

    pButtonCancel = new QPushButton();

    connect(pButtonOk, SIGNAL(clicked()),
            this, SLOT(clickPushButtonOk()));
    connect(pButtonCancel, SIGNAL(clicked()),
            this, SLOT(clickPushButtonCancel()));

    hBoxLayoutButtons = new QHBoxLayout();
    hBoxLayoutButtons->addWidget(pButtonOk);
    hBoxLayoutButtons->addWidget(pButtonCancel);

    vBoxLayoutMain = new QVBoxLayout();
    vBoxLayoutMain->addWidget(groupBoxRadioButtonSelectInsertMode);
    vBoxLayoutMain->addLayout(hBoxLayoutButtons);

    setLayout(vBoxLayoutMain);

    result = UDialogInsertMode::None;
}

/// PUBLIC SLOTS
void UDialogInsertMode::setLanguage(QString pathToTheFileLanguage)
{
    QSettings ini(pathToTheFileLanguage,
                       QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("DialogInsert");

    setWindowTitle(ini.value("Title").toString());

    groupBoxRadioButtonSelectInsertMode->setTitle(ini.value("GroupBoxTitle").toString());
    radioButtonTop->setText(ini.value("Top").toString());
    radioButtonEnd->setText(ini.value("End").toString());
    radioButtonMiddle->setText(ini.value("Middle").toString());
    radioButtonPositionCursor->setText(ini.value("PositionCursor").toString());
    pButtonOk->setText(ini.value("Ok").toString());
    pButtonCancel->setText(ini.value("Cancel").toString());

    // TOOL TIP
    radioButtonTop->setToolTip(ini.value("ToolTipTop").toString());
    radioButtonEnd->setToolTip(ini.value("ToolTipEnd").toString());
    radioButtonMiddle->setToolTip(ini.value("ToolTipMiddle").toString());
    radioButtonPositionCursor->setToolTip(ini.value("ToolTipPositionCursor").toString());
    pButtonOk->setToolTip(ini.value("ToolTipOk").toString());
    pButtonCancel->setToolTip(ini.value("ToolTipCancel").toString());

    ini.endGroup();
}

void UDialogInsertMode::showDialog()
{
    pushButtonCancel = false;

    exec();

    if(!pushButtonCancel)
    {
        if(radioButtonTop->isChecked())        
            result = UDialogInsertMode::Top;

        else if(radioButtonEnd->isChecked())
            result = UDialogInsertMode::End;

        else if(radioButtonMiddle->isChecked())
            result = UDialogInsertMode::Middle;

        else if(radioButtonPositionCursor->isChecked())
            result = UDialogInsertMode::PositionCursor;

    }else
        result = UDialogInsertMode::None;
}

/// PRIVATE SLOTS
void UDialogInsertMode::clickPushButtonCancel()
{
    pushButtonCancel = true;

    close();
}
