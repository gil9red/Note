#include "TablesManager.h"

/// PUBLIC
UTableNotes::UTableNotes(QWidget *parent):
    QTableWidget(parent),
    visibleContextMenu(false),
    bShowContextMenuHorizontalHeader(false)
{    
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenu(QPoint)));
    connect(this, SIGNAL(cellPressed(int,int)),
            this, SLOT(signalCellPressed(int,int)));

    horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenuHorizontalHeader(QPoint)));

    connect(horizontalHeader(), SIGNAL(sectionResized(int,int,int)),
            this, SLOT(resizeColumn(int,int,int)));

    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    setAlternatingRowColors(true);

    setItemDelegateForColumn(indexColumnVisible,           new ReadDelegate());
    setItemDelegateForColumn(indexColumnDateOfCreating,    new ReadDelegate());
    setItemDelegateForColumn(indexColumnCountTextSymbols,  new ReadDelegate());
    setItemDelegateForColumn(indexColumnCountTextLines,    new ReadDelegate());
    setItemDelegateForColumn(indexColumnDateLastChange,    new ReadDelegate());
    setItemDelegateForColumn(indexColumnLock,              new ReadDelegate());
    setItemDelegateForColumn(indexColumnDateOfLastRemoval, new ReadDelegate());
    setItemDelegateForColumn(indexColumnDateOfLastRestore, new ReadDelegate());

    setRowCount(0);

    actionDeleteItem = new QAction(this);
    actionShowItem   = new QAction(this);
    actionHideItem   = new QAction(this);
    actionSettings   = new QAction(this);

    actionDeleteItem->setIcon(QIcon(IconRecycleBin_Full));
    actionShowItem->setIcon(QIcon(IconShow));
    actionHideItem->setIcon(QIcon(IconHide));
    actionSettings->setIcon(QIcon(IconSettingsNote));

    connect(actionDeleteItem, SIGNAL(triggered()),
            this, SLOT(clickActionDelete()));
    connect(actionShowItem, SIGNAL(triggered()),
            this, SLOT(clickActionShow()));
    connect(actionHideItem, SIGNAL(triggered()),
            this, SLOT(clickActionHide()));
    connect(actionSettings, SIGNAL(triggered()),
            this, SIGNAL(clickActionSettingsItem()));

    connect(this, SIGNAL(cellPressed(int,int)),
            this, SLOT(signalCellPressed(int,int)));

    actionVisibleNote = new QAction(this);
    actionDateCreateNote = new QAction(this);
    actionCountTextSymbols = new QAction(this);
    actionCountTextLine = new QAction(this);
    actionDateLastChange = new QAction(this);
    actionLock = new QAction(this);
    actionDateOfLastRemoval = new QAction(this);
    actionDateOfLastRestore = new QAction(this);

    contextMenuHorizontalHeader = new QMenu(this);
    contextMenuHorizontalHeader->addAction(actionVisibleNote);
    contextMenuHorizontalHeader->addAction(actionLock);
    contextMenuHorizontalHeader->addAction(actionDateLastChange);
    contextMenuHorizontalHeader->addAction(actionDateCreateNote);
    contextMenuHorizontalHeader->addAction(actionDateOfLastRemoval);
    contextMenuHorizontalHeader->addAction(actionDateOfLastRestore);
    contextMenuHorizontalHeader->addAction(actionCountTextSymbols);
    contextMenuHorizontalHeader->addAction(actionCountTextLine);

    QList <QAction *> listAction = contextMenuHorizontalHeader->actions();
    for(int i = 0; i < listAction.size(); i++)
        listAction.at(i)->setCheckable(true);
}

/// PRIVATE SLOTS
void UTableNotes::signalCellPressed(int row, int col)
{
    QTableWidgetItem *itemTable = item(row, indexColumnVisible);

    bool visible = itemTable->data(pointerOnDataColumnVisible).toBool();

    actionHideItem->setVisible(visible);
    actionShowItem->setVisible(!visible);

    itemTable = item(row, indexMainColumn);

    Note *note = itemTable->data(pointerOnDataItemNote).value<Note*>();
    actionSettings->setVisible(!note->dataNote.protectionActive);

    visibleContextMenu = true;
}

void UTableNotes::clickActionShow()
{
    QTableWidgetItem *curItem = item(currentRow(), indexColumnVisible);
    curItem->setData(pointerOnDataColumnVisible, true);
    curItem->setIcon(QIcon(IconShow));
    emit clickActionShowItem();
}

void UTableNotes::clickActionHide()
{
    QTableWidgetItem *curItem = item(currentRow(), indexColumnVisible);
    curItem->setData(pointerOnDataColumnVisible, false);
    curItem->setIcon(QIcon(IconHide));
    emit clickActionHideItem();
}

void UTableNotes::resizeColumn(int logicalIndex, int oldSize, int newSize)
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                       QSettings::IniFormat);

    ini.beginGroup("TableNotes");

    switch(logicalIndex)
    {
    case indexColumnTitle:
        if(columnWidth(logicalIndex))
            ini.setValue("WidthColumnTitle",
                              newSize);
        break;

    case indexColumnVisible:
        if(columnWidth(logicalIndex))
            ini.setValue("WidthColumnVisible",
                              newSize);
        break;

    case indexColumnLock:
        if(columnWidth(logicalIndex))
            ini.setValue("WidthColumnLock",
                              newSize);
        break;

    case indexColumnCountTextSymbols:
        if(columnWidth(logicalIndex))
            ini.setValue("WidthColumnCountTextSymbols",
                              newSize);
        break;

    case indexColumnCountTextLines:
        if(columnWidth(logicalIndex))
            ini.setValue("WidthColumnCountTextLine",
                              newSize);
        break;

    case indexColumnDateOfCreating:
        if(columnWidth(logicalIndex))
            ini.setValue("WidthColumnDateOfCreating",
                              newSize);
        break;

    case indexColumnDateLastChange:
        if(columnWidth(logicalIndex))
            ini.setValue("WidthColumnDateLastChange",
                              newSize);
        break;

    case indexColumnDateOfLastRemoval:
        if(columnWidth(logicalIndex))
            ini.setValue("WidthColumnDateOfLastRemoval",
                              newSize);
        break;

    case indexColumnDateOfLastRestore:
        if(columnWidth(logicalIndex))
            ini.setValue("WidthColumnDateOfLastRestore",
                              newSize);
        break;
    };

    ini.endGroup();
    ini.sync();
}

/// PUBLIC SLOTS
void UTableNotes::setVisibleColumnTable(int indexColumn, bool visible)
{
    setColumnHidden(indexColumn, !visible);
    listVisibleColumn.replace(indexColumn, visible);
}

void UTableNotes::setVisibleColumnTable(QList <bool> &list)
{
    listVisibleColumn = list;

    for(int i = 0; i < listVisibleColumn.size(); i++)
    {
        bool visible = listVisibleColumn.at(i);
        setColumnHidden(i, !visible);
    }
}

void UTableNotes::setLanguage(QString pathToTheFileLanguage)
{
    emit languageIsChange(pathToTheFileLanguage);

    QSettings langFile(pathToTheFileLanguage,
                       QSettings::IniFormat);
    langFile.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    langFile.beginGroup("TABLENOTES");

    setWindowTitle(langFile.value("Title").toString());

    actionDeleteItem->setText(langFile.value("MoveToRecycleBin").toString());
    actionShowItem->setText(langFile.value("Show").toString());
    actionHideItem->setText(langFile.value("Hide").toString());
    actionSettings->setText(langFile.value("Settings").toString());

    QStringList lst;

    for(int i = 0; i < countColumns; i++)
        lst.append("");

    lst.replace(indexColumnTitle,
                langFile.value("ColumnTitle").toString());

    lst.replace(indexColumnVisible,
                langFile.value("ColumnVisible").toString());

    lst.replace(indexColumnDateOfCreating,
                langFile.value("ColumnDateOfCreating").toString());

    lst.replace(indexColumnCountTextSymbols,
                langFile.value("ColumnCountTextSymbols").toString());

    lst.replace(indexColumnCountTextLines,
                langFile.value("ColumnCountTextLines").toString());

    lst.replace(indexColumnDateLastChange,
                langFile.value("ColumnDateLastChange").toString());

    lst.replace(indexColumnLock,
                langFile.value("ColumnLock").toString());

    lst.replace(indexColumnDateOfLastRemoval,
                langFile.value("ColumnDateOfLastRemoval").toString());

    lst.replace(indexColumnDateOfLastRestore,
                langFile.value("ColumnDateOfLastRestore").toString());

    langFile.endGroup();

    actionVisibleNote->setText(lst.at(indexColumnVisible));
    actionDateCreateNote->setText(lst.at(indexColumnDateOfCreating));
    actionCountTextSymbols->setText(lst.at(indexColumnCountTextSymbols));
    actionCountTextLine->setText(lst.at(indexColumnCountTextLines));
    actionDateLastChange->setText(lst.at(indexColumnDateLastChange));
    actionLock->setText(lst.at(indexColumnLock));
    actionDateOfLastRemoval->setText(lst.at(indexColumnDateOfLastRemoval));
    actionDateOfLastRestore->setText(lst.at(indexColumnDateOfLastRestore));

    setColumnCount(countColumns);
    setHorizontalHeaderLabels(lst);

    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);

    int magicNumber = 9934343245;
    // если записи нет
    if(ini.value("TableNotes/WidthColumnTitle", magicNumber).toInt()
            == magicNumber)
        // ширина колонок подбираетс€ по текст в них
        resizeColumnsToContents();
    else
        // установить пользовательскую ширину столбцов
        loadWidthColumns();
}

void UTableNotes::showContextMenuHorizontalHeader(QPoint p)
{
    if(bShowContextMenuHorizontalHeader)
    {
        QList <QAction *> list = contextMenuHorizontalHeader->actions();
        for(int i = 0; i < list.count(); i++)
        {
            QAction *action = list.at(i);
            action->setChecked(!isColumnHidden(i + 1));
        }

        contextMenuHorizontalHeader->exec(mapToGlobal(p));
    }
}

void UTableNotes::showContextMenu(QPoint p)
{
    if(visibleContextMenu)
    {
        QMenu menu(this);
        menu.addAction(actionDeleteItem);
        menu.addAction(actionShowItem);
        menu.addAction(actionHideItem);
        menu.addAction(actionSettings);

        p.setX(p.x() + 18);
        p.setY(p.y() + 22);
        menu.exec(mapToGlobal(p));

        visibleContextMenu = false;
    }
}

void UTableNotes::loadWidthColumns()
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("TableNotes");

    setColumnWidth(indexColumnTitle,   ini.value("WidthColumnTitle").toInt());
    setColumnWidth(indexColumnVisible, ini.value("WidthColumnVisible").toInt());
    setColumnWidth(indexColumnLock,    ini.value("WidthColumnLock").toInt());

    setColumnWidth(indexColumnCountTextSymbols,  ini.value("WidthColumnCountTextSymbols").toInt());
    setColumnWidth(indexColumnCountTextLines,    ini.value("WidthColumnCountTextLine").toInt());

    setColumnWidth(indexColumnDateOfCreating,    ini.value("WidthColumnDateOfCreating").toInt());
    setColumnWidth(indexColumnDateLastChange,    ini.value("WidthColumnDateLastChange").toInt());
    setColumnWidth(indexColumnDateOfLastRemoval, ini.value("WidthColumnDateOfLastRemoval").toInt());
    setColumnWidth(indexColumnDateOfLastRestore, ini.value("WidthColumnDateOfLastRestore").toInt());

    ini.endGroup();
}

/// TABLE RECYCLE BIN
UTableRecycleBin::UTableRecycleBin(QWidget *parent):
    QTableWidget(parent),
    visibleContextMenu(false),
    bShowContextMenuHorizontalHeader(false)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenu(QPoint)));
    connect(this, SIGNAL(cellPressed(int,int)),
            this, SLOT(signalCellPressed(int,int)));

    horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenuHorizontalHeader(QPoint)));

    connect(horizontalHeader(), SIGNAL(sectionResized(int,int,int)),
            this, SLOT(resizeColumn(int,int,int)));

    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    setAlternatingRowColors(true);

    setItemDelegateForColumn(indexColumnTitle,             new ReadDelegate());
    setItemDelegateForColumn(indexColumnVisible,           new ReadDelegate());
    setItemDelegateForColumn(indexColumnDateOfCreating,    new ReadDelegate());
    setItemDelegateForColumn(indexColumnCountTextSymbols,  new ReadDelegate());
    setItemDelegateForColumn(indexColumnCountTextLines,    new ReadDelegate());
    setItemDelegateForColumn(indexColumnDateLastChange,    new ReadDelegate());
    setItemDelegateForColumn(indexColumnLock,              new ReadDelegate());
    setItemDelegateForColumn(indexColumnDateOfLastRemoval, new ReadDelegate());
    setItemDelegateForColumn(indexColumnDateOfLastRestore, new ReadDelegate());

    setRowCount(0);

    actionDeleteItem  = new QAction(this);
    actionRestoreItem = new QAction(this);

    actionDeleteItem->setIcon(QIcon(IconDelete));
    actionRestoreItem->setIcon(QIcon(IconRestore));

    connect(actionDeleteItem, SIGNAL(triggered()),
            this, SLOT(clickActionDelete()));
    connect(actionRestoreItem, SIGNAL(triggered()),
            this, SLOT(clickActionRestore()));


    actionVisibleNote = new QAction(this);
    actionDateCreateNote = new QAction(this);
    actionCountTextSymbols = new QAction(this);
    actionCountTextLine = new QAction(this);
    actionDateLastChange = new QAction(this);
    actionLock = new QAction(this);
    actionDateOfLastRemoval = new QAction(this);
    actionDateOfLastRestore = new QAction(this);

    contextMenuHorizontalHeader = new QMenu(this);
    contextMenuHorizontalHeader->addAction(actionVisibleNote);
    contextMenuHorizontalHeader->addAction(actionLock);
    contextMenuHorizontalHeader->addAction(actionDateLastChange);
    contextMenuHorizontalHeader->addAction(actionDateCreateNote);
    contextMenuHorizontalHeader->addAction(actionDateOfLastRemoval);
    contextMenuHorizontalHeader->addAction(actionDateOfLastRestore);
    contextMenuHorizontalHeader->addAction(actionCountTextSymbols);
    contextMenuHorizontalHeader->addAction(actionCountTextLine);

    QList <QAction *> listAction = contextMenuHorizontalHeader->actions();
    for(int i = 0; i < listAction.size(); i++)
        listAction.at(i)->setCheckable(true);
}

/// PRIVATE SLOTS
void UTableRecycleBin::resizeColumn(int logicalIndex, int oldSize, int newSize)
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("TableRecycleBin");

    switch(logicalIndex)
    {
    case indexColumnTitle:
        if(columnWidth(logicalIndex))
            ini.setValue("WidthColumnTitle",
                         newSize);
        break;

    case indexColumnVisible:
        if(columnWidth(logicalIndex))
            ini.setValue("WidthColumnVisible",
                         newSize);
        break;

    case indexColumnLock:
        if(columnWidth(logicalIndex))
            ini.setValue("WidthColumnLock",
                         newSize);
        break;

    case indexColumnCountTextSymbols:
        if(columnWidth(logicalIndex))
            ini.setValue("WidthColumnCountTextSymbols",
                         newSize);
        break;

    case indexColumnCountTextLines:
        if(columnWidth(logicalIndex))
            ini.setValue("WidthColumnCountTextLine",
                         newSize);
        break;

    case indexColumnDateOfCreating:
        if(columnWidth(logicalIndex))
            ini.setValue("WidthColumnDateOfCreating",
                         newSize);
        break;

    case indexColumnDateLastChange:
        if(columnWidth(logicalIndex))
            ini.setValue("WidthColumnDateLastChange",
                         newSize);
        break;

    case indexColumnDateOfLastRemoval:
        if(columnWidth(logicalIndex))
            ini.setValue("WidthColumnDateOfLastRemoval",
                         newSize);
        break;

    case indexColumnDateOfLastRestore:
        if(columnWidth(logicalIndex))
            ini.setValue("WidthColumnDateOfLastRestore",
                         newSize);
        break;
    };

    ini.endGroup();

    ini.sync();
}

/// PUBLIC SLOTS
void UTableRecycleBin::setVisibleColumnTable(int indexColumn, bool visible)
{
    setColumnHidden(indexColumn, !visible);
    listVisibleColumn.replace(indexColumn, visible);
}

void UTableRecycleBin::setVisibleColumnTable(QList <bool> &list)
{
    listVisibleColumn = list;

    for(int i = 0; i < listVisibleColumn.size(); i++)
    {
        bool visible = listVisibleColumn.at(i);
        setColumnHidden(i, !visible);
    }
}

void UTableRecycleBin::setLanguage(QString pathToTheFileLanguage)
{
    emit languageIsChange(pathToTheFileLanguage);

    QSettings langFile(pathToTheFileLanguage,
                       QSettings::IniFormat);
    langFile.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    langFile.beginGroup("TABLERECYCLEBIN");

    setWindowTitle(langFile.value("Title").toString());

    actionDeleteItem->setText(langFile.value("Delete").toString());
    actionRestoreItem->setText(langFile.value("Restore").toString());

    QStringList lst;

    for(int i = 0; i < countColumns; i++)
        lst.append("");

    lst.replace(indexColumnTitle,
                langFile.value("ColumnTitle").toString());

    lst.replace(indexColumnVisible,
                langFile.value("ColumnVisible").toString());

    lst.replace(indexColumnDateOfCreating,
                langFile.value("ColumnDateOfCreating").toString());

    lst.replace(indexColumnCountTextSymbols,
                langFile.value("ColumnCountTextSymbols").toString());

    lst.replace(indexColumnCountTextLines,
                langFile.value("ColumnCountTextLines").toString());

    lst.replace(indexColumnDateLastChange,
                langFile.value("ColumnDateLastChange").toString());

    lst.replace(indexColumnLock,
                langFile.value("ColumnLock").toString());

    lst.replace(indexColumnDateOfLastRemoval,
                langFile.value("ColumnDateOfLastRemoval").toString());

    lst.replace(indexColumnDateOfLastRestore,
                langFile.value("ColumnDateOfLastRestore").toString());

    langFile.endGroup();

    actionVisibleNote->setText(lst.at(indexColumnVisible));
    actionDateCreateNote->setText(lst.at(indexColumnDateOfCreating));
    actionCountTextSymbols->setText(lst.at(indexColumnCountTextSymbols));
    actionCountTextLine->setText(lst.at(indexColumnCountTextLines));
    actionDateLastChange->setText(lst.at(indexColumnDateLastChange));
    actionLock->setText(lst.at(indexColumnLock));
    actionDateOfLastRemoval->setText(lst.at(indexColumnDateOfLastRemoval));
    actionDateOfLastRestore->setText(lst.at(indexColumnDateOfLastRestore));

    setColumnCount(countColumns);
    setHorizontalHeaderLabels(lst);

    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    int magicNumber = 9934343245;
    // если записи нет
    if(ini.value("TableRecycleBin/WidthColumnTitle", magicNumber).toInt()
            == magicNumber)
        // ширина колонок подбираетс€ по текст в них
        resizeColumnsToContents();
    else
        // установить пользовательскую ширину столбцов
        loadWidthColumns();
}

void UTableRecycleBin::showContextMenuHorizontalHeader(QPoint p)
{
    if(bShowContextMenuHorizontalHeader)
    {
        QList <QAction *> list = contextMenuHorizontalHeader->actions();
        for(int i = 0; i < list.count(); i++)
        {
            QAction *action = list.at(i);
            action->setChecked(!isColumnHidden(i + 1));
        }
        contextMenuHorizontalHeader->exec(mapToGlobal(p));
    }
}

void UTableRecycleBin::showContextMenu(QPoint p)
{
    if(visibleContextMenu)
    {
        QMenu menu(this);
        menu.addAction(actionDeleteItem);
        menu.addAction(actionRestoreItem);

        p.setX(p.x() + 18);
        p.setY(p.y() + 22);
        menu.exec(this->mapToGlobal(p));

        visibleContextMenu = false;
    }
}

void UTableRecycleBin::loadWidthColumns()
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("TableRecycleBin");

    setColumnWidth(indexColumnTitle,             ini.value("WidthColumnTitle").toInt());
    setColumnWidth(indexColumnVisible,           ini.value("WidthColumnVisible").toInt());
    setColumnWidth(indexColumnLock,              ini.value("WidthColumnLock").toInt());

    setColumnWidth(indexColumnCountTextSymbols,  ini.value("WidthColumnCountTextSymbols").toInt());
    setColumnWidth(indexColumnCountTextLines,    ini.value("WidthColumnCountTextLine").toInt());

    setColumnWidth(indexColumnDateOfCreating,    ini.value("WidthColumnDateOfCreating").toInt());
    setColumnWidth(indexColumnDateLastChange,    ini.value("WidthColumnDateLastChange").toInt());
    setColumnWidth(indexColumnDateOfLastRemoval, ini.value("WidthColumnDateOfLastRemoval").toInt());
    setColumnWidth(indexColumnDateOfLastRestore, ini.value("WidthColumnDateOfLastRestore").toInt());

    ini.endGroup();
}

/// UAreaResultsTable
UAreaResultsTable::UAreaResultsTable(QWidget *parent):
    QScrollArea(parent)
{
    setWidgetResizable(true);

    mainWidget = new QWidget();
    vBoxLayoutMain = new QVBoxLayout();

    tableNotes = new UTableNotes();
    tableRecycleBin = new UTableRecycleBin();

    // убираем массовое выделение строк
    tableNotes->setSelectionMode(QAbstractItemView::SingleSelection);
    tableRecycleBin->setSelectionMode(QAbstractItemView::SingleSelection);

    groupBoxTableNotes = new QGroupBox();
    groupBoxTableRecycleBin = new QGroupBox();

    groupBoxTableNotes->setCheckable(true);
    groupBoxTableNotes->setChecked(true);
    connect(groupBoxTableNotes, SIGNAL(toggled(bool)),
            this, SLOT(setVisibleContentGroupBoxTableNotes(bool)));

    groupBoxTableRecycleBin->setCheckable(true);
    groupBoxTableRecycleBin->setChecked(true);
    connect(groupBoxTableRecycleBin, SIGNAL(toggled(bool)),
            this, SLOT(setVisibleContentGroupBoxTableRecycleBin(bool)));


    pButtonUnselectTableNotesPage = new QPushButton();
    pButtonSelectAllTableNotesPage = new QPushButton();

    connect(pButtonUnselectTableNotesPage, SIGNAL(clicked()),
            this, SLOT(unselectTableNotesPage()));
    connect(pButtonSelectAllTableNotesPage, SIGNAL(clicked()),
            this, SLOT(selectAllTableNotesPage()));

    QHBoxLayout *hBoxLayoutButtonsTableNotesPage = new QHBoxLayout();
    hBoxLayoutButtonsTableNotesPage->addWidget(pButtonSelectAllTableNotesPage);
    hBoxLayoutButtonsTableNotesPage->addWidget(pButtonUnselectTableNotesPage);
    hBoxLayoutButtonsTableNotesPage->addStretch();


    QVBoxLayout *vBoxLayoutGroupBoxTableNotes = new QVBoxLayout();
    vBoxLayoutGroupBoxTableNotes->addLayout(hBoxLayoutButtonsTableNotesPage);
    vBoxLayoutGroupBoxTableNotes->addWidget(tableNotes);
    groupBoxTableNotes->setLayout(vBoxLayoutGroupBoxTableNotes);


    pButtonUnselectTableRecycleBinPage = new QPushButton();
    pButtonSelectAllTableRecycleBinPage = new QPushButton();

    connect(pButtonUnselectTableRecycleBinPage, SIGNAL(clicked()),
            this, SLOT(unselectTableRecycleBinPage()));
    connect(pButtonSelectAllTableRecycleBinPage, SIGNAL(clicked()),
            this, SLOT(selectAllTableRecycleBinPage()));

    QHBoxLayout *hBoxLayoutButtonsTableRecycleBin = new QHBoxLayout();
    hBoxLayoutButtonsTableRecycleBin->addWidget(pButtonSelectAllTableRecycleBinPage);
    hBoxLayoutButtonsTableRecycleBin->addWidget(pButtonUnselectTableRecycleBinPage);
    hBoxLayoutButtonsTableRecycleBin->addStretch();

    QVBoxLayout *vBoxLayoutGroupBoxTableRecycleBin = new QVBoxLayout();
    vBoxLayoutGroupBoxTableRecycleBin->addLayout(hBoxLayoutButtonsTableRecycleBin);
    vBoxLayoutGroupBoxTableRecycleBin->addWidget(tableRecycleBin);
    groupBoxTableRecycleBin->setLayout(vBoxLayoutGroupBoxTableRecycleBin);

    vBoxLayoutMain->addWidget(groupBoxTableNotes);
    vBoxLayoutMain->addWidget(groupBoxTableRecycleBin);
    vBoxLayoutMain->addStretch();

    mainWidget->setLayout(vBoxLayoutMain);
    setWidget(mainWidget);


    //
    tableNotes->setItemDelegateForColumn(indexColumnTitle,
                                         new ReadDelegate());
    tableNotes->setItemDelegateForColumn(indexColumnVisible,
                                         new ReadDelegate());
    tableNotes->setItemDelegateForColumn(indexColumnDateOfCreating,
                                         new ReadDelegate());
    tableNotes->setItemDelegateForColumn(indexColumnCountTextSymbols,
                                         new ReadDelegate());
    tableNotes->setItemDelegateForColumn(indexColumnCountTextLines,
                                         new ReadDelegate());
    tableNotes->setItemDelegateForColumn(indexColumnDateLastChange,
                                         new ReadDelegate());
    tableNotes->setItemDelegateForColumn(indexColumnLock,
                                         new ReadDelegate());
    tableNotes->setItemDelegateForColumn(indexColumnDateOfLastRemoval,
                                         new ReadDelegate());
    tableNotes->setItemDelegateForColumn(indexColumnDateOfLastRestore,
                                         new ReadDelegate());


    connect(tableNotes, SIGNAL(clickActionDeleteItem()),
            this, SLOT(converterMoveNoteToRecycleBin()));
    connect(tableNotes, SIGNAL(clickActionSettingsItem()),
            this, SLOT(converterShowSettingsNote()));
    connect(tableNotes, SIGNAL(clickActionShowItem()),
            this, SLOT(converterShowNote()));
    connect(tableNotes, SIGNAL(clickActionHideItem()),
            this, SLOT(converterHideNote()));

    connect(tableRecycleBin, SIGNAL(clickActionRestoreItem(int)),
            this, SLOT(converterRestoreNote(int)));
    connect(tableRecycleBin, SIGNAL(clickActionDeleteItem(int)),
            this, SLOT(converterDeleteNote(int)));
}

/// PUBLIC SLOTS
void UAreaResultsTable::setLanguage(QString pathToTheFileLanguage)
{
    emit languageIsChange(pathToTheFileLanguage);

    tableNotes->setLanguage(pathToTheFileLanguage);
    tableRecycleBin->setLanguage(pathToTheFileLanguage);

    QSettings ini(pathToTheFileLanguage,
                       QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("NOTEMANAGER");

    pButtonUnselectTableNotesPage->setText(ini.value("Unselect").toString());
    pButtonSelectAllTableNotesPage->setText(ini.value("SelectAll").toString());

    pButtonUnselectTableRecycleBinPage->setText(ini.value("Unselect").toString());
    pButtonSelectAllTableRecycleBinPage->setText(ini.value("SelectAll").toString());

    ini.endGroup();

    refreshCountsNotesInTables();
}

void UAreaResultsTable::refreshCountsNotesInTables()
{
    // ƒл€ отображени€ заголовка groupBox на странице –езультата поиска
    groupBoxTableNotes->setTitle(QString("%1 (%2)")
                                 .arg(tableNotes->windowTitle())
                                 .arg(tableNotes->rowCount()));

    groupBoxTableRecycleBin->setTitle(QString("%1 (%2)")
                                      .arg(tableRecycleBin->windowTitle())
                                      .arg(tableRecycleBin->rowCount()));
    // если записей 0
    if(!tableNotes->rowCount())
    {
        // убираем флажок у groupbox,
        // а его содержимое пр€чем
        groupBoxTableNotes->setChecked(false);
        tableNotes->setVisible(false);
    }else
    {
        groupBoxTableNotes->setChecked(true);
        tableNotes->setVisible(true);
    }

    if(!tableRecycleBin->rowCount())
    {
        groupBoxTableRecycleBin->setChecked(false);
        tableRecycleBin->setVisible(false);
    }else
    {
        groupBoxTableRecycleBin->setChecked(true);
        tableRecycleBin->setVisible(true);
    }
}

/// PRIVATE SLOTS
void UAreaResultsTable::converterMoveNoteToRecycleBin()
{
    QTableWidgetItem *currentItem = tableNotes->item(tableNotes->currentRow(),
                                                     indexMainColumn);
    if(currentItem)
    {
        Note *note = currentItem->data(pointerOnDataItemNote).value<Note*>();
        emit moveNoteToRecycleBin(note);
        emit refreshTables();
    }
}
void UAreaResultsTable::converterShowSettingsNote()
{
    QTableWidgetItem *currentItem = tableNotes->item(tableNotes->currentRow(),
                                                     indexMainColumn);
    if(currentItem)
    {
        Note *note = currentItem->data(pointerOnDataItemNote).value<Note*>();
        emit showSettingsNote(note);
        emit refreshTables();
    }
}
void UAreaResultsTable::converterShowNote()
{
    QTableWidgetItem *currentItem = tableNotes->item(tableNotes->currentRow(),
                                                     indexMainColumn);
    if(currentItem)
    {
        Note *note = currentItem->data(pointerOnDataItemNote).value<Note*>();
        emit showNote(note);
        emit refreshTables();
    }
}
void UAreaResultsTable::converterHideNote()
{
    QTableWidgetItem *currentItem = tableNotes->item(tableNotes->currentRow(),
                                                     indexMainColumn);
    if(currentItem)
    {
        Note *note = currentItem->data(pointerOnDataItemNote).value<Note*>();
        emit hideNote(note);
        emit refreshTables();
    }
}

void UAreaResultsTable::converterRestoreNote(int row)
{
    QTableWidgetItem *item = tableRecycleBin->item(row, indexMainColumn);
    if(item)
    {
        QString nameFile = item->data(pointerOnDataItemNote).value<QString>();
        emit restoreNote(nameFile);
        emit refreshTables();
    }
}
void UAreaResultsTable::converterDeleteNote(int row)
{
    QTableWidgetItem *item = tableRecycleBin->item(row, indexMainColumn);
    if(item)
    {
        QString nameFile = item->data(pointerOnDataItemNote).value<QString>();
        emit deleteNote(nameFile);
        emit refreshTables();
    }
}

void UAreaResultsTable::unselectTableNotesPage()
{
    for (int i = 0; i < tableNotes->rowCount(); i++)
    {
        QTableWidgetItem *item = tableNotes->item(i, indexMainColumn);
        item->setCheckState(Qt::Unchecked);
    }
}
void UAreaResultsTable::selectAllTableNotesPage()
{
    for (int i = 0; i < tableNotes->rowCount(); i++)
    {
        QTableWidgetItem *item = tableNotes->item(i, indexMainColumn);
        item->setCheckState(Qt::Checked);
    }
}

void UAreaResultsTable::unselectTableRecycleBinPage()
{
    for (int i = 0; i < tableRecycleBin->rowCount(); i++)
    {
        QTableWidgetItem *item = tableRecycleBin->item(i, indexMainColumn);
        item->setCheckState(Qt::Unchecked);
    }
}
void UAreaResultsTable::selectAllTableRecycleBinPage()
{
    for (int i = 0; i < tableRecycleBin->rowCount(); i++)
    {
        QTableWidgetItem *item = tableRecycleBin->item(i, indexMainColumn);
        item->setCheckState(Qt::Checked);
    }
}

void UAreaResultsTable::setVisibleContentGroupBoxTableNotes(bool visible)
{
    tableNotes->setVisible(visible);
    pButtonUnselectTableNotesPage->setVisible(visible);
    pButtonSelectAllTableNotesPage->setVisible(visible);
}
void UAreaResultsTable::setVisibleContentGroupBoxTableRecycleBin(bool visible)
{
    tableRecycleBin->setVisible(visible);
    pButtonUnselectTableRecycleBinPage->setVisible(visible);
    pButtonSelectAllTableRecycleBinPage->setVisible(visible);
}
