#ifndef UBUTTONWITHEXTENDEDCONTEXTMENU_H
#define UBUTTONWITHEXTENDEDCONTEXTMENU_H

#include <QtGui>
#include <QtCore>

#include "Globals.h"

class UWidgetExtendedContextMenu: public QWidget
{
    Q_OBJECT

public slots:
    void show();
    void hide();
    void setVisible(bool visible);

signals:
    void changeVisible_Hide();
    void changeVisible_Show();

protected:
    void paintEvent(QPaintEvent *);
};

class UButtonWithExtendedContextMenu: public QPushButton
{
    Q_OBJECT

public:
    UButtonWithExtendedContextMenu(QWidget *parent = 0);
    void setToShowBottomExtendedContextMenu(bool bValue){ showBottomExtendedContextMenu = bValue; }
    void addWidgetToExtendedContextMenu(QWidget *widget,
                                        int row,
                                        int column,
                                        Qt::Alignment alignment = 0);

private:    
    UWidgetExtendedContextMenu *widgetExtendedContextMenu;
    QGridLayout *gLayoutExtendedContextMenu;
    bool showBottomExtendedContextMenu;

public slots:
    void showExtendedContextMenu();
};

#endif // UBUTTONWITHEXTENDEDCONTEXTMENU_H
