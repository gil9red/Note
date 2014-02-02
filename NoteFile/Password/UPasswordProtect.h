#ifndef UPASSWORDPROTECT_H
#define UPASSWORDPROTECT_H

#include <QtGui>
#include <QtCore>

#include "ULineEditWithButton/ULineEditWithClearButton.h"

#include "Globals.h"

class UPasswordProtect: public QWidget
{
    Q_OBJECT
public:
    explicit UPasswordProtect(QWidget *parent = 0);

    void setPassword(QString str){ password = str; }
    QString getPassword(){ return password; }

private:
    bool testInputData();

private:    
    QLabel *labelInputPassword;
    ULineEditWithClearButton *lineEditInputPassword;

    QPushButton *pButtonOk;

    QString strTitle;
    QString strTextError;

    QString password;

public slots:
    void setLanguage(QString pathToTheFileLanguage);
    void clickPButtonOk();

signals:
    void enteredThePasswordRight();
};

#endif // UPASSWORDPROTECT_H
