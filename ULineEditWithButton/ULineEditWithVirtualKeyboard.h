#ifndef ULINEEDITWITHVIRTUALKEYBOARD_H
#define ULINEEDITWITHVIRTUALKEYBOARD_H

#include "ULineEditWithClearButton.h"

class ULineEditWithVirtualKeyboard: public ULineEditWithClearButton
{
    Q_OBJECT

public:
    ULineEditWithVirtualKeyboard(QWidget *parent = 0):
        ULineEditWithClearButton(parent)
    {
        QTextCodec::setCodecForTr(QTextCodec::codecForName("windows-1251"));

        createGUI();
    }

    void createGUI()
    {
        tButtonShowHideVirtualKeyboard = new QToolButton();
        tButtonShowHideVirtualKeyboard->setCheckable(true);

        connect(tButtonShowHideVirtualKeyboard, SIGNAL(toggled(bool)),
                this, SLOT(setToggled(bool)));

        addButton(tButtonShowHideVirtualKeyboard);
        widget = new QWidget();
        connect(widget, SIGNAL(destroyed()),
                this, SLOT(closeVirtualKeyboard()));
        setToggled(tButtonShowHideVirtualKeyboard->isChecked());
    }

private:
    QToolButton *tButtonShowHideVirtualKeyboard;

    QPixmap pixmapPressed;
    QPixmap pixmapRelease;

    QWidget *widget;

public slots:
    void setIconButton(QPixmap pixmapPressed, QPixmap pixmapRelease)
    {
        this->pixmapPressed = pixmapPressed;
        this->pixmapRelease = pixmapRelease;

        setToggled(tButtonShowHideVirtualKeyboard->isChecked());
    }

    void setIconClearText(QPixmap pixmap)
    { ULineEditWithClearButton::setIconButton(pixmap); }

    void closeVirtualKeyboard()
    {
        setToggled(false);
    }

private slots:
    void setToggled(bool toggled)
    {
        if(widget)
        widget->setVisible(toggled);

        if(toggled)
            tButtonShowHideVirtualKeyboard->setIcon(pixmapPressed);
        else
            tButtonShowHideVirtualKeyboard->setIcon(pixmapRelease);
    }
};

#endif // ULINEEDITWITHVIRTUALKEYBOARD_H
