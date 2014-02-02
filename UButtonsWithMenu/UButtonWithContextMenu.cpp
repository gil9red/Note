#include "UButtonWithContextMenu.h"

/// PUBLIC
UButtonWithContextMenu::UButtonWithContextMenu(QWidget *parent):
    QPushButton(parent)
{       
    contextMenu           = new QMenu();
    showBottomContextMenu = true;
    visibleContextMenu    = false;

    connect(this, SIGNAL(clicked()),
            this, SLOT(showContextMenu()));
}

/// PUBLIC SLOTS
void UButtonWithContextMenu::showContextMenu()
{
    if(contextMenu)
    {
        visibleContextMenu = true;
        if(showBottomContextMenu)
            contextMenu->exec(mapToGlobal(QPoint(0, height())));
        else
            contextMenu->exec(mapToGlobal(QPoint(width(), 0)));
        visibleContextMenu = false;
    }
}
