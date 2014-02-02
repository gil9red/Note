#ifndef UBUTTONWITHCONTEXTMENU_H
#define UBUTTONWITHCONTEXTMENU_H

#include <QtGui>
#include <QtCore>

class UButtonWithContextMenu: public QPushButton
{
    Q_OBJECT

public:
    UButtonWithContextMenu(QWidget *parent = 0);
    void setContextMenu(QMenu *menu){ contextMenu->addActions(menu->actions()); }
    void setToShowBottomContextMenu(bool bValue = true){ showBottomContextMenu = bValue; }
    QMenu * getContextMenu(){ return contextMenu; }
    bool contextMenuIsVisible(){ return visibleContextMenu; }

private:
    QMenu *contextMenu;
    bool showBottomContextMenu;
    bool visibleContextMenu;

public slots:
    void showContextMenu();
};

#endif // UBUTTONWITHCONTEXTMENU_H
