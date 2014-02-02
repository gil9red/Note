#include "UWidgetSelectHotkey.h"

/// PROTECTED
void UShortcutLineEdit::keyPressEvent(QKeyEvent *event)
{
    Qt::Key vkey = (Qt::Key)event->nativeVirtualKey();
    Qt::KeyboardModifiers modifier = event->modifiers();
    QString textKey = event->text();

    if(textKey.isEmpty())
        if(modifier & Qt::CTRL
                || modifier & Qt::SHIFT
                || modifier & Qt::ALT)
        {
            emit pressModifier(modifier);
            return;
        }
    QString a_zSymbol = getStrKeyA_Z(vkey);
    if(!a_zSymbol.isEmpty())
    {
        setText(a_zSymbol);
        return;
    }
}


/// PUBLIC
UWidgetSelectHotkey::UWidgetSelectHotkey(QWidget *parent):
    QGroupBox(parent)
{
    checkBoxIncludeShift = new QCheckBox("Shift");
    checkBoxIncludeCtrl = new QCheckBox("Ctrl");
    checkBoxIncludeAlt = new QCheckBox("Alt");   

    lineEdit = new UShortcutLineEdit();
    connect(lineEdit, SIGNAL(pressModifier(Qt::KeyboardModifiers)),
            this, SLOT(pressedModifier(Qt::KeyboardModifiers)));

    QVBoxLayout *vBoxLayout2 = new QVBoxLayout();
    vBoxLayout2->addWidget(checkBoxIncludeCtrl);
    vBoxLayout2->addWidget(checkBoxIncludeShift);

    lineEditHotkey = new QLineEdit();
    lineEditHotkey->setReadOnly(true);

    QHBoxLayout *hBoxLayout3 = new QHBoxLayout();
    hBoxLayout3->addLayout(vBoxLayout2);
    hBoxLayout3->addWidget(checkBoxIncludeAlt);
    hBoxLayout3->addWidget(new QLabel("+"));
    hBoxLayout3->addWidget(lineEdit);
    hBoxLayout3->addWidget(new QLabel("="));
    hBoxLayout3->addWidget(lineEditHotkey);
    hBoxLayout3->addStretch();

    setLayout(hBoxLayout3);

    refreshResultHotkey();

    connect(checkBoxIncludeShift, SIGNAL(toggled(bool)),
            this, SLOT(refreshResultHotkey()));
    connect(checkBoxIncludeCtrl, SIGNAL(toggled(bool)),
            this, SLOT(refreshResultHotkey()));
    connect(checkBoxIncludeAlt, SIGNAL(toggled(bool)),
            this, SLOT(refreshResultHotkey()));

    connect(lineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(refreshResultHotkey()));
}

/// PUBLIC SLOTS
void UWidgetSelectHotkey::setHotkey(QString hotkey)
{
    checkBoxIncludeCtrl->setChecked(false);
    checkBoxIncludeShift->setChecked(false);
    checkBoxIncludeAlt->setChecked(false);

    QStringList listHotKey = hotkey.split(separatorHotkey);
    for(int i = 0; i < listHotKey.count(); i++)
    {
        if(listHotKey.at(i) == "Ctrl")
            checkBoxIncludeCtrl->setChecked(true);

        else if(listHotKey.at(i) == "Shift")
            checkBoxIncludeShift->setChecked(true);

        else if(listHotKey.at(i) == "Alt")
            checkBoxIncludeAlt->setChecked(true);

        else
            lineEdit->setText(listHotKey.at(i));
    }

    refreshResultHotkey();
}

/// PRIVATE SLOTS
void UWidgetSelectHotkey::refreshResultHotkey()
{
    QString hotkey;
    bool includeCtrl  = checkBoxIncludeCtrl->isChecked();
    bool includeShift = checkBoxIncludeShift->isChecked();
    bool includeAlt   = checkBoxIncludeAlt->isChecked();

    if(includeCtrl)
        hotkey.append("Ctrl");

    if(includeShift)
        if(!hotkey.isEmpty())
            hotkey.append(separatorHotkey + "Shift");
        else
            hotkey.append("Shift");

    if(includeAlt)
        if(!hotkey.isEmpty())
            hotkey.append(separatorHotkey + "Alt");
        else
            hotkey.append("Alt");

    if(!lineEdit->text().isEmpty()
            && !hotkey.isEmpty())
        hotkey.append(separatorHotkey + lineEdit->text());
    else
        hotkey.append(lineEdit->text());

    lineEditHotkey->setText(hotkey);
}

void UWidgetSelectHotkey::pressedModifier(Qt::KeyboardModifiers keyModifier)
{
    if(keyModifier & Qt::CTRL)
        checkBoxIncludeCtrl->setChecked(!checkBoxIncludeCtrl->isChecked());

    if(keyModifier & Qt::SHIFT)
        checkBoxIncludeShift->setChecked(!checkBoxIncludeShift->isChecked());

    if(keyModifier & Qt::ALT)
        checkBoxIncludeAlt->setChecked(!checkBoxIncludeAlt->isChecked());
}
