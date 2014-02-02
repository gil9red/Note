#include "UEditedExpandableComboBoxDate.h"

/// PUBLIC
UDialogChangesComboBoxDate::UDialogChangesComboBoxDate(QWidget *parent/* = 0*/):
    QDialog(parent)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("windows-1251"));

    setWindowFlags(Qt::Dialog
                   | Qt::WindowCloseButtonHint);

    createGUI();

    setSizeGripEnabled(true);
}

void UDialogChangesComboBoxDate::setListItemsDate(QStringList *list)
{
    listComboBoxDate.clear();

    for(int i = 0; i < list->count(); i++)
    {
        listComboBoxDate.append(list->at(i));

        QListWidgetItem *item = new QListWidgetItem();
        item->setText(list->at(i));
        item->setFlags(item->flags()
                       | Qt::ItemIsEditable);

        listWidget->addItem(item);
    }
}

/// PRIVATE
void UDialogChangesComboBoxDate::createGUI()
{
    pButtonAdd = new QPushButton();
    connect(pButtonAdd, SIGNAL(clicked()),
            this, SLOT(clickAdd()));

    pButtonDelete = new QPushButton();
    connect(pButtonDelete, SIGNAL(clicked()),
            this, SLOT(clickDelete()));

    pButtonOk = new QPushButton();
    connect(pButtonOk, SIGNAL(clicked()),
            this, SLOT(clickOk()));

    pButtonCancel = new QPushButton();
    connect(pButtonCancel, SIGNAL(clicked()),
            this, SLOT(clickCancel()));

    listWidget = new QListWidget();
    listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(editedItem(QListWidgetItem*)));


    indicatorDateTimeListWidget = new UIndicatorDateTime();

    connect(listWidget, SIGNAL(itemPressed(QListWidgetItem*)),
            this, SLOT(selectedItemListWidget(QListWidgetItem*)));

    indicatorDateTimeListWidget->setInterval(10);
    indicatorDateTimeListWidget->start();

    QHBoxLayout *hBoxButtonsListWidget = new QHBoxLayout();
    hBoxButtonsListWidget->addWidget(pButtonAdd);
    hBoxButtonsListWidget->addWidget(pButtonDelete);
    hBoxButtonsListWidget->addStretch();

    QHBoxLayout *hBoxCommandButtons = new QHBoxLayout();
    hBoxCommandButtons->addStretch();
    hBoxCommandButtons->addWidget(pButtonOk);
    hBoxCommandButtons->addWidget(pButtonCancel);

    QVBoxLayout *vBox = new QVBoxLayout();
    vBox->addWidget(listWidget);
    vBox->addWidget(indicatorDateTimeListWidget);

    QVBoxLayout *vBoxLayoutMain = new QVBoxLayout();
    vBoxLayoutMain->addLayout(hBoxButtonsListWidget);
    vBoxLayoutMain->addLayout(vBox);
    vBoxLayoutMain->addLayout(hBoxCommandButtons);

    setLayout(vBoxLayoutMain);
}

/// PRIVATE SLOTS
void UDialogChangesComboBoxDate::clickAdd()
{
    UDialogAddToListWidget *dialogAddToListWidget = new UDialogAddToListWidget();

    dialogAddToListWidget->setLanguage(currentLanguage);

    connect(this, SIGNAL(languageIsChange(QString)),
            dialogAddToListWidget, SLOT(setLanguage(QString)));

    dialogAddToListWidget->exec();

    if(dialogAddToListWidget->getDialogReturnCode())
    {
        QString text = dialogAddToListWidget->getFormatDate();

        if(text.isEmpty()
                || text.isNull())
            return;

        QListWidgetItem *item = new QListWidgetItem();
        item->setText(text);

        listWidget->addItem(item);
    }
}
void UDialogChangesComboBoxDate::editedItem(QListWidgetItem *item)
{
    UDialogAddToListWidget *dialogAddToListWidget = new UDialogAddToListWidget();
    dialogAddToListWidget->setLanguage(currentLanguage);

    dialogAddToListWidget->setFormatDate(item->text());

    connect(this, SIGNAL(languageIsChange(QString)),
            dialogAddToListWidget, SLOT(setLanguage(QString)));

    dialogAddToListWidget->exec();

    QString text = dialogAddToListWidget->getFormatDate();

    item->setText(text);
}

void UDialogChangesComboBoxDate::clickDelete()
{
    int currentRow = listWidget->currentRow();
    listWidget->removeItemWidget(listWidget->takeItem(currentRow));
}

void UDialogChangesComboBoxDate::clickOk()
{
    listComboBoxDate.clear();

    for(int i = 0; i < listWidget->count(); i++)
        listComboBoxDate.append(listWidget->item(i)->text());

    dialogReturnCode = 1;

    close();
}
void UDialogChangesComboBoxDate::clickCancel()
{
    dialogReturnCode = 0;

    close();
}

/// PUBLIC SLOTS
void UDialogChangesComboBoxDate::setLanguage(QString pathToTheFileLanguage)
{
    emit languageIsChange(pathToTheFileLanguage);
    currentLanguage = pathToTheFileLanguage;

    QSettings ini(pathToTheFileLanguage,
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("UDialogChangesComboBoxDate");

    setWindowTitle(ini.value("Title").toString());
    pButtonAdd->setText(ini.value("Add").toString());
    pButtonDelete->setText(ini.value("Delete").toString());
    pButtonOk->setText(ini.value("Ok").toString());
    pButtonCancel->setText(ini.value("Cancel").toString());

    ini.endGroup();
}



/// PUBLIC
UEditedExpandableComboBoxDate::UEditedExpandableComboBoxDate(QWidget *parent):
    QWidget(parent)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("windows-1251"));

    createGUI();
}

void UEditedExpandableComboBoxDate::setListItemsDate(QStringList *list)
{
    comboBox->clear();

    comboBox->addItems(*list);
}

void UEditedExpandableComboBoxDate::setCurrentItemComboBoxDate(QString text)
{
    int index = comboBox->findText(text);
    comboBox->setCurrentIndex(index);

    indicatorDateTime->setFormatDateTime(text);
}

QStringList * UEditedExpandableComboBoxDate::getListDateTimeFormat()
{
    QStringList *list = new QStringList();

    for(int i = 0; i < comboBox->count(); i++)
        list->append(comboBox->itemText(i));

    return list;
}

/// PRIVATE
void UEditedExpandableComboBoxDate::createGUI()
{
    comboBox = new QComboBox();
    comboBox->setMinimumWidth(100);

    connect(comboBox, SIGNAL(activated(QString)),
            this, SIGNAL(activatedItemComboBoxDate(QString)));

    pButton = new QPushButton();
    connect(pButton, SIGNAL(clicked()),
            this, SLOT(clickButton()));

    indicatorDateTime = new UIndicatorDateTime();

    connect(comboBox, SIGNAL(activated(QString)),
            indicatorDateTime, SLOT(setFormatDateTime(QString)));

    indicatorDateTime->setFormatDateTime(comboBox->currentText());
    indicatorDateTime->setInterval(10);
    indicatorDateTime->start();

    groupBoxShowIndicatorDateTime = new QGroupBox();
    groupBoxShowIndicatorDateTime->setFlat(true);
    groupBoxShowIndicatorDateTime->setCheckable(true);

    connect(groupBoxShowIndicatorDateTime, SIGNAL(toggled(bool)),
            indicatorDateTime, SLOT(setVisible(bool)));

    groupBoxShowIndicatorDateTime->setChecked(false);

    QVBoxLayout *vBoxLayoutGroupBoxShowIndicatorDateTime = new QVBoxLayout();
    vBoxLayoutGroupBoxShowIndicatorDateTime->addWidget(indicatorDateTime);

    groupBoxShowIndicatorDateTime->setLayout(vBoxLayoutGroupBoxShowIndicatorDateTime);

    QVBoxLayout *vBox = new QVBoxLayout();
    vBox->addWidget(pButton);

    QHBoxLayout *hBox = new QHBoxLayout();
    hBox->addWidget(comboBox);
    hBox->addLayout(vBox);

    QVBoxLayout *vBoxLayoutMain = new QVBoxLayout();
    vBoxLayoutMain->addLayout(hBox);
    vBoxLayoutMain->addWidget(groupBoxShowIndicatorDateTime);
    vBoxLayoutMain->addStretch();

    setLayout(vBoxLayoutMain);
}

/// PUBLIC SLOTS
void UEditedExpandableComboBoxDate::setLanguage(QString pathToTheFileLanguage)
{
    currentLanguage = pathToTheFileLanguage;

    emit languageIsChange(pathToTheFileLanguage);

    QSettings ini(pathToTheFileLanguage,
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("UEditedExpandableComboBoxDate");

    groupBoxShowIndicatorDateTime->setTitle(ini.value("ShowIndicatorDateTime").toString());
    pButton->setText(ini.value("Edit").toString());

    ini.endGroup();
}

/// PRIVATE SLOTS
void UEditedExpandableComboBoxDate::clickButton()
{
    QStringList list;

    for(int i = 0; i < comboBox->count(); i++)
        list.append(comboBox->itemText(i));

    // первый индекс - всегда пустой
    list.removeAt(0);

    QString lastCurrentText = comboBox->currentText();

    dialog = new UDialogChangesComboBoxDate();
    dialog->setLanguage(currentLanguage);

    connect(this, SIGNAL(languageIsChange(QString)),
            dialog, SLOT(setLanguage(QString)));

    dialog->setListItemsDate(&list);

    dialog->exec();

    if(dialog->getDialogReturnCode())
    {
        QStringList *listItemDate = dialog->getListItemDate();

        comboBox->clear();

        // восстанавливаем первый пустой индекс
        listItemDate->insert(0, " ");

        for(int i = 0; i < listItemDate->count(); i++)
            comboBox->addItem(listItemDate->at(i));

        int index = comboBox->findText(lastCurrentText);
        comboBox->setCurrentIndex(index);

        emit saveListDateTimeFormat();
    }

    delete dialog;
}
