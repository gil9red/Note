#include "USettingsManager.h"

/// PUBLIC
USettingsManager::USettingsManager(QWidget *parent):
    QDialog(parent)
{                
    tabGeneralSettings = new USettings();
    connect(tabGeneralSettings, SIGNAL(languageIsChange(QString)),
            this, SLOT(setLanguage(QString)));

    createTabManager();
    createTabNewNote();
    createTabHotkey();

    tabWidgetSettings = new QTabWidget();
    tabWidgetSettings->addTab(tabGeneralSettings,
                              QIcon(IconGeneral),
                              tr(""));
    tabWidgetSettings->addTab(tabWidgetManager, tr(""));
    tabWidgetSettings->addTab(areaNewNote, tr(""));
    tabWidgetSettings->addTab(areaHotkey, tr(""));

    QVBoxLayout *vBoxLayoutMain = new QVBoxLayout();
    vBoxLayoutMain->addWidget(tabWidgetSettings);

    setLayout(vBoxLayoutMain);
}

/// PRIVATE
void USettingsManager::createTabManager()
{
    // VISIBLE COLUMN TABLE NOTES
    checkBoxNotesVisibleNote = new QCheckBox();
    checkBoxNotesVisibleNote->setChecked(true);
    connect(checkBoxNotesVisibleNote, SIGNAL(toggled(bool)),
            this, SLOT(clickCheckNotesVisibleNote(bool)));

    checkBoxNotesDateCreateNote = new QCheckBox();
    checkBoxNotesDateCreateNote->setChecked(true);
    connect(checkBoxNotesDateCreateNote, SIGNAL(toggled(bool)),
            this, SLOT(clickCheckNotesDateCreateNote(bool)));

    checkBoxNotesCountTextSymbols = new QCheckBox();
    checkBoxNotesCountTextSymbols->setChecked(true);
    connect(checkBoxNotesCountTextSymbols, SIGNAL(toggled(bool)),
            this, SLOT(clickCheckNotesCountTextSymbols(bool)));

    checkBoxNotesCountTextLine = new QCheckBox();
    checkBoxNotesCountTextLine->setChecked(true);
    connect(checkBoxNotesCountTextLine, SIGNAL(toggled(bool)),
            this, SLOT(clickCheckNotesCountTextLine(bool)));

    checkBoxNotesDateLastChange = new QCheckBox();
    checkBoxNotesDateLastChange->setChecked(true);
    connect(checkBoxNotesDateLastChange, SIGNAL(toggled(bool)),
            this, SLOT(clickCheckNotesDateLastChange(bool)));

    checkBoxNotesLock = new QCheckBox();
    checkBoxNotesLock->setChecked(true);
    connect(checkBoxNotesLock, SIGNAL(toggled(bool)),
            this, SLOT(clickCheckNotesLock(bool)));

    checkBoxNotesDateOfLastRemoval = new QCheckBox();
    checkBoxNotesDateOfLastRemoval->setChecked(true);
    connect(checkBoxNotesDateOfLastRemoval, SIGNAL(toggled(bool)),
            this, SLOT(clickCheckNotesDateOfLastRemoval(bool)));

    checkBoxNotesDateOfLastRestore = new QCheckBox();
    checkBoxNotesDateOfLastRestore->setChecked(true);
    connect(checkBoxNotesDateOfLastRestore, SIGNAL(toggled(bool)),
            this, SLOT(clickCheckNotesDateOfLastRestore(bool)));

    QVBoxLayout *vBoxLayoutGroupBoxVisibleColumnNotes = new QVBoxLayout();
    vBoxLayoutGroupBoxVisibleColumnNotes->addWidget(checkBoxNotesVisibleNote);
    vBoxLayoutGroupBoxVisibleColumnNotes->addWidget(checkBoxNotesDateCreateNote);
    vBoxLayoutGroupBoxVisibleColumnNotes->addWidget(checkBoxNotesCountTextSymbols);
    vBoxLayoutGroupBoxVisibleColumnNotes->addWidget(checkBoxNotesCountTextLine);
    vBoxLayoutGroupBoxVisibleColumnNotes->addWidget(checkBoxNotesDateLastChange);
    vBoxLayoutGroupBoxVisibleColumnNotes->addWidget(checkBoxNotesLock);
    vBoxLayoutGroupBoxVisibleColumnNotes->addWidget(checkBoxNotesDateOfLastRemoval);
    vBoxLayoutGroupBoxVisibleColumnNotes->addWidget(checkBoxNotesDateOfLastRestore);
    vBoxLayoutGroupBoxVisibleColumnNotes->addStretch();

    groupBoxVisibleColumnNotes = new QGroupBox();
    groupBoxVisibleColumnNotes->setCheckable(true);
    groupBoxVisibleColumnNotes->setChecked(true);

    groupBoxVisibleColumnNotes->setLayout(vBoxLayoutGroupBoxVisibleColumnNotes);

    connect(groupBoxVisibleColumnNotes, SIGNAL(clicked(bool)),
            this, SLOT(checkGroupBoxVisibleColumnNotes(bool)));
    // VISIBLE COLUMN TABLE NOTES

    // VISIBLE COLUMN TABLE RECYCLE BIN
    checkBoxRecycleBinVisibleNote = new QCheckBox();
    checkBoxRecycleBinVisibleNote->setChecked(true);
    connect(checkBoxRecycleBinVisibleNote, SIGNAL(toggled(bool)),
            this, SLOT(clickCheckRecycleBinVisibleNote(bool)));

    checkBoxRecycleBinDateCreateNote = new QCheckBox();
    checkBoxRecycleBinDateCreateNote->setChecked(true);
    connect(checkBoxRecycleBinDateCreateNote, SIGNAL(toggled(bool)),
            this, SLOT(clickCheckRecycleBinDateCreateNote(bool)));

    checkBoxRecycleBinCountTextSymbols = new QCheckBox();
    checkBoxRecycleBinCountTextSymbols->setChecked(true);
    connect(checkBoxRecycleBinCountTextSymbols, SIGNAL(toggled(bool)),
            this, SLOT(clickCheckRecycleBinCountTextSymbols(bool)));

    checkBoxRecycleBinCountTextLine = new QCheckBox();
    checkBoxRecycleBinCountTextLine->setChecked(true);
    connect(checkBoxRecycleBinCountTextLine, SIGNAL(toggled(bool)),
            this, SLOT(clickCheckRecycleBinCountTextLine(bool)));

    checkBoxRecycleBinDateLastChange = new QCheckBox();
    checkBoxRecycleBinDateLastChange->setChecked(true);
    connect(checkBoxRecycleBinDateLastChange, SIGNAL(toggled(bool)),
            this, SLOT(clickCheckRecycleBinDateLastChange(bool)));

    checkBoxRecycleBinLock = new QCheckBox();
    checkBoxRecycleBinLock->setChecked(true);
    connect(checkBoxRecycleBinLock, SIGNAL(toggled(bool)),
            this, SLOT(clickCheckRecycleBinLock(bool)));

    checkBoxRecycleBinDateOfLastRemoval = new QCheckBox();
    checkBoxRecycleBinDateOfLastRemoval->setChecked(true);
    connect(checkBoxRecycleBinDateOfLastRemoval, SIGNAL(toggled(bool)),
            this, SLOT(clickCheckRecycleBinDateOfLastRemoval(bool)));

    checkBoxRecycleBinDateOfLastRestore = new QCheckBox();
    checkBoxRecycleBinDateOfLastRestore->setChecked(true);
    connect(checkBoxRecycleBinDateOfLastRestore, SIGNAL(toggled(bool)),
            this, SLOT(clickCheckRecycleBinDateOfLastRestore(bool)));

    QVBoxLayout *vBoxLayoutGroupBoxVisibleColumnRecycleBin = new QVBoxLayout();
    vBoxLayoutGroupBoxVisibleColumnRecycleBin->addWidget(checkBoxRecycleBinVisibleNote);
    vBoxLayoutGroupBoxVisibleColumnRecycleBin->addWidget(checkBoxRecycleBinDateCreateNote);
    vBoxLayoutGroupBoxVisibleColumnRecycleBin->addWidget(checkBoxRecycleBinCountTextSymbols);
    vBoxLayoutGroupBoxVisibleColumnRecycleBin->addWidget(checkBoxRecycleBinCountTextLine);
    vBoxLayoutGroupBoxVisibleColumnRecycleBin->addWidget(checkBoxRecycleBinDateLastChange);
    vBoxLayoutGroupBoxVisibleColumnRecycleBin->addWidget(checkBoxRecycleBinLock);
    vBoxLayoutGroupBoxVisibleColumnRecycleBin->addWidget(checkBoxRecycleBinDateOfLastRemoval);
    vBoxLayoutGroupBoxVisibleColumnRecycleBin->addWidget(checkBoxRecycleBinDateOfLastRestore);
    vBoxLayoutGroupBoxVisibleColumnRecycleBin->addStretch();

    groupBoxVisibleColumnRecycleBin = new QGroupBox();
    groupBoxVisibleColumnRecycleBin->setCheckable(true);
    groupBoxVisibleColumnRecycleBin->setChecked(true);

    groupBoxVisibleColumnRecycleBin->setLayout(vBoxLayoutGroupBoxVisibleColumnRecycleBin);

    connect(groupBoxVisibleColumnRecycleBin, SIGNAL(clicked(bool)),
            this, SLOT(checkGroupBoxVisibleColumnRecycleBin(bool)));
    // VISIBLE COLUMN TABLE RECYCLE BIN

    QHBoxLayout *hBoxLayoutGroupBoxVisibleColumnToTable = new QHBoxLayout();

    hBoxLayoutGroupBoxVisibleColumnToTable->addWidget(groupBoxVisibleColumnNotes, 0, Qt::AlignTop);
    hBoxLayoutGroupBoxVisibleColumnToTable->addWidget(groupBoxVisibleColumnRecycleBin, 0, Qt::AlignTop);
    hBoxLayoutGroupBoxVisibleColumnToTable->addStretch();
    //

    // Visible Text Buttons Panel
    checkBoxVisibleTextButtonsPanel = new QCheckBox();
    connect(checkBoxVisibleTextButtonsPanel, SIGNAL(clicked(bool)),
            this, SIGNAL(visibleTextButtonsPanel(bool)));

    QVBoxLayout *vBoxLayoutTabPageIconsPanel = new QVBoxLayout();
    vBoxLayoutTabPageIconsPanel->addWidget(checkBoxVisibleTextButtonsPanel);
    // Visible Text Buttons Panel


    // COMBOBOX SELECT STYLES
    // берем памку со стилями
    QDir dir = QDir(absolutePathToTheFolderStyles());

    dir.setFilter(QDir::Dirs
                  | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Name);

    // получаем список папок, удовлетворяющих фильтру
    QStringList listFolderStyles = dir.entryList();

    for(int i = 0; i < listFolderStyles.count(); i++)
    {
        QDir dirStyle = QDir(absolutePathToTheFolderStyles()
                             + QDir::separator()
                             + listFolderStyles.at(i));
        QStringList filters;
        dirStyle.setFilter(QDir::Files);
        dirStyle.setSorting(QDir::Name);

        filters << QString("*." + suffixStyleFiles);
        dirStyle.setNameFilters(filters);

        if(dirStyle.entryList().isEmpty())
            listFolderStyles.takeAt(i);
    }

    comboBoxStyle = new QComboBox();
    comboBoxStyle->setEditable(false);

    connect(comboBoxStyle, SIGNAL(activated(int)),
            this, SLOT(changeActivatedItemComboBoxStyle(int)));

    comboBoxStyle->addItem("Default", pathToDefaultStyle);

    for(int i = 0; i < listFolderStyles.count(); i++)
    {
        QString nameStyle = listFolderStyles.at(i);
        QString pathStyleFile = absolutePathToTheFolderStyles()
                + QDir::separator()
                + nameStyle
                + QDir::separator()
                + nameStyle + "." + suffixStyleFiles;
        comboBoxStyle->addItem(nameStyle,
                               pathStyleFile);
    }

    QSettings ini(absolutePathToTheConfigurationProgram(),
                      QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    QString currentNameStyle = ini.value("Style", QFileInfo(pathToDefaultStyle).baseName()).toString();

    QString pathToStyle = absolutePathToTheFolderStyles()
            + QDir::separator()
            + currentNameStyle
            + QDir::separator()
            + currentNameStyle + "." + suffixStyleFiles;

    // если нет стиля сохраненного в последний раз как текущий,
    // выбираем стандартный
    if(!QFileInfo(pathToStyle).isFile())
        pathToStyle = pathToDefaultStyle;

    QString nameStyle = QFileInfo(pathToStyle).baseName();
    int indexItemStyle = comboBoxStyle->findText(nameStyle);

    comboBoxStyle->setCurrentIndex(indexItemStyle);

    QVBoxLayout *vBoxLayoutGroupBoxSelectStyle = new QVBoxLayout();
    vBoxLayoutGroupBoxSelectStyle->addWidget(comboBoxStyle);

    groupBoxSelectStyle = new QGroupBox();
    groupBoxSelectStyle->setLayout(vBoxLayoutGroupBoxSelectStyle);
    // COMBOBOX SELECT STYLES

    QVBoxLayout *vBoxLayoutMain = new QVBoxLayout();
    vBoxLayoutMain->addWidget(groupBoxSelectStyle, 0, Qt::AlignLeft);
    vBoxLayoutMain->addLayout(vBoxLayoutTabPageIconsPanel);
    vBoxLayoutMain->addSpacing(10);
    vBoxLayoutMain->addLayout(hBoxLayoutGroupBoxVisibleColumnToTable);
    vBoxLayoutMain->addStretch();

    tabWidgetManager = new QWidget();
    tabWidgetManager->setLayout(vBoxLayoutMain);
}

void USettingsManager::createTabNewNote()
{
    checkBoxRandomColorNewNote = new QCheckBox();
    checkBoxRandomPositionOnScreenNewNote = new QCheckBox();

    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("NewNote");

    bool randomColor = ini.value("RandomColor", false).toBool();
    bool randomPos   = ini.value("RandomPositionOnScreen", false).toBool();

    checkBoxRandomColorNewNote->setChecked(randomColor);
    checkBoxRandomPositionOnScreenNewNote->setChecked(randomPos);

    connect(checkBoxRandomColorNewNote, SIGNAL(clicked(bool)),
            this, SLOT(setRandomColor(bool)));
    connect(checkBoxRandomPositionOnScreenNewNote, SIGNAL(clicked(bool)),
            this, SLOT(setRandomPositionOnScreen(bool)));


    // SIZE
    int valueWidth  = ini.value("Width", minimalWidthNote).toInt();
    int valueHeight = ini.value("Height", minimalHeightNote).toInt();

    spinBoxWidthNewNote = new QSpinBox();
    spinBoxWidthNewNote->setMinimumWidth(70);
    spinBoxWidthNewNote->setRange(minimalWidthNote, maximalWidthNote());
    spinBoxWidthNewNote->setValue(valueWidth);
    connect(spinBoxWidthNewNote, SIGNAL(valueChanged(int)),
            this, SLOT(setWidthNewNote(int)));

    spinBoxHeightNewNote = new QSpinBox();
    spinBoxHeightNewNote->setMinimumWidth(70);
    spinBoxHeightNewNote->setRange(minimalHeightNote, maximalHeightNote());
    spinBoxHeightNewNote->setValue(valueHeight);
    connect(spinBoxHeightNewNote, SIGNAL(valueChanged(int)),
            this, SLOT(setHeightNewNote(int)));

    labelWidthNewNote = new QLabel();
    labelWidthNewNote->setMinimumWidth(60);

    labelHeightNewNote = new QLabel();
    labelHeightNewNote->setMinimumWidth(60);

    labelWidthPixelNewNote = new QLabel();
    labelHeightPixelNewNote = new QLabel();

    QGridLayout *gLayoutSizeNewNote = new QGridLayout();
    gLayoutSizeNewNote->addWidget(labelWidthNewNote,       0, 0);
    gLayoutSizeNewNote->addWidget(spinBoxWidthNewNote,     0, 1);
    gLayoutSizeNewNote->addWidget(labelWidthPixelNewNote,  0, 2);

    gLayoutSizeNewNote->addWidget(labelHeightNewNote,      1, 0);
    gLayoutSizeNewNote->addWidget(spinBoxHeightNewNote,    1, 1);
    gLayoutSizeNewNote->addWidget(labelHeightPixelNewNote, 1, 2);

    groupBoxSizeNewNote = new QGroupBox();
    groupBoxSizeNewNote->setLayout(gLayoutSizeNewNote);
    // SIZE

    // POSITION
    int valuePosX = ini.value("PositionX", widthDesktop() / 2).toInt();
    int valuePosY = ini.value("PositionY", heightDesktop() / 2).toInt();

    spinBoxPositionXNewNote = new QSpinBox();
    spinBoxPositionXNewNote->setMinimumWidth(70);
    spinBoxPositionXNewNote->setRange(0, maximalWidthNote());
    spinBoxPositionXNewNote->setValue(valuePosX);
    connect(spinBoxPositionXNewNote, SIGNAL(valueChanged(int)),
            this, SLOT(setPositionXNewNote(int)));

    spinBoxPositionYNewNote = new QSpinBox();
    spinBoxPositionYNewNote->setMinimumWidth(70);
    spinBoxPositionYNewNote->setRange(0, maximalHeightNote());
    spinBoxPositionYNewNote->setValue(valuePosY);
    connect(spinBoxPositionYNewNote, SIGNAL(valueChanged(int)),
            this, SLOT(setPositionYNewNote(int)));

    labelPositionXNewNote = new QLabel();
    labelPositionXNewNote->setMinimumWidth(60);

    labelPositionYNewNote = new QLabel();
    labelPositionYNewNote->setMinimumWidth(60);

    labelPositionXPixelNewNote = new QLabel();
    labelPositionYPixelNewNote = new QLabel();

    QGridLayout *gLayoutPositionNewNote = new QGridLayout();
    gLayoutPositionNewNote->addWidget(labelPositionXNewNote,       0, 0);
    gLayoutPositionNewNote->addWidget(spinBoxPositionXNewNote,     0, 1);
    gLayoutPositionNewNote->addWidget(labelPositionXPixelNewNote,  0, 2);

    gLayoutPositionNewNote->addWidget(labelPositionYNewNote,       1, 0);
    gLayoutPositionNewNote->addWidget(spinBoxPositionYNewNote,     1, 1);
    gLayoutPositionNewNote->addWidget(labelPositionYPixelNewNote,  1, 2);

    groupBoxPositionNewNote = new QGroupBox();
    groupBoxPositionNewNote->setLayout(gLayoutPositionNewNote);
    // POSITION

    // TITLE
    QString title = ini.value("Title", "").toString();

    lineEditTitleNewNote = new ULineEditWithClearButton();
    lineEditTitleNewNote->setText(title);

    QPixmap pixmap_enter(IconClearTextEnter);
    QPixmap pixmap_leave(IconClearTextLeave);

    int widthIcon = 20;
    int heightIcon = 20;
    pixmap_enter = pixmap_enter.scaled(widthIcon, heightIcon, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pixmap_leave = pixmap_leave.scaled(widthIcon, heightIcon, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    lineEditTitleNewNote->setIconButton(pixmap_leave);
    lineEditTitleNewNote->setIconButton(ULineEditWithClearButton::EnterEvent,
                                        pixmap_enter);
    lineEditTitleNewNote->setIconButton(ULineEditWithClearButton::LeaveEvent,
                                        pixmap_leave);

    connect(lineEditTitleNewNote, SIGNAL(textEdited(QString)),
            this, SLOT(setTitleNewNote(QString)));

    QStringList stringList = ini.value("ListFormatDateTime").toStringList();
    QString string = ini.value("TitlePatternDateTime").toString();

    stringList.insert(0, "");

    editedExpandableComboBoxDate = new UEditedExpandableComboBoxDate();
    connect(this, SIGNAL(languageIsChange(QString)),
            editedExpandableComboBoxDate, SLOT(setLanguage(QString)));

    editedExpandableComboBoxDate->setListItemsDate(&stringList);

    editedExpandableComboBoxDate->setCurrentItemComboBoxDate(string);

    connect(editedExpandableComboBoxDate, SIGNAL(activatedItemComboBoxDate(QString)),
            this, SLOT(comboBoxDateTimeItemActivate(QString)));

    connect(editedExpandableComboBoxDate, SIGNAL(saveListDateTimeFormat()),
            this, SLOT(saveToIniListDateTimeFormat()));

    QVBoxLayout *vBox1 = new QVBoxLayout();
    vBox1->addSpacing(10);
    vBox1->addWidget(lineEditTitleNewNote);
    vBox1->addStretch();

    QVBoxLayout *vBox2 = new QVBoxLayout();
    vBox2->addWidget(editedExpandableComboBoxDate);
    vBox2->addStretch();

    QHBoxLayout *hBoxGroupBoxTitleNewNote = new QHBoxLayout();
    hBoxGroupBoxTitleNewNote->addLayout(vBox1);
    hBoxGroupBoxTitleNewNote->addLayout(vBox2);

    groupBoxTitleNewNote = new QGroupBox();
    groupBoxTitleNewNote->setLayout(hBoxGroupBoxTitleNewNote);
    // TITLE

    // TEXT
    QString text = ini.value("Text", "").toString();

    textEditTextNewNote = new QTextEdit(text);
    connect(textEditTextNewNote, SIGNAL(textChanged()),
            this, SLOT(setTextNewNote()));

    QVBoxLayout *vBoxGroupBoxTextNewNote = new QVBoxLayout();
    vBoxGroupBoxTextNewNote->addWidget(textEditTextNewNote);

    groupBoxTextNewNote = new QGroupBox();
    groupBoxTextNewNote->setLayout(vBoxGroupBoxTextNewNote);
    // TEXT

    // BACKGROUND COLOR
    QString strColor = ini.value("BackgroundColor", "FFFF00").toString();
    QColor bgColor = getColorFromHexColorForStyleSheet(strColor);

    pButtonSelectBackgroundColorNewNote = new UPushButtonSelectColor();
    pButtonSelectBackgroundColorNewNote->setFixedSize(100,
                                                      pButtonSelectBackgroundColorNewNote->sizeHint().height());
    setColorToButtonSelectColor(bgColor);
    connect(pButtonSelectBackgroundColorNewNote, SIGNAL(selectedColor(QColor)),
            this, SLOT(setBackgroundColorNewNote(QColor)));
    connect(this, SIGNAL(languageIsChange(QString)),
            pButtonSelectBackgroundColorNewNote, SLOT(setLanguage(QString)));

    QVBoxLayout *vBoxGroupBoxBackgroundColorNewNote = new QVBoxLayout();
    vBoxGroupBoxBackgroundColorNewNote->addWidget(pButtonSelectBackgroundColorNewNote);

    groupBoxSelectBackgroundColorNewNote = new QGroupBox();
    groupBoxSelectBackgroundColorNewNote->setLayout(vBoxGroupBoxBackgroundColorNewNote);
    // BACKGROUND COLOR

    // VISIBLE
    bool visibleNewNote = ini.value("Visible", true).toBool();
    checkBoxVisibleNewNote = new QCheckBox();
    checkBoxVisibleNewNote->setChecked(visibleNewNote);
    connect(checkBoxVisibleNewNote, SIGNAL(clicked(bool)),
            this, SLOT(setVisibleNewNote(bool)));
    // VISIBLE

    // MINIMIZE
    bool minimizeNewNote = ini.value("Minimize", false).toBool();
    checkBoxMinimizeNewNote = new QCheckBox();
    checkBoxMinimizeNewNote->setChecked(minimizeNewNote);
    connect(checkBoxMinimizeNewNote, SIGNAL(clicked(bool)),
            this, SLOT(setMinimizeNewNote(bool)));
    // MINIMIZE

    // OPACITY
    int opacityNewNote = ini.value("Opacity", maximalOpacity).toInt();
    sliderOpacityNewNote = new QSlider(Qt::Horizontal);
    sliderOpacityNewNote->setRange(minimalOpacity, maximalOpacity);
    sliderOpacityNewNote->setSliderPosition(opacityNewNote);
    connect(sliderOpacityNewNote, SIGNAL(valueChanged(int)),
            this, SLOT(setOpacityNewNote(int)));

    QHBoxLayout *hBoxLayoutGroupBoxOpacityNewNote = new QHBoxLayout();
    hBoxLayoutGroupBoxOpacityNewNote->addWidget(new QLabel(QString::number(minimalOpacity) + "%"));
    hBoxLayoutGroupBoxOpacityNewNote->addWidget(sliderOpacityNewNote);
    hBoxLayoutGroupBoxOpacityNewNote->addWidget(new QLabel(QString::number(maximalOpacity) + "%"));

    groupBoxOpacityNewNote = new QGroupBox();
    groupBoxOpacityNewNote->setLayout(hBoxLayoutGroupBoxOpacityNewNote);
    // OPACITY

    // LOCK
    bool lockNewNote = ini.value("Lock", false).toBool();
    checkBoxLockNewNote = new QCheckBox();
    checkBoxLockNewNote->setChecked(lockNewNote);
    connect(checkBoxLockNewNote, SIGNAL(clicked(bool)),
            this, SLOT(setLockNewNote(bool)));
    // LOCK

    // Z-POSITION
    QString zPositionNewNote = ini.value("zPosition",
                                         defaultZPositionNote).toString();

    if(zPositionNewNote != UzPosition::AlwaysOnBottom
            || zPositionNewNote != UzPosition::AlwaysOnTop)
    {
        ini.setValue("zPosition", defaultZPositionNote);
        zPositionNewNote = defaultZPositionNote;
    }    

    radioButtonWindowStaysOnBottomHint = new QRadioButton();
    radioButtonWindowStaysOnTopHint = new QRadioButton();

    if(zPositionNewNote == UzPosition::AlwaysOnBottom)
        radioButtonWindowStaysOnBottomHint->setChecked(true);
    else
        radioButtonWindowStaysOnTopHint->setChecked(true);

    connect(radioButtonWindowStaysOnBottomHint, SIGNAL(clicked()),
            this, SLOT(clickRadioButtonChangeZPosition()));
    connect(radioButtonWindowStaysOnTopHint, SIGNAL(clicked()),
            this, SLOT(clickRadioButtonChangeZPosition()));

    QHBoxLayout *hBoxLayoutGroupBoxSelectZPosition = new QHBoxLayout();
    hBoxLayoutGroupBoxSelectZPosition->addWidget(radioButtonWindowStaysOnBottomHint);
    hBoxLayoutGroupBoxSelectZPosition->addWidget(radioButtonWindowStaysOnTopHint);

    groupBoxSelectZPosition = new QGroupBox();
    groupBoxSelectZPosition->setLayout(hBoxLayoutGroupBoxSelectZPosition);
    // Z-POSITION

    QHBoxLayout *hBoxLayoutRandomParametersNewNote = new QHBoxLayout();
    hBoxLayoutRandomParametersNewNote->addWidget(checkBoxRandomColorNewNote);
    hBoxLayoutRandomParametersNewNote->addSpacing(30);
    hBoxLayoutRandomParametersNewNote->addWidget(checkBoxRandomPositionOnScreenNewNote);
    hBoxLayoutRandomParametersNewNote->addStretch();

    QHBoxLayout *hBoxLayoutSizePosNewNote = new QHBoxLayout();
    hBoxLayoutSizePosNewNote->addWidget(groupBoxSizeNewNote, 0, Qt::AlignLeft);
    hBoxLayoutSizePosNewNote->addWidget(groupBoxPositionNewNote,  0, Qt::AlignLeft);
    hBoxLayoutSizePosNewNote->addStretch();

    QVBoxLayout *vBoxLayoutGroupBoxAdditionally = new QVBoxLayout();
    vBoxLayoutGroupBoxAdditionally->addWidget(groupBoxTitleNewNote,  0, Qt::AlignLeft);
    vBoxLayoutGroupBoxAdditionally->addWidget(groupBoxTextNewNote,  0, Qt::AlignLeft);
    vBoxLayoutGroupBoxAdditionally->addWidget(groupBoxSelectBackgroundColorNewNote,  0, Qt::AlignLeft);
    vBoxLayoutGroupBoxAdditionally->addWidget(checkBoxVisibleNewNote,  0, Qt::AlignLeft);
    vBoxLayoutGroupBoxAdditionally->addWidget(checkBoxMinimizeNewNote,  0, Qt::AlignLeft);
    vBoxLayoutGroupBoxAdditionally->addWidget(groupBoxOpacityNewNote,  0, Qt::AlignLeft);
    vBoxLayoutGroupBoxAdditionally->addWidget(checkBoxLockNewNote,  0, Qt::AlignLeft);
    vBoxLayoutGroupBoxAdditionally->addWidget(groupBoxSelectZPosition, 0, Qt::AlignLeft);
    vBoxLayoutGroupBoxAdditionally->addStretch();

    bool visibleAdditionally = false;
    setVisibleAdditionally(visibleAdditionally);

    groupBoxAdditionally = new QGroupBox();
    groupBoxAdditionally->setFlat(true);
    groupBoxAdditionally->setCheckable(true);
    groupBoxAdditionally->setChecked(visibleAdditionally);
    groupBoxAdditionally->setLayout(vBoxLayoutGroupBoxAdditionally);
    connect(groupBoxAdditionally, SIGNAL(toggled(bool)),
            this, SLOT(setVisibleAdditionally(bool)));

    vBoxLayoutWidgetNewNote = new QVBoxLayout();
    vBoxLayoutWidgetNewNote->addLayout(hBoxLayoutRandomParametersNewNote);
    vBoxLayoutWidgetNewNote->addLayout(hBoxLayoutSizePosNewNote);
    vBoxLayoutWidgetNewNote->addWidget(groupBoxAdditionally);
    vBoxLayoutWidgetNewNote->addStretch();

    tabWidgetNewNote = new QWidget();
    tabWidgetNewNote->setLayout(vBoxLayoutWidgetNewNote);

    areaNewNote = new QScrollArea();
    areaNewNote->setWidgetResizable(true);
    areaNewNote->setWidget(tabWidgetNewNote);

    ini.endGroup();
}

void USettingsManager::createTabHotkey()
{
    hotkeyNewNote = new UWidgetSelectHotkey();
    hotkeyNewNoteFromClipboard = new UWidgetSelectHotkey();
    hotkeyHideAllNote = new UWidgetSelectHotkey();
    hotkeyShowAllNote = new UWidgetSelectHotkey();
    hotkeyShowHideManager = new UWidgetSelectHotkey();

    pButtonOkHotkey = new QPushButton();
    connect(pButtonOkHotkey, SIGNAL(clicked()),
            this, SIGNAL(changeHotkeys()));

    QVBoxLayout *vBoxLayoutGroupBoxUseHotkeys = new QVBoxLayout();
    vBoxLayoutGroupBoxUseHotkeys->addWidget(hotkeyNewNote);
    vBoxLayoutGroupBoxUseHotkeys->addWidget(hotkeyNewNoteFromClipboard);
    vBoxLayoutGroupBoxUseHotkeys->addWidget(hotkeyHideAllNote);
    vBoxLayoutGroupBoxUseHotkeys->addWidget(hotkeyShowAllNote);
    vBoxLayoutGroupBoxUseHotkeys->addWidget(hotkeyShowHideManager);
    vBoxLayoutGroupBoxUseHotkeys->addWidget(pButtonOkHotkey,
                                            0,
                                            Qt::AlignRight);

    groupBoxUseHotkeys = new QGroupBox();
    connect(groupBoxUseHotkeys, SIGNAL(clicked(bool)),
            this, SLOT(setActivateHotkeys(bool)));

    groupBoxUseHotkeys->setCheckable(true);
    groupBoxUseHotkeys->setFlat(true);
    groupBoxUseHotkeys->setLayout(vBoxLayoutGroupBoxUseHotkeys);

    QVBoxLayout *vBoxLayout = new QVBoxLayout();
    vBoxLayout->addWidget(groupBoxUseHotkeys);

    tabWidgetHotkey = new QWidget();
    tabWidgetHotkey->setLayout(vBoxLayout);

    areaHotkey = new QScrollArea();
    areaHotkey->setWidgetResizable(true);
    areaHotkey->setWidget(tabWidgetHotkey);


    QSettings ini(absolutePathToTheConfigurationProgram(),
                      QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("Hotkeys");

    bool activateHotkey = ini.value("Activate",
                                        false).toBool();
    groupBoxUseHotkeys->setChecked(activateHotkey);

    hotkeyNewNote->setHotkey(ini.value("NewNote", "").toString());
    hotkeyNewNoteFromClipboard->setHotkey(ini.value("NewNoteFromClipboard", "").toString());
    hotkeyShowAllNote->setHotkey(ini.value("ShowAllNotes", "").toString());
    hotkeyHideAllNote->setHotkey(ini.value("HideAllNotes", "").toString());
    hotkeyShowHideManager->setHotkey(ini.value("ShowHideManager", "").toString());

    ini.endGroup();
}

/// PUBLIC SLOTS
void USettingsManager::setLanguage(QString pathToTheFileLanguage)
{
    emit languageIsChange(pathToTheFileLanguage);

    QSettings ini(pathToTheFileLanguage,
                       QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("SettingsManager");

    checkBoxNotesVisibleNote->setText(ini.value("NotesVisible").toString());
    checkBoxNotesDateCreateNote->setText(ini.value("NotesDateCreate").toString());
    checkBoxNotesCountTextSymbols->setText(ini.value("NotesCountTextSymbols").toString());
    checkBoxNotesCountTextLine->setText(ini.value("NotesCountTextLine").toString());
    checkBoxNotesDateLastChange->setText(ini.value("NotesDateLastChange").toString());
    checkBoxNotesLock->setText(ini.value("NotesLock").toString());
    checkBoxNotesDateOfLastRemoval->setText(ini.value("NotesDateOfLastRemoval").toString());
    checkBoxNotesDateOfLastRestore->setText(ini.value("NotesDateOfLastRestore").toString());

    checkBoxRecycleBinVisibleNote->setText(ini.value("RecycleBinVisible").toString());
    checkBoxRecycleBinDateCreateNote->setText(ini.value("RecycleBinDateCreate").toString());
    checkBoxRecycleBinCountTextSymbols->setText(ini.value("RecycleBinCountTextSymbols").toString());
    checkBoxRecycleBinCountTextLine->setText(ini.value("RecycleBinCountTextLine").toString());
    checkBoxRecycleBinDateLastChange->setText(ini.value("RecycleBinDateLastChange").toString());
    checkBoxRecycleBinLock->setText(ini.value("RecycleBinLock").toString());
    checkBoxRecycleBinDateOfLastRemoval->setText(ini.value("RecycleBinDateOfLastRemoval").toString());
    checkBoxRecycleBinDateOfLastRestore->setText(ini.value("RecycleBinDateOfLastRestore").toString());

    tabWidgetSettings->setTabText(1, ini.value("TitleManager").toString());
    tabWidgetSettings->setTabText(2, ini.value("TitleNewNote").toString());
    tabWidgetSettings->setTabText(3, ini.value("TitleHotkey").toString());

    checkBoxRandomColorNewNote->setText(ini.value("RandomColor").toString());
    checkBoxRandomPositionOnScreenNewNote->setText(ini.value("RandomPositionOnScreen").toString());

    groupBoxVisibleColumnNotes->setTitle(ini.value("SettingsNotes").toString());
    groupBoxVisibleColumnRecycleBin->setTitle(ini.value("SettingsRecycleBin").toString());
    checkBoxVisibleTextButtonsPanel->setText(ini.value("VisibleTextIconsPanel").toString());
    groupBoxSelectStyle->setTitle(ini.value("TitleSelectStyle").toString());

    groupBoxSizeNewNote->setTitle(ini.value("TitleSize").toString());
    labelWidthNewNote->setText(ini.value("Width").toString());
    labelHeightNewNote->setText(ini.value("Height").toString());
    labelWidthPixelNewNote->setText(ini.value("Pixel").toString());
    labelHeightPixelNewNote->setText(ini.value("Pixel").toString());

    groupBoxPositionNewNote->setTitle(ini.value("TitlePosition").toString());
    labelPositionXNewNote->setText(ini.value("PositionX").toString());
    labelPositionYNewNote->setText(ini.value("PositionY").toString());
    labelPositionXPixelNewNote->setText(ini.value("Pixel").toString());
    labelPositionYPixelNewNote->setText(ini.value("Pixel").toString());

    groupBoxAdditionally->setTitle(ini.value("TitleAdditionally").toString());

    groupBoxTitleNewNote->setTitle(ini.value("TitleTitle").toString());
    groupBoxTextNewNote->setTitle(ini.value("TitleText").toString());
    groupBoxSelectBackgroundColorNewNote->setTitle(ini.value("TitleBackgroundColor").toString());

    checkBoxVisibleNewNote->setText(ini.value("Visible").toString());
    checkBoxMinimizeNewNote->setText(ini.value("Minimize").toString());

    groupBoxOpacityNewNote->setTitle(ini.value("TitleOpacity").toString());
    checkBoxLockNewNote->setText(ini.value("Lock").toString());

    // HOTKEY
    groupBoxUseHotkeys->setTitle(ini.value("UseHotkeys").toString());
    hotkeyNewNote->setTitle(ini.value("HotkeyNewNote").toString());
    hotkeyNewNoteFromClipboard->setTitle(ini.value("HotkeyNewNoteFromClipboard").toString());
    hotkeyHideAllNote->setTitle(ini.value("HotkeyHideAllNote").toString());
    hotkeyShowAllNote->setTitle(ini.value("HotkeyShowAllNote").toString());
    hotkeyShowHideManager->setTitle(ini.value("HotkeyShowHideManager").toString());
    pButtonOkHotkey->setText(ini.value("Ok").toString());
    // HOTKEY

    // TOOL TIP
    checkBoxNotesVisibleNote->setToolTip(ini.value("ToolTipVisible").toString());
    checkBoxNotesDateCreateNote->setToolTip(ini.value("ToolTipDateCreate").toString());
    checkBoxNotesCountTextSymbols->setToolTip(ini.value("ToolTipCountTextSymbols").toString());
    checkBoxNotesCountTextLine->setToolTip(ini.value("ToolTipCountTextLine").toString());
    checkBoxNotesDateLastChange->setToolTip(ini.value("ToolTipDateLastChange").toString());
    checkBoxNotesLock->setToolTip(ini.value("ToolTipLock").toString());
    checkBoxNotesDateOfLastRemoval->setToolTip(ini.value("ToolTipDateOfLastRemoval").toString());
    checkBoxNotesDateOfLastRestore->setToolTip(ini.value("ToolTipDateOfLastRestore").toString());

    checkBoxRecycleBinVisibleNote->setToolTip(checkBoxNotesVisibleNote->toolTip());
    checkBoxRecycleBinDateCreateNote->setToolTip(checkBoxNotesDateCreateNote->toolTip());
    checkBoxRecycleBinCountTextSymbols->setToolTip(checkBoxNotesCountTextSymbols->toolTip());
    checkBoxRecycleBinCountTextLine->setToolTip(checkBoxNotesCountTextLine->toolTip());
    checkBoxRecycleBinDateLastChange->setToolTip(checkBoxNotesDateLastChange->toolTip());
    checkBoxRecycleBinLock->setToolTip(checkBoxNotesLock->toolTip());
    checkBoxRecycleBinDateOfLastRemoval->setToolTip(checkBoxNotesDateOfLastRemoval->toolTip());
    checkBoxRecycleBinDateOfLastRestore->setToolTip(checkBoxNotesDateOfLastRestore->toolTip());

    ini.endGroup();

    ini.beginGroup("SettingsNote");

    radioButtonWindowStaysOnBottomHint->setText(ini.value("WindowStaysOnBottomHint").toString());
    radioButtonWindowStaysOnTopHint->setText(ini.value("WindowStaysOnTopHint").toString());

    groupBoxSelectZPosition->setTitle(ini.value("SelectZPosition").toString());
    groupBoxSelectZPosition->setToolTip(ini.value("ToolTipSelectZPosition").toString());

    radioButtonWindowStaysOnBottomHint->setToolTip(ini.value("ToolTipWindowStaysOnBottomHint").toString());
    radioButtonWindowStaysOnTopHint->setToolTip(ini.value("ToolTipWindowStaysOnTopHint").toString());

    ini.endGroup();

    tabWidgetSettings->setTabText(0, ini.value("SETTINGSPROGRAM/Title").toString());
}

void USettingsManager::setCheckedNotesCheckBox(QList <bool> &list)
{
    checkBoxNotesVisibleNote->setChecked(list.at(indexColumnVisible));
    checkBoxNotesDateCreateNote->setChecked(list.at(indexColumnDateOfCreating));
    checkBoxNotesCountTextSymbols->setChecked(list.at(indexColumnCountTextSymbols));
    checkBoxNotesCountTextLine->setChecked(list.at(indexColumnCountTextLines));
    checkBoxNotesDateLastChange->setChecked(list.at(indexColumnDateLastChange));
    checkBoxNotesLock->setChecked(list.at(indexColumnLock));
    checkBoxNotesDateOfLastRemoval->setChecked(list.at(indexColumnDateOfLastRemoval));
    checkBoxNotesDateOfLastRestore->setChecked(list.at(indexColumnDateOfLastRestore));
}

void USettingsManager::setCheckedRecycleBinCheckBox(QList <bool> &list)
{
    checkBoxRecycleBinVisibleNote->setChecked(list.at(indexColumnVisible));
    checkBoxRecycleBinDateCreateNote->setChecked(list.at(indexColumnDateOfCreating));
    checkBoxRecycleBinCountTextSymbols->setChecked(list.at(indexColumnCountTextSymbols));
    checkBoxRecycleBinCountTextLine->setChecked(list.at(indexColumnCountTextLines));
    checkBoxRecycleBinDateLastChange->setChecked(list.at(indexColumnDateLastChange));
    checkBoxRecycleBinLock->setChecked(list.at(indexColumnLock));
    checkBoxRecycleBinDateOfLastRemoval->setChecked(list.at(indexColumnDateOfLastRemoval));
    checkBoxRecycleBinDateOfLastRestore->setChecked(list.at(indexColumnDateOfLastRestore));
}

/// PRIVATE SLOTS
void USettingsManager::clickCheckNotesVisibleNote(bool checked)
{
    checkBoxNotesVisibleNote->setChecked(checked);
    emit columnTableNotesVisibleChange(indexColumnVisible,
                                       checkBoxNotesVisibleNote->isChecked());
}
void USettingsManager::clickCheckNotesDateCreateNote(bool checked)
{
    checkBoxNotesDateCreateNote->setChecked(checked);
    emit columnTableNotesVisibleChange(indexColumnDateOfCreating,
                                       checkBoxNotesDateCreateNote->isChecked());
}
void USettingsManager::clickCheckNotesCountTextSymbols(bool checked)
{
    checkBoxNotesCountTextSymbols->setChecked(checked);
    emit columnTableNotesVisibleChange(indexColumnCountTextSymbols,
                                       checkBoxNotesCountTextSymbols->isChecked());
}
void USettingsManager::clickCheckNotesCountTextLine(bool checked)
{
    checkBoxNotesCountTextLine->setChecked(checked);
    emit columnTableNotesVisibleChange(indexColumnCountTextLines,
                                       checkBoxNotesCountTextLine->isChecked());
}

void USettingsManager::clickCheckNotesDateLastChange(bool checked)
{
    checkBoxNotesDateLastChange->setChecked(checked);
    emit columnTableNotesVisibleChange(indexColumnDateLastChange,
                                       checkBoxNotesDateLastChange->isChecked());
}

void USettingsManager::clickCheckNotesLock(bool checked)
{
    checkBoxNotesLock->setChecked(checked);
    emit columnTableNotesVisibleChange(indexColumnLock,
                                       checkBoxNotesLock->isChecked());
}

void USettingsManager::clickCheckNotesDateOfLastRemoval(bool checked)
{
    checkBoxNotesDateOfLastRemoval->setChecked(checked);
    emit columnTableNotesVisibleChange(indexColumnDateOfLastRemoval,
                                       checkBoxNotesDateOfLastRemoval->isChecked());
}

void USettingsManager::clickCheckNotesDateOfLastRestore(bool checked)
{
    checkBoxNotesDateOfLastRestore->setChecked(checked);
    emit columnTableNotesVisibleChange(indexColumnDateOfLastRestore,
                                       checkBoxNotesDateOfLastRestore->isChecked());
}

void USettingsManager::clickCheckRecycleBinVisibleNote(bool checked)
{
    checkBoxRecycleBinVisibleNote->setChecked(checked);
    emit columnTableRecycleBinVisibleChange(indexColumnVisible,
                                            checkBoxRecycleBinVisibleNote->isChecked());
}
void USettingsManager::clickCheckRecycleBinDateCreateNote(bool checked)
{
    checkBoxRecycleBinDateCreateNote->setChecked(checked);
    emit columnTableRecycleBinVisibleChange(indexColumnDateOfCreating,
                                            checkBoxRecycleBinDateCreateNote->isChecked());
}
void USettingsManager::clickCheckRecycleBinCountTextSymbols(bool checked)
{
    checkBoxRecycleBinCountTextSymbols->setChecked(checked);
    emit columnTableRecycleBinVisibleChange(indexColumnCountTextSymbols,
                                            checkBoxRecycleBinCountTextSymbols->isChecked());
}
void USettingsManager::clickCheckRecycleBinCountTextLine(bool checked)
{
    checkBoxRecycleBinCountTextLine->setChecked(checked);
    emit columnTableRecycleBinVisibleChange(indexColumnCountTextLines,
                                            checkBoxRecycleBinCountTextLine->isChecked());
}
void USettingsManager::clickCheckRecycleBinDateLastChange(bool checked)
{
    checkBoxRecycleBinDateLastChange->setChecked(checked);
    emit columnTableRecycleBinVisibleChange(indexColumnDateLastChange,
                                            checkBoxRecycleBinDateLastChange->isChecked());
}
void USettingsManager::clickCheckRecycleBinLock(bool checked)
{
    checkBoxRecycleBinLock->setChecked(checked);
    emit columnTableRecycleBinVisibleChange(indexColumnLock,
                                            checkBoxRecycleBinLock->isChecked());
}

void USettingsManager::clickCheckRecycleBinDateOfLastRemoval(bool checked)
{
    checkBoxRecycleBinDateOfLastRemoval->setChecked(checked);
    emit columnTableRecycleBinVisibleChange(indexColumnDateOfLastRemoval,
                                            checkBoxRecycleBinDateOfLastRemoval->isChecked());
}

void USettingsManager::clickCheckRecycleBinDateOfLastRestore(bool checked)
{
    checkBoxRecycleBinDateOfLastRestore->setChecked(checked);
    emit columnTableRecycleBinVisibleChange(indexColumnDateOfLastRestore,
                                            checkBoxRecycleBinDateOfLastRestore->isChecked());
}

void USettingsManager::checkGroupBoxVisibleColumnNotes(bool checked)
{
    // получаем список checkbox'ов
    QList <QCheckBox *> listCheckBox = groupBoxVisibleColumnNotes->findChildren<QCheckBox *>();

    for(int i = 0; i < listCheckBox.count(); i++)
    {
        QCheckBox *checkBox = listCheckBox.at(i);
        checkBox->setEnabled(true);
        checkBox->setChecked(checked);
    }
}
void USettingsManager::checkGroupBoxVisibleColumnRecycleBin(bool checked)
{
    // получаем список checkbox'ов
    QList <QCheckBox *> listCheckBox = groupBoxVisibleColumnRecycleBin->findChildren<QCheckBox *>();

    for(int i = 0; i < listCheckBox.count(); i++)
    {
        QCheckBox *checkBox = listCheckBox.at(i);
        checkBox->setEnabled(true);
        checkBox->setChecked(checked);
    }
}

void USettingsManager::setVisibleAdditionally(bool visible)
{
    groupBoxTitleNewNote->setVisible(visible);
    groupBoxTextNewNote->setVisible(visible);
    groupBoxSelectBackgroundColorNewNote->setVisible(visible);
    checkBoxVisibleNewNote->setVisible(visible);
    checkBoxMinimizeNewNote->setVisible(visible);
    groupBoxOpacityNewNote->setVisible(visible);
    checkBoxLockNewNote->setVisible(visible);
    groupBoxSelectZPosition->setVisible(visible);
}
void USettingsManager::setRandomColor(bool bValue)
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setValue("NewNote/RandomColor", bValue);
    ini.sync();
}
void USettingsManager::setRandomPositionOnScreen(bool bValue)
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setValue("NewNote/RandomPositionOnScreen", bValue);
    ini.sync();
}
void USettingsManager::setWidthNewNote(int width)
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setValue("NewNote/Width", width);
    ini.sync();
}
void USettingsManager::setHeightNewNote(int height)
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setValue("NewNote/Height", height);
    ini.sync();
}
void USettingsManager::setPositionXNewNote(int x)
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setValue("NewNote/PositionX", x);
    ini.sync();
}
void USettingsManager::setPositionYNewNote(int y)
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setValue("NewNote/PositionY", y);
    ini.sync();
}
void USettingsManager::setTitleNewNote(QString title)
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setValue("NewNote/Title", title);
    ini.sync();
}
void USettingsManager::setTextNewNote()
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);

    QString text = textEditTextNewNote->toPlainText();

    ini.setValue("NewNote/Text", text);
    ini.sync();
}
void USettingsManager::setBackgroundColorNewNote(QColor color)
{
    QString hexColor = getStringHexColorForStyleSheet(color);

    QPixmap pixmap = QPixmap(pButtonSelectBackgroundColorNewNote->width() - 10,
                             pButtonSelectBackgroundColorNewNote->height()- 10);
    pixmap.fill(color);
    pButtonSelectBackgroundColorNewNote->setIcon(QIcon(pixmap));
    pButtonSelectBackgroundColorNewNote->setIconSize(pixmap.size());

    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setValue("NewNote/BackgroundColor", hexColor);
    ini.sync();
}
void USettingsManager::setColorToButtonSelectColor(QColor color)
{
    QPixmap pixmap = QPixmap(pButtonSelectBackgroundColorNewNote->width() - 10,
                             pButtonSelectBackgroundColorNewNote->height()- 10);
    pixmap.fill(color);
    pButtonSelectBackgroundColorNewNote->setIcon(QIcon(pixmap));
    pButtonSelectBackgroundColorNewNote->setIconSize(pixmap.size());

}
void USettingsManager::setVisibleNewNote(bool visible)
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setValue("NewNote/Visible", visible);
    ini.sync();
}
void USettingsManager::setMinimizeNewNote(bool minimize)
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setValue("NewNote/Minimize", minimize);
    ini.sync();
}
void USettingsManager::setOpacityNewNote(int opacity)
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setValue("NewNote/Opacity", opacity);
    ini.sync();
}
void USettingsManager::setLockNewNote(bool lock)
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setValue("NewNote/Lock", lock);
    ini.sync();
}
void USettingsManager::setZPositionNewNote(QString zPosition)
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setValue("NewNote/zPosition", zPosition);
    ini.sync();
}
void USettingsManager::clickRadioButtonChangeZPosition()
{
    QString zPosition;

    if(radioButtonWindowStaysOnTopHint->isChecked())
        zPosition = UzPosition::AlwaysOnTop;
    else
        zPosition = UzPosition::AlwaysOnBottom;

    setZPositionNewNote(zPosition);
}
void USettingsManager::changeActivatedItemComboBoxStyle(int index)
{
    QString pathToFileStyle = comboBoxStyle->itemData(index).toString();
    emit selectedAnotherStyle(pathToFileStyle);
}

void USettingsManager::comboBoxDateTimeItemActivate(QString text)
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.setValue("NewNote/TitlePatternDateTime", text);
    ini.sync();
}
void USettingsManager::saveToIniListDateTimeFormat()
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    QStringList *listComboBox;
    listComboBox = editedExpandableComboBoxDate->getListDateTimeFormat();

    QStringList list;

    for(int i = 0; i < listComboBox->count(); i++)
        list.append(listComboBox->at(i));

    // первый индекс - всегда пустой
    list.removeAt(0);

    ini.setValue("NewNote/ListFormatDateTime", list);
    ini.sync();
}

void USettingsManager::setActivateHotkeys(bool activate)
{
    groupBoxUseHotkeys->setChecked(activate);

    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.setValue("Hotkeys/Activate", activate);
    ini.sync();

    emit activateHotkeys(activate);
}
