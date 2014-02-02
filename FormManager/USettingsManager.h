#ifndef USETTINGSMANAGER_H
#define USETTINGSMANAGER_H

#include <QtGui>
#include <QtCore>

#include "TablesManager.h"
#include "Globals.h"
#include "USettings.h"
#include "HotKey/UWidgetSelectHotkey.h"
#include "UEditedExpandableComboBoxDate.h"
#include "ULineEditWithButton/ULineEditWithClearButton.h"

class USettingsManager: public QDialog
{
    Q_OBJECT

public:
    USettingsManager(QWidget *parent = 0);
    void showPageGeneral()
    { tabWidgetSettings->setCurrentWidget(tabGeneralSettings); }

private:
    void createTabManager();
    void createTabNewNote();
    void createTabHotkey();

public:
    QTabWidget *tabWidgetSettings;

    // вкладка "Основные"
    USettings *tabGeneralSettings;
    // вкладка "Основные"

    // вкладка "Менеджер"
    QWidget *tabWidgetManager;

    QGroupBox *groupBoxVisibleColumnNotes;
    QGroupBox *groupBoxVisibleColumnRecycleBin;

    QGroupBox *groupBoxSelectStyle;
    QComboBox *comboBoxStyle;
    // вкладка "Менеджер"

    // вкладка "Новая заметка"
    QScrollArea *areaNewNote;
    QWidget *tabWidgetNewNote;
    QVBoxLayout *vBoxLayoutWidgetNewNote;
    QCheckBox *checkBoxRandomColorNewNote;
    QCheckBox *checkBoxRandomPositionOnScreenNewNote;

    QGroupBox *groupBoxSizeNewNote;
    QLabel *labelWidthNewNote;
    QLabel *labelHeightNewNote;
    QLabel *labelWidthPixelNewNote;
    QLabel *labelHeightPixelNewNote;
    QSpinBox *spinBoxWidthNewNote;
    QSpinBox *spinBoxHeightNewNote;

    QGroupBox *groupBoxPositionNewNote;
    QLabel *labelPositionXNewNote;
    QLabel *labelPositionYNewNote;
    QLabel *labelPositionXPixelNewNote;
    QLabel *labelPositionYPixelNewNote;
    QSpinBox *spinBoxPositionXNewNote;
    QSpinBox *spinBoxPositionYNewNote;

    QGroupBox *groupBoxAdditionally;
    QGroupBox *groupBoxTitleNewNote;
    ULineEditWithClearButton *lineEditTitleNewNote;

    UEditedExpandableComboBoxDate *editedExpandableComboBoxDate;

    QGroupBox *groupBoxTextNewNote;
    QTextEdit *textEditTextNewNote;

    QGroupBox *groupBoxSelectBackgroundColorNewNote;
    UPushButtonSelectColor *pButtonSelectBackgroundColorNewNote;

    QCheckBox *checkBoxVisibleNewNote;
    QCheckBox *checkBoxMinimizeNewNote;

    QGroupBox *groupBoxOpacityNewNote;
    QSlider *sliderOpacityNewNote;

    QCheckBox *checkBoxLockNewNote;

    QGroupBox *groupBoxSelectZPosition;    

    QRadioButton *radioButtonWindowStaysOnBottomHint;
    QRadioButton *radioButtonWindowStaysOnTopHint;
    // вкладка "Новая заметка"

    // вкладка "Горячие клавиши"
    QScrollArea *areaHotkey;
    QWidget *tabWidgetHotkey;
    QGroupBox *groupBoxUseHotkeys;
    UWidgetSelectHotkey *hotkeyNewNote;
    UWidgetSelectHotkey *hotkeyNewNoteFromClipboard;
    UWidgetSelectHotkey *hotkeyHideAllNote;
    UWidgetSelectHotkey *hotkeyShowAllNote;
    UWidgetSelectHotkey *hotkeyShowHideManager;
    QPushButton *pButtonOkHotkey;
    // вкладка "Горячие клавиши"

public:
    // вкладка "Менеджер"
    QCheckBox *checkBoxNotesVisibleNote;
    QCheckBox *checkBoxNotesDateCreateNote;
    QCheckBox *checkBoxNotesCountTextSymbols;
    QCheckBox *checkBoxNotesCountTextLine;
    QCheckBox *checkBoxNotesDateLastChange;
    QCheckBox *checkBoxNotesLock;
    QCheckBox *checkBoxNotesDateOfLastRemoval;
    QCheckBox *checkBoxNotesDateOfLastRestore;

    QCheckBox *checkBoxRecycleBinVisibleNote;
    QCheckBox *checkBoxRecycleBinDateCreateNote;
    QCheckBox *checkBoxRecycleBinCountTextSymbols;
    QCheckBox *checkBoxRecycleBinCountTextLine;
    QCheckBox *checkBoxRecycleBinDateLastChange;
    QCheckBox *checkBoxRecycleBinLock;
    QCheckBox *checkBoxRecycleBinDateOfLastRemoval;
    QCheckBox *checkBoxRecycleBinDateOfLastRestore;

    QCheckBox *checkBoxVisibleTextButtonsPanel;
    // вкладка "Менеджер"

public slots:
    void setLanguage(QString pathToTheFileLanguage);
    void setCheckedNotesCheckBox(QList <bool> &list);
    void setCheckedRecycleBinCheckBox(QList <bool> &list);

private slots:
    // вкладка "Менеджер"
    void clickCheckNotesVisibleNote(bool checked);
    void clickCheckNotesDateCreateNote(bool checked);
    void clickCheckNotesCountTextSymbols(bool checked);
    void clickCheckNotesCountTextLine(bool checked);
    void clickCheckNotesDateLastChange(bool checked);
    void clickCheckNotesLock(bool checked);
    void clickCheckNotesDateOfLastRemoval(bool checked);
    void clickCheckNotesDateOfLastRestore(bool checked);

    void clickCheckRecycleBinVisibleNote(bool checked);
    void clickCheckRecycleBinDateCreateNote(bool checked);
    void clickCheckRecycleBinCountTextSymbols(bool checked);
    void clickCheckRecycleBinCountTextLine(bool checked);
    void clickCheckRecycleBinDateLastChange(bool checked);
    void clickCheckRecycleBinLock(bool checked);
    void clickCheckRecycleBinDateOfLastRemoval(bool checked);
    void clickCheckRecycleBinDateOfLastRestore(bool checked);

    void checkGroupBoxVisibleColumnNotes(bool checked);
    void checkGroupBoxVisibleColumnRecycleBin(bool checked);
    // вкладка "Менеджер"

    // вкладка "Новая заметка"
    void setVisibleAdditionally(bool visible);

    void setRandomColor(bool bValue);
    void setRandomPositionOnScreen(bool bValue);

    void setWidthNewNote(int width);
    void setHeightNewNote(int height);
    void setPositionXNewNote(int x);
    void setPositionYNewNote(int y);

    void setTitleNewNote(QString title);
    void setTextNewNote();

    void setBackgroundColorNewNote(QColor color);
    void setColorToButtonSelectColor(QColor color);

    void setVisibleNewNote(bool visible);
    void setMinimizeNewNote(bool minimize);

    void setOpacityNewNote(int opacity);
    void setLockNewNote(bool lock);

    void setZPositionNewNote(QString zPosition);
    void clickRadioButtonChangeZPosition();

    void changeActivatedItemComboBoxStyle(int index);

    void comboBoxDateTimeItemActivate(QString text);
    void saveToIniListDateTimeFormat();
    // вкладка "Новая заметка"

    // вкладка "Горячие клавиши"
    void setActivateHotkeys(bool activate);
    // вкладка "Горячие клавиши"

signals:
    void languageIsChange(QString);

    // вкладка "Менеджер"
    void columnTableNotesVisibleChange(int indexColumn, bool check);
    void columnTableRecycleBinVisibleChange(int indexColumn, bool check);

    void visibleTextButtonsPanel(bool);
    void sizeIconsPanel(int,int);

    void selectedAnotherStyle(QString);
    // вкладка "Менеджер"

    // вкладка "Горячие клавиши"
    void changeHotkeys();
    void activateHotkeys(bool);
    // вкладка "Горячие клавиши"
};

#endif // USETTINGSMANAGER_H
