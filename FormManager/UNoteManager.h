#ifndef UNOTEMANAGER_H
#define UNOTEMANAGER_H

#include <QtGui>
#include <QtCore>

#include "USettingsManager.h"
#include "TablesManager.h"
#include "UWidgetFindNote.h"

#include "Globals.h"

class UNoteManager: public QWidget
{
    Q_OBJECT
    
public:
    UNoteManager(QWidget *parent = 0);

    void addWidgetOnNotesPage(QWidget *widget,
                               int row,
                               int col);
    void addWidgetOnRecycleBinPage(QWidget *widget,
                                   int row,
                                   int col);

    bool recycleBinIsEmpty();
    void refreshRecycleBinIcons();    

    IndexPageManager currentPageManager();

private:
    void createSelectPageButton();
    void createPageNotes();
    void createPageRecycleBin();
    void createPageResultSearch();
    void createPageSettings();

public:
    QFrame *widgetTables;
    QHBoxLayout *hBoxLayoutWidgetTables;

    QListWidget *contentsWidget;
    QListWidgetItem *lWidgetItemNotes;
    QListWidgetItem *lWidgetItemRecycleBin;
    QListWidgetItem *lWidgetItemResultSearch;
    QListWidgetItem *lWidgetItemSettings;

    QStackedWidget *pagesWidget;

    // QFrame был выбран для того чтобы
    // окна таблиц и настроек могли обзавестись
    // рамкой
    QWidget *widgetTableNotesPage;
    QVBoxLayout *vBoxLayoutTableEdit;
    QLabel *labelWidgetNotes;
    UTableNotes *tableWidgetNotes;
    QGridLayout *gLayoutCommandButtonsNotes;
    QPushButton *pButtonUnselectTableNotesPage;
    QPushButton *pButtonSelectAllTableNotesPage;

    QWidget *widgetTableRecycleBinPage;
    QVBoxLayout *vBoxLayoutTableRecycleBin;
    QLabel *labelWidgetRecycleBin;
    UTableRecycleBin *tableWidgetRecycleBin;
    QGridLayout *gLayoutCommandButtonsRecycleBin;
    QPushButton *pButtonUnselectTableRecycleBinPage;
    QPushButton *pButtonSelectAllTableRecycleBinPage;

    QWidget *widgetTableResultSearchPage;
    QVBoxLayout *vBoxLayoutTableResultSearch;
    QLabel *labelWidgetResultSearch;
    UAreaResultsTable *areaResultsTable;

    QWidget *widgetTableSettingsPage;
    QVBoxLayout *vBoxLayoutSettings;
    QLabel *labelWidgetSettings;
    USettingsManager *settingsManager;

    QString strNotesTable;
    QString strRecycleBinTable;
    QString strResultSearchTable;
    QString strResultSettings;

    bool visibleTextIconsPanel;

private:
    QString strShowWindowEditingNotes;
    QString strHideWindowEditingNotes;

public slots:
    void setLanguage(QString pathToTheFileLanguage);
    void refreshCountsNotesInTables();
    void changePage(QListWidgetItem *current,
                    QListWidgetItem *previous);

    void showPageNotes();
    void showPageRecycleBin();
    void showPageResultSearch();
    void showPageSettings();

    void findIsEnd();

    void setVisibleTextIconsPanel(bool visible);

private slots:
    void unselectTableNotesPage();
    void selectAllTableNotesPage();

    void unselectTableRecycleBinPage();
    void selectAllTableRecycleBinPage();
};

#endif // UNOTEMANAGER_H
