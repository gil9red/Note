#ifndef ULINEEDITWITHCLEARBUTTON_H
#define ULINEEDITWITHCLEARBUTTON_H

#include "ULineEditWithButtons.h"

class ULineEditWithClearButton: public ULineEditWithButtons
{
    Q_OBJECT

public:
    enum UClearButtonEvent
    {
        EnterEvent,
        LeaveEvent
    };

public:
    inline ULineEditWithClearButton(QWidget *parent = 0):
        ULineEditWithButtons(parent),
        activateButtonClearText(true)
    { createGUI(); }

private:
    void createGUI()
    {
        tButtonClearText = new QToolButton();
        tButtonClearText->hide();

        setActivateButtonClearText(activateButtonClearText);

        addButton(tButtonClearText);
    }

private:
    QToolButton *tButtonClearText;
    QPixmap pixmapEnter;
    QPixmap pixmapLeave;
    bool activateButtonClearText;

public slots:
    virtual void setIconButton(QPixmap pixmap)
    {
        pixmapEnter = pixmap;
        pixmapLeave = pixmap;

        tButtonClearText->setFixedSize(pixmap.size());
        tButtonClearText->setIcon(pixmap);
    }

    void setIconButton(UClearButtonEvent event, QPixmap pixmap)
    {
        switch(event)
        {
        case EnterEvent:
            pixmapEnter = pixmap;
            break;

        case LeaveEvent:
            pixmapLeave = pixmap;
            break;
        }
    }

    void clear()
    {
        ULineEditWithButtons::clear();
        emit textEdited("");
    }

    void hiddenCloseButton(QString text)
    {
        tButtonClearText->setVisible(!text.isEmpty());
        refreshMetrics();
    }

    void setActivateButtonClearText(bool activate)
    {
        activateButtonClearText = activate;

        if(activateButtonClearText)
        {
            hiddenCloseButton(text());

            tButtonClearText->installEventFilter(this);

            connect(this, SIGNAL(textChanged(QString)),
                    this, SLOT(hiddenCloseButton(QString)));

            connect(tButtonClearText, SIGNAL(clicked()),
                    this, SLOT(clear()));
        }else
        {
            tButtonClearText->hide();

            tButtonClearText->removeEventFilter(this);

            disconnect(this, SIGNAL(textChanged(QString)),
                       this, SLOT(hiddenCloseButton(QString)));

            disconnect(tButtonClearText, SIGNAL(clicked()),
                       this, SLOT(clear()));
        }

    }

    bool isActivateButtonClearText()
    { return activateButtonClearText; }

protected:
    bool eventFilter(QObject *object, QEvent *event)
    {
        if(object == tButtonClearText)
        {
            QEvent::Type type = event->type();

            if(type == QEvent::Enter)
            {
                tButtonClearText->setIcon(pixmapEnter);
                return true;

            }else if(type == QEvent::Leave)
            {
                tButtonClearText->setIcon(pixmapLeave);
                return true;

            }else
                return false;

        }else
            return ULineEditWithButtons::eventFilter(object, event);
    }
};

#endif // ULINEEDITWITHCLEARBUTTON_H
