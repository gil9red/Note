#ifndef UDIALOGSELECTLISTNOTES_H
#define UDIALOGSELECTLISTNOTES_H

#include <QtGui>
#include <QtCore>

#include "Globals.h"

const QSize maximimSizeDialogSelectListNotes(400, 300);
const int minimumWidthDialogSelectListNotes = 300;

class UDialogSelectListNotes: public QDialog
{    
    Q_OBJECT

public:
    enum UModeMessage
    {
        MoveSelectedNotesToRecycleBin,
        DeleteSelectedNotes,
        RestoreSelectedNotes,

        MoveSelectedNoteToRecycleBin,
        DeleteSelectedNote,
        RestoreSelectedNote
    };

public:
    UDialogSelectListNotes(QWidget *parent = 0);
    
    void setModeMessage(UModeMessage mode);

    QList <int> getListRows(){ return listRows; }

private:
    void createGUI();
    bool isModeMultipleSelection();

private:
    QTableWidget *tableWidgetNotes;

    QLabel *labelMessage;

    QPushButton *pButtonOk;
    QPushButton *pButtonCancel;

    QPushButton *pButtonUnselect;
    QPushButton *pButtonSelectAll;

    QList <int> listRows;

    UModeMessage currentModeMessage;

    QString currentLanguage;

public slots:
    // Set language program
    void setLanguage(QString pathToTheFileLanguage);
    // Set language program

    void selectTableRow(QTableWidget *table,
                        QList <int> *tableListRows);

private slots:
    void clickButtonOk();
    void clickButtonCancel();

    void unselect();
    void selectAll();

    void checkedUncheckedRowTable(int row);

protected:
    void showEvent(QShowEvent *);
};

#endif // UDIALOGSELECTLISTNOTES_H
