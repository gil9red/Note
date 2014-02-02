#include "UDialogSelectListNotes.h"

/// PUBLIC
UDialogSelectListNotes::UDialogSelectListNotes(QWidget *parent):
    QDialog(parent),
    currentModeMessage(DeleteSelectedNotes)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName(textCodec.toUtf8()));

    setWindowFlags(Qt::Dialog
                   | Qt::WindowSystemMenuHint);

    setSizeGripEnabled(true);
}

void UDialogSelectListNotes::setModeMessage(UModeMessage mode)
{
    currentModeMessage = mode;

    createGUI();
}

/// PRIVATE
void UDialogSelectListNotes::createGUI()
{
    QVBoxLayout *vBoxLayoutMain = new QVBoxLayout();

    labelMessage = new QLabel();
    labelMessage->setFont(QFont("", 11));

    pButtonOk = new QPushButton();
    pButtonCancel = new QPushButton();

    connect(pButtonOk, SIGNAL(clicked()),
            this, SLOT(clickButtonOk()));
    connect(pButtonCancel, SIGNAL(clicked()),
            this, SLOT(clickButtonCancel()));

    QHBoxLayout *hBoxLayoutCommandButtons = new QHBoxLayout();
    hBoxLayoutCommandButtons->addStretch();
    hBoxLayoutCommandButtons->addWidget(pButtonOk);
    hBoxLayoutCommandButtons->addWidget(pButtonCancel);

    vBoxLayoutMain->addWidget(labelMessage,
                              0,
                              Qt::AlignCenter);

    if(isModeMultipleSelection())
    {
        pButtonUnselect = new QPushButton();
        pButtonSelectAll = new QPushButton();

        connect(pButtonUnselect, SIGNAL(clicked()),
                this, SLOT(unselect()));
        connect(pButtonSelectAll, SIGNAL(clicked()),
                this, SLOT(selectAll()));

        QHBoxLayout *hBoxLayoutCommandButtons2 = new QHBoxLayout();
        hBoxLayoutCommandButtons2->addWidget(pButtonSelectAll);
        hBoxLayoutCommandButtons2->addWidget(pButtonUnselect);
        hBoxLayoutCommandButtons2->addStretch();

        vBoxLayoutMain->addLayout(hBoxLayoutCommandButtons2);
    }

    tableWidgetNotes = new QTableWidget();
    connect(tableWidgetNotes, SIGNAL(cellDoubleClicked(int,int)),
            this, SLOT(checkedUncheckedRowTable(int)));

    if(!isModeMultipleSelection())    
        tableWidgetNotes->verticalHeader()->hide();    

    vBoxLayoutMain->addWidget(tableWidgetNotes);
    vBoxLayoutMain->addLayout(hBoxLayoutCommandButtons);

    setLayout(vBoxLayoutMain);

    pButtonOk->setFocus();
}

bool UDialogSelectListNotes::isModeMultipleSelection()
{
    return currentModeMessage == MoveSelectedNotesToRecycleBin
            || currentModeMessage == DeleteSelectedNotes
            || currentModeMessage == RestoreSelectedNotes;
}

/// PUBLIC SLOTS
void UDialogSelectListNotes::setLanguage(QString pathToTheFileLanguage)
{
    currentLanguage = pathToTheFileLanguage;

    QSettings ini(pathToTheFileLanguage,
                       QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("UDialogSelectListNotes");

    setWindowTitle(ini.value("Title").toString());

    switch(currentModeMessage)
    {
    case MoveSelectedNotesToRecycleBin:
        labelMessage->setText(ini.value("TextMessageMoveSelectedNotesToRecycleBin").toString());
        break;

    case DeleteSelectedNotes:
        labelMessage->setText(ini.value("TextMessageDeleteSelectedNotes").toString());
        break;

    case RestoreSelectedNotes:
        labelMessage->setText(ini.value("TextMessageRestoreSelectedNotes").toString());
        break;

    case MoveSelectedNoteToRecycleBin:
        labelMessage->setText(ini.value("TextMessageMoveSelectedNoteToRecycleBin").toString());
        break;

    case DeleteSelectedNote:
        labelMessage->setText(ini.value("TextMessageDeleteSelectedNote").toString());
        break;

    case RestoreSelectedNote:
        labelMessage->setText(ini.value("TextMessageRestoreSelectedNote").toString());
        break;
    }

    if(isModeMultipleSelection())
    {
        pButtonUnselect->setText(ini.value("Unselect").toString());
        pButtonSelectAll->setText(ini.value("SelectAll").toString());
    }

    pButtonOk->setText(ini.value("TextButtonOk").toString());
    pButtonCancel->setText(ini.value("TextButtonCancel").toString());

    ini.endGroup();
}

void UDialogSelectListNotes::selectTableRow(QTableWidget *table,
                                            QList <int> *tableListRows)
{
    listRows.clear();

    tableWidgetNotes->setColumnCount(table->columnCount() + 1);

    tableWidgetNotes->setSelectionMode(QAbstractItemView::ContiguousSelection);
    tableWidgetNotes->setSelectionBehavior(QAbstractItemView::SelectRows);

    tableWidgetNotes->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableWidgetNotes->setAlternatingRowColors(true);

    QStringList listColumnHeaders;
    listColumnHeaders.append(tr("№"));

    for (int i = 0; i < table->columnCount(); i++)
        listColumnHeaders.append(table->horizontalHeaderItem(i)->text());    

    tableWidgetNotes->setHorizontalHeaderLabels(listColumnHeaders);

    for (int i = 0; i < tableListRows->count(); i++)
    {
        // добавляем строку
        tableWidgetNotes->setRowCount(tableWidgetNotes->rowCount() + 1);
        int currentRow = tableWidgetNotes->rowCount() - 1;

        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(QString::number(tableListRows->at(i) + 1));

        if(isModeMultipleSelection())
            item->setCheckState(Qt::Checked);

        tableWidgetNotes->setItem(currentRow,
                                  0,
                                  item);

        QString text = table->item(tableListRows->at(i), 0)->text();

        tableWidgetNotes->setItem(currentRow,
                                  1,
                                  new QTableWidgetItem(text));

        for (int i = 1; i < table->columnCount(); i++)
        {
            item = table->item(currentRow, i)->clone();
            tableWidgetNotes->setItem(currentRow,
                                      i + 1,
                                      item);
        }
    }
}

/// PRIVATE SLOTS
void UDialogSelectListNotes::clickButtonOk()
{
    listRows.clear();

    if(isModeMultipleSelection())
        for(int i = 0; i < tableWidgetNotes->rowCount(); i++)
        {
            QTableWidgetItem *item = tableWidgetNotes->item(i, 0);

            if(item->checkState() == Qt::Checked)
                listRows.append(item->text().toInt() - 1);
        }
    else
    {
        QTableWidgetItem *item = tableWidgetNotes->item(0, 0);
        listRows.append(item->text().toInt() - 1);
    }

    close();
}

void UDialogSelectListNotes::clickButtonCancel()
{
    listRows.clear();
    close();
}

void UDialogSelectListNotes::unselect()
{
    for (int i = 0; i < tableWidgetNotes->rowCount(); i++)
    {
        QTableWidgetItem *item = tableWidgetNotes->item(i, 0);
        item->setCheckState(Qt::Unchecked);
    }
}
void UDialogSelectListNotes::selectAll()
{
    for (int i = 0; i < tableWidgetNotes->rowCount(); i++)
    {
        QTableWidgetItem *item = tableWidgetNotes->item(i, 0);
        item->setCheckState(Qt::Checked);
    }
}

void UDialogSelectListNotes::checkedUncheckedRowTable(int row)
{
    QTableWidgetItem *item = tableWidgetNotes->item(row, 0);

    if(item->checkState() == Qt::Checked)
        item->setCheckState(Qt::Unchecked);
    else
        item->setCheckState(Qt::Checked);
}

/// PROTECTED
void UDialogSelectListNotes::showEvent(QShowEvent *)
{
    QHeaderView *headerView = tableWidgetNotes->verticalHeader();

    setMinimumWidth(minimumWidthDialogSelectListNotes);

    int height = headerView->length();
    height += headerView->sizeHint().height();

    if(height > maximimSizeDialogSelectListNotes.height())
        height = maximimSizeDialogSelectListNotes.height() / 2;

    tableWidgetNotes->resize(tableWidgetNotes->width(),
                             height
                             + tableWidgetNotes->frameWidth()
                             + tableWidgetNotes->horizontalScrollBar()->height()
                             + 10);

    if(layout()->minimumSize().height()
            > maximimSizeDialogSelectListNotes.height())
        resize(maximimSizeDialogSelectListNotes);

    else
        resize(layout()->minimumSize().width(),
               layout()->minimumSize().height()
               + tableWidgetNotes->height() / 2);
}
