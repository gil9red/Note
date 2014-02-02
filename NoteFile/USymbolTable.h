#ifndef USYMBOLTABLE_H
#define USYMBOLTABLE_H

#include <QtGui>
#include <QtCore>

#include "NoteFile/UOfficeWordStyleComboBox.h"

#include "ULineEditWithButton/ULineEditWithClearButton.h"

const QString patternRange = "%1-%2";

const int widthCell = 30;
const int heightCell = 30;

class USymbolTable: public QDialog
{
    Q_OBJECT
    
    struct URangeUnicode
    {
        URangeUnicode(QString name, QString range)
        {
            this->name = name;
            this->range = range;
        }

        QString name;
        QString range;
    };

public:
    USymbolTable(QWidget *parent = 0);

private:
    void createGUI();
    void addToTable(QChar symbol);

private:
    QTableWidget *table;
    QFontComboBox *fontComboBox;
    UOfficeWordStyleComboBox *comboBoxSymbolicPlane;
    int currentUsedRow;
    int currentUsedCol;
    QList <URangeUnicode *> listRangeUnicode;
    QPushButton *pButtonSelect;
    QPushButton *pButtonClose;
    QLabel *labelSymbol;
    QLabel *labelKeySymbol;
    QLineEdit *lineEditFindSymbol;
    QTableWidgetItem *lastItem;

public slots:
    void setLanguage(QString pathToTheFileLanguage);

    void displaySymbols(uint min, uint max);
    void refreshTable();
    void setTableFontFamily(QString family);
    void initComboBoxSymbolicPlane();
    void initListRangeUnicode();
    void findSymbol(QString text);

private slots:
    void selectedComboBoxSymbolicPlane(int index);
    void clickItemTableWidget(QTableWidgetItem *item);
    void doubleClickItemTableWidget(QTableWidgetItem *item);
    void clickSelectSymbol();
    void selectFontFamily(QString family);

signals:
    void symbolIsSelected(QString);
    void iClose();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void closeEvent(QCloseEvent *){ emit iClose(); }
};

#endif // USYMBOLTABLE_H
