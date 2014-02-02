#ifndef UWidgetSelectHotkey_H
#define UWidgetSelectHotkey_H

#include <QtGui>
#include <QtCore>

#include "ULineEditWithButton/ULineEditWithClearButton.h"

#include "Globals.h"

class UShortcutLineEdit: public QLineEdit
{
    Q_OBJECT

signals:
    void pressModifier(Qt::KeyboardModifiers keyModifier);

protected:
    void keyPressEvent(QKeyEvent *event);
};

class UWidgetSelectHotkey: public QGroupBox
{
    Q_OBJECT
    
public:
    UWidgetSelectHotkey(QWidget *parent = 0);
    QString getHotkey(){ return lineEditHotkey->text(); }

private:
    QPushButton *pButtonOk;

    QGroupBox *groupBoxHotkeyAction;
    QCheckBox *checkBoxIncludeShift;
    QCheckBox *checkBoxIncludeCtrl;
    QCheckBox *checkBoxIncludeAlt;
    UShortcutLineEdit *lineEdit;
    QLineEdit *lineEditHotkey;

public slots:
    void setHotkey(QString hotkey);

private slots:
    void refreshResultHotkey();
    void pressedModifier(Qt::KeyboardModifiers keyModifier);

signals:
    void changeHotkey(QString hotkey);
};

#endif // UWidgetSelectHotkey_H
