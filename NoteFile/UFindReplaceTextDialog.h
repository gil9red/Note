#ifndef UFindReplaceTextDialog_H
#define UFindReplaceTextDialog_H

#include <QtGui>
#include <QtCore>

#include "UPushButtonSelectColor.h"
#include "ULineEditWithButton/ULineEditWithClearButton.h"

#include "Globals.h"

class UFindReplaceTextDialog: public QWidget
{
    Q_OBJECT

public:
    UFindReplaceTextDialog(QWidget *parent = 0);
    void setTextEdit(QTextEdit *textEdit);    
    void setDeleteWhenClose(bool _delete){ deleteWhenClose = _delete; }
    void deselect();

private:
    void createForm();

public:
    QTextEdit *textEdit;
    QTextCursor textCursor;

private:
    QPushButton *pButtonFind;
    QPushButton *pButtonReplace;
    QPushButton *pButtonReplaceAll;
    QPushButton *pButtonClose;

    QLabel *labelFind;
    QLabel *labelReplaceTo;

    ULineEditWithClearButton *lineEditFindText;
    ULineEditWithClearButton *lineEditReplaceText;

    QLabel *labelStatus;

    QRadioButton *rButtonUp;
    QRadioButton *rButtonDown;

    QGroupBox *groupBoxDirection;

    QCheckBox *checkBoxWholeWords;
    QCheckBox *checkBoxCaseSensitive;
    QCheckBox *checkBoxRegExp;

    QGroupBox *groupBoxSettings;

    UPushButtonSelectColor *pButtonSelectColor;
    QColor colorSelection;

    bool deleteWhenClose;

    QString strNoMatchFound;
    QString strReplaced;

public slots:
    void setLanguage(QString pathToTheFileLanguage);

    void find(bool down);
    void find(){ find(rButtonDown->isChecked()); }
    void replace();
    void replaceAll();

private slots:
    void showError(const QString &error);
    void showMessage(const QString &message);

    void selectColor(QColor color);

signals:
    void languageIsChange(QString);
    void iClose();

protected:
    void closeEvent(QCloseEvent * event);
    void hideReplaceWidgets();
};


class UFindTextDialog: public UFindReplaceTextDialog
{
public:
    UFindTextDialog(QWidget *parent = 0):
        UFindReplaceTextDialog(parent){ hideReplaceWidgets(); }
};


class UUnitedFindReplaceTextDialog: public QWidget
{
    Q_OBJECT

public:
    UUnitedFindReplaceTextDialog(QWidget *parent = 0);
    void setTextEdit(QTextEdit *textEdit);
    void showFind(){ tabWidget->setCurrentWidget(findTextDialog); }
    void showReplace(){ tabWidget->setCurrentWidget(findReplaceTextDialog); }

private:
    UFindReplaceTextDialog *findReplaceTextDialog;
    UFindTextDialog *findTextDialog;
    QTabWidget *tabWidget;

public slots:
    void setLanguage(QString pathToTheFileLanguage);

private slots:   
    void selectTab(int index){ setWindowTitle(tabWidget->tabText(index)); }

signals:
    void languageIsChange(QString);
    void iClose();

protected:
    void closeEvent(QCloseEvent * event);
};

#endif // UFindReplaceTextDialog_H
