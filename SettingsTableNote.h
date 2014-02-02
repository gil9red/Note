#ifndef SETTINGSTABLENOTE_H
#define SETTINGSTABLENOTE_H

#include <QDialog>
#include <QTabWidget>
#include <QGridLayout>
#include <QCheckBox>
#include <QTextCodec>

const short countColumns                = 7;

const short indexColumnTitle            = 0;
const short indexColumnVisible          = 1;
const short indexColumnDateOfCreating   = 2;
const short indexColumnCountTextSymbols = 3;
const short indexColumnCountTextLines   = 4;
const short indexColumnDateLastChange   = 5;
const short indexColumnStatusLock       = 6;


const short indexMainColumn = indexColumnTitle;

class SettingsTableNote: public QDialog
{
    Q_OBJECT
public:
    SettingsTableNote(QWidget *parent = 0);

private:
    QTabWidget *tabWidgetSettings;

    QVBoxLayout *vBoxLayoutMain;

    QWidget *tabPageColumns;
    QGridLayout *gLayoutTabPageColumns;

public:
    QCheckBox *checkBoxVisibleNote;
    QCheckBox *checkBoxDateCreateNote;
    QCheckBox *checkBoxCountTextSymbols;
    QCheckBox *checkBoxCountTextLine;
    QCheckBox *checkBoxDateLastChange;
    QCheckBox *checkBoxStatusLock;

public slots:
    void setLanguage(QString language);
    void setCheckedCheckBox(QList <bool> &list);

private slots:
    void clickCheckVisibleNote();
    void clickCheckDateCreateNote();
    void clickCheckCountTextSymbols();
    void clickCheckCountTextLine();
    void clickCheckDateLastChange();
    void clickCheckStatusLock();

signals:
    void columnTableVisibleChange(int indexColumn, bool check);
};

#endif // SETTINGSTABLENOTE_H
