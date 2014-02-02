#include "UButtonWithExtendedContextMenu.h"

/// PUBLIC SLOTS
void UWidgetExtendedContextMenu::show()
{
    QWidget::show();
    emit changeVisible_Show();
}
void UWidgetExtendedContextMenu::hide()
{
    QWidget::hide();
    emit changeVisible_Hide();
}
void UWidgetExtendedContextMenu::setVisible(bool visible)
{
    QWidget::setVisible(visible);

    if(visible)
        emit changeVisible_Show();
    else
        emit changeVisible_Hide();
}

/// PROTECTED
void UWidgetExtendedContextMenu::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setPen(QPen(QBrush(Qt::black),
                        3.0));

    painter.drawRect(0,
                     0,
                     width()  - 1,
                     height() - 1);
}


/// PUBLIC
UButtonWithExtendedContextMenu::UButtonWithExtendedContextMenu(QWidget *parent):
    QPushButton(parent)
{       
    QTextCodec::setCodecForTr(QTextCodec::codecForName(textCodec.toUtf8()));

    showBottomExtendedContextMenu = true;

    gLayoutExtendedContextMenu = new QGridLayout();

    widgetExtendedContextMenu = new UWidgetExtendedContextMenu();
    widgetExtendedContextMenu->setWindowFlags(Qt::Popup
                                              | Qt::FramelessWindowHint);
    widgetExtendedContextMenu->setFocusPolicy(Qt::NoFocus);

    widgetExtendedContextMenu->setLayout(gLayoutExtendedContextMenu);

    connect(this, SIGNAL(clicked()),
            this, SLOT(showExtendedContextMenu()));
}

void UButtonWithExtendedContextMenu::addWidgetToExtendedContextMenu(QWidget *widget,
                                                                    int row,
                                                                    int column,
                                                                    Qt::Alignment alignment)
{
    widget->setFocusPolicy(Qt::NoFocus);
    gLayoutExtendedContextMenu->addWidget(widget,
                                          row,
                                          column,
                                          alignment);
}

/// PUBLIC SLOTS
void UButtonWithExtendedContextMenu::showExtendedContextMenu()
{    
    if(showBottomExtendedContextMenu)
    {
        widgetExtendedContextMenu->show();
        widgetExtendedContextMenu->move(mapToGlobal(QPoint(0, height())));
    }else
    {
        widgetExtendedContextMenu->show();
        widgetExtendedContextMenu->move(mapToGlobal(QPoint(width(), 0)));
    }
}
