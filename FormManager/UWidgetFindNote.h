#ifndef UWIDGETFINDNOTE_H
#define UWIDGETFINDNOTE_H

#include <QtGui>
#include <QtCore>

#include "Globals.h"
#include "TablesManager.h"
#include "ULineEditWithButton/ULineEditWithClearButton.h"
#include "QProgressIndicator.h"

class UWidgetFindNote: public QWidget
{
    Q_OBJECT

public:
    UWidgetFindNote(QWidget *parent = 0);
    void setLinkInputOutput(QTableWidget *tInputNotes,
                            QTableWidget *tInputRecycleBin,
                            QTableWidget *tOutputNotes,
                            QTableWidget *tOutputRecycleBin);

private:
    void createWidgets();

private:
    QVBoxLayout *vBoxLayoutMain;

    QHBoxLayout *hBoxLineEditFind;
    QLabel *labelInputFindText;
    ULineEditWithClearButton *lineEdit;

    QVBoxLayout *vBoxLayoutCommandButtons;
    QPushButton *pButtonFind;
    QPushButton *pButtonClose;
    QPushButton *pButtonCount;

    // настройки поиска
    QGroupBox *groupBoxFindSettings;
    QGridLayout *gBoxLayoutGroupBoxFindSettings;
    // с учетом регистра
    QCheckBox *checkBoxCaseSensitive;
    // только слово целиком
    QCheckBox *checkBoxWholeWordsOnly;

    // Искать в
    QGroupBox *groupBoxSearchIn;
    QVBoxLayout *vBoxLayoutGroupBoxSearchIn;
    QCheckBox *checkBoxNotes;
    QCheckBox *checkBoxRecycleBin;
    // Искать в

    // Учитывать видимость заметки
    QGroupBox *groupBoxConsiderVisibilityNotes;
    QVBoxLayout *vBoxLayoutGroupBoxConsiderVisibilityNotes;
    QRadioButton *rButtonNoteVisibleTrue;
    QRadioButton *rButtonNoteVisibleFalse;

    // Учитывать блокировку заметки
    QGroupBox *groupBoxConsiderLockNotes;
    QVBoxLayout *vBoxLayoutGroupBoxConsiderLockNotes;
    QRadioButton *rButtonNoteLock;
    QRadioButton *rButtonNoteUnlock;

    // Для поиска и вывода
    QTableWidget *tableInputNotes;
    QTableWidget *tableInputRecycleBin;
    QTableWidget *tableOutputNotes;
    QTableWidget *tableOutputRecycleBin;

    QString lastFindText;

    QString strMsgCountTitle;
    QString strMsgCountText;

    QPushButton *pButtonShowHideAdditionalSearchSettings;

    QString strHideAdditionalSearchSettings;
    QString strShowAdditionalSearchSettings;        

public slots:
    void setLanguage(QString pathToTheFileLanguage);

    void find();
    void find(QString searchText);

    void count();
    int count(QString searchText);

    void refreshFind();

private slots:
    void clickCheckBoxNotes();
    void clickCheckBoxRecycleBin();

    void setVisibleConsiderVisibilityNotes(bool visible);
    void setVisibleConsiderLockNotes(bool visible);

    void setTheVisibleForMoreSearchOptions(bool visible);
    void setTheCheckedForMoreSearchOptions(bool checked);

    void clickButtonShowHideAdditionalSearchSettings();

signals:
    void searchIsComplete();
    void iClose();

protected:
    void closeEvent(QCloseEvent *);
};

#endif // UWIDGETFINDNOTE_H
