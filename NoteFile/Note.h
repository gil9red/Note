#ifndef NOTE_H
#define NOTE_H

#include <QtGui>
#include <QtCore>

#ifdef Q_WS_WIN
#include <windows.h>
#endif

#include "abstractnote.h"

#include "DialogSettingsNote.h"
#include "UTextEdit.h"
#include "Password/UPasswordProtect.h"
#include "UFindReplaceTextDialog.h"

#include "Globals.h"

class Note: public AbstractNote
{
    Q_OBJECT
    
public:
    Note(QString path);
    Note(QString text, QString path);
    Note(DataNote *dataNewNote, QString path);

    QString getPathInFileNote(){ return pathIni; }

    void testAutocompleterLanguage();

private:
    void initNote();
    void initNote(QString text);
    void initNote(DataNote *_data);
    void createWidgetsToNote();
    void setProtectNote(bool bValue);
    void setVisibleCommandButtons(bool bValue);
    void setEnabledCommandButtons(bool bValue);

    // Action Note
    void createActions();
    // Action Note

public:    
    DataNote dataNote;
    UTextEdit *textEdit;

private:
    UPasswordProtect *passwordProtect;

    UFormatingPanelText *formatingPanelText;

    DialogSettingsNote *dialogSettings;
    QPoint positionNote;

    QString strTitleMessage;
    QString strTextMessageMoveNoteToRecycleBin;
    QString strOk;
    QString strCancel;
    QString pathIni;

    QString currentLanguage;

    // для того чтобы при инициализации заметки,
    // при заполнении textedit не происходило обновление
    // даты последнего изменения
    bool initNodeIsEnd;

    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;

    QAction *actionSelectAll;

    QAction *actionSettings;
    QAction *actionHide;
    QAction *actionLock;
    QAction *actionUnlock;
    QAction *actionMoveToRecycleBin;
    QAction *actionDuplicate;

    QAction *actionSaveAs;
    QAction *actionOpen;

    QAction *actionPrint;
    QAction *actionPreviewPrint;

    QAction *actionFind;
    QAction *actionReplace;
    UUnitedFindReplaceTextDialog *unitedFindReplaceTextDialog;

    QMenu *menuOpacity;
    QAction *actionOpacity_Other;
    QAction *actionOpacity_20;
    QAction *actionOpacity_30;
    QAction *actionOpacity_40;
    QAction *actionOpacity_50;
    QAction *actionOpacity_60;
    QAction *actionOpacity_70;
    QAction *actionOpacity_80;
    QAction *actionOpacity_90;
    QAction *actionOpacity_100;

    QMenu *menuColor;
    UPushButtonSelectColor *colors;

    QAction *actionTitle;

    QHBoxLayout *hBoxLayoutCommandButtons;
    QToolButton *buttonSettings;
    QToolButton *buttonOnOffAutocomplete;
    QToolButton *buttonHide;
    QToolButton *buttonLockUnlock;
    QToolButton *buttonHideShowFormatingPanelText;

    QLabel *labelTitleWindow;

    bool autocompletionFileNotFound;

    // изменит свое состояние -
    // когда сработает событие enterEvent и leaveEvent,
    // будет такой костыль, пока я не придумаю
    // другой способ не показывать кнопку автодополнения
    // в событии changeEvent
    bool onetimeCounter;

    QString theirFormats;
    QString textFormats;
    QString imageFormats;
    QString filterFormats;

    QTimer *timerAnimationOpacity;

    bool visibleContextMenu;

    UMiniDialogSetTitle *miniDialogSetTitle;
    UMiniDialogSetOpacity *miniDialogSetOpacity;

public slots:
    virtual void setWindowTitle(const QString &title);
    void setWindowTitleWithSignal(const QString &title);
    virtual void setWindowOpacity(qreal level);

    void setDataNote(DataNote data);

    void showSettings();

    void showMiniDialogSetTitle();
    void showMiniDialogSetOpacity();

    void deleteMiniDialogSetTitle();
    void deleteMiniDialogSetOpacity();

    void loadDataNote();
    void saveDataNote();

    void setLock(bool bValue);
    void setAutocomplete(bool bValue);
    void setVisibleNote(bool visible);
    void clickMoveToRecycleBin();
    void clickHide(){ setVisibleNote(false); }
    void clickLock(){ setLock(true); }
    void clickUnlock(){ setLock(false); }
    void clickButtonLockUnlock();
    void clickButtonOnOffAutocomplete();    
    void refreshNote();

//  SET LANGUAGE
    void setLanguage(QString pathToTheFileLanguage);
//  SET LANGUAGE

    // Save as and Open
    void clickActionSaveAs();
    void clickActionOpen();
    // Save as and Open

    void opacity_other(){ showSettings(); }
    void opacity_20();
    void opacity_30();
    void opacity_40();
    void opacity_50();
    void opacity_60();
    void opacity_70();
    void opacity_80();
    void opacity_90();
    void opacity_100();

    void showContextMenu(QPoint point);

    // сделать копию
    void pushDuplicate(){ emit makeDuplicate(&dataNote); }

//    Control FormatingPanelText
    void hideFormatingPanelText(){ setVisibleFormatingPanelText(false); }
    void clickbuttonHideShowFormatingPanelText();
    void setVisibleFormatingPanelText(bool bValue);
//    Control FormatingPanelText

    void setMinimize(bool bValue);

    void setColorBackground(QColor color);

    void setZPosition(QString zPosition);

    // Print and preview print
    void filePrint();
    void filePrintPreview();
    void printPreview(QPrinter *);
    // Print and preview print

    // Find/Replace
    void findText();
    void replaceText();
    void hideFindReplaceText();
    // Find/Replace

    // сохранение отдельных параметров заметки в ее файл
    void savePosition();
    void saveSize();
    void saveTitle();
    void saveLock();
    void saveText();
    void saveVisible();
    void saveOpacity();
    void saveColor();
    void saveZPosition();
    void saveDateOfCreating();
    void saveDateLastChange();
    void saveDateOfLastRemoval();
    void saveDateOfLastRestore();
    void saveCountTextSymbols();
    void saveCountTextLines();
    void savePassword();
    void saveProtectionActive();
    void saveAutocomplete();
    void saveMinimize();
    // сохранение отдельных параметров заметки в ее файл

private slots:
    void textEditChangeText();
    void removeProtection()
    { setProtectNote(false); }

    void opacityMinus();

    void noteFocus_True();
    void noteFocus_False();

    void deleteDialogSettings();

    void enabledTrueCommandButtons()
    { setEnabledCommandButtons(true); }

    void setPassword(QString password, bool active);    

signals:
    void titleChange(Note *note);
    void visibleChange(Note *note);
    void lockIsChange(Note *note);

    void noteDelete(Note *note);

    void languageIsChange(QString);

    void propertiesTextEditNote(Note *note);

    void makeDuplicate(DataNote*);

    void noteIsMinimize(bool);

protected:
    void closeEvent(QCloseEvent *)
    { saveDataNote(); }

    void dropEvent(QDropEvent *dropEvent);
    void dragEnterEvent(QDragEnterEvent *dragEnter);
    void contextMenuEvent(QContextMenuEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void focusInEvent(QFocusEvent *);
    void focusOutEvent(QFocusEvent *);

    bool eventFilter(QObject *object, QEvent *event);

    void resizeEvent(QResizeEvent *);
    void moveEvent(QMoveEvent *);

    void mouseDoubleClickEvent(QMouseEvent *event);

    void changeEvent(QEvent *event);
};

#include "ureadertextfiles.h"
#include "uwritertextfiles.h"

Q_DECLARE_METATYPE(Note*)

#endif // NOTE_H
