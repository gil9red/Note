#include "SettingsTableNote.h"

SettingsTableNote::SettingsTableNote(QWidget *parent):
    QDialog(parent)
{
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("windows-1251"));

    checkBoxVisibleNote = new QCheckBox();

    checkBoxVisibleNote->setChecked(true);
    connect(checkBoxVisibleNote, SIGNAL(clicked()),
            this, SLOT(clickCheckVisibleNote()));

    checkBoxDateCreateNote = new QCheckBox();

    checkBoxDateCreateNote->setChecked(true);
    connect(checkBoxDateCreateNote, SIGNAL(clicked()),
            this, SLOT(clickCheckDateCreateNote()));

    checkBoxCountTextSymbols = new QCheckBox();

    checkBoxCountTextSymbols->setChecked(true);
    connect(checkBoxCountTextSymbols, SIGNAL(clicked()),
            this, SLOT(clickCheckCountTextSymbols()));

    checkBoxCountTextLine = new QCheckBox();

    checkBoxCountTextLine->setChecked(true);
    connect(checkBoxCountTextLine, SIGNAL(clicked()),
            this, SLOT(clickCheckCountTextLine()));

    checkBoxDateLastChange = new QCheckBox();

    checkBoxDateLastChange->setChecked(true);
    connect(checkBoxDateLastChange, SIGNAL(clicked()),
            this, SLOT(clickCheckDateLastChange()));

    checkBoxStatusLock = new QCheckBox();

    checkBoxStatusLock->setChecked(true);
    connect(checkBoxStatusLock, SIGNAL(clicked()),
            this, SLOT(clickCheckStatusLock()));

    gLayoutTabPageColumns = new QGridLayout();
    gLayoutTabPageColumns->addWidget(checkBoxVisibleNote,      0, 0);
    gLayoutTabPageColumns->addWidget(checkBoxDateCreateNote,   1, 0);
    gLayoutTabPageColumns->addWidget(checkBoxCountTextSymbols, 2, 0);
    gLayoutTabPageColumns->addWidget(checkBoxCountTextLine,    3, 0);
    gLayoutTabPageColumns->addWidget(checkBoxDateLastChange,   4, 0);
    gLayoutTabPageColumns->addWidget(checkBoxStatusLock,       5, 0);


    tabPageColumns = new QWidget();
    tabPageColumns->setLayout(gLayoutTabPageColumns);

    tabWidgetSettings = new QTabWidget();

    tabWidgetSettings->addTab(tabPageColumns, tr(""));

    vBoxLayoutMain = new QVBoxLayout();
    vBoxLayoutMain->addWidget(tabWidgetSettings);

    setLayout(vBoxLayoutMain);

    QString language = "ru";
    setLanguage(language);
}

// SLOTS

void SettingsTableNote::setLanguage(QString language)
{
    if(language == "English" || language == "en" || language == "En")
    {
        checkBoxVisibleNote->setText(tr("Visible"));
        checkBoxDateCreateNote->setText(tr("Date Create"));
        checkBoxCountTextSymbols->setText(tr("Count Text Symbols"));
        checkBoxCountTextLine->setText(tr("Count Text Line"));
        checkBoxDateLastChange->setText(tr("Date Last Change"));
        checkBoxStatusLock->setText(tr("Lock"));

        tabWidgetSettings->setTabText(0, tr("Columns"));
    }
    if(language == "Russian" || language == "ru" || language == "Ru" || language == tr("Русский"))
    {
        checkBoxVisibleNote->setText(tr("Видимость"));
        checkBoxDateCreateNote->setText(tr("Дата создания"));
        checkBoxCountTextSymbols->setText(tr("Количество символов"));
        checkBoxCountTextLine->setText(tr("Количество строк"));
        checkBoxDateLastChange->setText(tr("Дата последнего изменения"));
        checkBoxStatusLock->setText(tr("Заблокирован"));

        tabWidgetSettings->setTabText(0, tr("Столбцы"));
    }
}

void SettingsTableNote::setCheckedCheckBox(QList <bool> &list)
{
    checkBoxVisibleNote->setChecked(list.at(indexColumnVisible));
    checkBoxDateCreateNote->setChecked(list.at(indexColumnDateOfCreating));
    checkBoxCountTextSymbols->setChecked(list.at(indexColumnCountTextSymbols));
    checkBoxCountTextLine->setChecked(list.at(indexColumnCountTextLines));
    checkBoxDateLastChange->setChecked(list.at(indexColumnDateLastChange));
    checkBoxStatusLock->setChecked(list.at(indexColumnStatusLock));
}

void SettingsTableNote::clickCheckVisibleNote()
{
    emit columnTableVisibleChange(indexColumnVisible,
                                  checkBoxVisibleNote->isChecked());
}
void SettingsTableNote::clickCheckDateCreateNote()
{
    emit columnTableVisibleChange(indexColumnDateOfCreating,
                                  checkBoxDateCreateNote->isChecked());
}
void SettingsTableNote::clickCheckCountTextSymbols()
{
    emit columnTableVisibleChange(indexColumnCountTextSymbols,
                                  checkBoxCountTextSymbols->isChecked());
}
void SettingsTableNote::clickCheckCountTextLine()
{
    emit columnTableVisibleChange(indexColumnCountTextLines,
                                  checkBoxCountTextLine->isChecked());
}

void SettingsTableNote::clickCheckDateLastChange()
{
    emit columnTableVisibleChange(indexColumnDateLastChange,
                                  checkBoxDateLastChange->isChecked());
}

void SettingsTableNote::clickCheckStatusLock()
{
    emit columnTableVisibleChange(indexColumnStatusLock,
                                  checkBoxStatusLock->isChecked());
}
