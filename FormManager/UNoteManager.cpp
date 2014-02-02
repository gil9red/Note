#include "UNoteManager.h"

/// PUBLIC
UNoteManager::UNoteManager(QWidget *parent):
    QWidget(parent),
    visibleTextIconsPanel(true)
{    
    contentsWidget = new QListWidget();
    contentsWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setMovement(QListView::Static);

    createSelectPageButton();

    int widthIconItems = USizeIcons::size64x64.width();
    int heightIconItems = USizeIcons::size64x64.height();
    contentsWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    contentsWidget->setIconSize(QSize(widthIconItems, heightIconItems));

    int widthListWidgetItems = widthIconItems + 40;

    for(int i = 0; i < contentsWidget->count(); i++)
    {
        QListWidgetItem *item = contentsWidget->item(i);
        item->setSizeHint(QSize(widthListWidgetItems,
                                heightIconItems + 34));
    }

    contentsWidget->setFixedWidth(widthListWidgetItems);

    createPageNotes();
    createPageRecycleBin();
    createPageResultSearch();
    createPageSettings();

    // создаем зоны с прокруткой
    QScrollArea *scrollAreaPageNotes = new QScrollArea();
    QScrollArea *scrollAreaPageRecycleBin = new QScrollArea();
    QScrollArea *scrollAreaPageResultSearch = new QScrollArea();
    QScrollArea *scrollAreaPageSettings = new QScrollArea();

    // зоны с прокруткой могут менять размер виджетов в них
    scrollAreaPageRecycleBin->setWidgetResizable(true);
    scrollAreaPageNotes->setWidgetResizable(true);
    scrollAreaPageResultSearch->setWidgetResizable(true);
    scrollAreaPageSettings->setWidgetResizable(true);

    // добавляем в зоны с прокруткой страницы менеджера
    scrollAreaPageNotes->setWidget(widgetTableNotesPage);
    scrollAreaPageRecycleBin->setWidget(widgetTableRecycleBinPage);
    scrollAreaPageResultSearch->setWidget(widgetTableResultSearchPage);
    scrollAreaPageSettings->setWidget(widgetTableSettingsPage);

    pagesWidget = new QStackedWidget();
    pagesWidget->insertWidget(PageNotes, scrollAreaPageNotes);
    pagesWidget->insertWidget(PageRecycleBin, scrollAreaPageRecycleBin);
    pagesWidget->insertWidget(PageResultSearch, scrollAreaPageResultSearch);
    pagesWidget->insertWidget(PageSettings, scrollAreaPageSettings);

    pagesWidget->setFrameShape(contentsWidget->frameShape());

    contentsWidget->setCurrentRow(PageNotes);

    QHBoxLayout *hBoxLayoutMain = new QHBoxLayout();
    hBoxLayoutMain->addWidget(contentsWidget);
    hBoxLayoutMain->addWidget(pagesWidget);

    setLayout(hBoxLayoutMain);

    connect(settingsManager, SIGNAL(columnTableNotesVisibleChange(int,bool)),
            tableWidgetNotes, SLOT(setVisibleColumnTable(int,bool)));
    connect(settingsManager, SIGNAL(columnTableRecycleBinVisibleChange(int,bool)),
            tableWidgetRecycleBin, SLOT(setVisibleColumnTable(int,bool)));
    connect(settingsManager, SIGNAL(visibleTextButtonsPanel(bool)),
            this, SLOT(setVisibleTextIconsPanel(bool)));

    QSettings ini(absolutePathToTheConfigurationProgram(),
                      QSettings::IniFormat);
    bool visibleTextIconsPanel = ini.value("Manager/VisibilityOfTheTextPanelButtons", true).toBool();

    setVisibleTextIconsPanel(visibleTextIconsPanel);
    settingsManager->checkBoxVisibleTextButtonsPanel->setChecked(visibleTextIconsPanel);

    connect(tableWidgetNotes->actionCountTextLine, SIGNAL(triggered(bool)),
            settingsManager, SLOT(clickCheckNotesCountTextLine(bool)));
    connect(tableWidgetNotes->actionCountTextSymbols, SIGNAL(triggered(bool)),
            settingsManager, SLOT(clickCheckNotesCountTextSymbols(bool)));

    connect(tableWidgetNotes->actionDateCreateNote, SIGNAL(triggered(bool)),
            settingsManager, SLOT(clickCheckNotesDateCreateNote(bool)));
    connect(tableWidgetNotes->actionDateLastChange, SIGNAL(triggered(bool)),
            settingsManager, SLOT(clickCheckNotesDateLastChange(bool)));
    connect(tableWidgetNotes->actionDateOfLastRemoval, SIGNAL(triggered(bool)),
            settingsManager, SLOT(clickCheckNotesDateOfLastRemoval(bool)));
    connect(tableWidgetNotes->actionDateOfLastRestore, SIGNAL(triggered(bool)),
            settingsManager, SLOT(clickCheckNotesDateOfLastRestore(bool)));

    connect(tableWidgetNotes->actionVisibleNote, SIGNAL(triggered(bool)),
            settingsManager, SLOT(clickCheckNotesVisibleNote(bool)));
    connect(tableWidgetNotes->actionLock, SIGNAL(triggered(bool)),
            settingsManager, SLOT(clickCheckNotesLock(bool)));

    tableWidgetNotes->setShowContextMenuHorizontalHeader(true);


    connect(tableWidgetRecycleBin->actionCountTextLine, SIGNAL(triggered(bool)),
            settingsManager, SLOT(clickCheckRecycleBinCountTextLine(bool)));
    connect(tableWidgetRecycleBin->actionCountTextSymbols, SIGNAL(triggered(bool)),
            settingsManager, SLOT(clickCheckRecycleBinCountTextSymbols(bool)));

    connect(tableWidgetRecycleBin->actionDateCreateNote, SIGNAL(triggered(bool)),
            settingsManager, SLOT(clickCheckRecycleBinDateCreateNote(bool)));
    connect(tableWidgetRecycleBin->actionDateLastChange, SIGNAL(triggered(bool)),
            settingsManager, SLOT(clickCheckRecycleBinDateLastChange(bool)));
    connect(tableWidgetRecycleBin->actionDateOfLastRemoval, SIGNAL(triggered(bool)),
            settingsManager, SLOT(clickCheckRecycleBinDateOfLastRemoval(bool)));
    connect(tableWidgetRecycleBin->actionDateOfLastRestore, SIGNAL(triggered(bool)),
            settingsManager, SLOT(clickCheckRecycleBinDateOfLastRestore(bool)));

    connect(tableWidgetRecycleBin->actionVisibleNote, SIGNAL(triggered(bool)),
            settingsManager, SLOT(clickCheckRecycleBinVisibleNote(bool)));
    connect(tableWidgetRecycleBin->actionLock, SIGNAL(triggered(bool)),
            settingsManager, SLOT(clickCheckRecycleBinLock(bool)));
    tableWidgetRecycleBin->setShowContextMenuHorizontalHeader(true);
}

void UNoteManager::addWidgetOnNotesPage(QWidget *widget, int row, int col)
{
    if(row < 0) row = 0;
    if(col < 0) col = 0;

    gLayoutCommandButtonsNotes->addWidget(widget,
                                          row,
                                          col);
}
void UNoteManager::addWidgetOnRecycleBinPage(QWidget *widget,
                                             int row,
                                             int col)
{
    if(row < 0) row = 0;
    if(col < 0) col = 0;

    gLayoutCommandButtonsRecycleBin->addWidget(widget,
                                               row,
                                               col);
}

bool UNoteManager::recycleBinIsEmpty()
{
    return tableWidgetRecycleBin->rowCount();
}

void UNoteManager::refreshRecycleBinIcons()
{
    // if no empty
    if(!recycleBinIsEmpty())
        lWidgetItemRecycleBin->setIcon(QIcon(IconRecycleBin_Empty));
    else
        lWidgetItemRecycleBin->setIcon(QIcon(IconRecycleBin_Full));
}

IndexPageManager UNoteManager::currentPageManager()
{
    QListWidgetItem *item = contentsWidget->currentItem();

    if(item == lWidgetItemNotes)
        return PageNotes;

    else if(item == lWidgetItemRecycleBin)
        return PageRecycleBin;

    else if(item == lWidgetItemResultSearch)
        return PageResultSearch;

    else if(item == lWidgetItemSettings)
        return PageSettings;

    else
        return PageUnknown;
}

/// PRIVATE
void UNoteManager::createSelectPageButton()
{
    lWidgetItemNotes = new QListWidgetItem();
    lWidgetItemNotes->setIcon(QIcon(IconEditor));
    lWidgetItemNotes->setTextAlignment(Qt::AlignHCenter);
    lWidgetItemNotes->setFlags(Qt::ItemIsEnabled
                               | Qt::ItemIsSelectable);

    lWidgetItemRecycleBin = new QListWidgetItem();
    lWidgetItemRecycleBin->setIcon(QIcon(IconRecycleBin_Empty));
    lWidgetItemRecycleBin->setTextAlignment(Qt::AlignHCenter);
    lWidgetItemRecycleBin->setFlags(Qt::ItemIsEnabled
                                    | Qt::ItemIsSelectable);

    lWidgetItemResultSearch = new QListWidgetItem();
    lWidgetItemResultSearch->setIcon(QIcon(IconResultSearch));
    lWidgetItemResultSearch->setTextAlignment(Qt::AlignHCenter);
    lWidgetItemResultSearch->setFlags(Qt::ItemIsEnabled
                                      | Qt::ItemIsSelectable);

    lWidgetItemSettings = new QListWidgetItem();
    lWidgetItemSettings->setIcon(QIcon(IconSettingsManager));
    lWidgetItemSettings->setTextAlignment(Qt::AlignHCenter);
    lWidgetItemSettings->setFlags(Qt::ItemIsEnabled
                                  | Qt::ItemIsSelectable);

    contentsWidget->insertItem(PageNotes, lWidgetItemNotes);
    contentsWidget->insertItem(PageRecycleBin, lWidgetItemRecycleBin);
    contentsWidget->insertItem(PageResultSearch, lWidgetItemResultSearch);
    contentsWidget->insertItem(PageSettings, lWidgetItemSettings);

    connect(contentsWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}

void UNoteManager::createPageNotes()
{
    tableWidgetNotes = new UTableNotes();

    labelWidgetNotes = new QLabel();
    labelWidgetNotes->setFont(QFont("Times New Roman", 16));

    gLayoutCommandButtonsNotes = new QGridLayout();

    pButtonUnselectTableNotesPage = new QPushButton();
    pButtonSelectAllTableNotesPage = new QPushButton();

    connect(pButtonUnselectTableNotesPage, SIGNAL(clicked()),
            this, SLOT(unselectTableNotesPage()));
    connect(pButtonSelectAllTableNotesPage, SIGNAL(clicked()),
            this, SLOT(selectAllTableNotesPage()));

    QHBoxLayout *hBoxLayoutButtons = new QHBoxLayout();
    hBoxLayoutButtons->addWidget(pButtonSelectAllTableNotesPage);
    hBoxLayoutButtons->addWidget(pButtonUnselectTableNotesPage);
    hBoxLayoutButtons->addStretch();

    vBoxLayoutTableEdit = new QVBoxLayout();
    vBoxLayoutTableEdit->addWidget(labelWidgetNotes);
    vBoxLayoutTableEdit->addLayout(hBoxLayoutButtons);
    vBoxLayoutTableEdit->addWidget(tableWidgetNotes);
    vBoxLayoutTableEdit->addLayout(gLayoutCommandButtonsNotes);

    widgetTableNotesPage = new QWidget();
    widgetTableNotesPage->setLayout(vBoxLayoutTableEdit);
}

void UNoteManager::createPageRecycleBin()
{
    labelWidgetRecycleBin = new QLabel();
    labelWidgetRecycleBin->setFont(QFont("Times New Roman", 16));

    tableWidgetRecycleBin = new UTableRecycleBin();

    gLayoutCommandButtonsRecycleBin = new QGridLayout();

    pButtonUnselectTableRecycleBinPage = new QPushButton();
    pButtonSelectAllTableRecycleBinPage = new QPushButton();

    connect(pButtonUnselectTableRecycleBinPage, SIGNAL(clicked()),
            this, SLOT(unselectTableRecycleBinPage()));
    connect(pButtonSelectAllTableRecycleBinPage, SIGNAL(clicked()),
            this, SLOT(selectAllTableRecycleBinPage()));

    QHBoxLayout *hBoxLayoutButtons = new QHBoxLayout();
    hBoxLayoutButtons->addWidget(pButtonSelectAllTableRecycleBinPage);
    hBoxLayoutButtons->addWidget(pButtonUnselectTableRecycleBinPage);
    hBoxLayoutButtons->addStretch();

    vBoxLayoutTableRecycleBin = new QVBoxLayout();
    vBoxLayoutTableRecycleBin->addWidget(labelWidgetRecycleBin);
    vBoxLayoutTableRecycleBin->addLayout(hBoxLayoutButtons);
    vBoxLayoutTableRecycleBin->addWidget(tableWidgetRecycleBin);
    vBoxLayoutTableRecycleBin->addLayout(gLayoutCommandButtonsRecycleBin);

    widgetTableRecycleBinPage = new QWidget();
    widgetTableRecycleBinPage->setLayout(vBoxLayoutTableRecycleBin);
}

void UNoteManager::createPageResultSearch()
{
    labelWidgetResultSearch = new QLabel();
    labelWidgetResultSearch->setFont(QFont("Times New Roman", 16));

    areaResultsTable = new UAreaResultsTable();

    vBoxLayoutTableResultSearch = new QVBoxLayout();
    vBoxLayoutTableResultSearch->addWidget(labelWidgetResultSearch);
    vBoxLayoutTableResultSearch->addWidget(areaResultsTable);

    widgetTableResultSearchPage = new QWidget();
    widgetTableResultSearchPage->setLayout(vBoxLayoutTableResultSearch);
}

void UNoteManager::createPageSettings()
{
    labelWidgetSettings = new QLabel();
    labelWidgetSettings->setFont(QFont("Times New Roman", 16));

    settingsManager = new USettingsManager();

    vBoxLayoutSettings = new QVBoxLayout();
    vBoxLayoutSettings->addWidget(labelWidgetSettings);
    vBoxLayoutSettings->addWidget(settingsManager);

    widgetTableSettingsPage = new QFrame();
    widgetTableSettingsPage->setLayout(vBoxLayoutSettings);
}

/// PUBLIC SLOTS
void UNoteManager::setLanguage(QString pathToTheFileLanguage)
{
    tableWidgetNotes->setLanguage(pathToTheFileLanguage);
    tableWidgetRecycleBin->setLanguage(pathToTheFileLanguage);
    areaResultsTable->setLanguage(pathToTheFileLanguage);
    settingsManager->setLanguage(pathToTheFileLanguage);

    QSettings ini(pathToTheFileLanguage,
                       QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("NOTEMANAGER");

    strNotesTable = ini.value("Notes").toString();
    strRecycleBinTable = ini.value("RecycleBin").toString();
    strResultSearchTable = ini.value("ResultSearch").toString();
    strResultSettings = ini.value("Settings").toString();

    labelWidgetNotes->setText(QString("<html> <font color='#1177ff'> <b>%1</b> </font> </html>")
                              .arg(ini.value("Notes").toString()));
    labelWidgetRecycleBin->setText(QString("<html> <font color='#1177ff'> <b>%1</b> </font> </html>")
                                   .arg(ini.value("RecycleBin").toString()));
    labelWidgetResultSearch->setText(QString("<html> <font color='#1177ff'> <b>%1</b> </font> </html>")
                                     .arg(ini.value("ResultSearch").toString()));
    labelWidgetSettings->setText(QString("<html> <font color='#1177ff'> <b>%1</b> </font> </html>")
                                 .arg(ini.value("Settings").toString()));

    strShowWindowEditingNotes = ini.value("ShowWindowEditingNotes").toString();
    strHideWindowEditingNotes = ini.value("HideWindowEditingNotes").toString();

    pButtonUnselectTableNotesPage->setText(ini.value("Unselect").toString());
    pButtonSelectAllTableNotesPage->setText(ini.value("SelectAll").toString());

    pButtonUnselectTableRecycleBinPage->setText(ini.value("Unselect").toString());
    pButtonSelectAllTableRecycleBinPage->setText(ini.value("SelectAll").toString());

    // TEXT AND TOOL TIP
    // добавляем к названию количество заметок
    refreshCountsNotesInTables();

    lWidgetItemNotes->setToolTip(ini.value("ToolTipNotes").toString());
    lWidgetItemRecycleBin->setToolTip(ini.value("ToolTipRecycleBin").toString());
    lWidgetItemResultSearch->setToolTip(ini.value("ToolTipResultSearch").toString());
    lWidgetItemSettings->setToolTip(ini.value("ToolTipSettings").toString());

    ini.endGroup();

    contentsWidget->viewport()->update();
}

void UNoteManager::refreshCountsNotesInTables()
{
    if(visibleTextIconsPanel)
    {
        lWidgetItemNotes->setText(QString("%1 \n(%2)")
                                  .arg(strNotesTable)
                                  .arg(tableWidgetNotes->rowCount()));
        lWidgetItemRecycleBin->setText(QString("%1 \n(%2)")
                                       .arg(strRecycleBinTable)
                                       .arg(tableWidgetRecycleBin->rowCount()));
        lWidgetItemResultSearch->setText(QString("%1 \n(%2, %3)")
                                         .arg(strResultSearchTable)
                                         .arg(areaResultsTable->tableNotes->rowCount())
                                         .arg(areaResultsTable->tableRecycleBin->rowCount()));
        lWidgetItemSettings->setText(strResultSettings);
    }
}

void UNoteManager::changePage(QListWidgetItem *current,
                              QListWidgetItem *previous)
{
    if(!current)
        current = previous;

    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}

void UNoteManager::showPageNotes()
{
    int row = contentsWidget->row(lWidgetItemNotes);
    pagesWidget->setCurrentIndex(row);
    contentsWidget->setCurrentRow(row);
}
void UNoteManager::showPageRecycleBin()
{
    int row = contentsWidget->row(lWidgetItemRecycleBin);
    pagesWidget->setCurrentIndex(row);
    contentsWidget->setCurrentRow(row);
}
void UNoteManager::showPageResultSearch()
{
    int row = contentsWidget->row(lWidgetItemResultSearch);
    pagesWidget->setCurrentIndex(row);
    contentsWidget->setCurrentRow(row);
}
void UNoteManager::showPageSettings()
{
    int row = contentsWidget->row(lWidgetItemSettings);
    pagesWidget->setCurrentIndex(row);
    contentsWidget->setCurrentRow(row);
}

void UNoteManager::findIsEnd()
{
    if(visibleTextIconsPanel)
        lWidgetItemResultSearch->setText(QString("%1 (%2, %3)")
                                         .arg(strResultSearchTable)
                                         .arg(areaResultsTable->tableNotes->rowCount())
                                         .arg(areaResultsTable->tableRecycleBin->rowCount()));

    areaResultsTable->refreshCountsNotesInTables();

    // если что то нашли, то автоматически переходим на
    // окно результата поиска
    if(areaResultsTable->tableNotes->rowCount()
            || areaResultsTable->tableRecycleBin->rowCount())
        showPageResultSearch();
}

void UNoteManager::setVisibleTextIconsPanel(bool visible)
{
    visibleTextIconsPanel = visible;

    if(visible)
    {
        lWidgetItemNotes->setText(QString("%1 (%2)")
                                  .arg(strNotesTable)
                                  .arg(tableWidgetNotes->rowCount()));

        lWidgetItemRecycleBin->setText(QString("%1 (%2)")
                                       .arg(strRecycleBinTable)
                                       .arg(tableWidgetRecycleBin->rowCount()));

        lWidgetItemResultSearch->setText(QString("%1 (%2, %3)")
                                         .arg(strResultSearchTable)
                                         .arg(areaResultsTable->tableNotes->rowCount())
                                         .arg(areaResultsTable->tableRecycleBin->rowCount()));

        lWidgetItemSettings->setText(strResultSettings);
    }else
    {
        lWidgetItemNotes->setText("");
        lWidgetItemRecycleBin->setText("");
        lWidgetItemResultSearch->setText("");
        lWidgetItemSettings->setText("");
    }

    contentsWidget->setSpacing(contentsWidget->spacing());

    QSettings ini(absolutePathToTheConfigurationProgram(),
                      QSettings::IniFormat);
    ini.setValue("Manager/VisibilityOfTheTextPanelButtons", visible);
    ini.sync();
}

/// PRIVATE SLOTS
void UNoteManager::unselectTableNotesPage()
{
    QTableWidget *table = tableWidgetNotes;

    for (int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem *item = table->item(i, indexMainColumn);
        item->setCheckState(Qt::Unchecked);
    }
}
void UNoteManager::selectAllTableNotesPage()
{
    QTableWidget *table = tableWidgetNotes;

    for (int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem *item = table->item(i, indexMainColumn);
        item->setCheckState(Qt::Checked);
    }
}

void UNoteManager::unselectTableRecycleBinPage()
{
    QTableWidget *table = tableWidgetRecycleBin;

    for (int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem *item = table->item(i, indexMainColumn);
        item->setCheckState(Qt::Unchecked);
    }
}
void UNoteManager::selectAllTableRecycleBinPage()
{
    QTableWidget *table = tableWidgetRecycleBin;

    for (int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem *item = table->item(i, indexMainColumn);
        item->setCheckState(Qt::Checked);
    }
}
