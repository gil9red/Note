#ifndef UREADERTEXTFILES_H
#define UREADERTEXTFILES_H

#include <QtGui>
#include <QtCore>

#include "udialoginsertmode.h"

#include "Note.h"

class UReaderTextFiles: public QObject
{
    Q_OBJECT

public:
    bool readUsingDialogIn(QTextEdit *textEdit, QString fileFilters);
    bool readIn(QTextEdit *textEdit,
                QString fileName);

public slots:
    void setLanguage(QString pathToTheFileLanguage);

private:
    UDialogInsertMode *dialogInsertMode;
    QString currentLanguage;

    QString titleMessageBox;
    QString textMessageBox;
    QString strOk;
    QString strNo;
    QString strCancel;
};

#endif // UREADERTEXTFILES_H
