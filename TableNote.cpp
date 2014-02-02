#include "TableNote.h"

MyTableWidget::MyTableWidget(QWidget *parent):
    QTableWidget(parent)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("windows-1251"));

    resize(400, 200);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenu(QPoint)));

    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

//    horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    setItemDelegateForColumn(indexColumnVisible,          new ComboBoxDelegate());
    setItemDelegateForColumn(indexColumnDateOfCreating,   new ReadDelegate());
    setItemDelegateForColumn(indexColumnCountTextSymbols, new ReadDelegate());
    setItemDelegateForColumn(indexColumnCountTextLines,   new ReadDelegate());
    setItemDelegateForColumn(indexColumnDateLastChange,   new ReadDelegate());
    setItemDelegateForColumn(indexColumnStatusLock,       new ComboBoxDelegate());

    setRowCount(0);

    actionDeleteItem = new QAction(this);
    actionShowItem   = new QAction(this);
    actionHideItem   = new QAction(this);
    actionSettings   = new QAction(this);

    settingsTable    = new SettingsTableNote();

    connect(settingsTable, SIGNAL(columnTableVisibleChange(int,bool)),
            this, SLOT(setVisibleColumnTable(int,bool)));
    connect(this, SIGNAL(languageIsChange(QString)),
            settingsTable, SLOT(setLanguage(QString)));

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

    visibleContextMenu = false;

    QString language = "ru";
    setLanguage(language);       
}

QList <bool> & MyTableWidget::getListVisibleColumn()
{
    return listVisibleColumn;
}

// SLOTS

void MyTableWidget::signalCellPressed(int row, int col)
{    
    QTableWidgetItem *itemTable = item(row, indexColumnVisible);

    if(QVariant(itemTable->text()).toBool())
    {
        actionHideItem->setVisible(true);
        actionShowItem->setVisible(false);
    }else
    {
        actionHideItem->setVisible(false);
        actionShowItem->setVisible(true);
    }

    visibleContextMenu = true;
}

void MyTableWidget::clickActionDelete()
{
    emit clickActionDeleteItem();
}

void MyTableWidget::clickActionShow()
{
    QTableWidgetItem *curItem = item(currentRow(),
                                     indexColumnVisible);
    curItem->setText(tr("true"));
    emit clickActionShowItem();
}

void MyTableWidget::clickActionHide()
{
    QTableWidgetItem *curItem = item(currentRow(),
                                     indexColumnVisible);
    curItem->setText(tr("false"));
    emit clickActionHideItem();
}

void MyTableWidget::setVisibleColumnTable(int indexColumn, bool visible)
{
    setColumnHidden(indexColumn, !visible);

    listVisibleColumn.removeAt(indexColumn);
    listVisibleColumn.insert(indexColumn, visible);
}

void MyTableWidget::setVisibleColumnTable(QList <bool> &list)
{
    listVisibleColumn = list;

    bool visible;

    for(int i = 0; i < listVisibleColumn.size(); i++)
    {
       visible = listVisibleColumn.at(i);
       setColumnHidden(i, !visible);
    }
}

void MyTableWidget::showSettings()
{
    settingsTable->exec();
}

void MyTableWidget::setLanguage(QString language)
{
    emit languageIsChange(language);

    if(language == "English" || language == "en" || language == "En")
    {
        actionDeleteItem->setText(tr("Delete"));
        actionShowItem->setText(tr("Show"));
        actionHideItem->setText(tr("Hide"));
        actionSettings->setText(tr("Settings"));

        QStringList  lst;
        lst << tr("Title")
            << tr("Visible")
            << tr("Create Data")
            << tr("Count Text Symbols")
            << tr("Count Text Line")
            << tr("Date Last Change")
            << tr("Lock");

        setColumnCount(countColumns);
        setHorizontalHeaderLabels(lst);
    }
    if(language == "Russian" || language == "ru" || language == "Ru" || language == tr("Русский"))
    {
        actionDeleteItem->setText(tr("Удалить"));
        actionShowItem->setText(tr("Показать"));
        actionHideItem->setText(tr("Спрятать"));
        actionSettings->setText(tr("Настройки"));

        QStringList lst;
        lst << tr("Заголовок")
            << tr("Видимость")
            << tr("Дата создания")
            << tr("Количество символов")
            << tr("Количество строк")
            << tr("Дата последнего изменения")
            << tr("Заблокирован");

        setColumnCount(countColumns);
        setHorizontalHeaderLabels(lst);
    }
}

void MyTableWidget::showContextMenu(QPoint p)
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
    menu.exec(this->mapToGlobal(p));

    visibleContextMenu = false;
    }
}
