QT += core gui network webkit

TARGET = Note
TEMPLATE = app

VERSION = 0.2.2
DESTDIR = ../bin

SOURCES += main.cpp\        
    ManagerNote.cpp \
    HelpBrowser.cpp \
    USettings.cpp \
    FormManager/UNoteManager.cpp \
    FormManager/TablesManager.cpp \
    FormManager/UWidgetFindNote.cpp \
    NoteFile/uwritertextfiles.cpp \
    NoteFile/ureadertextfiles.cpp \
    NoteFile/udialoginsertmode.cpp \
    NoteFile/Note.cpp \
    NoteFile/DialogSettingsNote.cpp \
    NoteFile/abstractnote.cpp \
    NoteFile/UTextEdit.cpp \
    NoteFile/Password/USettingsPassword.cpp \
    NoteFile/Password/UPasswordProtect.cpp \
    UAboutProgram.cpp \
    qtsingleapplication/qtsingleapplication.cpp \
    qtsingleapplication/qtlockedfile_win.cpp \    
    qtsingleapplication/qtlockedfile.cpp \
    qtsingleapplication/qtlocalpeer.cpp \
    NoteFile/UFormatingPanelText.cpp \
    UAnimatedSplashScreen.cpp \
    UButtonsWithMenu/UButtonWithExtendedContextMenu.cpp \
    UButtonsWithMenu/UButtonWithContextMenu.cpp \
    UPushButtonSelectColor.cpp \
    FormManager/USettingsManager.cpp \
    NoteFile/UFindReplaceTextDialog.cpp \
    UFileAssociationManager/UFileAssociationManager.cpp \
    HotKey/UWidgetSelectHotkey.cpp \
    UDialogSelectListNotes.cpp \
    QProgressIndicator.cpp \
    UEditedExpandableComboBoxDate.cpp \
    NoteFile/USymbolTable.cpp \
    ULineEditWithButton/ULineEditWithButtons.cpp


HEADERS  += ManagerNote.h \
    HelpBrowser.h \
    USettings.h \
    Globals.h \
    FormManager/UNoteManager.h \
    FormManager/TablesManager.h \
    FormManager/Delegates.h \
    FormManager/UWidgetFindNote.h \
    NoteFile/uwritertextfiles.h \
    NoteFile/ureadertextfiles.h \
    NoteFile/UOfficeWordStyleComboBox.h \
    NoteFile/udialoginsertmode.h \
    NoteFile/Note.h \
    NoteFile/DialogSettingsNote.h \
    NoteFile/abstractnote.h \
    NoteFile/UTextEdit.h \
    NoteFile/Password/USettingsPassword.h \
    NoteFile/Password/UPasswordProtect.h \
    UAboutProgram.h \
    qtsingleapplication/qtsingleapplication.h \
    qtsingleapplication/QtSingleApplication \
    qtsingleapplication/qtlockedfile.h \
    qtsingleapplication/QtLockedFile \
    qtsingleapplication/qtlocalpeer.h \
    NoteFile/UFormatingPanelText.h \
    UAnimatedSplashScreen.h \
    UButtonsWithMenu/UButtonWithExtendedContextMenu.h \
    UButtonsWithMenu/UButtonWithContextMenu.h \
    UPushButtonSelectColor.h \
    FormManager/USettingsManager.h \
    NoteFile/UFindReplaceTextDialog.h \
    UFileAssociationManager/UFileAssociationManager.h \
    HotKey/UWidgetSelectHotkey.h \
    UDialogSelectListNotes.h \
    QProgressIndicator.h \
    UEditedExpandableComboBoxDate.h \
    NoteFile/USymbolTable.h \
    ULineEditWithButton/ULineEditWithClearButton.h \
    ULineEditWithButton/ULineEditWithButtons.h

RC_FILE = resource/app.rc

RESOURCES += \
    rcfile.qrc

OTHER_FILES += \
    resource/license.txt \
    resource/history.txt \
    resource/app.rc
