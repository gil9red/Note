#ifndef TABLESMANAGER_H
#define TABLESMANAGER_H

#include <QtGui>
#include <QtCore>

#include "Delegates.h"
#include "Globals.h"

#include "NoteFile/Note.h"

class UTableNotes: public QTableWidget
{
    Q_OBJECT
public:
    UTableNotes(QWidget *parent = 0);

    QList <bool> & getListVisibleColumn(){ return listVisibleColumn; }
    void setShowContextMenuHorizontalHeader(bool show){ bShowContextMenuHorizontalHeader = show; }

public:
    QAction *actionDeleteItem;
    QAction *actionShowItem;
    QAction *actionHideItem;
    QAction *actionSettings;

    QAction *actionVisibleNote;
    QAction *actionDateCreateNote;
    QAction *actionCountTextSymbols;
    QAction *actionCountTextLine;
    QAction *actionDateLastChange;
    QAction *actionLock;
    QAction *actionDateOfLastRemoval;
    QAction *actionDateOfLastRestore;

private:
    bool visibleContextMenu;
    QList <bool> listVisibleColumn;
    QMenu *contextMenuHorizontalHeader;
    bool bShowContextMenuHorizontalHeader;

private slots:
    void signalCellPressed(int row, int col);
    void clickActionDelete(){ emit clickActionDeleteItem(); }
    void clickActionShow();
    void clickActionHide();

    void resizeColumn(int logicalIndex, int oldSize, int newSize);

public slots:
    void setVisibleColumnTable(int indexColumn,
                               bool visible);
    void setVisibleColumnTable(QList <bool> &list);
    void setLanguage(QString pathToTheFileLanguage);

    void showContextMenuHorizontalHeader(QPoint p);
    void showContextMenu(QPoint p);

    void loadWidthColumns();

signals:
    void clickActionDeleteItem();
    void clickActionShowItem();
    void clickActionHideItem();
    void clickActionSettingsItem();
    void languageIsChange(QString);
};

class UTableRecycleBin: public QTableWidget
{
    Q_OBJECT
public:
    UTableRecycleBin(QWidget *parent = 0);
    QList <bool> & getListVisibleColumn(){ return listVisibleColumn; }
    void setShowContextMenuHorizontalHeader(bool show){ bShowContextMenuHorizontalHeader = show; }

public:
    QAction *actionDeleteItem;
    QAction *actionRestoreItem;

    QAction *actionVisibleNote;
    QAction *actionDateCreateNote;
    QAction *actionCountTextSymbols;
    QAction *actionCountTextLine;
    QAction *actionDateLastChange;
    QAction *actionLock;
    QAction *actionDateOfLastRemoval;
    QAction *actionDateOfLastRestore;

private:
    bool visibleContextMenu;
    QList <bool> listVisibleColumn;

    QMenu *contextMenuHorizontalHeader;
    bool bShowContextMenuHorizontalHeader;

private slots:
    void clickActionDelete(){ emit clickActionDeleteItem(currentRow()); }
    void clickActionRestore(){ emit clickActionRestoreItem(currentRow()); }
    void signalCellPressed(int row, int col){ visibleContextMenu = true; }

    void resizeColumn(int logicalIndex, int oldSize, int newSize);

public slots:
    void setVisibleColumnTable(int indexColumn,
                               bool visible);
    void setVisibleColumnTable(QList <bool> &list);
    void setLanguage(QString pathToTheFileLanguage);

    void showContextMenuHorizontalHeader(QPoint p);
    void showContextMenu(QPoint p);

    void loadWidthColumns();

signals:
    void clickActionDeleteItem(int row);
    void clickActionRestoreItem(int row);
    void languageIsChange(QString);
};


// Будет этот класс хранить таблицы результатов поиска
// т.к. у каждой таблицы могут разная реализация, количество
// столбцов и разное контексное меню, но объединять все это
// в одну таблицу накладно, поэтому результатом будет
// несколько таблиц
class UAreaResultsTable: public QScrollArea
{
    Q_OBJECT

public:
    UAreaResultsTable(QWidget *parent = 0);

public:
    UTableNotes *tableNotes;
    UTableRecycleBin *tableRecycleBin;
    QGroupBox *groupBoxTableNotes;
    QGroupBox *groupBoxTableRecycleBin;

private:
    QWidget *mainWidget;
    QVBoxLayout *vBoxLayoutMain;

    QPushButton *pButtonUnselectTableNotesPage;
    QPushButton *pButtonSelectAllTableNotesPage;

    QPushButton *pButtonUnselectTableRecycleBinPage;
    QPushButton *pButtonSelectAllTableRecycleBinPage;

public slots:
    void setLanguage(QString pathToTheFileLanguage);
    void refreshCountsNotesInTables();

private slots:
    void converterMoveNoteToRecycleBin();
    void converterShowSettingsNote();
    void converterShowNote();
    void converterHideNote();

    void converterRestoreNote(int row);
    void converterDeleteNote(int row);

    void unselectTableNotesPage();
    void selectAllTableNotesPage();

    void unselectTableRecycleBinPage();
    void selectAllTableRecycleBinPage();

    void setVisibleContentGroupBoxTableNotes(bool visible);
    void setVisibleContentGroupBoxTableRecycleBin(bool visible);

signals:
    void languageIsChange(QString);

    void moveNoteToRecycleBin(Note*);
    void showSettingsNote(Note*);
    void showNote(Note*);
    void hideNote(Note*);

    void restoreNote(QString);
    void deleteNote(QString);

    void refreshTables();
};

#endif // TABLESMANAGER_H
