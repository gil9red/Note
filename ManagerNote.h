#ifndef ManagerNote_H
#define ManagerNote_H

#include <QtGui>
#include <QtCore>

#ifdef Q_WS_WIN
#include <windows.h>
#endif

#include "NoteFile/Note.h"
#include "HelpBrowser.h"
#include "USettings.h"
#include "UAboutProgram.h"
#include "FormManager/UNoteManager.h"
#include "UDialogSelectListNotes.h"
#include "Globals.h"

void qSort(QList <int> *list, int minIndex, int maxIndex);
void qSort(QList <int> *list);
void removeDuplicateItems(QList <int> *list);
QList <int> getListIndexSelectedRow(QTableWidget *table);

void moveTheFile(QString oldNameFile, QString pathInDir);

class ManagerNote: public QMainWindow
{
    Q_OBJECT

public:   
    ManagerNote(QWidget *parent = 0);

    // начать подготовительную фазу
    void startThePreparatoryPhase(int phaseNumber);

private:
    //---
    void createFormManager();
    void preparationOfSecondaryWindows();
    void createIconTray();
    void checkOrCreateFolderForUseBy();
    void initializationManager();
    void addingNotesToTables();
    void loadLanguage();
    void finalPreparations();
    //---

    void loadInTableWidgetNotesNote();
    void loadInTableWidgetRecycleBinNote();
    void addMenu();
    void addMenuSystemTrayIcon(QSystemTrayIcon *tray);
    void addCommandButtons();
    void readDataInTables();
    void writeDataTables();
    void cutPasteRowTable(QTableWidget *sender,
                          QTableWidget *receiver,
                          int row);

    void moveRowFromTheRecycleBin(int row);
    void moveRowToTheRecycleBin(int row);

    // set connect to note
    void setAllConnectForNote(Note *note);
    // set connect to note

    void addToTheEndTablesRow(QTableWidget *table,
                              QVariant dataMainCol,
                              DataNote *dataNote);

    QList <QString> * getListTranslaction() const;

private:    
    // Notes BUTTONS
    QPushButton *pButtonAddNote;
    QPushButton *pButtonHideNote;
    QPushButton *pButtonShowNote;
    QPushButton *pButtonMoveNoteToRecycleBin;

    UButtonWithContextMenu *pButtonAllNotesNotes;
    QMenu *menuForPButtonAllNotesNotes;
    QAction *actionMoveAllNotesToRecycleBin;
    QAction *actionShowAllNote;
    QAction *actionHideAllNote;

    // RECYCLE BIN PBUTTONS
    QPushButton *pButtonDeleteNote;
    QPushButton *pButtonRestoreNote;

    UButtonWithContextMenu *pButtonAllNotesRecycleBin;
    QMenu *menuForPButtonAllNotesRecycleBin;
    QAction *actionDeleteAllNote;
    QAction *actionRestoreAllNote;

    QVBoxLayout *vBoxLayoutMain;

public:
    UNoteManager *mainWidget;

    // System tray
    QSystemTrayIcon *systemTrayIcon;
    QMenu *menuSystemTrayIcon;
    QAction *actionTrayMenuQuit;
    QAction *actionTrayMenuAddNote;
    QAction *actionTrayMenuSettings;
    QAction *actionTrayMenuShowHideManager;
    QAction *actionTrayMenuShowAllNotes;
    QAction *actionTrayMenuHideAllNotes;

    QAction *actionTrayMenuActivateHotkeys;
    QAction *actionTrayMenuDeactivateHotkeys;

    QMenu *helpMenu;
    QAction *actionHelp;
    QAction *actionAbout;
    // System tray

    QMenuBar *menu;

    QMenu *fileMenu;
    QAction *actionOpen;
    QAction *actionNewNote;
    QAction *actionNewNoteFromClipboard;

    QMenu *editMenu;
    QAction *actionFindNote;

    QString strOk;
    QString strNo;
    QString strCancel;

    QString strTitleQuestion;
    QString strTitleHint;
    QString strTextAddNewNoteInTable;

    QString strTextMessageDeleteItem;

    QString strTextMessageDeleteAllItem;
    QString strTextMessageRestoreAllItem;

    QString strTextMessageClearList;
    QString strTextMessageMoveNoteToRecycleBin;

    QString strTextMessageMoveSelectedNotesToRecycleBin;
    QString strTextMessageDeleteSelectedNotes;
    QString strTextMessageRestoreSelectedNotes;

    QString strTextMessageSystemTray;

    QString strRowAndTitleItemInTable;

    QString currentLanguage;

    QString strDescriptionOfTheProgram;
    QString strAllNotes;
    QString strVisibleNotes;
    QString strInvisibleNotes;
    QString strNotesInTheRecycleBin;

    USettings *settingsProgram;

    UAboutProgram *aboutProgram;

    bool managerIsMaximized;
    // сворачивать менеджер в трей при закрытии
    bool minimizeToTrayManagerAtClosing;
    //
    bool showManagerAtStartProgram;

    // указатель на удаляемую заметку, используется только
    // в случаи, когда с заметки отправляется сигнал о удалении
    // ее самой
    Note *currentDeletedNote;

    // completer
    QCompleter *completer;
    // completer

    int indexSortColumnTableNotes;
    int indexSortColumnTableRecycleBin;

    Qt::SortOrder sortOrderTableNotes;
    Qt::SortOrder sortOrderTableRecycleBin;

    UWidgetFindNote *findNote;

    QList <Note*> *listLoadVisibleNotes;

public slots:
    // Notes
    void addNewNote();
    void addNewNoteFromClipboard();
    // если checked у столбца есть - действие над checked строками,
    // или действие над выбранной строки
    void clickButtonShowNote();
    void clickButtonHideNote();
    void clickButtonMoveNoteToRecycleBin();
    //
    void clickButtonMoveThisNoteToRecycleBin();
    void clickButtonMoveThisNoteToRecycleBin(Note *note);

    void clickButtonSettingsNote();
    void clickButtonSettingsNote(Note *note);

    void clickButtonShowThisNote();
    void clickButtonShowThisNote(Note *note);

    void clickButtonHideThisNote();
    void clickButtonHideThisNote(Note *note);

    void clickButtonShowAllNote();
    void clickButtonHideAllNote();

    void clickButtonClearListWidgetNote();
    // Notes

    // RECYCLE BIN
    void clickRestoreThisItem(int row);
    void clickRestoreThisItem(QString nameFile);

    void clickDeleteThisItem(int row);
    void clickDeleteThisItem(QString nameFile);

    void clickRestoreItem();
    void clickDeleteItem();

    void clickRestoreAllItem();
    void clickDeleteAllItem();
    // RECYCLE BIN

    void changeNote(Note *note);

    void showHide(QSystemTrayIcon::ActivationReason reason);

    // These slots are called when notes send a signal to remove themselves
    void deleteNote(Note *note);
    void deleteCurrentNote(){ delete currentDeletedNote; }
    // These slots are called when notes send a signal to remove themselves

    // Set language program
    void setLanguage(QString pathToTheFileLanguage);
    // Set language program

    void makeDublicateNote(DataNote *dataNewNote);
    void tableCellChange(int row, int col);
    void showSettingsProgram();

    void closeProgram();

    void showManager(){ showForegroundWindow(this); }
    void hideManager();
    void showHideManager();
    void setVisibleManager(bool visible);

    void saveInIni(){ writeDataTables(); }

    void addNewNoteInTable(QString nameFileNote,
                           QTableWidget *tableWidget);
    void addNewNoteInTable(QString nameFileNote,
                           QString textNote,
                           QTableWidget *tableWidget);
    void addNewNoteInTable(QString nameFileNote,
                           DataNote *dataNewNote,
                           QTableWidget *tableWidget);

    // OPEN FILE NOTE USING PATH TO FILE
    void openFileNote(QString pathToFile);

    // OPEN
    void open();    

    // сообщение получаемое от QtSingleApplication
    // при попытке создания второй копии программы
    void messageToApp(QString message);

    void refreshToolTipSystemTrayIcon();

    void setStyleSheetFromFile(QString path);

    void changeTrueFalseValueCells(int row, int col);

    // HOTKEY
    void setActivateHotkeys(bool activate);
    void registerHotKey(QString hotkey, int idHotkey);
    void unregisterHotKey(int idHotkey);
    void acceptHotkeys();
    // HOTKEY

private slots:
    void propertiesNoteTextIsChange(Note *note);

    // about
    void about();
    void deleteDialogAbout();
    // about

    void help();

    // find notes
    void showDialogFindNotes();
    void deleteDialogFindNotes();
    // find notes

    // HOTKEY
    void clickActionActivateHotkeys(){ emit activateHotkeys(true); }
    void clickActionDeactivateHotkeys(){ emit activateHotkeys(false); }
    // HOTKEY

signals:
    void setVisibleAllNote(bool visible);
    void closeAllNote();
    void languageIsChange(QString language);

    void changedTheNumberOfNotes();
    void changedTheStatusOfNotes();

    void activateHotkeys(bool);

protected:
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent * event);
    void dropEvent(QDropEvent *dropEvent);
    void dragEnterEvent(QDragEnterEvent *dragEnter);
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);

#ifdef Q_WS_WIN
    bool winEvent(MSG *message, long *result);
#endif

    void keyReleaseEvent(QKeyEvent *event);
};

#endif // ManagerNote_H
