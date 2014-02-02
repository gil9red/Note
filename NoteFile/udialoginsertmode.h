#ifndef UDIALOGINSERTMODE_H
#define UDIALOGINSERTMODE_H

#include <QtGui>
#include <QtCore>

#include "Globals.h"

class UDialogInsertMode: public QDialog
{
    Q_OBJECT

public:
    explicit UDialogInsertMode(QWidget *parent = 0);

    enum VariantInsertMode
    {
        None           = -1,
        Top            = 0,
        End            = 1,
        Middle         = 2,
        PositionCursor = 3
    };

private:
    QGroupBox *groupBoxRadioButtonSelectInsertMode;

    QVBoxLayout *vBoxLayoutGroupBox;

    QRadioButton *radioButtonTop;
    QRadioButton *radioButtonEnd;
    QRadioButton *radioButtonMiddle;
    QRadioButton *radioButtonPositionCursor;

    QVBoxLayout *vBoxLayoutMain;

    QHBoxLayout *hBoxLayoutButtons;
    QPushButton *pButtonOk;
    QPushButton *pButtonCancel;

    int result;

    bool pushButtonCancel;

public slots:
    void setLanguage(QString pathToTheFileLanguage);
    void showDialog();
    int getResultExecuteDialog(){ return result; }

private slots:
    void clickPushButtonOk(){ close(); }
    void clickPushButtonCancel();
};

#endif // UDIALOGINSERTMODE_H
