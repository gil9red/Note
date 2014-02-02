#include "ManagerNote.h"

void qSort(QList <int> *list, int minIndex, int maxIndex)
{
    // Алгоритм взят с википедии

    int i = minIndex;
    int j = maxIndex;

    // x - опорный элемент посредине между low и high
    int x = list->at((minIndex + maxIndex) / 2);
    do
    {
        while(list->at(i) < x)
            ++i;  // поиск элемента для переноса в старшую часть

        while(list->at(j) > x)
            --j;  // поиск элемента для переноса в младшую часть

        if(i <= j)
        {
            // обмен элементов местами:
            list->swap(i, j);
            // переход к следующим элементам:
            i++;
            j--;
        }

    }while(i < j);

    if(minIndex < j)
        qSort(list, minIndex, j);

    if(i < maxIndex)
        qSort(list, i, maxIndex);
}

void qSort(QList <int> *list)
{
    int minIndex = 0;
    int maxIndex = list->count() - 1;

    qSort(list, minIndex, maxIndex);
}

void removeDuplicateItems(QList <int> *list)
{
    qSort(list);

    QList <int> tempList;

    tempList.append(list->at(0));

    for(int i = 1; i < list->count() - 1; i++)
    {
        int current = list->at(i);
        int next = list->at(i + 1);

        if(current != next)
            tempList.append(next);
    }

    list->clear();

    for(int i = 0; i < tempList.count(); i++)
        list->append(tempList.at(i));
}

QList <int> getListIndexSelectedRow(QTableWidget *table)
{
    QList <int> list;

    QList <QTableWidgetItem *> selectedList = table->selectedItems();

    if(!selectedList.isEmpty())
    {
        for(int i = 0; i < selectedList.count(); i++)
            list.append(selectedList.at(i)->row());

        removeDuplicateItems(&list);
    }

    return list;
}

void moveTheFile(QString oldNameFile, QString pathInDir)
{
    QString newFileName = pathInDir
            + QDir::separator()
            + QFileInfo(oldNameFile).fileName();

    QFile::copy(oldNameFile,
                newFileName);

    QFile::remove(oldNameFile);
}


/// PUBLIC
ManagerNote::ManagerNote(QWidget *parent):
    QMainWindow(parent){}

void ManagerNote::startThePreparatoryPhase(int phaseNumber)
{
    switch(phaseNumber)
    {
    case 1:
        checkOrCreateFolderForUseBy();
        break;

    case 2:
        preparationOfSecondaryWindows();
        break;

    case 3:
        createFormManager();
        break;

    case 4:
        initializationManager();
        break;

    case 5:
        createIconTray();
        break;

    case 6:
        loadLanguage();
        break;

    case 7:
        addingNotesToTables();
        break;

    case 8:
        finalPreparations();
        break;
    }
}

void ManagerNote::createFormManager()
{
    mainWidget = new UNoteManager();

    USettingsManager *settingsManager = mainWidget->settingsManager;

    connect(settingsManager->tabGeneralSettings, SIGNAL(languageIsChange(QString)),
            this, SLOT(setLanguage(QString)));
    connect(this, SIGNAL(languageIsChange(QString)),
            mainWidget, SLOT(setLanguage(QString)));

    // hotkey
    connect(settingsManager, SIGNAL(changeHotkeys()),
            this, SLOT(acceptHotkeys()));

    connect(settingsManager, SIGNAL(activateHotkeys(bool)),
            this, SLOT(setActivateHotkeys(bool)));

    connect(this, SIGNAL(activateHotkeys(bool)),
            settingsManager, SLOT(setActivateHotkeys(bool)));
    // hotkey

    setCentralWidget(mainWidget);

    addMenu();
    addCommandButtons();
}

void ManagerNote::preparationOfSecondaryWindows()
{
    settingsProgram = 0;
    aboutProgram = 0;
    findNote = 0;
}

void ManagerNote::createIconTray()
{
    systemTrayIcon = new QSystemTrayIcon();
    systemTrayIcon->setIcon(QIcon(IconLogo36x36));

    addMenuSystemTrayIcon(systemTrayIcon);

    connect(systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(showHide(QSystemTrayIcon::ActivationReason)));

    // показываем значок программы в трее
    systemTrayIcon->show();
}

void ManagerNote::checkOrCreateFolderForUseBy()
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    currentLanguage = ini.value("Language").toString();

    QDir dir;
    dir.setPath(absolutePathToTheFolderLanguages());
    dir.setFilter(QDir::Files);

    // если папка с языками пуста
    if(dir.entryInfoList().isEmpty())
    {
        QFile::copy(strDefaultLanguageFile,
                    absolutePathToTheFolderLanguages()
                    + QDir::separator()
                    + fileNameRussianLanguage);

        currentLanguage = fileNameRussianLanguage;
    }

    // если нет файла языка, сохраненного в последний раз
    // как текущий язык
    if(!QFileInfo(absolutePathToTheFolderLanguages()
                  + QDir::separator()
                  + currentLanguage).isFile())
    {
        // то новым текущим будет первый из списка языков
        QDir dir = QDir(absolutePathToTheFolderLanguages());

        dir.setFilter(QDir::Files);
        dir.setSorting(QDir::Name);

        QStringList filters;
        filters << QString("*." + suffixLanguageFiles);
        dir.setNameFilters(filters);

        QStringList list = dir.entryList();

        currentLanguage = list.first();
    }
}

void ManagerNote::initializationManager()
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    showManagerAtStartProgram = ini.value("ShowManagerAtStartup", true).toBool();

    ini.beginGroup("Manager");

    minimizeToTrayManagerAtClosing = ini.value("MinimizeToTrayManagerAtClosing",
                                               isMinimizeToTrayManagerAtClosing).toBool();

    int positionX      = ini.value("PositionX", 100).toInt();
    int positionY      = ini.value("PositionY", 100).toInt();
    int width          = ini.value("Width", 600).toInt();
    int height         = ini.value("Height", 450).toInt();
    managerIsMaximized = ini.value("Maximized", false).toBool();

    ini.endGroup();

    setGeometry(positionX, positionY,
                width, height);

    if(managerIsMaximized)
        setWindowState(Qt::WindowMaximized);
}

void ManagerNote::addingNotesToTables()
{
    completer = new QCompleter(this);
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setWrapAround(false);

    loadInTableWidgetNotesNote();
    loadInTableWidgetRecycleBinNote();

    QTableWidget *tableNotes = mainWidget->tableWidgetNotes;
    QTableWidget *tableRecycleBin = mainWidget->tableWidgetRecycleBin;

    connect(tableNotes, SIGNAL(cellDoubleClicked(int,int)),
            this, SLOT(changeTrueFalseValueCells(int,int)));

    connect(tableNotes, SIGNAL(cellChanged(int,int)),
            this, SLOT(tableCellChange(int,int)));
    connect(tableNotes, SIGNAL(clickActionDeleteItem()),
            this, SLOT(clickButtonMoveThisNoteToRecycleBin()));
    connect(tableNotes, SIGNAL(clickActionSettingsItem()),
            this, SLOT(clickButtonSettingsNote()));
    connect(tableNotes, SIGNAL(clickActionShowItem()),
            this, SLOT(clickButtonShowThisNote()));
    connect(tableNotes, SIGNAL(clickActionHideItem()),
            this, SLOT(clickButtonHideThisNote()));

    connect(tableRecycleBin, SIGNAL(clickActionRestoreItem(int)),
            this, SLOT(clickRestoreThisItem(int)));
    connect(tableRecycleBin, SIGNAL(clickActionDeleteItem(int)),
            this, SLOT(clickDeleteThisItem(int)));

    //
    connect(mainWidget->areaResultsTable, SIGNAL(moveNoteToRecycleBin(Note*)),
            this, SLOT(clickButtonMoveThisNoteToRecycleBin(Note*)));
    connect(mainWidget->areaResultsTable, SIGNAL(showSettingsNote(Note*)),
            this, SLOT(clickButtonSettingsNote(Note*)));
    connect(mainWidget->areaResultsTable, SIGNAL(showNote(Note*)),
            this, SLOT(clickButtonShowThisNote(Note*)));
    connect(mainWidget->areaResultsTable, SIGNAL(hideNote(Note*)),
            this, SLOT(clickButtonHideThisNote(Note*)));

    connect(mainWidget->areaResultsTable, SIGNAL(restoreNote(QString)),
            this, SLOT(clickRestoreThisItem(QString)));
    connect(mainWidget->areaResultsTable, SIGNAL(deleteNote(QString)),
            this, SLOT(clickDeleteThisItem(QString)));
}

void ManagerNote::loadLanguage()
{
    currentLanguage = absolutePathToTheFolderLanguages()
            + QDir::separator()
            + currentLanguage;

    USettings *settings = mainWidget->settingsManager->tabGeneralSettings;
    settings->setLanguage(currentLanguage);
}

void ManagerNote::finalPreparations()
{
    // accept event drop
    setAcceptDrops(true);

    // считываем какие столбцы в таблицах нужно показывать
    readDataInTables();

    // обновляем данные tool tip иконки трея
    refreshToolTipSystemTrayIcon();

    connect(this, SIGNAL(changedTheNumberOfNotes()),
            this, SLOT(refreshToolTipSystemTrayIcon()));
    connect(this, SIGNAL(changedTheStatusOfNotes()),
            this, SLOT(refreshToolTipSystemTrayIcon()));

    QSettings ini(absolutePathToTheConfigurationProgram(),
                      QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    QString currentNameStyle = ini.value("Style", QFileInfo(pathToDefaultStyle).baseName()).toString();

    QString pathToStyle = absolutePathToTheFolderStyles()
            + QDir::separator()
            + currentNameStyle
            + QDir::separator()
            + currentNameStyle + "." + suffixStyleFiles;

    // если нет стиля сохраненного в последний раз как текущий,
    // выбираем стандартный
    if(!QFileInfo(pathToStyle).isFile())
        pathToStyle = pathToDefaultStyle;

    setStyleSheetFromFile(pathToStyle);

    connect(mainWidget->settingsManager, SIGNAL(selectedAnotherStyle(QString)),
            this, SLOT(setStyleSheetFromFile(QString)));

    // активирование горячих клавиш
    bool activateHotkeys = ini.value("Hotkeys/Activate",
                                         false).toBool();
    setActivateHotkeys(activateHotkeys);

    // сохраняемся
    saveInIni();

    // прячем окно с панели задач
    if(showManagerAtStartProgram)
    {
        if(managerIsMaximized)
            show();
        else
            showNormal();

    }else
        systemTrayIcon->showMessage(strTitleHint,
                                    strTextMessageSystemTray,
                                    QSystemTrayIcon::Information,
                                    2000);

    // после подготовки программы к запуску,
    // показываем все заметки, у которых visible = true
    while(!listLoadVisibleNotes->isEmpty())
    {
        Note *note = listLoadVisibleNotes->takeFirst();
        note->setVisibleNote(true);
    }

    // освобождаем память под список
    delete listLoadVisibleNotes;
    listLoadVisibleNotes = 0;
}

/// PRIVATE
void ManagerNote::loadInTableWidgetNotesNote()
{
    QDir dir = QDir(absolutePathToTheFolderNotes());

    dir.setFilter(QDir::Files);
    QStringList filters;
    filters << QString("*." + suffixFilesNote);
    dir.setNameFilters(filters);
    dir.setSorting(QDir::Name);

    listLoadVisibleNotes = new QList <Note*>();

    QFileInfoList list = dir.entryInfoList();

    for(int i = 0; i < list.size(); i++)
    {
        QFileInfo fileInfo = list.at(i);

        QTableWidget *table = mainWidget->tableWidgetNotes;

        Note *newNote = new Note(fileInfo.absoluteFilePath());

        // если заметка должна быть показана,
        // добавляем ее в список, и отобразим после
        // подготовки программы к запуску
        if(newNote->dataNote.visible)
            listLoadVisibleNotes->append(newNote);

        newNote->setVisible(false);

        newNote->setLanguage(currentLanguage);
        newNote->textEdit->setCompleter(completer);
        newNote->testAutocompleterLanguage();

        QVariant itemDataNote;
        itemDataNote.setValue(newNote);

        QString temp = newNote->dataNote.title;

        QString title = newNote->dataNote.title;
        if(title.isEmpty())
            title = noTitle;

        // название noTitle будет относиться для
        // отображения в таблице
        newNote->dataNote.title = title;

        addToTheEndTablesRow(table,
                             itemDataNote,
                             &newNote->dataNote);

        // а если заголовок пуст, то и пуст он будет
        // и на самой заметке
        newNote->dataNote.title = temp;

        setAllConnectForNote(newNote);

        // обновляем количество заметок, написанных на кнопках
        // выбора таблиц
        mainWidget->refreshCountsNotesInTables();
    }
}

void ManagerNote::loadInTableWidgetRecycleBinNote()
{
    QDir dir = QDir(absolutePathToTheFolderRemoteNotes());

    dir.setFilter(QDir::Files);
    QStringList filters;
    filters << QString("*." + suffixFilesNote);
    dir.setNameFilters(filters);
    dir.setSorting(QDir::Name);

    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); i++)
    {
        QFileInfo fileInfo = list.at(i);

        QSettings ini(absolutePathToTheFolderRemoteNotes()
                      + QDir::separator()
                      + fileInfo.fileName(),
                      QSettings::IniFormat);
        ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

        DataNote *dataNote = new DataNote();
        dataNote->title             = ini.value("Title").toString();
        dataNote->lock              = ini.value("Lock").toBool();
        dataNote->visible           = ini.value("Visible").toBool();
        dataNote->dateOfCreating    = ini.value("DateOfCreating").toString();
        dataNote->dateLastChange    = ini.value("DateLastChange").toString();
        dataNote->dateOfLastRemoval = ini.value("DateOfLastRemoval").toString();;
        dataNote->dateOfLastRestore = ini.value("DateOfLastRestore").toString();;
        dataNote->countTextSymbols  = ini.value("CountTextSymbols").toInt();
        dataNote->countTextLines    = ini.value("CountTextLines").toInt();

        if(dataNote->title.isEmpty())
            dataNote->title = noTitle;

        QVariant itemDataNote;
        itemDataNote.setValue(fileInfo.fileName());

        addToTheEndTablesRow(mainWidget->tableWidgetRecycleBin,
                             itemDataNote,
                             dataNote);
    }

    mainWidget->refreshRecycleBinIcons();
    // обновляем количество заметок, написанных на кнопках
    // выбора таблиц
    mainWidget->refreshCountsNotesInTables();
}

void ManagerNote::addMenu()
{
    actionOpen = new QAction(this);
    actionOpen->setIcon(QIcon(IconOpen));
    actionOpen->setShortcut(QKeySequence(QKeySequence::Open));
    connect(actionOpen, SIGNAL(triggered()),
            this, SLOT(open()));

    actionNewNote = new QAction(this);
    actionNewNote->setIcon(QIcon(IconAdd));
    connect(actionNewNote, SIGNAL(triggered()),
            this, SLOT(addNewNote()));

    actionNewNoteFromClipboard = new QAction(this);
    connect(actionNewNoteFromClipboard, SIGNAL(triggered()),
            this, SLOT(addNewNoteFromClipboard()));

    fileMenu = new QMenu(this);
    fileMenu->addAction(actionNewNote);
    fileMenu->addAction(actionNewNoteFromClipboard);
    fileMenu->addSeparator();
    fileMenu->addAction(actionOpen);

    actionFindNote = new QAction(this);
    actionFindNote->setIcon(QIcon(IconFind));
    connect(actionFindNote, SIGNAL(triggered()),
            this, SLOT(showDialogFindNotes()));

    editMenu = new QMenu(this);
    editMenu->addAction(actionFindNote);

    menu = new QMenuBar(this);
    menu->addMenu(fileMenu);
    menu->addMenu(editMenu);

    setMenuBar(menu);
}

void ManagerNote::addMenuSystemTrayIcon(QSystemTrayIcon *tray)
{
    menuSystemTrayIcon        = new QMenu();

    actionTrayMenuQuit        = new QAction(this);
    actionTrayMenuAddNote     = new QAction(this);
    actionTrayMenuSettings    = new QAction(this);
    actionTrayMenuShowHideManager = new QAction(this);
    actionTrayMenuShowAllNotes = new QAction(this);
    actionTrayMenuHideAllNotes = new QAction(this);

    actionTrayMenuActivateHotkeys = new QAction(this);
    actionTrayMenuDeactivateHotkeys = new QAction(this);

    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    bool activateHotkeys = ini.value("Hotkeys/Activate",
                                     false).toBool();

    actionTrayMenuActivateHotkeys->setVisible(!activateHotkeys);
    actionTrayMenuDeactivateHotkeys->setVisible(activateHotkeys);

    actionTrayMenuQuit->setIcon(QIcon(IconQuit));
    actionTrayMenuAddNote->setIcon(QIcon(IconAdd));
    actionTrayMenuSettings->setIcon(QIcon(IconSettingsProgram));
    actionTrayMenuShowHideManager->setIcon(QIcon(IconManager));
    actionTrayMenuShowAllNotes->setIcon(QIcon(IconShowAll));
    actionTrayMenuHideAllNotes->setIcon(QIcon(IconHideAll));

    connect(actionTrayMenuQuit, SIGNAL(triggered()),
            this, SLOT(closeProgram()));
    connect(actionTrayMenuAddNote, SIGNAL(triggered()),
            this, SLOT(addNewNote()));
    connect(actionTrayMenuShowHideManager, SIGNAL(triggered()),
            this, SLOT(showHideManager()));
    connect(actionTrayMenuSettings, SIGNAL(triggered()),
            this, SLOT(showSettingsProgram()));
    connect(actionTrayMenuShowAllNotes, SIGNAL(triggered()),
            this, SLOT(clickButtonShowAllNote()));
    connect(actionTrayMenuHideAllNotes, SIGNAL(triggered()),
            this, SLOT(clickButtonHideAllNote()));

    connect(actionTrayMenuActivateHotkeys, SIGNAL(triggered()),
            this, SLOT(clickActionActivateHotkeys()));
    connect(actionTrayMenuDeactivateHotkeys, SIGNAL(triggered()),
            this, SLOT(clickActionDeactivateHotkeys()));

    helpMenu = new QMenu();
    helpMenu->setIcon(QIcon(IconHelp));

    actionHelp = new QAction(helpMenu);
    actionHelp->setIcon(QIcon(IconHelp));
    actionHelp->setShortcut(QKeySequence(QKeySequence::HelpContents));

    actionAbout = new QAction(helpMenu);

    connect(actionHelp, SIGNAL(triggered()),
            this, SLOT(help()));
    connect(actionAbout, SIGNAL(triggered()),
            this, SLOT(about()));

    helpMenu->addAction(actionHelp);
    helpMenu->addSeparator();
    helpMenu->addAction(actionAbout);

    menuSystemTrayIcon->addAction(actionTrayMenuShowHideManager);
    menuSystemTrayIcon->addSeparator();
    menuSystemTrayIcon->addAction(actionTrayMenuAddNote);
    menuSystemTrayIcon->addAction(actionNewNoteFromClipboard);
    menuSystemTrayIcon->addSeparator();
    menuSystemTrayIcon->addAction(actionTrayMenuShowAllNotes);
    menuSystemTrayIcon->addAction(actionTrayMenuHideAllNotes);
    menuSystemTrayIcon->addSeparator();
    menuSystemTrayIcon->addAction(actionTrayMenuActivateHotkeys);
    menuSystemTrayIcon->addAction(actionTrayMenuDeactivateHotkeys);
    menuSystemTrayIcon->addSeparator();
    menuSystemTrayIcon->addAction(actionTrayMenuSettings);
    menuSystemTrayIcon->addSeparator();
    menuSystemTrayIcon->addMenu(helpMenu);
    menuSystemTrayIcon->addSeparator();
    menuSystemTrayIcon->addAction(actionTrayMenuQuit);

    tray->setContextMenu(menuSystemTrayIcon);
}

void ManagerNote::addCommandButtons()
{
    // Notes
    pButtonAddNote              = new QPushButton(this);
    pButtonHideNote             = new QPushButton(this);
    pButtonShowNote             = new QPushButton(this);
    pButtonMoveNoteToRecycleBin = new QPushButton(this);

    pButtonAllNotesNotes = new UButtonWithContextMenu(this);
    pButtonAllNotesNotes->setToShowBottomContextMenu();
    menuForPButtonAllNotesNotes = new QMenu();

    actionMoveAllNotesToRecycleBin = new QAction(this);
    actionShowAllNote              = new QAction(this);
    actionHideAllNote              = new QAction(this);

    actionMoveAllNotesToRecycleBin->setIcon(QIcon(IconClear));
    actionShowAllNote->setIcon(QIcon(IconShowAll));
    actionHideAllNote->setIcon(QIcon(IconHideAll));

    connect(actionShowAllNote, SIGNAL(triggered()),
            this, SLOT(clickButtonShowAllNote()));
    connect(actionHideAllNote, SIGNAL(triggered()),
            this, SLOT(clickButtonHideAllNote()));
    connect(actionMoveAllNotesToRecycleBin, SIGNAL(triggered()),
            this, SLOT(clickButtonClearListWidgetNote()));

    menuForPButtonAllNotesNotes->addAction(actionShowAllNote);
    menuForPButtonAllNotesNotes->addAction(actionHideAllNote);
    menuForPButtonAllNotesNotes->addAction(actionMoveAllNotesToRecycleBin);

    pButtonAllNotesNotes->setContextMenu(menuForPButtonAllNotesNotes);
    pButtonAllNotesNotes->setIcon(QIcon(IconAllNotes));


    pButtonAddNote->setIcon(QIcon(IconAdd));
    pButtonHideNote->setIcon(QIcon(IconHide));
    pButtonShowNote->setIcon(QIcon(IconShow));
    pButtonMoveNoteToRecycleBin->setIcon(QIcon(IconRecycleBin_Full));

    mainWidget->addWidgetOnNotesPage(pButtonAddNote,              0, 0);
    mainWidget->addWidgetOnNotesPage(pButtonMoveNoteToRecycleBin, 0, 1);
    mainWidget->addWidgetOnNotesPage(pButtonShowNote,             1, 0);
    mainWidget->addWidgetOnNotesPage(pButtonHideNote,             1, 1);
    mainWidget->addWidgetOnNotesPage(pButtonAllNotesNotes,       1, 2);

    connect(pButtonAddNote, SIGNAL(clicked()),
            this, SLOT(addNewNote()));
    connect(pButtonShowNote, SIGNAL(clicked()),
            this, SLOT(clickButtonShowNote()));
    connect(pButtonHideNote, SIGNAL(clicked()),
            this, SLOT(clickButtonHideNote()));
    connect(pButtonMoveNoteToRecycleBin, SIGNAL(clicked()),
            this, SLOT(clickButtonMoveNoteToRecycleBin()));

    // RECYCLE BIN
    pButtonDeleteNote     = new QPushButton(this);
    pButtonRestoreNote    = new QPushButton(this);

    pButtonDeleteNote->setIcon(QIcon(IconDelete));
    pButtonRestoreNote->setIcon(QIcon(IconRestore));

    connect(pButtonDeleteNote, SIGNAL(clicked()),
            this, SLOT(clickDeleteItem()));
    connect(pButtonRestoreNote, SIGNAL(clicked()),
            this, SLOT(clickRestoreItem()));

    pButtonAllNotesRecycleBin = new UButtonWithContextMenu(this);
    pButtonAllNotesRecycleBin->setToShowBottomContextMenu();
    menuForPButtonAllNotesRecycleBin = new QMenu();
    actionDeleteAllNote  = new QAction(this);
    actionRestoreAllNote = new QAction(this);

    actionDeleteAllNote->setIcon(QIcon(IconDeleteAll));
    actionRestoreAllNote->setIcon(QIcon(IconRestoreAll));

    connect(actionDeleteAllNote, SIGNAL(triggered()),
            this, SLOT(clickDeleteAllItem()));
    connect(actionRestoreAllNote, SIGNAL(triggered()),
            this, SLOT(clickRestoreAllItem()));

    menuForPButtonAllNotesRecycleBin->addAction(actionDeleteAllNote);
    menuForPButtonAllNotesRecycleBin->addAction(actionRestoreAllNote);

    pButtonAllNotesRecycleBin->setContextMenu(menuForPButtonAllNotesRecycleBin);
    pButtonAllNotesRecycleBin->setIcon(QIcon(IconAllNotes));

    mainWidget->addWidgetOnRecycleBinPage(pButtonDeleteNote,         0, 0);
    mainWidget->addWidgetOnRecycleBinPage(pButtonRestoreNote,        0, 1);
    mainWidget->addWidgetOnRecycleBinPage(pButtonAllNotesRecycleBin, 0, 2);
}

void ManagerNote::readDataInTables()
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);

    ini.beginGroup("TableNotes");

    bool visibleColumnVisible           = ini.value("VisibleColumnVisible", true).toBool();
    bool visibleColumnDateOfCreating    = ini.value("VisibleColumnDateOfCreating", true).toBool();
    bool visibleColumnCountTextSymbols  = ini.value("VisibleColumnCountTextSymbols", true).toBool();
    bool visibleColumnCountTextLine     = ini.value("VisibleColumnCountTextLine", true).toBool();
    bool visibleColumnDateLastChange    = ini.value("VisibleColumnDateLastChange", true).toBool();
    bool visibleColumnDateOfLastRemoval = ini.value("VisibleColumnDateOfLastRemoval", true).toBool();
    bool visibleColumnDateOfLastRestore = ini.value("VisibleColumnDateOfLastRestore", true).toBool();
    bool visibleColumnLock              = ini.value("VisibleColumnLock", true).toBool();

    ini.endGroup();


    QList <bool> list;
    list.insert(indexColumnTitle,             true);
    list.insert(indexColumnVisible,           visibleColumnVisible);
    list.insert(indexColumnDateOfCreating,    visibleColumnDateOfCreating);
    list.insert(indexColumnCountTextSymbols,  visibleColumnCountTextSymbols);
    list.insert(indexColumnCountTextLines,    visibleColumnCountTextLine);
    list.insert(indexColumnDateLastChange,    visibleColumnDateLastChange);
    list.insert(indexColumnDateOfLastRemoval, visibleColumnDateOfLastRemoval);
    list.insert(indexColumnDateOfLastRestore, visibleColumnDateOfLastRestore);
    list.insert(indexColumnLock,              visibleColumnLock);

    mainWidget->tableWidgetNotes->setVisibleColumnTable(list);
    mainWidget->settingsManager->setCheckedNotesCheckBox(list);


    ini.beginGroup("TableRecycleBin");

    visibleColumnVisible          = ini.value("VisibleColumnVisible", true).toBool();
    visibleColumnDateOfCreating   = ini.value("VisibleColumnDateOfCreating", true).toBool();
    visibleColumnCountTextSymbols = ini.value("VisibleColumnCountTextSymbols", true).toBool();
    visibleColumnCountTextLine    = ini.value("VisibleColumnCountTextLine", true).toBool();
    visibleColumnDateLastChange   = ini.value("VisibleColumnDateLastChange", true).toBool();
    visibleColumnDateOfLastRemoval = ini.value("VisibleColumnDateOfLastRemoval", true).toBool();
    visibleColumnDateOfLastRestore = ini.value("VisibleColumnDateOfLastRestore", true).toBool();
    visibleColumnLock              = ini.value("VisibleColumnLock", true).toBool();

    ini.endGroup();


    list = QList <bool> ();
    list.insert(indexColumnTitle,             true);
    list.insert(indexColumnVisible,           visibleColumnVisible);
    list.insert(indexColumnDateOfCreating,    visibleColumnDateOfCreating);
    list.insert(indexColumnCountTextSymbols,  visibleColumnCountTextSymbols);
    list.insert(indexColumnCountTextLines,    visibleColumnCountTextLine);
    list.insert(indexColumnDateLastChange,    visibleColumnDateLastChange);
    list.insert(indexColumnDateOfLastRemoval, visibleColumnDateOfLastRemoval);
    list.insert(indexColumnDateOfLastRestore, visibleColumnDateOfLastRestore);
    list.insert(indexColumnLock,              visibleColumnLock);

    mainWidget->tableWidgetRecycleBin->setVisibleColumnTable(list);
    mainWidget->settingsManager->setCheckedRecycleBinCheckBox(list);
}

void ManagerNote::writeDataTables()
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    QList <bool> list = mainWidget->tableWidgetNotes->getListVisibleColumn();

    bool visibleColumnVisible           = list.at(indexColumnVisible);
    bool visibleColumnDateOfCreating    = list.at(indexColumnDateOfCreating);
    bool visibleColumnCountTextSymbols  = list.at(indexColumnCountTextSymbols);
    bool visibleColumnCountTextLine     = list.at(indexColumnCountTextLines);
    bool visibleColumnDateLastChange    = list.at(indexColumnDateLastChange);
    bool visibleColumnDateOfLastRemoval = list.at(indexColumnDateOfLastRemoval);
    bool visibleColumnDateOfLastRestore = list.at(indexColumnDateOfLastRestore);
    bool visibleColumnLock              = list.at(indexColumnLock);

    ini.beginGroup("TableNotes");

    ini.setValue("VisibleColumnVisible",
                 visibleColumnVisible);
    ini.setValue("VisibleColumnDateOfCreating",
                 visibleColumnDateOfCreating);
    ini.setValue("VisibleColumnCountTextSymbols",
                 visibleColumnCountTextSymbols);
    ini.setValue("VisibleColumnCountTextLine",
                 visibleColumnCountTextLine);
    ini.setValue("VisibleColumnDateLastChange",
                 visibleColumnDateLastChange);
    ini.setValue("VisibleColumnDateOfLastRemoval",
                 visibleColumnDateOfLastRemoval);
    ini.setValue("VisibleColumnDateOfLastRestore",
                 visibleColumnDateOfLastRestore);
    ini.setValue("VisibleColumnLock",
                 visibleColumnLock);

    ini.endGroup();

    list = mainWidget->tableWidgetRecycleBin->getListVisibleColumn();

    visibleColumnVisible           = list.at(indexColumnVisible);
    visibleColumnDateOfCreating    = list.at(indexColumnDateOfCreating);
    visibleColumnCountTextSymbols  = list.at(indexColumnCountTextSymbols);
    visibleColumnCountTextLine     = list.at(indexColumnCountTextLines);
    visibleColumnDateLastChange    = list.at(indexColumnDateLastChange);
    visibleColumnDateOfLastRemoval = list.at(indexColumnDateOfLastRemoval);
    visibleColumnDateOfLastRestore = list.at(indexColumnDateOfLastRestore);
    visibleColumnLock              = list.at(indexColumnLock);

    ini.beginGroup("TableRecycleBin");

    ini.setValue("VisibleColumnVisible",
                 visibleColumnVisible);
    ini.setValue("VisibleColumnDateOfCreating",
                 visibleColumnDateOfCreating);
    ini.setValue("VisibleColumnCountTextSymbols",
                 visibleColumnCountTextSymbols);
    ini.setValue("VisibleColumnCountTextLine",
                 visibleColumnCountTextLine);
    ini.setValue("VisibleColumnDateLastChange",
                 visibleColumnDateLastChange);
    ini.setValue("VisibleColumnDateOfLastRemoval",
                 visibleColumnDateOfLastRemoval);
    ini.setValue("VisibleColumnDateOfLastRestore",
                 visibleColumnDateOfLastRestore);
    ini.setValue("VisibleColumnLock",
                 visibleColumnLock);

    ini.endGroup();

    ini.sync();
}

void ManagerNote::cutPasteRowTable(QTableWidget *sender,
                                   QTableWidget *receiver,
                                   int row)
{
    //sender - таблица у которой вырезается строка
    //receiver - таблица которая получает строку

    if(sender == mainWidget->tableWidgetNotes)
    {
        Note *note = sender->item(row, indexMainColumn)->data(pointerOnDataItemNote).value<Note*>();

        QDateTime dataTime = QDateTime::currentDateTime();
        QString str = QString("%1\n%2")
                .arg(dataTime.date().toString(formatDate))
                .arg(dataTime.time().toString(formatTime));

        note->dataNote.dateOfLastRemoval = str;
        note->saveDateOfLastRemoval();

        sender->item(row, indexColumnDateOfLastRemoval)->setText(str);
    }


    int currentRow;
    receiver->setRowCount(receiver->rowCount() + 1);
    currentRow = receiver->rowCount() - 1;

    for(int j = 0; j < sender->columnCount(); j++)
    {
        QTableWidgetItem *item = sender->item(row, j)->clone();
        receiver->setItem(currentRow,
                          j,
                          item);
    }
    sender->removeRow(row);
}

void ManagerNote::moveRowFromTheRecycleBin(int row)
{
    QTableWidget *tableNotes = mainWidget->tableWidgetNotes;
    QTableWidget *tableRecycleBin = mainWidget->tableWidgetRecycleBin;

    QTableWidgetItem *tableItemRecycleBin;
    tableItemRecycleBin = tableRecycleBin->item(row, indexMainColumn);
    if(tableItemRecycleBin->checkState() == Qt::Checked)
        tableItemRecycleBin->setCheckState(Qt::Unchecked);

    tableNotes->blockSignals(true);
    cutPasteRowTable(tableRecycleBin,
                     tableNotes,
                     row);
    tableNotes->blockSignals(false);

    int lastRow = tableNotes->rowCount() - 1;

    QTableWidgetItem *tableItem = tableNotes->item(lastRow,
                                                   indexMainColumn);
    QString nameFile = tableItem->data(pointerOnDataItemNote).value<QString>();

    QString oldFileName = absolutePathToTheFolderRemoteNotes()
            + QDir::separator()
            + nameFile;

    moveTheFile(oldFileName,
                absolutePathToTheFolderNotes());

    QString absolutePathInFileNote = absolutePathToTheFolderNotes()
            + QDir::separator()
            + nameFile;

    Note *newNote = new Note(absolutePathInFileNote);
    newNote->setVisibleNote(newNote->dataNote.visible);
    newNote->setLanguage(currentLanguage);
    newNote->textEdit->setCompleter(completer);
    newNote->testAutocompleterLanguage();

    QDateTime dataTime = QDateTime::currentDateTime();
    QString str = QString("%1\n%2")
            .arg(dataTime.date().toString(formatDate))
            .arg(dataTime.time().toString(formatTime));

    newNote->dataNote.dateOfLastRestore = str;
    newNote->saveDateOfLastRestore();
    tableNotes->item(lastRow, indexColumnDateOfLastRestore)->setText(str);

    QVariant itemDataNote;
    itemDataNote.setValue(newNote);
    tableItem->setData(pointerOnDataItemNote,
                       itemDataNote);

    setAllConnectForNote(newNote);

    mainWidget->refreshRecycleBinIcons();
    // обновляем количество заметок, написанных на кнопках
    // выбора таблиц
    mainWidget->refreshCountsNotesInTables();
}

void ManagerNote::moveRowToTheRecycleBin(int row)
{
    QTableWidget *tableNotes = mainWidget->tableWidgetNotes;
    QTableWidget *tableRecycleBin = mainWidget->tableWidgetRecycleBin;

    QTableWidgetItem *item = tableNotes->item(row, indexMainColumn);
    Note *note = item->data(pointerOnDataItemNote).value<Note*>();
    note->saveDataNote();

    item->setCheckState(Qt::Unchecked);

    cutPasteRowTable(tableNotes,
                     tableRecycleBin,
                     row);

    moveTheFile(note->getPathInFileNote(),
                absolutePathToTheFolderRemoteNotes());

    int lastRow = tableRecycleBin->rowCount() - 1;
    item = tableRecycleBin->item(lastRow,
                                 indexMainColumn);

    QString nameFileNote = QFileInfo(note->getPathInFileNote()).fileName();

    item->setData(pointerOnDataItemNote,
                  QVariant(nameFileNote));

    tableNotes->selectRow(tableNotes->rowCount() - 1);

    delete note;
}

void ManagerNote::setAllConnectForNote(Note *note)
{
    connect(note, SIGNAL(visibleChange(Note*)),
            this, SLOT(changeNote(Note*)));
    connect(note, SIGNAL(titleChange(Note*)),
            this, SLOT(changeNote(Note*)));
    connect(note, SIGNAL(lockIsChange(Note*)),
            this, SLOT(changeNote(Note*)));
    connect(note, SIGNAL(propertiesTextEditNote(Note*)),
            this, SLOT(propertiesNoteTextIsChange(Note*)));

    connect(note, SIGNAL(noteDelete(Note*)),
            this, SLOT(deleteNote(Note*)));
    connect(note, SIGNAL(makeDuplicate(DataNote*)),
            this, SLOT(makeDublicateNote(DataNote*)));
    connect(this, SIGNAL(closeAllNote()),
            note, SLOT(close()));
    connect(this, SIGNAL(languageIsChange(QString)),
            note, SLOT(setLanguage(QString)));
}

void ManagerNote::addToTheEndTablesRow(QTableWidget *table,
                                       QVariant dataMainCol,
                                       DataNote *dataNote)
{
    // добавляем строку
    table->setRowCount(table->rowCount() + 1);
    int currentRow = table->rowCount() - 1;

    QTableWidgetItem *itemTitle = new QTableWidgetItem();
    itemTitle->setData(pointerOnDataItemNote, dataMainCol);
    itemTitle->setText(dataNote->title);
    itemTitle->setCheckState(Qt::Unchecked);

    bool visible = dataNote->visible;
    QTableWidgetItem *itemVisible = new QTableWidgetItem();
    itemVisible->setData(pointerOnDataColumnVisible, visible);
    if(visible)
        itemVisible->setIcon(QIcon(IconShow));
    else
        itemVisible->setIcon(QIcon(IconHide));

    QTableWidgetItem *itemCreateDate = new QTableWidgetItem();
    itemCreateDate->setText(QVariant(dataNote->dateOfCreating).toString());

    QTableWidgetItem *itemCountTextSymbols = new QTableWidgetItem();
    itemCountTextSymbols->setText(QVariant(dataNote->countTextSymbols).toString());

    QTableWidgetItem *itemCountTextLines = new QTableWidgetItem();
    itemCountTextLines->setText(QVariant(dataNote->countTextLines).toString());

    QTableWidgetItem *itemDataLastChange = new QTableWidgetItem();
    itemDataLastChange->setText(dataNote->dateLastChange);

    bool lock = dataNote->lock;
    QTableWidgetItem *itemDataLock = new QTableWidgetItem();
    itemDataLock->setData(pointerOnDataColumnLock,
                          lock);
    if(lock)
        itemDataLock->setIcon(QIcon(IconLock));
    else
        itemDataLock->setIcon(QIcon(IconUnlock));

    QTableWidgetItem *itemDataDateOfRemoval = new QTableWidgetItem();
    itemDataDateOfRemoval->setText(dataNote->dateOfLastRemoval);

    QTableWidgetItem *itemDataDateOfRestore = new QTableWidgetItem();
    itemDataDateOfRestore->setText(dataNote->dateOfLastRestore);

    table->setItem(currentRow,
                   indexColumnTitle,
                   itemTitle);

    table->setItem(currentRow,
                   indexColumnVisible,
                   itemVisible);

    table->setItem(currentRow,
                   indexColumnDateOfCreating,
                   itemCreateDate);

    table->setItem(currentRow,
                   indexColumnCountTextSymbols,
                   itemCountTextSymbols);

    table->setItem(currentRow,
                   indexColumnCountTextLines,
                   itemCountTextLines);

    table->setItem(currentRow,
                   indexColumnDateLastChange,
                   itemDataLastChange);

    table->setItem(currentRow,
                   indexColumnLock,
                   itemDataLock);

    table->setItem(currentRow,
                   indexColumnDateOfLastRemoval,
                   itemDataDateOfRemoval);

    table->setItem(currentRow,
                   indexColumnDateOfLastRestore,
                   itemDataDateOfRestore);
}

QList <QString> * ManagerNote::getListTranslaction() const
{
    QList <QString> *listTranslaction = new QList <QString>;

    QDir dir = QDir(absolutePathToTheFolderLanguages());

    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);

    QStringList filters;
    filters << QString("*." + suffixLanguageFiles);
    dir.setNameFilters(filters);

    QFileInfoList list = dir.entryInfoList();

    for(int i = 0; i < list.size(); i++)
    {
        QFileInfo fileInfo = list.at(i);

        QSettings ini(fileInfo.absoluteFilePath(),
                      QSettings::IniFormat);
        ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

        ini.beginGroup("FILE");

        QString nameLanguage = ini.value("Name").toString();
        QString author       = ini.value("Author").toString();
        QString contact      = ini.value("Contact").toString();

        ini.endGroup();

        QString tran;

        if(!nameLanguage.isEmpty())
            tran += QString("&nbsp;&nbsp;&bull; <b>%1</b>")
                    .arg(nameLanguage);

        if(!author.isEmpty())
        {
            tran += " - ";
            tran += author;
        }

        if(!contact.isEmpty())
        {
            tran += "&nbsp;&nbsp;&nbsp;&nbsp;";
            tran += contact;
        }

        listTranslaction->append(tran);
    }

    return listTranslaction;
}

/// PUBLIC SLOTS
void ManagerNote::addNewNote()
{        
    QString fileName = QString("%1.%2")
            .arg(currentDataTime())
            .arg(suffixFilesNote);

    QString absolutePathInFileNote = absolutePathToTheFolderNotes()
            + QDir::separator()
            + fileName;

    QTableWidget *table = mainWidget->tableWidgetNotes;
    addNewNoteInTable(absolutePathInFileNote,
                      table);

    emit changedTheNumberOfNotes();
}

void ManagerNote::addNewNoteFromClipboard()
{
    QString fileName = QString("%1.%2")
            .arg(currentDataTime())
            .arg(suffixFilesNote);

    QString absolutePathInFileNote = absolutePathToTheFolderNotes()
            + QDir::separator()
            + fileName;

    QClipboard *clipboard = QApplication::clipboard();
    QString clipboardText = clipboard->text();

    QTableWidget *table = mainWidget->tableWidgetNotes;
    addNewNoteInTable(absolutePathInFileNote,
                      clipboardText,
                      table);

    emit changedTheNumberOfNotes();
}

void ManagerNote::clickButtonShowNote()
{
    QTableWidget *table = mainWidget->tableWidgetNotes;

    // иначе ищем те, у которых check = true
    QQueue <int> queue;
    for(int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem *item = table->item(i, indexMainColumn);
        if(item->checkState() == Qt::Checked)
            queue.append(i);
    }

    if(!queue.isEmpty())
    {
        do
        {
            int nextRow = queue.takeFirst();

            QTableWidgetItem *item = table->item(nextRow, indexMainColumn);
            Note *note = item->data(pointerOnDataItemNote).value<Note*>();
            note->setVisibleNote(true);

        }while(!queue.isEmpty());

        emit changedTheStatusOfNotes();
    }else
    {
        QList <int> listIndexSelectedRow = getListIndexSelectedRow(table);
        // если выделены строки
        if(!listIndexSelectedRow.isEmpty() && table->currentRow() != -1)
        {
            do
            {
                int row = listIndexSelectedRow.takeFirst();
                QTableWidgetItem *item = table->item(row, indexMainColumn);
                Note *note = item->data(pointerOnDataItemNote).value<Note*>();
                note->setVisibleNote(true);

            }while(!listIndexSelectedRow.isEmpty());

            emit changedTheStatusOfNotes();
        }
    }
}

void ManagerNote::clickButtonHideNote()
{
    QTableWidget *table = mainWidget->tableWidgetNotes;

    // иначе ищем те, у которых check = true
    QQueue <int> queue;
    for(int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem *item = table->item(i, indexMainColumn);
        if(item->checkState() == Qt::Checked)
            queue.append(i);
    }

    if(!queue.isEmpty())
    {
        do
        {
            int row = queue.takeFirst();

            QTableWidgetItem *item = table->item(row, indexMainColumn);
            Note *note = item->data(pointerOnDataItemNote).value<Note*>();
            note->setVisibleNote(false);

        }while(!queue.isEmpty());

        emit changedTheStatusOfNotes();
    }else
    {
        QList <int> listIndexSelectedRow = getListIndexSelectedRow(table);
        // если выделены строки
        if(!listIndexSelectedRow.isEmpty()
                && table->currentRow() != -1)
        {
            do
            {
                int row = listIndexSelectedRow.takeFirst();
                QTableWidgetItem *item = table->item(row, indexMainColumn);
                Note *note = item->data(pointerOnDataItemNote).value<Note*>();
                note->setVisibleNote(false);

            }while(!listIndexSelectedRow.isEmpty());

            emit changedTheStatusOfNotes();
        }
    }
}

void ManagerNote::clickButtonMoveNoteToRecycleBin()
{       
    QTableWidget *table = mainWidget->tableWidgetNotes;

    QList <int> listIndexSelectedRow = getListIndexSelectedRow(table);

    // ищем те, у которых check = true
    QList <int> list;
    for(int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem *item = table->item(i, indexMainColumn);

        if(item->checkState() == Qt::Checked)
            list.append(i); // добавляем в контейнер индекс строки
    }

    if(!list.isEmpty())
    {
        UDialogSelectListNotes *dialogSelectListNotes = new UDialogSelectListNotes(this);
        if(list.count() == 1)
            dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::MoveSelectedNoteToRecycleBin);
        else
            dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::MoveSelectedNotesToRecycleBin);

        dialogSelectListNotes->setLanguage(currentLanguage);

        connect(this, SIGNAL(languageIsChange(QString)),
                dialogSelectListNotes, SLOT(setLanguage(QString)));

        dialogSelectListNotes->selectTableRow(table,
                                              &list);
        dialogSelectListNotes->exec();

        list = dialogSelectListNotes->getListRows();

        delete dialogSelectListNotes;

        // пока контейнер не опустеет
        while(!list.isEmpty())
        {
            // получаем первый элемент из контейнера
            int row = list.at(0);
            // удаляем его
            list.removeAt(0);

            // т.к. при удалении строки в таблицы, строки находящиеся
            // после удаляемой опустятся на 1 строку, то и мы должны
            // повторить это, но для элементов контейнера, иначе, будет
            // ошибка
            for(int i = 0; i < list.count(); i++)
                list[i] = list[i] - 1;

            moveRowToTheRecycleBin(row);
        }

        emit changedTheStatusOfNotes();

        mainWidget->refreshRecycleBinIcons();
        // обновляем количество заметок, написанных на кнопках
        // выбора таблиц
        mainWidget->refreshCountsNotesInTables();

        // если выделены строки и есть текущее выделение
    }else if(!listIndexSelectedRow.isEmpty()
             && table->currentRow() != -1)
    {
        QList <int> list = listIndexSelectedRow;

        UDialogSelectListNotes *dialogSelectListNotes = new UDialogSelectListNotes(this);
        if(list.count() == 1)
            dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::MoveSelectedNoteToRecycleBin);
        else
            dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::MoveSelectedNotesToRecycleBin);

        dialogSelectListNotes->setLanguage(currentLanguage);

        connect(this, SIGNAL(languageIsChange(QString)),
                dialogSelectListNotes, SLOT(setLanguage(QString)));

        dialogSelectListNotes->selectTableRow(table, &list);
        dialogSelectListNotes->exec();

        list = dialogSelectListNotes->getListRows();

        delete dialogSelectListNotes;

        while(!list.isEmpty())
        {
            // получаем первый элемент из контейнера
            int row = list.at(0);
            // удаляем его
            list.removeAt(0);
            // т.к. при удалении строки в таблицы, строки находящиеся
            // после удаляемой опустятся на 1 строку, то и мы должны
            // повторить это, но для элементов контейнера, иначе, будет
            // ошибка
            for(int i = 0; i < list.size(); i++)
                list[i] = list[i] - 1;

            moveRowToTheRecycleBin(row);
        }
        emit changedTheStatusOfNotes();

        mainWidget->refreshRecycleBinIcons();
        // обновляем количество заметок, написанных на кнопках
        // выбора таблиц
        mainWidget->refreshCountsNotesInTables();
    }
}

void ManagerNote::clickButtonMoveThisNoteToRecycleBin()
{
    QTableWidget *table = mainWidget->tableWidgetNotes;

    int row = table->currentRow();

    QList <int> list;
    list.append(row);

    UDialogSelectListNotes *dialogSelectListNotes = new UDialogSelectListNotes(this);
    dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::MoveSelectedNoteToRecycleBin);

    dialogSelectListNotes->setLanguage(currentLanguage);

    connect(this, SIGNAL(languageIsChange(QString)),
            dialogSelectListNotes, SLOT(setLanguage(QString)));

    dialogSelectListNotes->selectTableRow(table, &list);
    dialogSelectListNotes->exec();

    list = dialogSelectListNotes->getListRows();

    delete dialogSelectListNotes;

    if(!list.isEmpty())
    {
        moveRowToTheRecycleBin(list.first());

        mainWidget->refreshRecycleBinIcons();

        // обновляем количество заметок, написанных на кнопках
        // выбора таблиц
        mainWidget->refreshCountsNotesInTables();

        emit changedTheNumberOfNotes();
    }
}
void ManagerNote::clickButtonMoveThisNoteToRecycleBin(Note *note)
{
    QTableWidget *table = mainWidget->tableWidgetNotes;

    for(int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem *item;
        item = table->item(i, indexMainColumn);

        Note *itemNote = item->data(pointerOnDataItemNote).value<Note*>();

        if(itemNote == note && note)
        {
            int row = table->row(item);
            table->selectRow(row);
            clickButtonMoveThisNoteToRecycleBin();
            break;
        }
    }
}

void ManagerNote::clickButtonSettingsNote()
{
    QTableWidget *table = mainWidget->tableWidgetNotes;

    int row = table->currentRow();
    QTableWidgetItem *item = table->item(row, indexMainColumn);
    if(item)
    {
        Note *note = item->data(pointerOnDataItemNote).value<Note*>();
        note->showSettings();
    }
}
void ManagerNote::clickButtonSettingsNote(Note *note)
{
    QTableWidget *table = mainWidget->tableWidgetNotes;

    for(int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem *item = table->item(i, indexMainColumn);
        Note *itemNote = item->data(pointerOnDataItemNote).value<Note*>();
        if(itemNote == note && note)
        {
            int row = table->row(item);
            table->selectRow(row);
            clickButtonSettingsNote();
            break;
        }
    }
}

void ManagerNote::clickButtonShowThisNote()
{
    QTableWidget *table = mainWidget->tableWidgetNotes;

    int row = table->currentRow();
    QTableWidgetItem *item = table->item(row, indexMainColumn);
    if(item)
    {
        Note *note = item->data(pointerOnDataItemNote).value<Note*>();
        note->setVisibleNote(true);
    }

    emit changedTheStatusOfNotes();
}
void ManagerNote::clickButtonShowThisNote(Note *note)
{
    QTableWidget *table = mainWidget->tableWidgetNotes;

    for(int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem *item = table->item(i, indexMainColumn);
        Note *itemNote = item->data(pointerOnDataItemNote).value<Note*>();
        if(itemNote == note && note)
        {
            int row = table->row(item);
            table->selectRow(row);
            clickButtonShowThisNote();
            break;
        }
    }
}

void ManagerNote::clickButtonHideThisNote()
{
    QTableWidget *table = mainWidget->tableWidgetNotes;

    int row = table->currentRow();
    QTableWidgetItem *item = table->item(row, indexMainColumn);
    if(item)
    {
        Note *note = item->data(pointerOnDataItemNote).value<Note*>();
        note->setVisibleNote(false);
    }

    emit changedTheStatusOfNotes();
}
void ManagerNote::clickButtonHideThisNote(Note *note)
{
    QTableWidget *table = mainWidget->tableWidgetNotes;

    for(int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem *item = table->item(i, indexMainColumn);
        Note *itemNote = item->data(pointerOnDataItemNote).value<Note*>();
        if(itemNote == note && note)
        {
            int row = table->row(item);
            table->selectRow(row);
            clickButtonHideThisNote();
            break;
        }
    }
}

void ManagerNote::clickButtonShowAllNote()
{
    QTableWidget *table = mainWidget->tableWidgetNotes;

    for(int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem *item = table->item(i, indexMainColumn);
        Note *note = item->data(pointerOnDataItemNote).value<Note*>();
        note->setVisibleNote(true);
    }

    emit changedTheStatusOfNotes();
}
void ManagerNote::clickButtonHideAllNote()
{
    QTableWidget *table = mainWidget->tableWidgetNotes;

    for(int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem *item = table->item(i, indexMainColumn);
        Note *note = item->data(pointerOnDataItemNote).value<Note*>();
        note->setVisibleNote(false);
    }

    emit changedTheStatusOfNotes();
}

void ManagerNote::clickButtonClearListWidgetNote()
{
    QTableWidget *table = mainWidget->tableWidgetNotes;

    // если есть записи в таблице
    if(table->rowCount())
    {
        QList <int> list;

        for(int i = 0; i < table->rowCount(); i++)
            list.append(i);

        UDialogSelectListNotes *dialogSelectListNotes = new UDialogSelectListNotes(this);

        if(list.count() == 1)
            dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::MoveSelectedNoteToRecycleBin);
        else
            dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::MoveSelectedNotesToRecycleBin);

        dialogSelectListNotes->setLanguage(currentLanguage);

        connect(this, SIGNAL(languageIsChange(QString)),
                dialogSelectListNotes, SLOT(setLanguage(QString)));

        dialogSelectListNotes->selectTableRow(table, &list);
        dialogSelectListNotes->exec();

        list = dialogSelectListNotes->getListRows();

        delete dialogSelectListNotes;

        while(!list.isEmpty())
        {
            // получаем первый элемент из контейнера
            int row = list.at(0);
            // удаляем его
            list.removeAt(0);
            // т.к. при удалении строки в таблицы, строки находящиеся
            // после удаляемой опустятся на 1 строку, то и мы должны
            // повторить это, но для элементов контейнера, иначе, будет
            // ошибка
            for(int i = 0; i < list.size(); i++)
                list[i] = list[i] - 1;

            moveRowToTheRecycleBin(row);
        }

        mainWidget->refreshRecycleBinIcons();
        // обновляем количество заметок, написанных на кнопках
        // выбора таблиц
        mainWidget->refreshCountsNotesInTables();

        emit changedTheNumberOfNotes();
    }
}

void ManagerNote::changeNote(Note *note)
{
    QTableWidget *table = mainWidget->tableWidgetNotes;

    for(int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem *item = table->item(i, indexMainColumn);
        Note *itemNote = item->data(pointerOnDataItemNote).value<Note*>();
        if(itemNote == note)
        {
            QString title = note->dataNote.title;
            if(title.isEmpty())
                title = noTitle;

            int row = table->row(item);
            QTableWidgetItem *itemTitle = table->item(row, indexColumnTitle);
            QTableWidgetItem *itemVisible = table->item(row, indexColumnVisible);
            QTableWidgetItem *itemLock = table->item(row, indexColumnLock);

            itemTitle->setText(title);

            bool visible = note->dataNote.visible;
            itemVisible->setData(pointerOnDataColumnVisible, visible);
            if(visible)
                itemVisible->setIcon(QIcon(IconShow));
            else
                itemVisible->setIcon(QIcon(IconHide));


            bool lock = note->dataNote.lock;
            itemLock->setData(pointerOnDataColumnLock, lock);
            if(lock)
                itemLock->setIcon(QIcon(IconLock));
            else
                itemLock->setIcon(QIcon(IconUnlock));

            emit changedTheStatusOfNotes();
            break;
        }
    }
}

void ManagerNote::showHide(QSystemTrayIcon::ActivationReason reason)
{
    //если нажато левой кнопкой продолжаем
    if(reason == QSystemTrayIcon::Trigger)
    {
        //если окно было не видимо - отображаем его
        if(!isVisible())
        {
            // показываем окно в нормальном состоянии
            if(managerIsMaximized)
                show();
            else
                showNormal();

            // переводим фокус на окно
            activateWindow();
        }else
            // прячем окно с панели задач
            hideManager();
    }
}

// Вызывается когда заметка удаляет себя
// т.к. при освобождении памяти в этом слоте происходит
// ошибка, то освобождение было реализовано в классе самой заметки
void ManagerNote::deleteNote(Note *note)
{
    QTableWidget *tableNotes = mainWidget->tableWidgetNotes;
    QTableWidget *tableRecycleBin = mainWidget->tableWidgetRecycleBin;

    for(int i = 0; i < tableNotes->rowCount(); i++)
    {
        QTableWidgetItem *item = tableNotes->item(i, indexMainColumn);
        Note *itemNote = item->data(pointerOnDataItemNote).value<Note*>();
        if(itemNote == note && note)
        {
            int row = tableNotes->row(item);
            note->saveDataNote();

            item->setCheckState(Qt::Unchecked);

            cutPasteRowTable(tableNotes,
                             tableRecycleBin,
                             row);

            moveTheFile(note->getPathInFileNote(),
                        absolutePathToTheFolderRemoteNotes());

            int lastRow = tableRecycleBin->rowCount() - 1;
            QTableWidgetItem *item = tableRecycleBin->item(lastRow, indexMainColumn);
            QString nameFileNote = QFileInfo(note->getPathInFileNote()).fileName();

            //--- без этой конструкции при освобождении памяти заметки
            // будет ошибка вида:
            // Invalid Address specified to RtlFreeHeap, и не поймешь почему
            currentDeletedNote = note;
            //---

            item->setData(pointerOnDataItemNote,
                          QVariant(nameFileNote));

            tableNotes->selectRow(tableNotes->rowCount() - 1);

            QTimer::singleShot(0,
                               this,
                               SLOT(deleteCurrentNote()));
            break;
        }
    }

    mainWidget->refreshRecycleBinIcons();
    // обновляем количество заметок, написанных на кнопках
    // выбора таблиц
    mainWidget->refreshCountsNotesInTables();
    emit changedTheNumberOfNotes();
}

void ManagerNote::setLanguage(QString pathToTheFileLanguage)
{
    currentLanguage = pathToTheFileLanguage;
    emit languageIsChange(pathToTheFileLanguage);

    QSettings ini(pathToTheFileLanguage,
                       QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("Manager");

    setWindowTitle(ini.value("Title").toString());

    pButtonAddNote->setText(ini.value("Add").toString());
    pButtonShowNote->setText(ini.value("Show").toString());
    pButtonHideNote->setText(ini.value("Hide").toString());
    pButtonMoveNoteToRecycleBin->setText(ini.value("MoveNoteToRecycleBin").toString());
    pButtonAllNotesNotes->setText(ini.value("AllNotes").toString());

    actionMoveAllNotesToRecycleBin->setText(ini.value("MoveNoteToRecycleBin").toString());
    actionShowAllNote->setText(ini.value("Show").toString());
    actionHideAllNote->setText(ini.value("Hide").toString());

    pButtonDeleteNote->setText(ini.value("Delete").toString());
    pButtonRestoreNote->setText(ini.value("Restore").toString());
    pButtonAllNotesRecycleBin->setText(ini.value("AllNotes").toString());

    actionDeleteAllNote->setText(ini.value("Delete").toString());
    actionRestoreAllNote->setText(ini.value("Restore").toString());

    fileMenu->setTitle(ini.value("File").toString());
    actionOpen->setText(ini.value("Open").toString());    

    strCancel = ini.value("Cancel").toString();
    strOk     = ini.value("Ok").toString();
    strNo     = ini.value("No").toString();

    strTitleQuestion = ini.value("TitleQuestion").toString();
    strTitleHint    = ini.value("TitleHint").toString();
    strTextMessageClearList      = ini.value("TextMessageClearList").toString();
    strTextMessageDeleteItem     = ini.value("TextMessageDeleteItem").toString();
    strTextMessageDeleteAllItem  = ini.value("TextMessageDeleteAllItem").toString();
    strTextMessageRestoreAllItem = ini.value("TextMessageRestoreAllItem").toString();
    strTextMessageMoveNoteToRecycleBin = ini.value("TextMessageMoveNoteToRecycleBin").toString();
    strTextAddNewNoteInTable = ini.value("TextAddNewNoteInTable").toString();
    strTextMessageMoveSelectedNotesToRecycleBin = ini.value("TextMessageMoveSelectedNotesToRecycleBin").toString();
    strTextMessageDeleteSelectedNotes  = ini.value("TextMessageDeleteSelectedNotes").toString();
    strTextMessageRestoreSelectedNotes = ini.value("TextMessageRestoreSelectedNotes").toString();
    strTextMessageSystemTray = ini.value("TextMessageSystemTray").toString();
    strRowAndTitleItemInTable = ini.value("TextRowAndTitleItemInTable").toString();

    editMenu->setTitle(ini.value("Edit").toString());
    actionFindNote->setText(ini.value("FindNote").toString());

    // TOOL TIP
    pButtonAddNote->setToolTip(ini.value("ToolTipAddNote").toString());
    pButtonShowNote->setToolTip(ini.value("ToolTipShowNote").toString());
    pButtonHideNote->setToolTip(ini.value("ToolTipHideNote").toString());
    pButtonMoveNoteToRecycleBin->setToolTip(ini.value("ToolTipMoveNoteToRecycleBin").toString());
    pButtonAllNotesNotes->setToolTip(ini.value("ToolTipAllNotes").toString());
    pButtonDeleteNote->setToolTip(ini.value("ToolTipDeleteNote").toString());
    pButtonRestoreNote->setToolTip(ini.value("ToolTipRestoreNote").toString());

    ini.endGroup();


    // System tray
    ini.beginGroup("SystemTray");

    actionNewNote->setText(ini.value("AddNote").toString());

    strDescriptionOfTheProgram = ini.value("DescriptionOfTheProgram").toString();
    strAllNotes = ini.value("AllNotes").toString();
    strVisibleNotes = ini.value("VisibleNotes").toString();
    strInvisibleNotes = ini.value("InvisibleNotes").toString();
    strNotesInTheRecycleBin = ini.value("NotesInTheRecycleBin").toString();

    helpMenu->setTitle(ini.value("HelpMenu").toString());
    actionHelp->setText(ini.value("Help").toString());
    actionAbout->setText(ini.value("About").toString());
    actionTrayMenuQuit->setText(ini.value("Quit").toString());
    actionTrayMenuAddNote->setText(ini.value("AddNote").toString());
    actionTrayMenuSettings->setText(ini.value("Settings").toString());
    actionTrayMenuShowHideManager->setText(ini.value("ShowHideManager").toString());
    actionNewNoteFromClipboard->setText(ini.value("AddNoteFromClipboard").toString());
    actionTrayMenuShowAllNotes->setText(ini.value("ShowAllNotes").toString());
    actionTrayMenuHideAllNotes->setText(ini.value("HideAllNotes").toString());

    actionTrayMenuActivateHotkeys->setText(ini.value("ActivateHotkeys").toString());
    actionTrayMenuDeactivateHotkeys->setText(ini.value("DeactivateHotkeys").toString());

    ini.endGroup();
    // System tray

    pButtonAllNotesRecycleBin->setToolTip(pButtonAllNotesNotes->toolTip());
}

void ManagerNote::makeDublicateNote(DataNote *dataNewNote)
{
    QString fileName = QString("%1.%2")
            .arg(currentDataTime())
            .arg(suffixFilesNote);

    QString absolutePathInFileNote = absolutePathToTheFolderNotes()
            + QDir::separator()
            + fileName;

    QTableWidget *table = mainWidget->tableWidgetNotes;
    addNewNoteInTable(absolutePathInFileNote,
                      dataNewNote,
                      table);

    emit changedTheNumberOfNotes();
}

void ManagerNote::tableCellChange(int row, int col)
{
    QTableWidget *table = mainWidget->tableWidgetNotes;
    QTableWidgetItem *item = table->item(row, indexColumnTitle);
    Note *note = item->data(pointerOnDataItemNote).value<Note*>();

    if(col == indexColumnTitle)
    {
        QString title = item->text();

        if(title == noTitle)
            title = "";
        else if(title == "")
            item->setText(noTitle);

        note->setWindowTitle(title);
    }
}

void ManagerNote::showSettingsProgram()
{
    showManager();
    showNormal();
    mainWidget->showPageSettings();
    mainWidget->settingsManager->showPageGeneral();
}

void ManagerNote::closeProgram()
{
    //
    systemTrayIcon->hide();

    //
    emit closeAllNote();

    //
    saveInIni();

    // unregister hotkey from OS
    setActivateHotkeys(false);
    // unregister hotkey from OS

    // закрываем главный поток
    qApp->quit();
}

void ManagerNote::hideManager()
{
    // прячем окно с панели задач
    QTimer::singleShot(0,
                       this,
                       SLOT(hide()));
}
void ManagerNote::showHideManager()
{
    if(isVisible())
        hideManager();
    else
        showManager();
}
void ManagerNote::setVisibleManager(bool visible)
{
    if(visible)
        showManager();
    else
        hideManager();
}

void ManagerNote::addNewNoteInTable(QString nameFileNote,
                                    QTableWidget *tableWidget)
{
    Note *newNote = new Note(nameFileNote);

    newNote->setVisibleNote(newNote->dataNote.visible);
    newNote->setLanguage(currentLanguage);
    newNote->textEdit->setCompleter(completer);
    newNote->testAutocompleterLanguage();

    QVariant itemDataNote;
    itemDataNote.setValue(newNote);

    QString temp = newNote->dataNote.title;

    QString title = newNote->dataNote.title;

    if(title.isEmpty())
        title = noTitle;

    // название noTitle будет относиться для
    // отображения в таблице
    newNote->dataNote.title = title;

    addToTheEndTablesRow(tableWidget,
                         itemDataNote,
                         &newNote->dataNote);

    // а если заголовок пуст, то и пуст он будет
    // и на самой заметке
    newNote->dataNote.title = temp;

    setAllConnectForNote(newNote);

    // обновляем количество заметок, написанных на кнопках
    // выбора таблиц
    mainWidget->refreshCountsNotesInTables();
}
void ManagerNote::addNewNoteInTable(QString nameFileNote,
                                    QString textNote,
                                    QTableWidget *tableWidget)
{
    Note *newNote = new Note(textNote,
                             nameFileNote);

    newNote->setVisibleNote(newNote->dataNote.visible);
    newNote->setLanguage(currentLanguage);
    newNote->textEdit->setCompleter(completer);
    newNote->testAutocompleterLanguage();

    QVariant itemDataNote;
    itemDataNote.setValue(newNote);

    QString temp = newNote->dataNote.title;

    QString title = newNote->dataNote.title;

    if(title.isEmpty())
        title = noTitle;

    // название noTitle будет относиться для
    // отображения в таблице
    newNote->dataNote.title = title;

    addToTheEndTablesRow(tableWidget,
                         itemDataNote,
                         &newNote->dataNote);

    // а если заголовок пуст, то и пуст он будет
    // и на самой заметке
    newNote->dataNote.title = temp;

    setAllConnectForNote(newNote);

    // обновляем количество заметок, написанных на кнопках
    // выбора таблиц
    mainWidget->refreshCountsNotesInTables();
}
void ManagerNote::addNewNoteInTable(QString nameFileNote,
                                    DataNote *dataNewNote,
                                    QTableWidget *tableWidget)
{
    Note *newNote = new Note(dataNewNote,
                             nameFileNote);
    newNote->setLanguage(currentLanguage);
    newNote->textEdit->setCompleter(completer);
    newNote->testAutocompleterLanguage();

    QVariant itemDataNote;
    itemDataNote.setValue(newNote);

    QString temp = newNote->dataNote.title;

    QString title = newNote->dataNote.title;

    if(title.isEmpty())
        title = noTitle;

    newNote->dataNote.title = title;

    addToTheEndTablesRow(tableWidget,
                         itemDataNote,
                         &newNote->dataNote);

    newNote->dataNote.title = temp;

    setAllConnectForNote(newNote);

    // обновляем количество заметок, написанных на кнопках
    // выбора таблиц
    mainWidget->refreshCountsNotesInTables();
}

void ManagerNote::openFileNote(QString pathToFile)
{
    // имя файлов заметок - текущая дата + время
    // это попытка предотвращения коллизий имен файлов
    QString name = currentDataTime();

    QString oldFileName = pathToFile;
    QString newFileName = QString("%1%2%3.%4")
            .arg(absolutePathToTheFolderNotes())
            .arg(QDir::separator())
            .arg(name)
            .arg(suffixFilesNote);

    // указываем в какую папку копировать и даем другое название
    // файла
    bool successful = QFile::copy(oldFileName,
                                  newFileName);
    // если скопировалось удачно
    if(successful)
    {
        QTableWidget *table = mainWidget->tableWidgetNotes;
        addNewNoteInTable(newFileName, table);

        emit changedTheNumberOfNotes();
    }
}

void ManagerNote::open()
{
    QString filters = QString(tr("File Note (*.%1)")
                              .arg(suffixFilesNote));

    // получаем список путей файлов
    QStringList listFileNames = QFileDialog::getOpenFileNames(this,
                                                              QString(),
                                                              QString(),
                                                              filters);
    // если не пуст
    if(!listFileNames.isEmpty())
        for(int i = 0; i < listFileNames.count(); i++)
            openFileNote(listFileNames.at(i));
}

void ManagerNote::clickRestoreThisItem(int row)
{
    QTableWidget *table = mainWidget->tableWidgetRecycleBin;

    QList <int> list;
    list.append(row);

    UDialogSelectListNotes *dialogSelectListNotes = new UDialogSelectListNotes(this);
    dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::RestoreSelectedNote);

    dialogSelectListNotes->setLanguage(currentLanguage);

    connect(this, SIGNAL(languageIsChange(QString)),
            dialogSelectListNotes, SLOT(setLanguage(QString)));

    dialogSelectListNotes->selectTableRow(table,
                                          &list);
    dialogSelectListNotes->exec();

    list = dialogSelectListNotes->getListRows();

    delete dialogSelectListNotes;

    if(!list.isEmpty())
    {
        moveRowFromTheRecycleBin(list.first());
        emit changedTheNumberOfNotes();
    }
}
void ManagerNote::clickRestoreThisItem(QString nameFile)
{
    QTableWidget *table = mainWidget->tableWidgetRecycleBin;

    for(int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem *item;
        item = table->item(i, indexMainColumn);
        QString dataNameFile = item->data(pointerOnDataItemNote).value<QString>();
        if(dataNameFile == nameFile)
        {
            int row = table->row(item);
            clickRestoreThisItem(row);
            break;
        }
    }
}

void ManagerNote::clickDeleteThisItem(int row)
{
    QTableWidget *table = mainWidget->tableWidgetRecycleBin;

    QList <int> list;
    list.append(row);

    UDialogSelectListNotes *dialogSelectListNotes = new UDialogSelectListNotes(this);
    dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::DeleteSelectedNote);

    dialogSelectListNotes->setLanguage(currentLanguage);

    connect(this, SIGNAL(languageIsChange(QString)),
            dialogSelectListNotes, SLOT(setLanguage(QString)));

    dialogSelectListNotes->selectTableRow(table, &list);
    dialogSelectListNotes->exec();

    list = dialogSelectListNotes->getListRows();

    delete dialogSelectListNotes;

    if(!list.isEmpty())
    {
        QTableWidgetItem *tableItemRecycleBin;
        tableItemRecycleBin = table->item(list.first(),
                                          indexMainColumn);
        QString nameFile = tableItemRecycleBin->data(pointerOnDataItemNote)
                .value<QString>();
        QString pathInFile = "";
        pathInFile += absolutePathToTheFolderRemoteNotes();
        pathInFile += QDir::separator();
        pathInFile += nameFile;

        QFile::remove(pathInFile);

        table->removeRow(list.first());

        mainWidget->refreshRecycleBinIcons();
        // обновляем количество заметок, написанных на кнопках
        // выбора таблиц
        mainWidget->refreshCountsNotesInTables();

        emit changedTheNumberOfNotes();
    }
}
void ManagerNote::clickDeleteThisItem(QString nameFile)
{
    QTableWidget *table = mainWidget->tableWidgetRecycleBin;

    for(int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem *item;
        item = table->item(i,
                           indexMainColumn);
        QString dataNameFile = item->data(pointerOnDataItemNote).value<QString>();
        if(dataNameFile == nameFile)
        {
            int row = table->row(item);
            clickDeleteThisItem(row);
            break;
        }
    }
}

void ManagerNote::clickRestoreItem()
{
    QTableWidget *table = mainWidget->tableWidgetRecycleBin;

    QList <int> listIndexSelectedRow = getListIndexSelectedRow(table);

    // ищем те, у которых check = true
    QList <int> list;

    for(int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem *currentItem = table->item(i, indexMainColumn);

        if(currentItem->checkState() == Qt::Checked)
            // добавляем в контейнер индекс строки
            list.append(i);
    }

    if(!list.empty())
    {
        UDialogSelectListNotes *dialogSelectListNotes = new UDialogSelectListNotes(this);

        if(list.count() == 1)
            dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::RestoreSelectedNote);
        else
            dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::RestoreSelectedNotes);

        dialogSelectListNotes->setLanguage(currentLanguage);

        connect(this, SIGNAL(languageIsChange(QString)),
                dialogSelectListNotes, SLOT(setLanguage(QString)));

        dialogSelectListNotes->selectTableRow(table, &list);
        dialogSelectListNotes->exec();

        list = dialogSelectListNotes->getListRows();

        delete dialogSelectListNotes;

        // пока контейнер не опустеет
        while(!list.isEmpty())
        {
            // получаем первый элемент из контейнера
            int row = list.at(0);
            // удаляем его
            list.removeAt(0);
            // т.к. при удалении строки в таблицы, строки находящиеся
            // после удаляемой опустятся на 1 строку, то и мы должны
            // повторить это, но для элементов контейнера, иначе, будет
            // ошибка
            for(int i = 0; i < list.size(); i++)
                list[i] = list[i] - 1;

            moveRowFromTheRecycleBin(row);
        }

        emit changedTheStatusOfNotes();

        mainWidget->refreshRecycleBinIcons();
        // обновляем количество заметок, написанных на кнопках
        // выбора таблиц
        mainWidget->refreshCountsNotesInTables();

        // если выделены строки и есть текущее выделение
    }else if(!listIndexSelectedRow.isEmpty()
             && table->currentRow() != -1)
    {
        QList <int> list = listIndexSelectedRow;

        UDialogSelectListNotes *dialogSelectListNotes = new UDialogSelectListNotes(this);

        if(list.count() == 1)
            dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::RestoreSelectedNote);
        else
            dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::RestoreSelectedNotes);

        dialogSelectListNotes->setLanguage(currentLanguage);

        connect(this, SIGNAL(languageIsChange(QString)),
                dialogSelectListNotes, SLOT(setLanguage(QString)));

        dialogSelectListNotes->selectTableRow(table, &list);
        dialogSelectListNotes->exec();

        list = dialogSelectListNotes->getListRows();

        delete dialogSelectListNotes;

        while(!list.isEmpty())
        {
            // получаем первый элемент из контейнера
            int row = list.at(0);

            // удаляем его
            list.removeAt(0);

            // т.к. при удалении строки в таблицы, строки находящиеся
            // после удаляемой опустятся на 1 строку, то и мы должны
            // повторить это, но для элементов контейнера, иначе, будет
            // ошибка
            for(int i = 0; i < list.size(); i++)
                list[i] = list[i] - 1;

            moveRowFromTheRecycleBin(row);
        }

        emit changedTheStatusOfNotes();
        mainWidget->refreshRecycleBinIcons();
        // обновляем количество заметок, написанных на кнопках
        // выбора таблиц
        mainWidget->refreshCountsNotesInTables();
    }
}

void ManagerNote::clickDeleteItem()
{
    QTableWidget *table = mainWidget->tableWidgetRecycleBin;

    QList <int> listIndexSelectedRow = getListIndexSelectedRow(table);

    // ищем те, у которых check = true
    QList <int> list;

    for(int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem *currentItem = table->item(i, indexMainColumn);
        if(currentItem->checkState() == Qt::Checked)
            // добавляем в контейнер индекс строки
            list.append(i);
    }

    if(!list.isEmpty())
    {
        UDialogSelectListNotes *dialogSelectListNotes = new UDialogSelectListNotes(this);

        if(list.count() == 1)
            dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::DeleteSelectedNote);
        else
            dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::DeleteSelectedNotes);

        dialogSelectListNotes->setLanguage(currentLanguage);

        connect(this, SIGNAL(languageIsChange(QString)),
                dialogSelectListNotes, SLOT(setLanguage(QString)));

        dialogSelectListNotes->selectTableRow(table, &list);
        dialogSelectListNotes->exec();

        list = dialogSelectListNotes->getListRows();

        delete dialogSelectListNotes;

        // пока контейнер не опустеет
        while(!list.isEmpty())
        {
            int nextRow = list.at(0);
            list.removeAt(0);

            for(int i = 0; i < list.size(); i++)
                list[i] = list[i] - 1;

            QTableWidgetItem *tableItemRecycleBin;
            tableItemRecycleBin = table->item(nextRow,
                                              indexMainColumn);

            QString nameFile = tableItemRecycleBin->data(pointerOnDataItemNote)
                    .value<QString>();

            QString pathInFile = "";
            pathInFile += absolutePathToTheFolderRemoteNotes();
            pathInFile += QDir::separator();
            pathInFile += nameFile;

            QFile::remove(pathInFile);

            table->removeRow(nextRow);
        }

        emit changedTheStatusOfNotes();

        mainWidget->refreshRecycleBinIcons();
        // обновляем количество заметок, написанных на кнопках
        // выбора таблиц
        mainWidget->refreshCountsNotesInTables();

        // если выделены строки и есть текущее выделение
    }else if(!listIndexSelectedRow.isEmpty()
             && table->currentRow() != -1)
    {
        QList <int> list = listIndexSelectedRow;

        UDialogSelectListNotes *dialogSelectListNotes = new UDialogSelectListNotes(this);

        if(list.count() == 1)
            dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::DeleteSelectedNote);
        else
            dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::DeleteSelectedNotes);

        dialogSelectListNotes->setLanguage(currentLanguage);

        connect(this, SIGNAL(languageIsChange(QString)),
                dialogSelectListNotes, SLOT(setLanguage(QString)));

        dialogSelectListNotes->selectTableRow(table, &list);
        dialogSelectListNotes->exec();

        list = dialogSelectListNotes->getListRows();

        delete dialogSelectListNotes;

        while(!list.isEmpty())
        {
            // получаем первый элемент из контейнера
            int row = list.at(0);

            // удаляем его
            list.removeAt(0);

            // т.к. при удалении строки в таблицы, строки находящиеся
            // после удаляемой опустятся на 1 строку, то и мы должны
            // повторить это, но для элементов контейнера, иначе, будет
            // ошибка
            for(int i = 0; i < list.size(); i++)
                list[i] = list[i] - 1;

            QTableWidgetItem *tableItemRecycleBin;
            tableItemRecycleBin = table->item(row, indexMainColumn);

            QString nameFile = tableItemRecycleBin->data(pointerOnDataItemNote)
                    .value<QString>();
            QString pathInFile = "";
            pathInFile += absolutePathToTheFolderRemoteNotes();
            pathInFile += QDir::separator();
            pathInFile += nameFile;

            QFile::remove(pathInFile);

            table->removeRow(row);
        }

        emit changedTheStatusOfNotes();

        mainWidget->refreshRecycleBinIcons();
        // обновляем количество заметок, написанных на кнопках
        // выбора таблиц
        mainWidget->refreshCountsNotesInTables();
    }
}

void ManagerNote::clickRestoreAllItem()
{
    QTableWidget *table = mainWidget->tableWidgetRecycleBin;

    // если есть записи в таблице
    if(table->rowCount())
    {
        QList <int> list;

        for(int i = 0; i < table->rowCount(); i++)
            list.append(i);

        UDialogSelectListNotes *dialogSelectListNotes = new UDialogSelectListNotes(this);

        if(list.count() == 1)
            dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::RestoreSelectedNote);
        else
            dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::RestoreSelectedNotes);

        dialogSelectListNotes->setLanguage(currentLanguage);

        connect(this, SIGNAL(languageIsChange(QString)),
                dialogSelectListNotes, SLOT(setLanguage(QString)));

        dialogSelectListNotes->selectTableRow(table, &list);
        dialogSelectListNotes->exec();

        list = dialogSelectListNotes->getListRows();

        delete dialogSelectListNotes;

        while(!list.isEmpty())
        {
            // получаем первый элемент из контейнера
            int row = list.at(0);
            // удаляем его
            list.removeAt(0);
            // т.к. при удалении строки в таблицы, строки находящиеся
            // после удаляемой опустятся на 1 строку, то и мы должны
            // повторить это, но для элементов контейнера, иначе, будет
            // ошибка
            for(int i = 0; i < list.size(); i++)
                list[i] = list[i] - 1;

            moveRowFromTheRecycleBin(row);
        }

        mainWidget->refreshRecycleBinIcons();
        // обновляем количество заметок, написанных на кнопках
        // выбора таблиц
        mainWidget->refreshCountsNotesInTables();

        emit changedTheNumberOfNotes();
    }
}

void ManagerNote::clickDeleteAllItem()
{
    QTableWidget *table = mainWidget->tableWidgetRecycleBin;
    // если есть записи в таблице
    if(table->rowCount())
    {
        QList <int> list;

        for(int i = 0; i < table->rowCount(); i++)
            list.append(i);

        UDialogSelectListNotes *dialogSelectListNotes = new UDialogSelectListNotes(this);
        if(list.count() == 1)
            dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::DeleteSelectedNote);
        else
            dialogSelectListNotes->setModeMessage(UDialogSelectListNotes::DeleteSelectedNotes);

        dialogSelectListNotes->setLanguage(currentLanguage);

        connect(this, SIGNAL(languageIsChange(QString)),
                dialogSelectListNotes, SLOT(setLanguage(QString)));

        dialogSelectListNotes->selectTableRow(table, &list);
        dialogSelectListNotes->exec();

        list = dialogSelectListNotes->getListRows();

        delete dialogSelectListNotes;

        while(!list.isEmpty())
        {
            // получаем первый элемент из контейнера
            int row = list.at(0);
            // удаляем его
            list.removeAt(0);
            // т.к. при удалении строки в таблицы, строки находящиеся
            // после удаляемой опустятся на 1 строку, то и мы должны
            // повторить это, но для элементов контейнера, иначе, будет
            // ошибка
            for(int i = 0; i < list.size(); i++)
                list[i] = list[i] - 1;

            // get pointer on deleted item
            QTableWidgetItem *tableItemRecycleBin;
            tableItemRecycleBin = table->item(row, indexMainColumn);
            QString nameFile = tableItemRecycleBin->data(pointerOnDataItemNote)
                    .value<QString>();

            QString pathInFile = "";
            pathInFile += absolutePathToTheFolderRemoteNotes();
            pathInFile += QDir::separator();
            pathInFile += nameFile;

            QFile::remove(pathInFile);

            table->removeRow(row);
        }
    }

    mainWidget->refreshRecycleBinIcons();
    // обновляем количество заметок, написанных на кнопках
    // выбора таблиц
    mainWidget->refreshCountsNotesInTables();

    emit changedTheNumberOfNotes();
}

void ManagerNote::messageToApp(QString message)
{
    if(message == messageFromQtSingleApplication_Show)
        showManager();
    else
    {
        // получаем список файлов, которые откроем
        QStringList listFilesNote = message.split(separatorListFilesNote);

        // открываем все файлы
        for(int i = 0; i < listFilesNote.count(); i++)
            openFileNote(listFilesNote.at(i));
    }
}

void ManagerNote::refreshToolTipSystemTrayIcon()
{
    QTableWidget *tableNotes = mainWidget->tableWidgetNotes;
    QTableWidget *tableRecycleBin = mainWidget->tableWidgetRecycleBin;

    int count = tableNotes->rowCount()
            + tableRecycleBin->rowCount();
    int countToRecycleBin = tableRecycleBin->rowCount();

    int countVisible   = 0;
    int countInvisible = 0;

    for(int i = 0; i < tableNotes->rowCount(); i++)
    {
        bool visible = tableNotes->item(i, indexColumnVisible)->data(pointerOnDataColumnVisible).toBool();

        if(visible)
            countVisible++;
        else
            countInvisible++;
    }

    systemTrayIcon->setToolTip(QString("%1\n%2\n%3\n%4\n%5\n%6")
                               .arg(nameProject
                                    + " "
                                    + versionProject)
                               .arg(strDescriptionOfTheProgram)
                               .arg(strAllNotes
                                    + " "
                                    + QString::number(count))
                               .arg(strVisibleNotes
                                    + " "
                                    + QString::number(countVisible))
                               .arg(strInvisibleNotes
                                    + " "
                                    + QString::number(countInvisible))
                               .arg(strNotesInTheRecycleBin
                                    + " "
                                    + QString::number(countToRecycleBin)));
}

void ManagerNote::setStyleSheetFromFile(QString path)
{
    QSettings confIni(absolutePathToTheConfigurationProgram(),
                      QSettings::IniFormat);
    confIni.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    confIni.setValue("Style", QFileInfo(path).baseName());
    confIni.sync();

    QFile fileStyle(path);
    fileStyle.open(QFile::ReadOnly);

    QString styleSheet = QLatin1String(fileStyle.readAll());

    QString pathToDir = QFileInfo(path).absoluteDir().path();

    // заменяем метки на абсолютные пути
    styleSheet.replace("$$PATH_TO_DIR", pathToDir);

    setStyleSheet(styleSheet);
}

void ManagerNote::changeTrueFalseValueCells(int row, int col)
{
    QTableWidget *table = mainWidget->tableWidgetNotes;
    QTableWidgetItem *item = table->item(row, col);
    QTableWidgetItem *itemMain = table->item(row, indexMainColumn);
    Note *note = itemMain->data(pointerOnDataItemNote).value<Note*>();

    if(col == indexColumnVisible)
    {
        bool visible = item->data(pointerOnDataColumnVisible).toBool();
        visible = !visible;

        item->setData(pointerOnDataColumnVisible, visible);

        if(visible)
            item->setIcon(QIcon(IconShow));
        else
            item->setIcon(QIcon(IconHide));

        note->setVisibleNote(visible);

        emit changedTheStatusOfNotes();

    }else if(col == indexColumnLock)
    {
        bool lock = item->data(pointerOnDataColumnLock).toBool();
        lock = !lock;

        item->setData(pointerOnDataColumnLock, lock);

        if(lock)
            item->setIcon(QIcon(IconLock));
        else
            item->setIcon(QIcon(IconUnlock));

        note->setLock(lock);

        emit changedTheStatusOfNotes();
    }
}

void ManagerNote::setActivateHotkeys(bool activate)
{
    actionTrayMenuActivateHotkeys->setVisible(!activate);
    actionTrayMenuDeactivateHotkeys->setVisible(activate);

    if(activate)
    {
        USettingsManager *settings = mainWidget->settingsManager;
        UWidgetSelectHotkey *hotkeyNewNote = settings->hotkeyNewNote;
        UWidgetSelectHotkey *hotkeyNewNoteFromClipboard = settings->hotkeyNewNoteFromClipboard;
        UWidgetSelectHotkey *hotkeyShowAllNote = settings->hotkeyShowAllNote;
        UWidgetSelectHotkey *hotkeyHideAllNote = settings->hotkeyHideAllNote;
        UWidgetSelectHotkey *hotkeyShowHideManager = settings->hotkeyShowHideManager;

        QString strHotkeyNewNote = hotkeyNewNote->getHotkey();
        QString strHotkeyNewNoteFromClipboard = hotkeyNewNoteFromClipboard->getHotkey();
        QString strHotkeyShowAllNote = hotkeyShowAllNote->getHotkey();
        QString strHotkeyHideAllNote = hotkeyHideAllNote->getHotkey();
        QString strHotkeyShowHideManager = hotkeyShowHideManager->getHotkey();

        // регистрация горячих клавиш в ОС и привязка их к
        // окну менеджера
        registerHotKey(strHotkeyNewNote,
                       hotkeyActionNewNote);

        registerHotKey(strHotkeyNewNoteFromClipboard,
                       hotkeyActionNewNoteFromClipboard);

        registerHotKey(strHotkeyShowAllNote,
                       hotkeyActionShowAllNote);

        registerHotKey(strHotkeyHideAllNote,
                       hotkeyActionHideAllNote);

        registerHotKey(strHotkeyShowHideManager,
                       hotkeyActionShowHideManager);


        actionTrayMenuAddNote->setShortcut(strHotkeyNewNote);
        actionNewNoteFromClipboard->setShortcut(strHotkeyNewNoteFromClipboard);
        actionTrayMenuShowAllNotes->setShortcut(strHotkeyShowAllNote);
        actionTrayMenuHideAllNotes->setShortcut(strHotkeyHideAllNote);
        actionTrayMenuShowHideManager->setShortcut(strHotkeyShowHideManager);

    }else
    {
        // Уберем регистрацию горячих клавиш в ОС
        unregisterHotKey(hotkeyActionNewNote);
        unregisterHotKey(hotkeyActionNewNoteFromClipboard);
        unregisterHotKey(hotkeyActionHideAllNote);
        unregisterHotKey(hotkeyActionShowAllNote);
        unregisterHotKey(hotkeyActionShowHideManager);

        // Уберем отображение горячих клавиш в пунктах меню трея
        actionTrayMenuAddNote->setShortcut(0);
        actionNewNoteFromClipboard->setShortcut(0);
        actionTrayMenuShowAllNotes->setShortcut(0);
        actionTrayMenuHideAllNotes->setShortcut(0);
        actionTrayMenuShowHideManager->setShortcut(0);
    }
}
void ManagerNote::registerHotKey(QString hotkey, int idHotkey)
{
    int modifier = 0;
    int key = 0;

    QStringList listHotKey = hotkey.split(separatorHotkey);

    for(int i = 0; i < listHotKey.count(); i++)
    {
        if(listHotKey.at(i) == "Ctrl")
            modifier |= MOD_CONTROL;
        else if(listHotKey.at(i) == "Shift")
            modifier |= MOD_SHIFT;
        else if(listHotKey.at(i) == "Alt")
            modifier |= MOD_ALT;
        else
            key = getIntKeyA_Z(listHotKey.at(i));
    }

#ifdef Q_WS_WIN
    RegisterHotKey(winId(), idHotkey,
                   modifier, key);
#endif
}
void ManagerNote::unregisterHotKey(int idHotkey)
{
#ifdef Q_WS_WIN
    UnregisterHotKey(winId(), idHotkey);
#endif
}
void ManagerNote::acceptHotkeys()
{
    USettingsManager *settings = mainWidget->settingsManager;
    UWidgetSelectHotkey *hotkeyNewNote = settings->hotkeyNewNote;
    UWidgetSelectHotkey *hotkeyNewNoteFromClipboard = settings->hotkeyNewNoteFromClipboard;
    UWidgetSelectHotkey *hotkeyShowAllNote = settings->hotkeyShowAllNote;
    UWidgetSelectHotkey *hotkeyHideAllNote = settings->hotkeyHideAllNote;
    UWidgetSelectHotkey *hotkeyShowHideManager = settings->hotkeyShowHideManager;

    QString strHotkeyNewNote = hotkeyNewNote->getHotkey();
    QString strHotkeyNewNoteFromClipboard = hotkeyNewNoteFromClipboard->getHotkey();
    QString strHotkeyShowAllNote = hotkeyShowAllNote->getHotkey();
    QString strHotkeyHideAllNote = hotkeyHideAllNote->getHotkey();
    QString strHotkeyShowHideManager = hotkeyShowHideManager->getHotkey();

    // регистрация горячих клавиш в ОС и привязка их к
    // окну менеджера
    registerHotKey(strHotkeyNewNote,
                   hotkeyActionNewNote);

    registerHotKey(strHotkeyNewNoteFromClipboard,
                   hotkeyActionNewNoteFromClipboard);

    registerHotKey(strHotkeyShowAllNote,
                   hotkeyActionShowAllNote);

    registerHotKey(strHotkeyHideAllNote,
                   hotkeyActionHideAllNote);

    registerHotKey(strHotkeyShowHideManager,
                   hotkeyActionShowHideManager);


    actionTrayMenuAddNote->setShortcut(strHotkeyNewNote);
    actionNewNoteFromClipboard->setShortcut(strHotkeyNewNoteFromClipboard);
    actionTrayMenuShowAllNotes->setShortcut(strHotkeyShowAllNote);
    actionTrayMenuHideAllNotes->setShortcut(strHotkeyHideAllNote);
    actionTrayMenuShowHideManager->setShortcut(strHotkeyShowHideManager);

    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("Hotkeys");

    ini.setValue("NewNote", strHotkeyNewNote);
    ini.setValue("NewNoteFromClipboard", strHotkeyNewNoteFromClipboard);
    ini.setValue("ShowAllNotes", strHotkeyShowAllNote);
    ini.setValue("HideAllNotes", strHotkeyHideAllNote);
    ini.setValue("ShowHideManager", strHotkeyShowHideManager);

    ini.endGroup();

    ini.sync();
}

/// PRIVATE SLOTS
void ManagerNote::propertiesNoteTextIsChange(Note *note)
{
    QTableWidget *table = mainWidget->tableWidgetNotes;

    for(int i = 0; i < table->rowCount(); i++)
    {
        QTableWidgetItem *item = table->item(i, indexMainColumn);
        Note *itemNote = item->data(pointerOnDataItemNote).value <Note *> ();

        if(itemNote == note
                && note)
        {
            int row = table->row(item);

            QTableWidgetItem *itemCountTextSymbols;
            itemCountTextSymbols = table->item(row, indexColumnCountTextSymbols);

            QTableWidgetItem *itemCountTextLines;
            itemCountTextLines = table->item(row, indexColumnCountTextLines);

            QTableWidgetItem *itemDataLastChange;
            itemDataLastChange = table->item(row, indexColumnDateLastChange);

            QString countSymbols = QString::number(note->textEdit->getCountTextSymbols());
            QString countLines   = QString::number(note->textEdit->getCountTextLines());

            itemCountTextSymbols->setText(countSymbols);
            itemCountTextLines->setText(countLines);
            itemDataLastChange->setText(note->dataNote.dateLastChange);
            break;
        }
    }
}

void ManagerNote::about()
{
    // if aboutProgram is not create then create
    if(!aboutProgram)
    {
        aboutProgram = new UAboutProgram();
        aboutProgram->addTranslations(getListTranslaction());
        aboutProgram->setLanguage(currentLanguage);

        connect(this, SIGNAL(languageIsChange(QString)),
                aboutProgram, SLOT(setLanguage(QString)));
        connect(aboutProgram, SIGNAL(iClose()),
                this, SLOT(deleteDialogAbout()));

        aboutProgram->show();

    }else
        showForegroundWindow(aboutProgram);
}
void ManagerNote::deleteDialogAbout()
{
    if(aboutProgram)
    {
        delete aboutProgram;
        aboutProgram = 0;
    }
}

void ManagerNote::help()
{
    HelpBrowser *helpBrowser = new HelpBrowser(":/resource/help/", "index.html");
    helpBrowser->setWindowTitle(QString(tr("Help")));
    helpBrowser->setWindowIcon(QIcon(IconHelp));
    helpBrowser->resize(400, 300);
    helpBrowser->show();

    QWebView *webView = new QWebView();
    webView->setWindowTitle(QString(tr("Help")));
    webView->setWindowIcon(QIcon(IconHelp));
//    webView->resize(400, 300);

    webView->show();

    webView->setUrl(QUrl::fromLocalFile(":resource/help/index.html"));

//    webView->setUrl(QUrl("file://resource/help/index.html"));

//    webVie
}

void ManagerNote::showDialogFindNotes()
{
    if(!findNote)
    {
        //-- создадим и настроим окно поиска заметок
        findNote = new UWidgetFindNote();
        findNote->show();
        findNote->setLanguage(currentLanguage);
        findNote->setLinkInputOutput(mainWidget->tableWidgetNotes,
                                     mainWidget->tableWidgetRecycleBin,
                                     mainWidget->areaResultsTable->tableNotes,
                                     mainWidget->areaResultsTable->tableRecycleBin);
        connect(this, SIGNAL(languageIsChange(QString)),
                findNote, SLOT(setLanguage(QString)));

        connect(findNote, SIGNAL(searchIsComplete()),
                mainWidget, SLOT(findIsEnd()));
        connect(findNote, SIGNAL(iClose()),
                this, SLOT(deleteDialogFindNotes()));
        connect(mainWidget->areaResultsTable, SIGNAL(refreshTables()),
                findNote, SLOT(refreshFind()));
        //--

    }else
        showForegroundWindow(findNote);
}

void ManagerNote::deleteDialogFindNotes()
{
    delete findNote;
    findNote = 0;
}

/// PROTECTED
void ManagerNote::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::WindowStateChange)
    {
        // if manager is maximized when size is no save
        managerIsMaximized = isMaximized();

        QSettings ini(absolutePathToTheConfigurationProgram(),
                      QSettings::IniFormat);
        ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

        ini.beginGroup("Manager");

        ini.setValue("Maximized", isMaximized());

        if(isMinimized())
        {          
            bool minimizeToTray = ini.value("MinimizeToTrayManagerAtMinimize", false).toBool();

            if(minimizeToTray)
            {
                // прячем окно с панели задач
                hideManager();

                event->ignore();
            }
        }

        ini.endGroup();

        ini.sync();
    }
}

void ManagerNote::closeEvent(QCloseEvent *event)
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    bool minimizeToTray = ini.value("Manager/MinimizeToTrayManagerAtClosing",
                                    isMinimizeToTrayManagerAtClosing).toBool();

    if(minimizeToTray)
    {
        // прячем окно с панели задач
        hideManager();

        event->ignore();

    }else
        closeProgram();
}

void ManagerNote::dropEvent(QDropEvent *dropEvent)
{
    // если выбрана вкладка редактора заметок и
    // принимаем только адреса
    if(mainWidget->contentsWidget->currentItem() == mainWidget->lWidgetItemNotes
            && dropEvent->mimeData()->hasUrls())
    {
        // получаем список адресов
        QList <QUrl> list = dropEvent->mimeData()->urls();

        for(int i = 0; i < list.size(); i++)
        {
            QUrl urlDropFile = list.at(i);
            if(urlDropFile.isLocalFile())
            {
                QString oldfileName = urlDropFile.toLocalFile();

                if(QFileInfo(oldfileName).suffix() == suffixFilesNote)
                {
                    QString newFileName = QString("%1%2%3.%4")
                            .arg(absolutePathToTheFolderNotes())
                            .arg(QDir::separator())
                            .arg(currentDataTime())
                            .arg(suffixFilesNote);

                    // указываем в какую папку копировать и даем другое название
                    // файла
                    bool successful = QFile::copy(oldfileName,
                                                  newFileName);
                    // если скопировалось удачно
                    if(successful)
                    {
                        QTableWidget *table = mainWidget->tableWidgetNotes;
                        addNewNoteInTable(QFileInfo(newFileName).absoluteFilePath(),
                                          table);

                        emit changedTheNumberOfNotes();
                    }
                }
            }
        }
    }

    dropEvent->acceptProposedAction();
}

void ManagerNote::dragEnterEvent(QDragEnterEvent *dragEnter)
{
    if (dragEnter->mimeData()->hasUrls()
            && mainWidget->contentsWidget->currentItem() == mainWidget->lWidgetItemNotes)

        dragEnter->acceptProposedAction();
}

void ManagerNote::resizeEvent(QResizeEvent *event)
{
    if(!managerIsMaximized)
    {
        QSettings ini(absolutePathToTheConfigurationProgram(),
                      QSettings::IniFormat);

        ini.beginGroup("Manager");

        ini.setValue("PositionX", geometry().x());
        ini.setValue("PositionY", geometry().y());
        ini.setValue("Width",     event->size().width());
        ini.setValue("Height",    event->size().height());

        ini.endGroup();

        ini.sync();
    }
}

void ManagerNote::moveEvent(QMoveEvent *event)
{
    if(!managerIsMaximized)
    {
        QSettings ini(absolutePathToTheConfigurationProgram(),
                      QSettings::IniFormat);

        ini.beginGroup("Manager");

        ini.setValue("PositionX", event->pos().x());
        ini.setValue("PositionY", event->pos().y());

        ini.endGroup();

        ini.sync();
    }
}

#ifdef Q_WS_WIN
bool ManagerNote::winEvent(MSG *message, long *result)
{
    // отловили событие нажатия
    if(message->message == WM_HOTKEY)
    {
        if(message->wParam == hotkeyActionNewNote)
            addNewNote();

        else if(message->wParam == hotkeyActionNewNoteFromClipboard)
            addNewNoteFromClipboard();

        else if(message->wParam == hotkeyActionHideAllNote)
            clickButtonHideAllNote();

        else if(message->wParam == hotkeyActionShowAllNote)
            clickButtonShowAllNote();

        else if(message->wParam == hotkeyActionShowHideManager)
            showHideManager();

        return false;
    }

    return false;
}
#endif

void ManagerNote::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        if(mainWidget->currentPageManager() == PageNotes)
            clickButtonMoveNoteToRecycleBin();

        else if(mainWidget->currentPageManager() == PageRecycleBin)
            clickDeleteItem();
    }
}
