#include "USymbolTable.h"

/// PUBLIC
USymbolTable::USymbolTable(QWidget *parent):
    QDialog(parent),
    currentUsedRow(0),
    currentUsedCol(0),
    lastItem(0)
{
    setWindowFlags(Qt::WindowCloseButtonHint);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("windows-1251"));

    createGUI();

    initComboBoxSymbolicPlane();

    refreshTable();
}

/// PRIVATE
void USymbolTable::createGUI()
{
    QHBoxLayout *hBoxLayoutMain = new QHBoxLayout();

    QVBoxLayout *vBoxLayoutMain = new QVBoxLayout();

    QHBoxLayout *hBoxLayout1 = new QHBoxLayout();

    comboBoxSymbolicPlane = new UOfficeWordStyleComboBox();
    comboBoxSymbolicPlane->setMinimumWidth(150);
    comboBoxSymbolicPlane->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    comboBoxSymbolicPlane->view()->setCornerWidget(new QSizeGrip(comboBoxSymbolicPlane));

    connect(comboBoxSymbolicPlane, SIGNAL(activated(int)),
            this, SLOT(selectedComboBoxSymbolicPlane(int)));

    hBoxLayout1->addWidget(comboBoxSymbolicPlane);

    fontComboBox = new QFontComboBox();
    connect(fontComboBox, SIGNAL(activated(QString)),
            this, SLOT(setTableFontFamily(QString)));
    connect(fontComboBox, SIGNAL(activated(QString)),
            this, SLOT(selectFontFamily(QString)));

    hBoxLayout1->addWidget(fontComboBox);

    vBoxLayoutMain->addLayout(hBoxLayout1);

    table = new QTableWidget();
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->horizontalHeader()->hide();
    table->verticalHeader()->hide();

    table->installEventFilter(this);

    connect(table, SIGNAL(itemClicked(QTableWidgetItem*)),
            this, SLOT(clickItemTableWidget(QTableWidgetItem*)));
    connect(table, SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this, SLOT(doubleClickItemTableWidget(QTableWidgetItem*)));

    vBoxLayoutMain->addWidget(table);

    hBoxLayoutMain->addLayout(vBoxLayoutMain);

    QVBoxLayout *vBoxLayout1 = new QVBoxLayout();

    pButtonSelect = new QPushButton(tr("Выбрать"));
    pButtonSelect->setEnabled(false);
    connect(pButtonSelect, SIGNAL(clicked()),
            this, SLOT(clickSelectSymbol()));
    vBoxLayout1->addWidget(pButtonSelect);

    pButtonClose = new QPushButton(tr("Закрыть"));
    connect(pButtonClose, SIGNAL(clicked()),
            this, SLOT(close()));

    vBoxLayout1->addWidget(pButtonClose);
    vBoxLayout1->addStretch();

    labelSymbol = new QLabel();
    QFont font = labelSymbol->font();
    font.setPointSize(40);
    labelSymbol->setFont(font);

    vBoxLayout1->addWidget(labelSymbol, 0, Qt::AlignCenter);

    labelKeySymbol = new QLabel();

    vBoxLayout1->addWidget(labelKeySymbol, 0, Qt::AlignCenter);

    lineEditFindSymbol = new QLineEdit();

    QRegExpValidator *validator = new QRegExpValidator();
    validator->setRegExp(QRegExp("[0-9a-fA-F]*"));

    lineEditFindSymbol->setValidator(validator);
    connect(lineEditFindSymbol, SIGNAL(textChanged(QString)),
            this, SLOT(findSymbol(QString)));
    connect(lineEditFindSymbol, SIGNAL(textEdited(QString)),
            this, SLOT(findSymbol(QString)));

    vBoxLayout1->addWidget(lineEditFindSymbol, 0, Qt::AlignLeft);
    vBoxLayout1->addStretch();

    hBoxLayoutMain->addLayout(vBoxLayout1);

    setLayout(hBoxLayoutMain);
}

void USymbolTable::addToTable(QChar symbol)
{
    currentUsedCol++;

    if(currentUsedRow == 0)
    {
        currentUsedRow++;
        table->setRowCount(currentUsedRow);
    }

    if(currentUsedCol >= table->columnCount())
    {
        currentUsedRow++;
        table->setRowCount(currentUsedRow);

        currentUsedCol = 0;
    }

    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(symbol);
    item->setData(32, symbol.unicode());

    QFont font(item->font());
    font.setPointSize(20);

    item->setFont(font);

    item->setTextAlignment(Qt::AlignCenter);
    item->setFlags(Qt::ItemIsSelectable
                   | Qt::ItemIsEnabled);

    table->setItem(currentUsedRow - 1, currentUsedCol - 1,
                   item);
}

/// PUBLIC SLOTS
void USymbolTable::setLanguage(QString pathToTheFileLanguage){}

void USymbolTable::displaySymbols(uint min, uint max)
{
    table->clear();

    currentUsedRow = 0;
    currentUsedCol = 0;

    lastItem = 0;

    for(uint key = min; key <= max; key++)
        addToTable(QChar(key));

    for(int i = 0; i < table->columnCount(); i++)
        table->horizontalHeader()->resizeSection(i, widthCell);

    for(int i = 0; i < table->rowCount(); i++)
        table->verticalHeader()->resizeSection(i, heightCell);

    setTableFontFamily(fontComboBox->currentText());
}

void USymbolTable::refreshTable()
{
    int width = table->width() - table->verticalScrollBar()->width() - table->columnCount();
    table->setColumnCount(width / widthCell);

    selectedComboBoxSymbolicPlane(comboBoxSymbolicPlane->currentIndex());
}

void USymbolTable::setTableFontFamily(QString family)
{
    for(int i = 0; i < table->rowCount(); i++)
        for(int j = 0; j < table->columnCount(); j++)
        {
            QTableWidgetItem *item = table->item(i, j);

            if(item)
                item->setFont(QFont(family));
            else
            {
                item = new QTableWidgetItem();
                item->setFlags(Qt::NoItemFlags);

                table->setItem(i, j, item);
            }
        }
}

void USymbolTable::initComboBoxSymbolicPlane()
{
    initListRangeUnicode();

    for(int i = 0; i < listRangeUnicode.count(); i++)
        comboBoxSymbolicPlane->addItem(listRangeUnicode.at(i)->name,
                                       listRangeUnicode.at(i)->range);
}

void USymbolTable::initListRangeUnicode()
{
    listRangeUnicode.clear();

    //    listRangeUnicode.append(new URangeUnicode(tr("Управляющие символы C0"),
    //                                              QString(patternRange)
    //                                              .arg(0x0000).arg(0x001F)));

    listRangeUnicode.append(new URangeUnicode(tr("Основная латиница"),
                                              QString(patternRange)
                                              .arg(0x0020).arg(0x007F)));

    listRangeUnicode.append(new URangeUnicode(tr("Дополнительные символы Latin-1"),
                                              QString(patternRange)
                                              .arg(0x00A0).arg(0x00FF)));

    listRangeUnicode.append(new URangeUnicode(tr("Расширенная латиница-A"),
                                              QString(patternRange)
                                              .arg(0x0100).arg(0x017F)));

    listRangeUnicode.append(new URangeUnicode(tr("Расширенная латиница-B"),
                                              QString(patternRange)
                                              .arg(0x0180).arg(0x024F)));

    listRangeUnicode.append(new URangeUnicode(tr("Международный фонетический алфавит"),
                                              QString(patternRange)
                                              .arg(0x0250).arg(0x02AF)));

    listRangeUnicode.append(new URangeUnicode(tr("Некомбинируемые протяжённые символы-модификаторы"),
                                              QString(patternRange)
                                              .arg(0x02B0).arg(0x02FF)));

    listRangeUnicode.append(new URangeUnicode(tr("Комбинируемые диакритические знаки"),
                                              QString(patternRange)
                                              .arg(0x0300).arg(0x036F)));

    listRangeUnicode.append(new URangeUnicode(tr("Греческий и коптский алфавиты"),
                                              QString(patternRange)
                                              .arg(0x0370).arg(0x03FF)));

    listRangeUnicode.append(new URangeUnicode(tr("Кириллица"),
                                              QString(patternRange)
                                              .arg(0x0400).arg(0x04FF)));

    listRangeUnicode.append(new URangeUnicode(tr("Кириллица. Дополнительные символы"),
                                              QString(patternRange)
                                              .arg(0x0500).arg(0x052F)));

    listRangeUnicode.append(new URangeUnicode(tr("Армянский алфавит"),
                                              QString(patternRange)
                                              .arg(0x0530).arg(0x058F)));

    listRangeUnicode.append(new URangeUnicode(tr("Иврит"),
                                              QString(patternRange)
                                              .arg(0x0590).arg(0x05FF)));

    listRangeUnicode.append(new URangeUnicode(tr("Арабский"),
                                              QString(patternRange)
                                              .arg(0x0600).arg(0x06FF)));

    listRangeUnicode.append(new URangeUnicode(tr("Сирийский"),
                                              QString(patternRange)
                                              .arg(0x0700).arg(0x074F)));

    listRangeUnicode.append(new URangeUnicode(tr("Дополнительные символы арабского письма"),
                                              QString(patternRange)
                                              .arg(0x0750).arg(0x077F)));

    listRangeUnicode.append(new URangeUnicode(tr("Тана"),
                                              QString(patternRange)
                                              .arg(0x0780).arg(0x07BF)));

    listRangeUnicode.append(new URangeUnicode(tr("Нко"),
                                              QString(patternRange)
                                              .arg(0x07C0).arg(0x07FF)));

    listRangeUnicode.append(new URangeUnicode(tr("Самаритянское письмо"),
                                              QString(patternRange)
                                              .arg(0x0800).arg(0x083F)));

    listRangeUnicode.append(new URangeUnicode(tr("Мандейский алфавит"),
                                              QString(patternRange)
                                              .arg(0x0840).arg(0x085F)));

    listRangeUnicode.append(new URangeUnicode(tr("Расширенный набор символов арабского письмаA"),
                                              QString(patternRange)
                                              .arg(0x08A0).arg(0x08FF)));

    listRangeUnicode.append(new URangeUnicode(tr("Деванагари"),
                                              QString(patternRange)
                                              .arg(0x0900).arg(0x097F)));

    listRangeUnicode.append(new URangeUnicode(tr("Бенгальский"),
                                              QString(patternRange)
                                              .arg(0x0980).arg(0x09FF)));

    listRangeUnicode.append(new URangeUnicode(tr("Гурмухки"),
                                              QString(patternRange)
                                              .arg(0x0A00).arg(0x0A7F)));

    listRangeUnicode.append(new URangeUnicode(tr("Гуджарати"),
                                              QString(patternRange)
                                              .arg(0x0A80).arg(0x0AFF)));
}

void USymbolTable::findSymbol(QString text)
{
    bool ok;
    uint key = text.toInt(&ok, 16);

    QComboBox *cb = comboBoxSymbolicPlane;

    // просмотрим области таблицы, и переберем
    // их значения диапозонов
    for(int i = 0; i < cb->count(); i++)
    {
        QString range = cb->itemData(i).toString();
        QStringList listRange = range.split("-");

        uint min = listRange.at(0).toUInt();
        uint max = listRange.at(1).toUInt();

        // ключ определен в области
        // показываем эту область в таблице
        // проверим есть ли смысл показывать - вдруг уже есть
        if(key >= min
                && key <= max
                && i != cb->currentIndex())
        {
            // отобразим область в таблице
            selectedComboBoxSymbolicPlane(i);
            cb->setCurrentIndex(i);

            // поищем символ в обновленной таблице
            for(int row = 0; row < table->rowCount(); row++)
                for(int col = 0; col < table->columnCount(); col++)
                {
                    QTableWidgetItem *item = table->item(row, col);

                    if(item->data(32).toUInt() == key)
                    {
                        item->setBackgroundColor(Qt::green);
                        lastItem = item;

                        pButtonSelect->setEnabled(true);

                        QString text = item->text();
                        uint key = item->data(32).toUInt();

                        lineEditFindSymbol->blockSignals(true);
                        lineEditFindSymbol->setText(QString::number(key, 16).toUpper());
                        lineEditFindSymbol->blockSignals(false);

                        labelSymbol->setText(text);

                        QFont font = labelSymbol->font();
                        font.setFamily(fontComboBox->currentText());

                        labelSymbol->setFont(font);

                        labelKeySymbol->setText("U+" + QString::number(key, 16));

                        break;
                    }
                }
            break;
        }
    }
}

/// PRIVATE
void USymbolTable::selectedComboBoxSymbolicPlane(int index)
{
    QString range = comboBoxSymbolicPlane->itemData(index).toString();
    QStringList listRange = range.split("-");

    uint min = listRange.at(0).toUInt();
    uint max = listRange.at(1).toUInt();

    displaySymbols(min, max);
}

void USymbolTable::clickItemTableWidget(QTableWidgetItem *item)
{
    if(!item)
        return;

    pButtonSelect->setEnabled(true);


    if(lastItem)
        lastItem->setBackgroundColor(Qt::white);


    QString text = item->text();
    uint key = item->data(32).toUInt();

    lineEditFindSymbol->blockSignals(true);
    lineEditFindSymbol->setText(QString::number(key, 16).toUpper());
    lineEditFindSymbol->blockSignals(false);

    labelSymbol->setText(text);

    QFont font = labelSymbol->font();
    font.setFamily(fontComboBox->currentText());

    labelSymbol->setFont(font);

    labelKeySymbol->setText("U+" + QString::number(key, 16));
}

void USymbolTable::doubleClickItemTableWidget(QTableWidgetItem *item)
{
    if(!item)
        return;

    pButtonSelect->setEnabled(true);

    QString text = item->text();
    uint key = item->data(32).toUInt();

    lineEditFindSymbol->blockSignals(true);
    lineEditFindSymbol->setText(QString::number(key, 16).toUpper());
    lineEditFindSymbol->blockSignals(false);

    labelSymbol->setText(text);

    QFont font = labelSymbol->font();
    font.setFamily(fontComboBox->currentText());

    labelSymbol->setFont(font);

    labelKeySymbol->setText("U+" + QString::number(key, 16));

    QTextEdit textEdit;
    textEdit.setText(text);
    textEdit.selectAll();

    QTextCharFormat textCharFormat;
    textCharFormat.setFontFamily(font.family());
    textEdit.mergeCurrentCharFormat(textCharFormat);

    emit symbolIsSelected(textEdit.toHtml());
}

void USymbolTable::clickSelectSymbol()
{
    QString text = labelSymbol->text();
    QFont font = labelSymbol->font();

    QTextEdit textEdit;
    textEdit.setText(text);
    textEdit.selectAll();

    QTextCharFormat textCharFormat;
    textCharFormat.setFontFamily(font.family());
    textEdit.mergeCurrentCharFormat(textCharFormat);

    emit symbolIsSelected(textEdit.toHtml());
}

void USymbolTable::selectFontFamily(QString family)
{
    QFont font = labelSymbol->font();
    font.setFamily(family);
    labelSymbol->setFont(font);
}

bool USymbolTable::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == table)
        if(event->type() == QEvent::Resize)
            refreshTable();

    QObject::eventFilter(obj, event);
}
