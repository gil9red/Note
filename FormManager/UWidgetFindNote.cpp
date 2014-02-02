#include "UWidgetFindNote.h"

/// PUBLIC
UWidgetFindNote::UWidgetFindNote(QWidget *parent):
    QWidget(parent)
{
    setWindowFlags(Qt::Tool);

    createWidgets();

    tableInputNotes       = 0;
    tableInputRecycleBin  = 0;
    tableOutputNotes      = 0;
    tableOutputRecycleBin = 0;
}

void UWidgetFindNote::setLinkInputOutput(QTableWidget *tInputNotes,
                                         QTableWidget *tInputRecycleBin,
                                         QTableWidget *tOutputNotes,
                                         QTableWidget *tOutputRecycleBin)
{
    tableInputNotes  = tInputNotes;
    tableInputRecycleBin = tInputRecycleBin;
    tableOutputNotes = tOutputNotes;
    tableOutputRecycleBin = tOutputRecycleBin;
}

/// PRIVATE
void UWidgetFindNote::createWidgets()
{
    labelInputFindText = new QLabel();


    QPixmap pixmap_enter(IconClearTextEnter);
    QPixmap pixmap_leave(IconClearTextLeave);

    int widthIcon = 20;
    int heightIcon = 20;
    pixmap_enter = pixmap_enter.scaled(widthIcon, heightIcon, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pixmap_leave = pixmap_leave.scaled(widthIcon, heightIcon, Qt::KeepAspectRatio, Qt::SmoothTransformation);


    lineEdit = new ULineEditWithClearButton();
    lineEdit->setIconButton(pixmap_leave);
    lineEdit->setIconButton(ULineEditWithClearButton::EnterEvent,
                            pixmap_enter);
    lineEdit->setIconButton(ULineEditWithClearButton::LeaveEvent,
                            pixmap_leave);

    lineEdit->setMinimumWidth(100);
    connect(lineEdit, SIGNAL(returnPressed()),
            this, SLOT(find()));

    hBoxLineEditFind = new QHBoxLayout();
    hBoxLineEditFind->addWidget(labelInputFindText);
    hBoxLineEditFind->addWidget(lineEdit);

    pButtonFind = new QPushButton();
    pButtonClose = new QPushButton();
    pButtonCount = new QPushButton();

    connect(pButtonFind, SIGNAL(clicked()),
            this, SLOT(find()));
    connect(pButtonClose, SIGNAL(clicked()),
            this, SLOT(close()));
    connect(pButtonCount, SIGNAL(clicked()),
            this, SLOT(count()));

    vBoxLayoutCommandButtons = new QVBoxLayout();
    vBoxLayoutCommandButtons->addWidget(pButtonFind);
    vBoxLayoutCommandButtons->addWidget(pButtonCount);
    vBoxLayoutCommandButtons->addWidget(pButtonClose);
    vBoxLayoutCommandButtons->addStretch();

    checkBoxCaseSensitive = new QCheckBox();
    checkBoxWholeWordsOnly = new QCheckBox();


    gBoxLayoutGroupBoxFindSettings = new QGridLayout();
    gBoxLayoutGroupBoxFindSettings->addWidget(checkBoxCaseSensitive,  0, 0);

    groupBoxFindSettings = new QGroupBox();
    groupBoxFindSettings->setLayout(gBoxLayoutGroupBoxFindSettings);

    // SearchIn
    checkBoxNotes = new QCheckBox();
    checkBoxNotes->setChecked(true);

    checkBoxRecycleBin = new QCheckBox();

    connect(checkBoxNotes, SIGNAL(clicked()),
            this, SLOT(clickCheckBoxNotes()));
    connect(checkBoxRecycleBin, SIGNAL(clicked()),
            this, SLOT(clickCheckBoxRecycleBin()));

    vBoxLayoutGroupBoxSearchIn = new QVBoxLayout();
    vBoxLayoutGroupBoxSearchIn->addWidget(checkBoxNotes);
    vBoxLayoutGroupBoxSearchIn->addWidget(checkBoxRecycleBin);

    groupBoxSearchIn = new QGroupBox();
    groupBoxSearchIn->setLayout(vBoxLayoutGroupBoxSearchIn);
    // SearchIn

    // ”читывать видимость заметки
    rButtonNoteVisibleTrue  = new QRadioButton();
    rButtonNoteVisibleTrue->setChecked(true);

    rButtonNoteVisibleFalse = new QRadioButton();

    vBoxLayoutGroupBoxConsiderVisibilityNotes = new QVBoxLayout();
    vBoxLayoutGroupBoxConsiderVisibilityNotes->addWidget(rButtonNoteVisibleTrue);
    vBoxLayoutGroupBoxConsiderVisibilityNotes->addWidget(rButtonNoteVisibleFalse);

    groupBoxConsiderVisibilityNotes = new QGroupBox();
    groupBoxConsiderVisibilityNotes->setCheckable(true);
    groupBoxConsiderVisibilityNotes->setChecked(false);
    groupBoxConsiderVisibilityNotes->setLayout(vBoxLayoutGroupBoxConsiderVisibilityNotes);
    // ”читывать видимость заметки

    // ”читывать блокировку заметки
    rButtonNoteLock  = new QRadioButton();

    rButtonNoteUnlock = new QRadioButton();
    rButtonNoteUnlock->setChecked(true);

    vBoxLayoutGroupBoxConsiderLockNotes = new QVBoxLayout();
    vBoxLayoutGroupBoxConsiderLockNotes->addWidget(rButtonNoteLock);
    vBoxLayoutGroupBoxConsiderLockNotes->addWidget(rButtonNoteUnlock);

    groupBoxConsiderLockNotes = new QGroupBox();
    groupBoxConsiderLockNotes->setCheckable(true);
    groupBoxConsiderLockNotes->setChecked(false);
    groupBoxConsiderLockNotes->setLayout(vBoxLayoutGroupBoxConsiderLockNotes);
    // ”читывать блокировку заметки

    pButtonShowHideAdditionalSearchSettings = new QPushButton();
    pButtonShowHideAdditionalSearchSettings->setCheckable(true);
    pButtonShowHideAdditionalSearchSettings->setChecked(false);

    connect(pButtonShowHideAdditionalSearchSettings, SIGNAL(clicked()),
            this, SLOT(clickButtonShowHideAdditionalSearchSettings()));

    setTheVisibleForMoreSearchOptions(false);
    setTheCheckedForMoreSearchOptions(false);

    QVBoxLayout *vBoxLayoutSector1 = new QVBoxLayout();
    vBoxLayoutSector1->addLayout(hBoxLineEditFind);

    vBoxLayoutSector1->addSpacing(32);
    QHBoxLayout *hBoxLayoutButtonShowHide = new QHBoxLayout();
    hBoxLayoutButtonShowHide->addWidget(pButtonShowHideAdditionalSearchSettings);
    hBoxLayoutButtonShowHide->addStretch();
    vBoxLayoutSector1->addLayout(hBoxLayoutButtonShowHide);
    vBoxLayoutSector1->addWidget(groupBoxFindSettings);
    vBoxLayoutSector1->addWidget(groupBoxSearchIn);

    vBoxLayoutSector1->addWidget(groupBoxConsiderVisibilityNotes);
    vBoxLayoutSector1->addWidget(groupBoxConsiderLockNotes);
    vBoxLayoutSector1->addStretch();

    QHBoxLayout *hBoxLayoutSector2 = new QHBoxLayout();
    hBoxLayoutSector2->addLayout(vBoxLayoutCommandButtons);

    QHBoxLayout *hBoxLayoutMain = new QHBoxLayout();
    hBoxLayoutMain->addLayout(vBoxLayoutSector1);
    hBoxLayoutMain->addLayout(hBoxLayoutSector2);

    vBoxLayoutMain = new QVBoxLayout();
    vBoxLayoutMain->addLayout(hBoxLayoutMain);

    setLayout(vBoxLayoutMain);
}

/// PUBLIC SLOTS
void UWidgetFindNote::setLanguage(QString pathToTheFileLanguage)
{
    QSettings ini(pathToTheFileLanguage,
                       QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("WidgetFindNote");

    setWindowTitle(ini.value("Title").toString());

    labelInputFindText->setText(ini.value("SearchByTitle").toString());
    pButtonFind->setText(ini.value("Find").toString());
    pButtonClose->setText(ini.value("Close").toString());
    pButtonCount->setText(ini.value("Count").toString());
    checkBoxCaseSensitive->setText(ini.value("CaseSensitive").toString());
    checkBoxWholeWordsOnly->setText(ini.value("WholeWordsOnly").toString());
    groupBoxFindSettings->setTitle(ini.value("FindSettings").toString());

    groupBoxSearchIn->setTitle(ini.value("SearchIn").toString());
    checkBoxNotes->setText(ini.value("Notes").toString());
    checkBoxRecycleBin->setText(ini.value("RecycleBin").toString());

    strMsgCountTitle = ini.value("Count").toString();
    strMsgCountText  = ini.value("TextCount").toString();


    groupBoxConsiderVisibilityNotes->setTitle(ini.value("ConsiderVisibilityNotes").toString());
    rButtonNoteVisibleTrue->setText(ini.value("Visible").toString());
    rButtonNoteVisibleFalse->setText(ini.value("Invisible").toString());

    groupBoxConsiderLockNotes->setTitle(ini.value("ConsiderLockNotes").toString());
    rButtonNoteLock->setText(ini.value("Locked").toString());
    rButtonNoteUnlock->setText(ini.value("Unlocked").toString());

    strHideAdditionalSearchSettings = ini.value("HideAdditionalSearchSettings").toString();
    strShowAdditionalSearchSettings = ini.value("ShowAdditionalSearchSettings").toString();

    ini.endGroup();

    pButtonShowHideAdditionalSearchSettings->setText(strShowAdditionalSearchSettings);
}

void UWidgetFindNote::find()
{
    find(lineEdit->text());
}

void UWidgetFindNote::find(QString searchText)
{
    // индикатор
    QProgressIndicator *progressIndicator = new QProgressIndicator();

    vBoxLayoutMain->addWidget(progressIndicator,
                              0,
                              Qt::AlignLeft);
    progressIndicator->startAnimation();
    pButtonFind->setEnabled(false);
    pButtonCount->setEnabled(false);


    /// Ќачинаетс€ код поиска
    // учитывать при поискe регист
    bool caseSensitive    = checkBoxCaseSensitive->isChecked();

    // поиск в определенных группах
    bool findInNotes      = checkBoxNotes->isChecked();
    bool findInRecycleBin = checkBoxRecycleBin->isChecked();

    // учитывать видимость заметок
    bool considerVisibilityNotes = groupBoxConsiderVisibilityNotes->isChecked();
    bool considerLockNotes = groupBoxConsiderLockNotes->isChecked();

    lastFindText = searchText;

    // очистим таблицу, метод clear() слишком
    // брутальный - удалит еще и заголовки таблицы,
    // а их не восстановишь здесь
    tableOutputNotes->setRowCount(0);
    tableOutputRecycleBin->setRowCount(0);

    if(findInNotes)
    {
        // ищем в таблице заметок
        for(int i = 0; i < tableInputNotes->rowCount(); i++)
        {
            qApp->processEvents();

            // ѕолучаем заголовок заметки исходной таблицы
            QString tableText  = tableInputNotes->item(i, indexColumnTitle)->text();

            bool bValueResult = false;

            if(caseSensitive)
                bValueResult = tableText.contains(searchText,
                                                  Qt::CaseSensitive);
            else
                bValueResult = tableText.contains(searchText,
                                                  Qt::CaseInsensitive);

            if(considerVisibilityNotes)
            {
                bool bValue;

                QTableWidgetItem *item = tableInputNotes->item(i, indexColumnVisible);

                if(rButtonNoteVisibleTrue->isChecked())
                    bValue = (true == item->data(pointerOnDataColumnVisible).toBool());
                else
                    bValue = (false == item->data(pointerOnDataColumnVisible).toBool());

                bValueResult = bValueResult & bValue;
            }

            if(considerLockNotes)
            {
                bool bValue;

                QTableWidgetItem *item = tableInputNotes->item(i, indexColumnLock);

                if(rButtonNoteLock->isChecked())
                    bValue = (true == item->data(pointerOnDataColumnLock).toBool());
                else
                    bValue = (false == item->data(pointerOnDataColumnLock).toBool());

                bValueResult = bValueResult & bValue;
            }

            // если нашли, добавл€ем в таблицу найденных заметок
            if(bValueResult)
            {
                // добавл€ем строку
                tableOutputNotes->setRowCount(tableOutputNotes->rowCount() + 1);

                int row = tableOutputNotes->rowCount() - 1;

                for(int j = 0; j < tableInputNotes->columnCount(); j++)
                {
                    qApp->processEvents();

                    tableOutputNotes->setItem(row, j,
                                              tableInputNotes->item(i, j)->clone());
                }

                // убираем флажки у строк
                tableOutputNotes->item(row, indexMainColumn)->setCheckState(Qt::Unchecked);
            }
        }
    }

    if(findInRecycleBin)
    {
        // ищем в таблице корзина
        for(int i = 0; i < tableInputRecycleBin->rowCount(); i++)
        {
            qApp->processEvents();

            // ѕолучаем заголовок заметки исходной таблицы
            QString tableText = tableInputRecycleBin->item(i, indexColumnTitle)->text();

            bool bValueResult = false;

            if(caseSensitive)
                bValueResult = tableText.contains(searchText,
                                                  Qt::CaseSensitive);
            else
                bValueResult = tableText.contains(searchText,
                                                  Qt::CaseInsensitive);

            if(considerVisibilityNotes)
            {
                bool bValue;

                QTableWidgetItem *item = tableInputRecycleBin->item(i, indexColumnVisible);

                if(rButtonNoteVisibleTrue->isChecked())
                    bValue = (true == item->data(pointerOnDataColumnVisible).toBool());
                else
                    bValue = (false == item->data(pointerOnDataColumnVisible).toBool());

                bValueResult = bValueResult & bValue;
            }

            if(considerLockNotes)
            {
                bool bValue;

                QTableWidgetItem *item = tableInputRecycleBin->item(i, indexColumnLock);

                if(rButtonNoteLock->isChecked())
                    bValue = (true == item->data(pointerOnDataColumnLock).toBool());
                else
                    bValue = (false == item->data(pointerOnDataColumnLock).toBool());

                bValueResult = bValueResult & bValue;
            }

            // если нашли, добавл€ем в таблицу найденных заметок
            if(bValueResult)
            {
                // добавл€ем строку
                tableOutputRecycleBin->setRowCount(tableOutputRecycleBin->rowCount() + 1);
                int row = tableOutputRecycleBin->rowCount() - 1;

                for(int j = 0; j < tableInputRecycleBin->columnCount(); j++)
                {
                    tableOutputRecycleBin->setItem(row, j,
                                                   tableInputRecycleBin->item(i, j)->clone());
                }
            }
        }
    }

    emit searchIsComplete();
    /// «акончен код поиска


    progressIndicator->stopAnimation();

    delete progressIndicator;

    pButtonFind->setEnabled(true);
    pButtonCount->setEnabled(true);
}

void UWidgetFindNote::count()
{    
    int findCount = count(lineEdit->text());    

    QMessageBox msg;
    msg.setWindowTitle(strMsgCountTitle);
    msg.setText(strMsgCountText.arg(findCount));
    msg.addButton(QMessageBox::Ok);
    msg.exec();
}
int UWidgetFindNote::count(QString searchText)
{
    // индикатор
    QProgressIndicator *progressIndicator = new QProgressIndicator();

    vBoxLayoutMain->addWidget(progressIndicator,
                              0,
                              Qt::AlignLeft);
    progressIndicator->startAnimation();
    pButtonFind->setEnabled(false);
    pButtonCount->setEnabled(false);


    /// Ќачинаетс€ код поиска
    int nCount = 0;

    // учитывать при поискe регист
    bool caseSensitive    = checkBoxCaseSensitive->isChecked();

    // поиск в определенных группах
    bool findInNotes      = checkBoxNotes->isChecked();
    bool findInRecycleBin = checkBoxRecycleBin->isChecked();

    // учитывать видимость заметок
    bool considerVisibilityNotes = groupBoxConsiderVisibilityNotes->isChecked();
    // учитывать блокировку заметок
    bool considerLockNotes = groupBoxConsiderLockNotes->isChecked();

    if(findInNotes)
    {
        // ищем в таблице заметок
        for(int i = 0; i < tableInputNotes->rowCount(); i++)
        {
            qApp->processEvents();

            // ѕолучаем заголовок заметки исходной таблицы
            QString tableText  = tableInputNotes->item(i, indexColumnTitle)->text();

            bool bValueResult = false;

            if(caseSensitive)
                bValueResult = tableText.contains(searchText,
                                                  Qt::CaseSensitive);
            else
                bValueResult = tableText.contains(searchText,
                                                  Qt::CaseInsensitive);

            if(considerVisibilityNotes)
            {
                bool bValue;

                QTableWidgetItem *item = tableInputNotes->item(i, indexColumnVisible);

                if(rButtonNoteVisibleTrue->isChecked())
                    bValue = (true == item->data(pointerOnDataColumnVisible).toBool());
                else
                    bValue = (false == item->data(pointerOnDataColumnVisible).toBool());

                bValueResult = bValueResult & bValue;
            }

            if(considerLockNotes)
            {
                bool bValue;

                QTableWidgetItem *item = tableInputNotes->item(i, indexColumnLock);

                if(rButtonNoteLock->isChecked())
                    bValue = (true == item->data(pointerOnDataColumnLock).toBool());
                else
                    bValue = (false == item->data(pointerOnDataColumnLock).toBool());

                bValueResult = bValueResult & bValue;
            }

            // если нашли: счетчик + 1
            if(bValueResult)
                nCount++;
        }
    }

    if(findInRecycleBin)
    {
        // ищем в таблице корзина
        for(int i = 0; i < tableInputRecycleBin->rowCount(); i++)
        {
            qApp->processEvents();

            // ѕолучаем заголовок заметки исходной таблицы
            QString tableText = tableInputRecycleBin->item(i, indexColumnTitle)->text();

            bool bValueResult = false;

            if(caseSensitive)
                bValueResult = tableText.contains(searchText,
                                                  Qt::CaseSensitive);
            else
                bValueResult = tableText.contains(searchText,
                                                  Qt::CaseInsensitive);

            if(considerVisibilityNotes)
            {
                bool bValue;

                QTableWidgetItem *item = tableInputRecycleBin->item(i, indexColumnVisible);

                if(rButtonNoteVisibleTrue->isChecked())
                    bValue = (true == item->data(pointerOnDataColumnVisible).toBool());
                else
                    bValue = (false == item->data(pointerOnDataColumnVisible).toBool());

                bValueResult = bValueResult & bValue;
            }

            if(considerLockNotes)
            {
                bool bValue;

                QTableWidgetItem *item = tableInputRecycleBin->item(i, indexColumnLock);

                if(rButtonNoteLock->isChecked())
                    bValue = (true == item->data(pointerOnDataColumnLock).toBool());
                else
                    bValue = (false == item->data(pointerOnDataColumnLock).toBool());

                bValueResult = bValueResult & bValue;
            }

            // если нашли: счетчик + 1
            if(bValueResult)
                nCount++;
        }
    }
    /// «акончен код поиска


    progressIndicator->stopAnimation();

    delete progressIndicator;

    pButtonFind->setEnabled(true);
    pButtonCount->setEnabled(true);

    return nCount;
}

void UWidgetFindNote::refreshFind()
{
    // индикатор
    QProgressIndicator *progressIndicator = new QProgressIndicator();

    vBoxLayoutMain->addWidget(progressIndicator,
                              0,
                              Qt::AlignLeft);
    progressIndicator->startAnimation();
    pButtonFind->setEnabled(false);
    pButtonCount->setEnabled(false);

    find(lastFindText);

    progressIndicator->stopAnimation();

    delete progressIndicator;

    pButtonFind->setEnabled(true);
    pButtonCount->setEnabled(true);
}

/// PRIVATE SLOTS
void UWidgetFindNote::clickCheckBoxNotes()
{
    if(!checkBoxRecycleBin->isChecked()
            && !checkBoxNotes->isChecked())
        checkBoxNotes->setChecked(true);
}
void UWidgetFindNote::clickCheckBoxRecycleBin()
{
    if(!checkBoxRecycleBin->isChecked()
            && !checkBoxNotes->isChecked())
        checkBoxRecycleBin->setChecked(true);
}

void UWidgetFindNote::setVisibleConsiderVisibilityNotes(bool visible)
{
    rButtonNoteVisibleTrue->setVisible(visible);
    rButtonNoteVisibleFalse->setVisible(visible);
}
void UWidgetFindNote::setVisibleConsiderLockNotes(bool visible)
{
    rButtonNoteLock->setVisible(visible);
    rButtonNoteUnlock->setVisible(visible);
}

void UWidgetFindNote::setTheVisibleForMoreSearchOptions(bool visible)
{
    groupBoxFindSettings->setVisible(visible);
    groupBoxSearchIn->setVisible(visible);
    groupBoxConsiderVisibilityNotes->setVisible(visible);
    groupBoxConsiderLockNotes->setVisible(visible);
}
void UWidgetFindNote::setTheCheckedForMoreSearchOptions(bool checked)
{
    groupBoxConsiderVisibilityNotes->setChecked(checked);
    groupBoxConsiderLockNotes->setChecked(checked);
}

void UWidgetFindNote::clickButtonShowHideAdditionalSearchSettings()
{
    if(pButtonShowHideAdditionalSearchSettings->isChecked())
        pButtonShowHideAdditionalSearchSettings->setText(strHideAdditionalSearchSettings);
    else
        pButtonShowHideAdditionalSearchSettings->setText(strShowAdditionalSearchSettings);

    setTheVisibleForMoreSearchOptions(pButtonShowHideAdditionalSearchSettings->isChecked());
    setTheCheckedForMoreSearchOptions(false);

    // обновл€ем размер компоновщика
    adjustSize();

    // устанавливаем оптимальный размер
    resize(sizeHint());
}

/// PROTECTED
void UWidgetFindNote::closeEvent(QCloseEvent *)
{
    hide();
    emit iClose();
}
