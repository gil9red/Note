#include "NoteFile/Note.h"

/// PUBLIC
Note::Note(QString path)
{
    initNodeIsEnd = false;
    onetimeCounter = false;

    initNote();

    pathIni = path;

    QString name = getTheCurrentLanguageKeyboardLayouts();

    QString fileName = absolutePathToTheFolderLocationLanguageToAutocomplete()
            + QDir::separator()
            + name
            + "."
            + suffixAutocompleteLanguageFiles;

    // если файл существует
    QFileInfo fileAutocompletion(fileName);

    if(fileAutocompletion.isFile())
        autocompletionFileNotFound = false;
    else
        autocompletionFileNotFound = true;

    // если ini существует
    QFileInfo file(pathIni);

    if(file.isFile())
        loadDataNote();
    else
        saveDataNote();

    createWidgetsToNote();

    setDataNote(dataNote);

    setVisibleCommandButtons(false);

    initNodeIsEnd = true;
}

Note::Note(QString text, QString path = "")
{
    initNodeIsEnd = false;
    onetimeCounter = false;

    initNote(text);

    pathIni = path;

    QString name = getTheCurrentLanguageKeyboardLayouts();

    QString fileName = absolutePathToTheFolderLocationLanguageToAutocomplete()
            + QDir::separator()
            + name
            + "."
            + suffixAutocompleteLanguageFiles;

    // если файл существует
    QFileInfo fileAutocompletion(fileName);

    if(fileAutocompletion.isFile())
        autocompletionFileNotFound = false;
    else
        autocompletionFileNotFound = true;

    // если ini существует
    QFileInfo file(pathIni);

    if(file.isFile())
        loadDataNote();
    else
        saveDataNote();

    createWidgetsToNote();

    setDataNote(dataNote);

    setVisibleCommandButtons(false);

    initNodeIsEnd = true;
}

Note::Note(DataNote *dataNewNote, QString path)
{
    initNodeIsEnd = false;
    onetimeCounter = false;

    initNote(dataNewNote);

    pathIni = path;

    QString name = getTheCurrentLanguageKeyboardLayouts();

    QString fileName = absolutePathToTheFolderLocationLanguageToAutocomplete()
            + QDir::separator()
            + name
            + "."
            + suffixAutocompleteLanguageFiles;

    // если файл существует
    QFileInfo fileAutocompletion(fileName);

    if(fileAutocompletion.isFile())
        autocompletionFileNotFound = false;
    else
        autocompletionFileNotFound = true;

    createWidgetsToNote();

    setDataNote(dataNote);

    setVisibleCommandButtons(false);

    initNodeIsEnd = true;
}

/// PUBLIC SLOTS
void Note::setWindowTitle(const QString &title)
{
    labelTitleWindow->setText(title);
    dataNote.title = title;

    if(dialogSettings)
        dialogSettings->lineEditSelectTitle->setText(title);

    if(miniDialogSetTitle)
        miniDialogSetTitle->lineEdit->setText(title);

    saveTitle();
}

void Note::setWindowTitleWithSignal(const QString &title)
{
    setWindowTitle(title);

    emit titleChange(this);
}

void Note::setWindowOpacity(qreal level)
{
    dataNote.opacity = level;

    QWidget::setWindowOpacity(level);

    saveOpacity();
}

void Note::testAutocompleterLanguage()
{
    if(textEdit->completer())
    {
        QString name = getTheCurrentLanguageKeyboardLayouts();

        QString fileName = absolutePathToTheFolderLocationLanguageToAutocomplete()
                + QDir::separator()
                + name
                + "."
                + suffixAutocompleteLanguageFiles;

        // если файл существует
        QFileInfo file(fileName);
        if(file.isFile())
        {
            textEdit->completer()->setModel(modelFromFile(fileName,
                                                          textEdit->completer()));
            autocompletionFileNotFound = false;
        }else
            autocompletionFileNotFound = true;

        if(onetimeCounter)
            // если словарь автодополнения не найден,
            // нет смысла показывать кнопку вкл/выкл автодополнения
            if(autocompletionFileNotFound)
                buttonOnOffAutocomplete->setVisible(false);
            else
                buttonOnOffAutocomplete->setVisible(true);
    }
}

/// PRIVATE
void Note::initNote()
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("NewNote");

    QString textNewNote  = ini.value("Text", "").toString();

    QString titleTextNewNote = ini.value("Title", "")
            .toString();
    QString patternDateTime = ini.value("TitlePatternDateTime", "")
            .toString();

    QString titleDateTimeNewNote = QDateTime::currentDateTime()
            .toString(patternDateTime);

    QString titleNote = "";

    bool textIsExists = qStringIsValue(titleTextNewNote);
    bool dateIsExists = qStringIsValue(titleDateTimeNewNote);

    if(textIsExists)
        titleNote.append(titleTextNewNote);

    if(dateIsExists)
    {
        if(textIsExists)
            titleNote.append(" ");

        titleNote.append(titleDateTimeNewNote);
    }

    bool visibleNewNote = ini.value("Visible", true).toBool();
    bool minimizeNewNote = ini.value("Minimize", false).toBool();

    bool lockNewNote = ini.value("Lock", false).toBool();
    int opacityNewNote = ini.value("Opacity", maximalOpacity).toInt();

    QString zPositionNewNote = ini.value("zPosition",
                                         defaultZPositionNote).toString();

    if(zPositionNewNote != UzPosition::AlwaysOnBottom
            && zPositionNewNote != UzPosition::AlwaysOnTop)
    {
        ini.setValue("zPosition", defaultZPositionNote);
        zPositionNewNote = defaultZPositionNote;
    }

    dataNote.title      = titleNote;
    dataNote.lock       = lockNewNote;
    dataNote.text       = textNewNote;
    dataNote.visible    = visibleNewNote;
    dataNote.zPosition  = zPositionNewNote;

    dataNote.opacity = (double)opacityNewNote / 100.0;

    bool randomColor = ini.value("RandomColor", false).toBool();
    bool randomPositionOnScreen = ini.value("RandomPositionOnScreen", false).toBool();

    int valueWidth  = ini.value("Width", minimalWidthNote).toInt();
    int valueHeight = ini.value("Height", minimalHeightNote).toInt();

    dataNote.width      = valueWidth;
    dataNote.height     = valueHeight;

    if(randomPositionOnScreen)
    {
        dataNote.positionX = intRandom(0, widthDesktop() - dataNote.width - 20);
        dataNote.positionY = intRandom(0, heightDesktop() - dataNote.height - 20);
    }else
    {
        int valuePosX   = ini.value("PositionX", widthDesktop() / 2).toInt();
        int valuePosY   = ini.value("PositionY", heightDesktop() / 2).toInt();

        dataNote.positionX = valuePosX;
        dataNote.positionY = valuePosY;
    }

    if(randomColor)
        dataNote.backgroundColor = QColor(intRandom(0, 255),
                                          intRandom(0, 255),
                                          intRandom(0, 255));
    else
    {
        QString strColor = ini.value("BackgroundColor", "FFFF00").toString();
        QColor bgColor = getColorFromHexColorForStyleSheet(strColor);

        dataNote.backgroundColor = bgColor;
    }

    ini.endGroup();

    QDateTime dataTime = QDateTime::currentDateTime();

    dataNote.dateOfCreating = QString("%1\n%2")
            .arg(dataTime.date().toString(formatDate))
            .arg(dataTime.time().toString(formatTime));

    dataNote.dateLastChange = dataNote.dateOfCreating;
    dataNote.dateOfLastRemoval  = QString("%1\n%2")
            .arg(QString(formatDate).replace(QRegExp("[dDMmYy]"), "-"))
            .arg(QString(formatTime).replace(QRegExp("[hHMmSs]"), "-"));
    dataNote.dateOfLastRestore = dataNote.dateOfLastRemoval;

    dataNote.countTextSymbols = 0;
    dataNote.countTextLines = 1;

    dataNote.password = "";
    dataNote.protectionActive = false;

    dataNote.autocomplete = true;

    dataNote.minimize = minimizeNewNote;
}
void Note::initNote(QString text)
{
    QSettings ini(absolutePathToTheConfigurationProgram(),
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("NewNote");

    QString titleTextNewNote = ini.value("Title", "")
            .toString();
    QString patternDateTime = ini.value("TitlePatternDateTime", "")
            .toString();

    QString titleDateTimeNewNote = QDateTime::currentDateTime()
            .toString(patternDateTime);

    QString titleNote = "";

    bool textIsExists = qStringIsValue(titleTextNewNote);
    bool dateIsExists = qStringIsValue(titleDateTimeNewNote);

    if(textIsExists)
        titleNote.append(titleTextNewNote);

    if(dateIsExists)
    {
        if(textIsExists)
            titleNote.append(" ");

        titleNote.append(titleDateTimeNewNote);
    }

    bool visibleNewNote = ini.value("Visible", true).toBool();
    bool minimizeNewNote = ini.value("Minimize", false).toBool();

    bool lockNewNote = ini.value("Lock", false).toBool();
    int opacityNewNote = ini.value("Opacity", maximalOpacity).toInt();

    QString zPositionNewNote = ini.value("zPosition",
                                         defaultZPositionNote).toString();

    if(zPositionNewNote != UzPosition::AlwaysOnBottom
            && zPositionNewNote != UzPosition::AlwaysOnTop)
    {
        ini.setValue("zPosition", defaultZPositionNote);
        zPositionNewNote = defaultZPositionNote;
    }

    dataNote.title      = titleNote;
    dataNote.lock       = lockNewNote;
    dataNote.text       = text;
    dataNote.visible    = visibleNewNote;
    dataNote.zPosition  = zPositionNewNote;

    dataNote.opacity = (double)opacityNewNote / 100.0;

    bool randomColor = ini.value("RandomColor", false).toBool();
    bool randomPositionOnScreen = ini.value("RandomPositionOnScreen", false).toBool();

    int valueWidth  = ini.value("Width", minimalWidthNote).toInt();
    int valueHeight = ini.value("Height", minimalHeightNote).toInt();

    dataNote.width      = valueWidth;
    dataNote.height     = valueHeight;

    if(randomPositionOnScreen)
    {
        dataNote.positionX = intRandom(0, widthDesktop() - dataNote.width - 20);
        dataNote.positionY = intRandom(0, heightDesktop() - dataNote.height - 20);
    }else
    {
        int valuePosX   = ini.value("PositionX", widthDesktop() / 2).toInt();
        int valuePosY   = ini.value("PositionY", heightDesktop() / 2).toInt();

        dataNote.positionX = valuePosX;
        dataNote.positionY = valuePosY;
    }

    if(randomColor)
        dataNote.backgroundColor = QColor(intRandom(0, 255),
                                          intRandom(0, 255),
                                          intRandom(0, 255));
    else
    {
        QString strColor = ini.value("BackgroundColor", "FFFF00").toString();
        QColor bgColor = getColorFromHexColorForStyleSheet(strColor);

        dataNote.backgroundColor = bgColor;
    }

    ini.endGroup();

    QDateTime dataTime = QDateTime::currentDateTime();

    dataNote.dateOfCreating = QString("%1\n%2")
            .arg(dataTime.date().toString(formatDate))
            .arg(dataTime.time().toString(formatTime));

    dataNote.dateLastChange = dataNote.dateOfCreating;
    dataNote.dateOfLastRemoval  = QString("%1\n%2")
            .arg(QString(formatDate).replace(QRegExp("[dDMmYy]"), "-"))
            .arg(QString(formatTime).replace(QRegExp("[hHMmSs]"), "-"));
    dataNote.dateOfLastRestore = dataNote.dateOfLastRemoval;

    dataNote.countTextSymbols = 0;
    dataNote.countTextLines = 1;

    dataNote.password = "";
    dataNote.protectionActive = false;

    dataNote.autocomplete = true;

    dataNote.minimize = minimizeNewNote;
}
void Note::initNote(DataNote *_data)
{
    dataNote.positionX = _data->positionX + 50;
    dataNote.positionY = _data->positionY + 50;
    dataNote.width      = _data->width;
    dataNote.height     = _data->height;
    dataNote.title      = _data->title;
    dataNote.lock       = _data->lock;
    dataNote.text       = _data->text;
    dataNote.visible    = _data->visible;
    dataNote.zPosition  = _data->zPosition;

    dataNote.opacity = _data->opacity;
    dataNote.backgroundColor   = _data->backgroundColor;

    dataNote.dateOfCreating = _data->dateOfCreating;
    dataNote.dateLastChange = _data->dateLastChange;
    dataNote.dateOfLastRemoval = _data->dateOfLastRemoval;
    dataNote.dateOfLastRestore = _data->dateOfLastRestore;

    dataNote.countTextSymbols = _data->countTextSymbols;
    dataNote.countTextLines   = _data->countTextLines;

    dataNote.password = _data->password;
    dataNote.protectionActive = _data->protectionActive;

    dataNote.autocomplete = true;

    dataNote.minimize = false;
}

void Note::createWidgetsToNote()
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName(textCodec.toUtf8()));

    labelTitleWindow = new QLabel(this);

    // Шрифт заголовка
    labelTitleWindow->setFont(QFont("Times", 13, -1, false));
    labelTitleWindow->setAlignment(Qt::AlignCenter);
    labelTitleWindow->setMinimumHeight(sizeButtonOnTitle);

    buttonSettings = new QToolButton(this);
    buttonSettings->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    buttonSettings->setIcon(QIcon(IconSettingsNote));
    buttonSettings->setFocusPolicy(Qt::NoFocus);
    buttonSettings->setFixedSize(sizeButtonOnTitle,
                                 sizeButtonOnTitle);

    buttonHide = new QToolButton(this);
    buttonHide->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    buttonHide->setIcon(QIcon(IconHide));
    buttonHide->setFocusPolicy(Qt::NoFocus);
    buttonHide->setFixedSize(sizeButtonOnTitle,
                             sizeButtonOnTitle);

    buttonLockUnlock = new QToolButton(this);
    buttonLockUnlock->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    buttonLockUnlock->setFocusPolicy(Qt::NoFocus);
    buttonLockUnlock->setFixedSize(sizeButtonOnTitle,
                                   sizeButtonOnTitle);

    buttonHideShowFormatingPanelText = new QToolButton(this);
    buttonHideShowFormatingPanelText->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    buttonHideShowFormatingPanelText->setFocusPolicy(Qt::NoFocus);
    buttonHideShowFormatingPanelText->setFixedSize(sizeButtonOnTitle,
                                                   sizeButtonOnTitle);
    buttonHideShowFormatingPanelText->setText(">");

    buttonOnOffAutocomplete = new QToolButton(this);
    buttonOnOffAutocomplete->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    buttonOnOffAutocomplete->setFocusPolicy(Qt::NoFocus);
    buttonOnOffAutocomplete->setFixedSize(sizeButtonOnTitle,
                                          sizeButtonOnTitle);

    connect(buttonSettings, SIGNAL(clicked()),
            this, SLOT(showSettings()));
    connect(buttonHide, SIGNAL(clicked()),
            this, SLOT(clickHide()));
    connect(buttonLockUnlock, SIGNAL(clicked()),
            this, SLOT(clickButtonLockUnlock()));
    connect(buttonOnOffAutocomplete, SIGNAL(clicked()),
            this, SLOT(clickButtonOnOffAutocomplete()));
    connect(buttonHideShowFormatingPanelText, SIGNAL(clicked()),
            this, SLOT(clickbuttonHideShowFormatingPanelText()));

    setVisibleCommandButtons(false);

    QWidget *gag = new QWidget();
    gag->setFixedHeight(sizePushButton);

    hBoxLayoutCommandButtons = new QHBoxLayout();
    hBoxLayoutCommandButtons->addStretch();
    hBoxLayoutCommandButtons->addWidget(gag);
    hBoxLayoutCommandButtons->addWidget(buttonSettings);
    hBoxLayoutCommandButtons->addWidget(buttonHide);
    hBoxLayoutCommandButtons->addWidget(buttonOnOffAutocomplete);
    hBoxLayoutCommandButtons->addWidget(buttonLockUnlock);
    hBoxLayoutCommandButtons->addWidget(buttonHideShowFormatingPanelText);

    hBoxLayoutCommandButtons->setSpacing(0);

    textEdit = new UTextEdit(this);
    textEdit->setFrameStyle(QFrame::NoFrame);
    // цвет text edit будет такой же как у его родителя (AbstractNote)
    // другими словами он стал прозрачным
    textEdit->viewport()->setAutoFillBackground(false);
    textEdit->installEventFilter(this);

    textEdit->setAcceptDrops(true);
    connect(textEdit, SIGNAL(textChanged()),
            this, SLOT(textEditChangeText()));

    passwordProtect = new UPasswordProtect(this);
    passwordProtect->setVisible(false);
    connect(this, SIGNAL(languageIsChange(QString)),
            passwordProtect, SLOT(setLanguage(QString)));
    connect(passwordProtect, SIGNAL(enteredThePasswordRight()),
            this, SLOT(removeProtection()));

    QVBoxLayout *vBoxLayoutMain = new QVBoxLayout();
    vBoxLayoutMain->addWidget(labelTitleWindow,
                              0,
                              Qt::AlignCenter);
    vBoxLayoutMain->addLayout(hBoxLayoutCommandButtons);
    vBoxLayoutMain->addWidget(textEdit);
    vBoxLayoutMain->addWidget(passwordProtect);

    vBoxLayoutMain->setSpacing(0);

    setLayout(vBoxLayoutMain);

    createActions();

    theirFormats = QString("File Format Notes (*.%1)\n")
            .arg(suffixFilesNote);

    using namespace UFormatsText::UDescriptionFilters;

    textFormats = TXT + "\n"
            + ODT + "\n"
            + RTF + "\n"
            + HTML + "\n"
            + PDF + "\n";

    imageFormats = UFormatsImage::getDescriptionFiltersImage();

    filterFormats = theirFormats + textFormats + imageFormats;

    timerAnimationOpacity = new QTimer(this);
    connect(timerAnimationOpacity, SIGNAL(timeout()),
            this, SLOT(opacityMinus()));

    visibleContextMenu = false;

    dialogSettings = 0;
    formatingPanelText = 0;
    unitedFindReplaceTextDialog = 0;
    miniDialogSetTitle = 0;
    miniDialogSetOpacity = 0;

    QString path = QString(":resource/styles/%1/%1.%2")
            .arg("Note")
            .arg(suffixStyleFiles);

    QFile file(path);
    file.open(QIODevice::ReadOnly);
    setStyleSheet(file.readAll());
}

void Note::setProtectNote(bool bValue)
{
    dataNote.protectionActive = bValue;

    passwordProtect->setVisible(bValue);

    if(bValue)
    {
        textEdit->setVisible(false);
        setVisibleCommandButtons(false);

        setVisibleFormatingPanelText(false);
    }else
    {
        textEdit->setVisible(true);
        setVisibleCommandButtons(true);

        textEdit->setVisibleScrollBar(true);
    }

    if(bValue)
    {
        actionSettings->setShortcut(0);
        actionLock->setShortcut(0);
        actionUnlock->setShortcut(0);
        actionDuplicate->setShortcut(0);
        actionOpen->setShortcut(0);
        actionSaveAs->setShortcut(0);
        actionPrint->setShortcut(0);
        actionPreviewPrint->setShortcut(0);
        actionFind->setShortcut(0);
        actionReplace->setShortcut(0);
        actionTitle->setShortcut(0);
    }else
    {
        actionSettings->setShortcut(ksSettings);
        actionHide->setShortcut(ksHide);
        actionLock->setShortcut(ksLock);
        actionUnlock->setShortcut(ksUnlock);
        actionMoveToRecycleBin->setShortcut(ksMoveToRecycleBin);
        actionDuplicate->setShortcut(ksDuplicate);
        actionOpen->setShortcut(ksOpen);
        actionSaveAs->setShortcut(ksSaveAs);
        actionPrint->setShortcut(ksPrint);
        actionPreviewPrint->setShortcut(ksPreviewPrint);
        actionFind->setShortcut(ksFind);
        actionReplace->setShortcut(ksReplace);
        actionTitle->setShortcut(ksTitle);
    }

    saveProtectionActive();
}

void Note::setVisibleCommandButtons(bool bValue)
{
    buttonSettings->setVisible(bValue);
    buttonHide->setVisible(bValue);
    buttonLockUnlock->setVisible(bValue);
    buttonHideShowFormatingPanelText->setVisible(bValue);

    // если словарь автодополнения не найден,
    // нет смысла показывать кнопку вкл/выкл автодополнения
    if(autocompletionFileNotFound)
        buttonOnOffAutocomplete->setVisible(false);
    else
        buttonOnOffAutocomplete->setVisible(bValue);
}
void Note::setEnabledCommandButtons(bool bValue)
{
    buttonSettings->setEnabled(bValue);
    buttonHide->setEnabled(bValue);
    buttonLockUnlock->setEnabled(bValue);
    buttonHideShowFormatingPanelText->setEnabled(bValue);
    buttonOnOffAutocomplete->setEnabled(bValue);
}

void Note::createActions()
{
    actionSelectAll = new QAction(this);
    actionSelectAll->setIcon(QIcon(IconSelectAll));
    connect(actionSelectAll, SIGNAL(triggered()),
            textEdit, SLOT(selectAll()));

    actionCut    = new QAction(this);
    actionCopy   = new QAction(this);
    actionPaste  = new QAction(this);

    actionCut->setIcon(QIcon(IconCut));
    actionCopy->setIcon(QIcon(IconCopy));
    actionPaste->setIcon(QIcon(IconPaste));

    connect(actionCut, SIGNAL(triggered()),
            textEdit, SLOT(cut()));
    connect(actionCopy, SIGNAL(triggered()),
            textEdit, SLOT(copy()));
    connect(actionPaste, SIGNAL(triggered()),
            textEdit, SLOT(paste()));

    actionSettings         = new QAction(this);
    actionHide             = new QAction(this);
    actionLock             = new QAction(this);
    actionUnlock           = new QAction(this);
    actionMoveToRecycleBin = new QAction(this);
    actionDuplicate        = new QAction(this);

    actionHide->setIcon(QIcon(IconHide));
    actionSettings->setIcon(QIcon(IconSettingsNote));
    actionLock->setIcon(QIcon(IconLock));
    actionUnlock->setIcon(QIcon(IconUnlock));
    actionMoveToRecycleBin->setIcon(QIcon(IconRecycleBin_Full));
    actionDuplicate->setIcon(QIcon(IconDuplicate));

    connect(actionSettings, SIGNAL(triggered()),
            this, SLOT(showSettings()));
    connect(actionHide, SIGNAL(triggered()),
            this, SLOT(clickHide()));
    connect(actionLock, SIGNAL(triggered()),
            this, SLOT(clickLock()));
    connect(actionUnlock, SIGNAL(triggered()),
            this, SLOT(clickUnlock()));
    connect(actionMoveToRecycleBin, SIGNAL(triggered()),
            this, SLOT(clickMoveToRecycleBin()));
    connect(actionDuplicate, SIGNAL(triggered()),
            this, SLOT(pushDuplicate()));

    actionOpen = new QAction(this);
    actionSaveAs = new QAction(this);

    actionOpen->setIcon(QIcon(IconOpen));
    actionSaveAs->setIcon(QIcon(IconSaveAs));

    connect(actionSaveAs, SIGNAL(triggered()),
            this, SLOT(clickActionSaveAs()));
    connect(actionOpen, SIGNAL(triggered()),
            this, SLOT(clickActionOpen()));

    actionPrint = new QAction(this);
    actionPreviewPrint = new QAction(this);

    actionPrint->setIcon(QIcon(IconFilePrint));
    actionPreviewPrint->setIcon(QIcon(IconPreviewFilePrint));

    connect(actionPrint, SIGNAL(triggered()),
            this, SLOT(filePrint()));
    connect(actionPreviewPrint, SIGNAL(triggered()),
            this, SLOT(filePrintPreview()));

    actionFind = new QAction(this);
    actionReplace = new QAction(this);

    actionFind->setIcon(QIcon(IconFind));
    actionReplace->setIcon(QIcon(IconReplace));

    connect(actionFind, SIGNAL(triggered()),
            this, SLOT(findText()));
    connect(actionReplace, SIGNAL(triggered()),
            this, SLOT(replaceText()));

    actionOpacity_Other = new QAction(this);
    actionOpacity_20 = new QAction("20%", this);
    actionOpacity_30 = new QAction("30%", this);
    actionOpacity_40 = new QAction("40%", this);
    actionOpacity_50 = new QAction("50%", this);
    actionOpacity_60 = new QAction("60%", this);
    actionOpacity_70 = new QAction("70%", this);
    actionOpacity_80 = new QAction("80%", this);
    actionOpacity_90 = new QAction("90%", this);
    actionOpacity_100 = new QAction("100%", this);

    actionOpacity_20->setIcon(QIcon(IconOpacity_20));
    actionOpacity_30->setIcon(QIcon(IconOpacity_30));
    actionOpacity_40->setIcon(QIcon(IconOpacity_40));
    actionOpacity_50->setIcon(QIcon(IconOpacity_50));
    actionOpacity_60->setIcon(QIcon(IconOpacity_60));
    actionOpacity_70->setIcon(QIcon(IconOpacity_70));
    actionOpacity_80->setIcon(QIcon(IconOpacity_80));
    actionOpacity_90->setIcon(QIcon(IconOpacity_90));
    actionOpacity_100->setIcon(QIcon(IconOpacity_100));

    menuOpacity = new QMenu();
    menuOpacity->setIcon(QIcon(IconOpacity));
    menuOpacity->addAction(actionOpacity_100);
    menuOpacity->addAction(actionOpacity_90);
    menuOpacity->addAction(actionOpacity_80);
    menuOpacity->addAction(actionOpacity_70);
    menuOpacity->addAction(actionOpacity_60);
    menuOpacity->addAction(actionOpacity_50);
    menuOpacity->addAction(actionOpacity_40);
    menuOpacity->addAction(actionOpacity_30);
    menuOpacity->addAction(actionOpacity_20);
    menuOpacity->addAction(actionOpacity_Other);

    connect(actionOpacity_Other, SIGNAL(triggered()),
            this, SLOT(showMiniDialogSetOpacity()));
    connect(actionOpacity_20, SIGNAL(triggered()),
            this, SLOT(opacity_20()));
    connect(actionOpacity_30, SIGNAL(triggered()),
            this, SLOT(opacity_30()));
    connect(actionOpacity_40, SIGNAL(triggered()),
            this, SLOT(opacity_40()));
    connect(actionOpacity_50, SIGNAL(triggered()),
            this, SLOT(opacity_50()));
    connect(actionOpacity_60, SIGNAL(triggered()),
            this, SLOT(opacity_60()));
    connect(actionOpacity_70, SIGNAL(triggered()),
            this, SLOT(opacity_70()));
    connect(actionOpacity_80, SIGNAL(triggered()),
            this, SLOT(opacity_80()));
    connect(actionOpacity_90, SIGNAL(triggered()),
            this, SLOT(opacity_90()));
    connect(actionOpacity_100, SIGNAL(triggered()),
            this, SLOT(opacity_100()));

    colors = new UPushButtonSelectColor(this);
    colors->hide();
    connect(colors, SIGNAL(selectedColor(QColor)),
            this, SLOT(setColorBackground(QColor)));
    connect(this, SIGNAL(languageIsChange(QString)),
            colors, SLOT(setLanguage(QString)));

    menuColor = new QMenu();
    menuColor->setIcon(QIcon(IconColor));
    menuColor->addActions(colors->actionsColors());

    actionTitle = new QAction(this);
    actionTitle->setIcon(QIcon(IconTitle));
    connect(actionTitle, SIGNAL(triggered()),
            this, SLOT(showMiniDialogSetTitle()));

    //
    actionSelectAll->setShortcut(ksSelectAll);
    actionCut->setShortcut(ksCut);
    actionCopy->setShortcut(ksCopy);
    actionPaste->setShortcut(ksPaste);
    actionSettings->setShortcut(ksSettings);
    actionHide->setShortcut(ksHide);
    actionLock->setShortcut(ksLock);
    actionUnlock->setShortcut(ksUnlock);
    actionMoveToRecycleBin->setShortcut(ksMoveToRecycleBin);
    actionDuplicate->setShortcut(ksDuplicate);
    actionOpen->setShortcut(ksOpen);
    actionSaveAs->setShortcut(ksSaveAs);
    actionPrint->setShortcut(ksPrint);
    actionPreviewPrint->setShortcut(ksPreviewPrint);
    actionFind->setShortcut(ksFind);
    actionReplace->setShortcut(ksReplace);
    actionTitle->setShortcut(ksTitle);

    // виджет будет отслеживать нажатие горячих клавиш
    addAction(actionSettings);
    addAction(actionHide);
    addAction(actionLock);
    addAction(actionUnlock);
    addAction(actionMoveToRecycleBin);
    addAction(actionDuplicate);
    addAction(actionOpen);
    addAction(actionSaveAs);
    addAction(actionFind);
    addAction(actionReplace);
    addAction(actionPrint);
    addAction(actionPreviewPrint);
    addAction(actionTitle);
}

// PUBLIC SLOTS
void Note::setDataNote(DataNote data)
{
    dataNote = data;

    setWindowTitle(dataNote.title);

    setGeometry(dataNote.positionX,
                dataNote.positionY,
                dataNote.width,
                dataNote.height);

    setLock(dataNote.lock);

    setAutocomplete(dataNote.autocomplete);

    Note::setMinimize(dataNote.minimize);

    QWidget::setWindowOpacity(dataNote.opacity);

    setColorBackground(dataNote.backgroundColor);

    textEdit->setText(dataNote.text);
    dataNote.countTextSymbols = textEdit->getCountTextSymbols();
    dataNote.countTextLines   = textEdit->getCountTextLines();

    setVisibleNote(dataNote.visible);

    setZPosition(dataNote.zPosition);

    if(dataNote.protectionActive)
        setProtectNote(dataNote.protectionActive);

    passwordProtect->setPassword(dataNote.password);

    saveDataNote();
}

void Note::showSettings()
{
    if(dialogSettings)
        dialogSettings->show();
    else
    {
        dialogSettings = new DialogSettingsNote(this);

        connect(this, SIGNAL(languageIsChange(QString)),
                dialogSettings, SLOT(setLanguage(QString)));

        dialogSettings->setLanguage(currentLanguage);

        connect(dialogSettings, SIGNAL(iClose()),
                this, SLOT(deleteDialogSettings()));
        connect(dialogSettings, SIGNAL(changeColor(QColor)),
                this, SLOT(setColorBackground(QColor)));
        connect(dialogSettings, SIGNAL(changeOpacity(qreal)),
                this, SLOT(setWindowOpacity(qreal)));
        connect(dialogSettings, SIGNAL(changeTitle(QString)),
                this, SLOT(setWindowTitleWithSignal(QString)));
        connect(dialogSettings, SIGNAL(changeZPosition(QString)),
                this, SLOT(setZPosition(QString)));
        connect(dialogSettings, SIGNAL(changePassword(QString,bool)),
                this, SLOT(setPassword(QString,bool)));
        connect(colors, SIGNAL(selectedColor(QColor)),
                dialogSettings, SLOT(setColorToButtonSelectColor(QColor)));

        if(miniDialogSetTitle)
        {
            connect(miniDialogSetTitle, SIGNAL(changeTitle(QString)),
                    dialogSettings->lineEditSelectTitle, SLOT(setText(QString)));
            connect(dialogSettings->lineEditSelectTitle, SIGNAL(textChanged(QString)),
                    miniDialogSetTitle->lineEdit, SLOT(setText(QString)));
        }
        if(miniDialogSetOpacity)
        {
            connect(miniDialogSetOpacity->slider, SIGNAL(valueChanged(int)),
                    dialogSettings->sliderSelectOpacity, SLOT(setValue(int)));
            connect(dialogSettings->sliderSelectOpacity, SIGNAL(valueChanged(int)),
                    miniDialogSetOpacity->slider, SLOT(setValue(int)));
        }

        int widthDesktop = QApplication::desktop()->width();

        // если не хватает места с правой стороны
        bool bValue1 = pos().x() + frameSize().width() + expectedWidthOfSettingsNote > widthDesktop;
        // если не хватает места с левой стороны
        bool bValue2 = pos().x() - expectedWidthOfSettingsNote < 0;
        QPoint posDialog;
        // если справа нет места а слева оно есть
        if(bValue1
                && !bValue2)
        {
            // диалог появится слева
            posDialog.setX(pos().x() - expectedWidthOfSettingsNote + 8);
            posDialog.setY(pos().y());

            // если справа есть места но нет места слева
        }else if(!bValue1 && bValue2)
        {
            // диалог появится справа
            posDialog.setX(pos().x() + frameSize().width());
            posDialog.setY(pos().y());

            // если слева и справа не хватает места для диалога
        }else if(bValue1 && bValue2)
        {
            // диалог появится на том же месте, где и заметка
            posDialog.setX(pos().x());
            posDialog.setY(pos().y());

        }else if(!bValue1 && !bValue2)
        {
            // диалог появится справа
            posDialog.setX(pos().x() + frameSize().width());
            posDialog.setY(pos().y());
        }

        dialogSettings->showDialog(&dataNote,
                                   posDialog);

        // напомним ему что не надо исчезать =)
        noteFocus_True();
    }
}

void Note::showMiniDialogSetTitle()
{
    if(miniDialogSetTitle)
        miniDialogSetTitle->show();
    else
    {
        miniDialogSetTitle = new UMiniDialogSetTitle(this);
        miniDialogSetTitle->setLanguage(currentLanguage);
        miniDialogSetTitle->lineEdit->setText(dataNote.title);

        connect(this, SIGNAL(languageIsChange(QString)),
                miniDialogSetTitle, SLOT(setLanguage(QString)));
        connect(miniDialogSetTitle, SIGNAL(iClose()),
                this, SLOT(deleteMiniDialogSetTitle()));
        connect(miniDialogSetTitle, SIGNAL(changeTitle(QString)),
                this, SLOT(setWindowTitleWithSignal(QString)));

        if(dialogSettings)
        {
            connect(miniDialogSetTitle, SIGNAL(changeTitle(QString)),
                    dialogSettings->lineEditSelectTitle, SLOT(setText(QString)));
            connect(dialogSettings->lineEditSelectTitle, SIGNAL(textChanged(QString)),
                    miniDialogSetTitle->lineEdit, SLOT(setText(QString)));
        }

        miniDialogSetTitle->show();
    }
}
void Note::showMiniDialogSetOpacity()
{
    if(miniDialogSetOpacity)
        miniDialogSetOpacity->show();
    else
    {
        miniDialogSetOpacity = new UMiniDialogSetOpacity(this);
        miniDialogSetOpacity->setLanguage(currentLanguage);
        miniDialogSetOpacity->slider->setValue(dataNote.opacity * 100);

        connect(this, SIGNAL(languageIsChange(QString)),
                miniDialogSetOpacity, SLOT(setLanguage(QString)));
        connect(miniDialogSetOpacity, SIGNAL(iClose()),
                this, SLOT(deleteMiniDialogSetOpacity()));
        connect(miniDialogSetOpacity, SIGNAL(changeOpacity(qreal)),
                this, SLOT(setWindowOpacity(qreal)));

        if(dialogSettings)
        {
            connect(miniDialogSetOpacity->slider, SIGNAL(valueChanged(int)),
                    dialogSettings->sliderSelectOpacity, SLOT(setValue(int)));
            connect(dialogSettings->sliderSelectOpacity, SIGNAL(valueChanged(int)),
                    miniDialogSetOpacity->slider, SLOT(setValue(int)));
        }

        miniDialogSetOpacity->show();
    }
}

void Note::deleteMiniDialogSetTitle()
{
    if(miniDialogSetTitle)
    {
        delete miniDialogSetTitle;
        miniDialogSetTitle = 0;
    }
}
void Note::deleteMiniDialogSetOpacity()
{
    if(miniDialogSetOpacity)
    {
        delete miniDialogSetOpacity;
        miniDialogSetOpacity = 0;
    }
}

void Note::loadDataNote()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    QString color = ini.value("BackgroundColor", "FFFF00").toString();

    QDateTime dataTime = QDateTime::currentDateTime();

    QString date = QString("%1\n%2")
            .arg(dataTime.date().toString(formatDate))
            .arg(dataTime.time().toString(formatTime));

    dataNote.dateLastChange = dataNote.dateOfCreating;

    dataNote.countTextSymbols = 0;
    dataNote.countTextLines = 1;

    dataNote.positionX        = ini.value("PositionX", 100).toInt();
    dataNote.positionY        = ini.value("PositionY", 100).toInt();
    dataNote.width            = ini.value("Width", 200).toInt();
    dataNote.height           = ini.value("Height", 200).toInt();
    dataNote.title            = ini.value("Title", "").toString();
    dataNote.lock             = ini.value("Lock", false).toBool();
    dataNote.text             = ini.value("Text", "").toString();
    dataNote.visible          = ini.value("Visible", true).toBool();
    dataNote.opacity          = ini.value("Opacity", 100).toFloat() / 100;
    dataNote.backgroundColor  = getColorFromHexColorForStyleSheet(color);

    dataNote.zPosition        = ini.value("zPosition",
                                          defaultZPositionNote).toString();

    if(dataNote.zPosition != UzPosition::AlwaysOnBottom
            && dataNote.zPosition != UzPosition::AlwaysOnTop)
    {
        ini.setValue("zPosition", defaultZPositionNote);
        dataNote.zPosition = defaultZPositionNote;
    }

    dataNote.dateOfCreating   = ini.value("DateOfCreating", date).toString();
    dataNote.dateLastChange   = ini.value("DateLastChange", date).toString();

    date = QString("%1\n%2")
            .arg(QString(formatDate).replace(QRegExp("[dDMmYy]"), "-"))
            .arg(QString(formatTime).replace(QRegExp("[hHMmSs]"), "-"));
    dataNote.dateOfLastRemoval = ini.value("DateOfLastRemoval", date).toString();
    dataNote.dateOfLastRestore = ini.value("DateOfLastRestore", date).toString();

    dataNote.countTextSymbols = ini.value("CountTextSymbols", 0).toInt();
    dataNote.countTextLines   = ini.value("CountTextLines", 1).toInt();
    dataNote.password         = ini.value("Password/Password", "").toString();
    dataNote.protectionActive = ini.value("Password/ProtectionActive", false).toBool();
    dataNote.autocomplete     = ini.value("Autocomplete", true).toBool();
    dataNote.minimize         = ini.value("Minimize", false).toBool();
}
void Note::saveDataNote()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    QString color = getStringHexColorForStyleSheet(dataNote.backgroundColor);

    ini.setValue("PositionX",        dataNote.positionX);
    ini.setValue("PositionY",        dataNote.positionY);
    ini.setValue("Width",             dataNote.width);
    ini.setValue("Height",            dataNote.height);
    ini.setValue("Title",             dataNote.title);
    ini.setValue("Lock",              dataNote.lock);
    ini.setValue("Text",              dataNote.text);
    ini.setValue("Visible",           dataNote.visible);
    ini.setValue("Opacity",           int(dataNote.opacity * 100));
    ini.setValue("BackgroundColor",   color);
    ini.setValue("zPosition",         dataNote.zPosition);
    ini.setValue("DateOfCreating",    dataNote.dateOfCreating);
    ini.setValue("DateLastChange",    dataNote.dateLastChange);
    ini.setValue("DateOfLastRemoval", dataNote.dateOfLastRemoval);
    ini.setValue("DateOfLastRestore", dataNote.dateOfLastRestore);
    ini.setValue("CountTextSymbols",  dataNote.countTextSymbols);
    ini.setValue("CountTextLines",    dataNote.countTextLines);
    ini.setValue("Password/Password", dataNote.password);
    ini.setValue("Password/ProtectionActive", dataNote.protectionActive);
    ini.setValue("Autocomplete",      dataNote.autocomplete);
    ini.setValue("Minimize",          dataNote.minimize);

    ini.sync();
}
void Note::setLock(bool bValue)
{
    emit lockIsChange(this);

    dataNote.lock = bValue;

    if(dataNote.lock)
    {
        textEdit->setReadOnly(true);
        buttonLockUnlock->setIcon(QIcon(IconUnlock));
    }else
    {
        textEdit->setReadOnly(false);
        buttonLockUnlock->setIcon(QIcon(IconLock));
    }

    saveLock();
}
void Note::setAutocomplete(bool bValue)
{
    dataNote.autocomplete = bValue;

    textEdit->setAutocomplete(dataNote.autocomplete);

    if(dataNote.autocomplete)
        buttonOnOffAutocomplete->setIcon(QIcon(IconClosedDictionary));
    else
        buttonOnOffAutocomplete->setIcon(QIcon(IconOpenDictionary));

    saveAutocomplete();
}
void Note::setVisibleNote(bool visible)
{
    setVisible(visible);
    dataNote.visible = visible;
    visibleChange(this);

    // скроллы не показываем
    textEdit->setVisibleScrollBar(false);

    // if formatingPanelText is create when delete formatingPanelText
    if(formatingPanelText)
        hideFormatingPanelText();
    hideFindReplaceText();
    if(dialogSettings)
        deleteDialogSettings();
    if(miniDialogSetTitle)
        deleteMiniDialogSetTitle();
    if(miniDialogSetOpacity)
        deleteMiniDialogSetOpacity();

    saveVisible();
}
void Note::clickMoveToRecycleBin()
{
    QMessageBox msgBox(QMessageBox::Question,
                       strTitleMessage,
                       strTextMessageMoveNoteToRecycleBin,
                       0,
                       this);

    msgBox.addButton(strOk, QMessageBox::YesRole);
    msgBox.addButton(strCancel, QMessageBox::RejectRole);

    if(msgBox.exec() == QMessageBox::AcceptRole)
    {
        // if formatingPanelText is create when delete formatingPanelText
        if(formatingPanelText)
            hideFormatingPanelText();
        hideFindReplaceText();
        if(dialogSettings)
            deleteDialogSettings();
        if(miniDialogSetTitle)
            deleteMiniDialogSetTitle();
        if(miniDialogSetOpacity)
            deleteMiniDialogSetOpacity();

        emit noteDelete(this);
    }
}
void Note::clickButtonLockUnlock()
{
    dataNote.lock = !dataNote.lock;

    setLock(dataNote.lock);
}
void Note::clickButtonOnOffAutocomplete()
{
    dataNote.autocomplete = !dataNote.autocomplete;

    setAutocomplete(dataNote.autocomplete);
}
void Note::refreshNote()
{
    setDataNote(dataNote);
}

void Note::setLanguage(QString pathToTheFileLanguage)
{
    emit languageIsChange(pathToTheFileLanguage);
    currentLanguage = pathToTheFileLanguage;

    QSettings ini(pathToTheFileLanguage,
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("Note");

    menuOpacity->setTitle(ini.value("Opacity").toString());

    menuColor->setTitle(ini.value("Color").toString());

    actionSelectAll->setText(ini.value("SelectAll").toString());

    actionCut->setText(ini.value("Cut").toString());
    actionCopy->setText(ini.value("Copy").toString());
    actionPaste->setText(ini.value("Paste").toString());
    actionSettings->setText(ini.value("Settings").toString());
    actionHide->setText(ini.value("Hide").toString());
    actionLock->setText(ini.value("Lock").toString());
    actionUnlock->setText(ini.value("Unlock").toString());
    actionMoveToRecycleBin->setText(ini.value("MoveToRecycleBin").toString());
    actionDuplicate->setText(ini.value("Duplicate").toString());
    actionSaveAs->setText(ini.value("SaveAs").toString());
    actionOpen->setText(ini.value("Open").toString());
    actionPrint->setText(ini.value("Print").toString());
    actionPreviewPrint->setText(ini.value("PreviewPrint").toString());

    actionFind->setText(ini.value("Find").toString());
    actionReplace->setText(ini.value("Replace").toString());

    actionOpacity_Other->setText(ini.value("OpacityOther").toString());
    actionTitle->setText(ini.value("Title").toString());

    strTitleMessage = ini.value("MessageTitleQuestion").toString();
    strTextMessageMoveNoteToRecycleBin = ini.value("MessageTextMoveNoteToRecycleBin").toString();
    strCancel = ini.value("MessageCancel").toString();
    strOk     = ini.value("MessageOk").toString();

    // TOOL TIP
    buttonSettings->setToolTip(ini.value("ToolTipSettings").toString());
    buttonHide->setToolTip(ini.value("ToolTipHide").toString());
    buttonLockUnlock->setToolTip(ini.value("ToolTipLockUnlock").toString());
    buttonHideShowFormatingPanelText->setToolTip(ini.value("ToolTipHideShowFormatingPanelText").toString());
    buttonOnOffAutocomplete->setToolTip(QString("<html> <b>%1</b> <br>%2 - %3<br>%4 - %5 </html>")
                                        .arg(ini.value("OnOffAutocomplete").toString())
                                        .arg("<img src='" + IconOpenDictionary + "' width='20' height='20'/>")
                                        .arg(ini.value("AutocompleteOn").toString())
                                        .arg("<img src='" + IconClosedDictionary + "' width='20' height='20'/>")
                                        .arg(ini.value("AutocompleteOff").toString()));

    ini.endGroup();
}

void Note::clickActionSaveAs()
{
    QString fileFilters = QString(filterFormats);

    QString titleFile = dataNote.title.isEmpty() ?
                noTitle : dataNote.title;

    QString text = "[:\/|*?\"<>]";
    int index = titleFile.indexOf(QRegExp(text));

    if(index != -1)
    {
        qDebug() << text;
        titleFile = titleFile.replace(QRegExp(text), "_");
    }

    // открываем диалог, по завершению его получаем строку с абсолютным
    // путем к файлу или пустую строку, если диалог был отменен
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    QString(),
                                                    titleFile,
                                                    fileFilters);

    if(!fileName.isEmpty())
    {
        using namespace UFormatsImage;
        using namespace UFormatsText;

        QString fileSuffix = QFileInfo(fileName).suffix();

        // сохранением в текстовые файлы будет происходить отдельно
        // от сохранения в изображения
        if(fileSuffix.contains(suffixFilesNote, Qt::CaseInsensitive)
                || fileSuffix.contains(TXT,  Qt::CaseInsensitive)
                || fileSuffix.contains(ODT,  Qt::CaseInsensitive)
                || fileSuffix.contains(RTF,  Qt::CaseInsensitive)
                || fileSuffix.contains(HTML, Qt::CaseInsensitive)
                || fileSuffix.contains(HTM,  Qt::CaseInsensitive)
                || fileSuffix.contains(PDF,  Qt::CaseInsensitive))
        {
            UWriterTextFiles writerTextFile;
            writerTextFile.writeOut(this,
                                    fileName);

        }else if(fileSuffix.contains(PNG,  Qt::CaseInsensitive)
                 || fileSuffix.contains(BMP,  Qt::CaseInsensitive)
                 || fileSuffix.contains(JPG,  Qt::CaseInsensitive)
                 || fileSuffix.contains(JPEG, Qt::CaseInsensitive)
                 || fileSuffix.contains(ICO,  Qt::CaseInsensitive)
                 || fileSuffix.contains(TIF,  Qt::CaseInsensitive)
                 || fileSuffix.contains(TIFF, Qt::CaseInsensitive)
                 || fileSuffix.contains(PPM,  Qt::CaseInsensitive)
                 || fileSuffix.contains(XBM,  Qt::CaseInsensitive)
                 || fileSuffix.contains(XPM,  Qt::CaseInsensitive)
                 )
        {
            // убираем фокус с текстового редактора заметки,
            // если он есть
            if(textEdit->isActiveWindow())
                setFocus();

            // получаем скриншот самого себя =)
            QPixmap pixmapNote = QPixmap::grabWidget(this);

            pixmapNote.save(fileName);
        }
    }
}
void Note::clickActionOpen()
{
    using namespace UFormatsText::UDescriptionFilters;

    QString fileFilters = TXT + "\n" + HTML;
    UReaderTextFiles readerTextFile;
    readerTextFile.setLanguage(currentLanguage);
    readerTextFile.readUsingDialogIn(textEdit,
                                     fileFilters);
}

void Note::opacity_20()
{
    setWindowOpacity(0.2);
    if(dialogSettings)
        dialogSettings->sliderSelectOpacity->setSliderPosition(windowOpacity() * 100);
}
void Note::opacity_30()
{
    setWindowOpacity(0.3);
    if(dialogSettings)
        dialogSettings->sliderSelectOpacity->setSliderPosition(windowOpacity() * 100);
}
void Note::opacity_40()
{
    setWindowOpacity(0.4);
    if(dialogSettings)
        dialogSettings->sliderSelectOpacity->setSliderPosition(windowOpacity() * 100);
}
void Note::opacity_50()
{
    setWindowOpacity(0.5);
    if(dialogSettings)
        dialogSettings->sliderSelectOpacity->setSliderPosition(windowOpacity() * 100);
}
void Note::opacity_60()
{
    setWindowOpacity(0.6);
    if(dialogSettings)
        dialogSettings->sliderSelectOpacity->setSliderPosition(windowOpacity() * 100);
}
void Note::opacity_70()
{
    setWindowOpacity(0.7);
    if(dialogSettings)
        dialogSettings->sliderSelectOpacity->setSliderPosition(windowOpacity() * 100);
}
void Note::opacity_80()
{
    setWindowOpacity(0.8);
    if(dialogSettings)
        dialogSettings->sliderSelectOpacity->setSliderPosition(windowOpacity() * 100);
}
void Note::opacity_90()
{
    setWindowOpacity(0.9);
    if(dialogSettings)
        dialogSettings->sliderSelectOpacity->setSliderPosition(windowOpacity() * 100);
}
void Note::opacity_100()
{
    setWindowOpacity(1.0);
    if(dialogSettings)
        dialogSettings->sliderSelectOpacity->setSliderPosition(windowOpacity() * 100);
}

void Note::showContextMenu(QPoint point)
{
    visibleContextMenu = true;

    if(!dataNote.protectionActive
            && !dataNote.minimize)
    {
        QTextCursor textCursor = textEdit->textCursor();

        if(textCursor.selectionEnd() - textCursor.selectionStart() <= 0)
        {
            actionCut->setEnabled(false);
            actionCopy->setEnabled(false);
        }else
        {
            actionCut->setEnabled(true);
            actionCopy->setEnabled(true);
        }

        if(textEdit->isReadOnly())
        {
            actionCut->setVisible(false);
            actionPaste->setVisible(false);
        }else
        {
            actionCut->setVisible(true);
            actionPaste->setVisible(true);
        }
    }

    QMenu *menu = new QMenu();

    if(!dataNote.protectionActive
            && !dataNote.minimize)
    {
        menu->addAction(actionCut);
        menu->addAction(actionCopy);
        menu->addAction(actionPaste);
        menu->addSeparator();
        menu->addAction(actionSelectAll);
    }

    if(!dataNote.protectionActive)
    {
        menu->addSeparator();
        menu->addAction(actionTitle);
        menu->addMenu(menuColor);
        menu->addMenu(menuOpacity);
        menu->addSeparator();
        menu->addAction(actionSettings);
        menu->addSeparator();
    }

    menu->addAction(actionHide);

    if(!dataNote.protectionActive
            && !dataNote.minimize)
    {
        menu->addAction(actionLock);
        menu->addAction(actionUnlock);
    }

    menu->addAction(actionMoveToRecycleBin);

    if(!dataNote.protectionActive
            && !dataNote.minimize)
    {
        menu->addSeparator();
        menu->addAction(actionDuplicate);
        menu->addSeparator();
        menu->addAction(actionFind);
        menu->addAction(actionReplace);
        menu->addSeparator();
        menu->addAction(actionOpen);
        menu->addAction(actionSaveAs);
        menu->addSeparator();
        menu->addAction(actionPrint);
        menu->addAction(actionPreviewPrint);
    }

    menu->exec(point);

    delete menu;

    visibleContextMenu = false;
}

void Note::clickbuttonHideShowFormatingPanelText()
{
    bool visible;
    // if formatingPanelText is create
    if(formatingPanelText)
        visible = false;
    else
        visible = true;

    setVisibleFormatingPanelText(visible);
}

void Note::setVisibleFormatingPanelText(bool bValue)
{
    if(bValue && !formatingPanelText)
    {
        formatingPanelText = new UFormatingPanelText();

        if(formatingPanelText)
            if(dataNote.zPosition == UzPosition::AlwaysOnBottom)
                formatingPanelText->setWindowFlags(Qt::Tool
                                                   | Qt::WindowStaysOnBottomHint);

            else
                formatingPanelText->setWindowFlags(Qt::Tool
                                                   | Qt::WindowStaysOnTopHint);


        formatingPanelText->installEventFilter(textEdit);

        formatingPanelText->move(pos().x() + width(),
                                 pos().y());

        formatingPanelText->setLanguage(currentLanguage);

        connect(formatingPanelText, SIGNAL(isHideMe()),
                this, SLOT(hideFormatingPanelText()));

        connect(this, SIGNAL(languageIsChange(QString)),
                formatingPanelText, SLOT(setLanguage(QString)));

        textEdit->setConnectWithFormatingPanelText(formatingPanelText);

        formatingPanelText->show();

        buttonHideShowFormatingPanelText->setText("<");

    }else
    {
        delete formatingPanelText;
        formatingPanelText = 0;

        buttonHideShowFormatingPanelText->setText(">");
    }
}

void Note::setMinimize(bool bValue)
{
    dataNote.minimize = bValue;

    if(!dataNote.protectionActive)
    {
        textEdit->setVisible(!bValue);
        setVisibleCommandButtons(!bValue);

        textEdit->setVisibleScrollBar(true);
    }

    // при минимизации прячем панель форматирования
    // if formatingPanelText is create when delete formatingPanelText
    if(formatingPanelText)
        hideFormatingPanelText();

    AbstractNote::setMinimize(bValue);

    if(bValue)
    {
        // отключаем комбинации, чтобы не сработали,
        // пока заметка минимизарована
        actionLock->setShortcut(0);
        actionUnlock->setShortcut(0);
        actionDuplicate->setShortcut(0);
        actionOpen->setShortcut(0);
        actionSaveAs->setShortcut(0);
        actionPrint->setShortcut(0);
        actionPreviewPrint->setShortcut(0);
        actionFind->setShortcut(0);
        actionReplace->setShortcut(0);
    }else
    {
        actionSettings->setShortcut(ksSettings);
        actionHide->setShortcut(ksHide);
        actionLock->setShortcut(ksLock);
        actionUnlock->setShortcut(ksUnlock);
        actionMoveToRecycleBin->setShortcut(ksMoveToRecycleBin);
        actionDuplicate->setShortcut(ksDuplicate);
        actionOpen->setShortcut(ksOpen);
        actionSaveAs->setShortcut(ksSaveAs);
        actionPrint->setShortcut(ksPrint);
        actionPreviewPrint->setShortcut(ksPreviewPrint);
        actionFind->setShortcut(ksFind);
        actionReplace->setShortcut(ksReplace);
        actionTitle->setShortcut(ksTitle);
    }

    emit noteIsMinimize(bValue);

    saveMinimize();
}
void Note::setColorBackground(QColor color)
{
    dataNote.backgroundColor = color;

    AbstractNote::setColorBackground(color);

    saveColor();
}
void Note::setZPosition(QString zPosition)
{
    dataNote.zPosition = zPosition;

    AbstractNote::setZPosition(zPosition);

    if(formatingPanelText)
        if(zPosition == UzPosition::AlwaysOnBottom)
        {
            formatingPanelText->setWindowFlags(Qt::Tool
                                               | Qt::WindowStaysOnBottomHint);
            formatingPanelText->show();

        }else
        {
            formatingPanelText->setWindowFlags(Qt::Tool
                                               | Qt::WindowStaysOnTopHint);
            formatingPanelText->show();
        }

    saveZPosition();
}

void Note::filePrint()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);

    if(textEdit->textCursor().hasSelection())
        dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);

    if(dlg->exec() == QDialog::Accepted)
        textEdit->print(&printer);

    delete dlg;
#endif
}

void Note::filePrintPreview()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);

    connect(&preview, SIGNAL(paintRequested(QPrinter*)),
            SLOT(printPreview(QPrinter*)));
    preview.exec();
#endif
}

void Note::printPreview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    textEdit->print(printer);
#endif
}

void Note::findText()
{
    if(unitedFindReplaceTextDialog)
        unitedFindReplaceTextDialog->showFind();

    else
    {
        unitedFindReplaceTextDialog = new UUnitedFindReplaceTextDialog(this);
        unitedFindReplaceTextDialog->setTextEdit(textEdit);
        unitedFindReplaceTextDialog->setLanguage(currentLanguage);
        unitedFindReplaceTextDialog->showFind();
        unitedFindReplaceTextDialog->show();

        connect(this, SIGNAL(languageIsChange(QString)),
                unitedFindReplaceTextDialog, SLOT(setLanguage(QString)));

        connect(unitedFindReplaceTextDialog, SIGNAL(iClose()),
                this, SLOT(hideFindReplaceText()));
    }
}
void Note::replaceText()
{
    if(unitedFindReplaceTextDialog)
        unitedFindReplaceTextDialog->showReplace();

    else
    {
        unitedFindReplaceTextDialog = new UUnitedFindReplaceTextDialog(this);
        unitedFindReplaceTextDialog->setTextEdit(textEdit);
        unitedFindReplaceTextDialog->setLanguage(currentLanguage);
        unitedFindReplaceTextDialog->showReplace();
        unitedFindReplaceTextDialog->show();

        connect(this, SIGNAL(languageIsChange(QString)),
                unitedFindReplaceTextDialog, SLOT(setLanguage(QString)));

        connect(unitedFindReplaceTextDialog, SIGNAL(iClose()),
                this, SLOT(hideFindReplaceText()));
    }
}
void Note::hideFindReplaceText()
{
    // if create
    if(unitedFindReplaceTextDialog)
    {
        delete unitedFindReplaceTextDialog;
        unitedFindReplaceTextDialog = 0;
    }
}

void Note::savePosition()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("PositionX", dataNote.positionX);
    ini.setValue("PositionY", dataNote.positionY);
    ini.sync();
}
void Note::saveSize()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("Width", dataNote.width);
    ini.setValue("Height", dataNote.height);
    ini.sync();
}
void Note::saveTitle()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("Title", dataNote.title);
    ini.sync();
}
void Note::saveLock()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("Lock", dataNote.lock);
    ini.sync();
}
void Note::saveText()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("Text", dataNote.text);
    ini.sync();
}
void Note::saveVisible()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("Visible", dataNote.visible);
    ini.sync();
}
void Note::saveOpacity()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("Opacity", int(dataNote.opacity * 100));
    ini.sync();
}
void Note::saveColor()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("BackgroundColor",
                 getStringHexColorForStyleSheet(dataNote.backgroundColor));
    ini.sync();
}
void Note::saveZPosition()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("zPosition", dataNote.zPosition);
    ini.sync();
}
void Note::saveDateOfCreating()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("DateOfCreating", dataNote.dateOfCreating);
    ini.sync();
}
void Note::saveDateLastChange()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("DateLastChange", dataNote.dateLastChange);
    ini.sync();
}
void Note::saveDateOfLastRemoval()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("DateOfLastRemoval", dataNote.dateOfLastRemoval);
    ini.sync();
}
void Note::saveDateOfLastRestore()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("DateOfLastRestore", dataNote.dateOfLastRestore);
    ini.sync();
}
void Note::saveCountTextSymbols()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("CountTextSymbols", dataNote.countTextSymbols);
    ini.sync();
}
void Note::saveCountTextLines()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("CountTextLines", dataNote.countTextLines);
    ini.sync();
}
void Note::savePassword()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("Password/Password", dataNote.password);
    ini.sync();
}
void Note::saveProtectionActive()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("Password/ProtectionActive", dataNote.protectionActive);
    ini.sync();
}
void Note::saveAutocomplete()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("Autocomplete", dataNote.autocomplete);
    ini.sync();
}
void Note::saveMinimize()
{
    QSettings ini(pathIni, QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
    ini.setValue("Minimize", dataNote.minimize);
    ini.sync();
}

/// PRIVATE SLOTS
void Note::textEditChangeText()
{
    if(initNodeIsEnd)
    {
        dataNote.text = textEdit->toHtml();
        dataNote.countTextSymbols = textEdit->getCountTextSymbols();
        dataNote.countTextLines   = textEdit->getCountTextLines();

        QDateTime dataTime = QDateTime::currentDateTime();
        dataNote.dateLastChange = QString("%1\n%2")
                .arg(dataTime.date().toString(formatDate))
                .arg(dataTime.time().toString(formatTime));

        saveText();
        saveCountTextSymbols();
        saveCountTextLines();
        saveDateLastChange();

        emit propertiesTextEditNote(this);
    }
}

void Note::opacityMinus()
{
    if(windowOpacity() > dataNote.opacity)
        QWidget::setWindowOpacity(windowOpacity() - 0.1);
    else
        timerAnimationOpacity->stop();
}
void Note::noteFocus_True()
{  
    if(!dataNote.minimize
            && !dataNote.protectionActive)
    {
        QString name = getTheCurrentLanguageKeyboardLayouts();

        QString fileName = absolutePathToTheFolderLocationLanguageToAutocomplete()
                + QDir::separator()
                + name
                + "."
                + suffixAutocompleteLanguageFiles;

        // если файл существует
        QFileInfo fileAutocompletion(fileName);

        if(fileAutocompletion.isFile())
            autocompletionFileNotFound = false;
        else
            autocompletionFileNotFound = true;

        setVisibleCommandButtons(true);

        textEdit->setVisibleScrollBar(true);
    }

    timerAnimationOpacity->stop();
    QWidget::setWindowOpacity(1.0);

    QTimer::singleShot(1, this,
                       SLOT(enabledTrueCommandButtons()));
}
void Note::noteFocus_False()
{
    if(!dataNote.minimize
            && !dataNote.protectionActive)
    {
        QString name = getTheCurrentLanguageKeyboardLayouts();

        QString fileName = absolutePathToTheFolderLocationLanguageToAutocomplete()
                + QDir::separator()
                + name
                + "."
                + suffixAutocompleteLanguageFiles;

        // если файл существует
        QFileInfo fileAutocompletion(fileName);

        if(fileAutocompletion.isFile())
            autocompletionFileNotFound = false;
        else
            autocompletionFileNotFound = true;

        setVisibleCommandButtons(false);

        textEdit->setVisibleScrollBar(false);
    }

    timerAnimationOpacity->start(50);

    setEnabledCommandButtons(false);
}
void Note::deleteDialogSettings()
{
    delete dialogSettings;
    dialogSettings = 0;
}

void Note::setPassword(QString password, bool active)
{
    dataNote.password = password;
    dataNote.protectionActive = active;

    passwordProtect->setPassword(password);
    setProtectNote(active);
    savePassword();
}

/// PROTECTED
void Note::dropEvent(QDropEvent *dropEvent)
{
    if(dropEvent->mimeData()->hasText())
        textEdit->setText(dropEvent->mimeData()->text());

    if(dropEvent->mimeData()->hasHtml())
        textEdit->setHtml(dropEvent->mimeData()->html());

    if(dropEvent->mimeData()->hasUrls())
    {
        QList <QUrl> list = dropEvent->mimeData()->urls();

        // "кидать" на заметку можно только 1 файл
        if(list.count() == 1)
        {
            // получаем первый (он же единственный элемент списка)
            QUrl urlDropFile = list.first();

            if(urlDropFile.isLocalFile())
            {
                QString fileName = urlDropFile.toLocalFile();

                UReaderTextFiles readerTextFiles;

                readerTextFiles.setLanguage(currentLanguage);

                readerTextFiles.readIn(textEdit,
                                       fileName);
            }
        }
    }

    dropEvent->acceptProposedAction();
}
void Note::dragEnterEvent(QDragEnterEvent *dragEnter)
{
    if(dragEnter->mimeData()->hasText()
            || dragEnter->mimeData()->hasHtml()
            || dragEnter->mimeData()->hasUrls())
        dragEnter->acceptProposedAction();
}
void Note::contextMenuEvent(QContextMenuEvent *event)
{
    showContextMenu(event->globalPos());
}
void Note::mousePressEvent(QMouseEvent *event)
{
    AbstractNote::mousePressEvent(event);

    if(event->button() == Qt::RightButton)
    {
        if(dataNote.lock)
        {
            actionLock->setVisible(false);
            actionUnlock->setVisible(true);
        }else
        {
            actionLock->setVisible(true);
            actionUnlock->setVisible(false);
        }
    }
}
void Note::focusInEvent(QFocusEvent *)
{
    if(!onetimeCounter)
        onetimeCounter = true;

    noteFocus_True();
}
void Note::focusOutEvent(QFocusEvent *)
{
    if(onetimeCounter)
        onetimeCounter = false;

    // если панель не видна и нет диалога поиска/замены
    // и не показано контекстное меню и не вызван диалог настроек
    if(!formatingPanelText
            && !unitedFindReplaceTextDialog
            && !visibleContextMenu
            && !dialogSettings
            && !miniDialogSetTitle
            && !miniDialogSetOpacity)
        noteFocus_False();
}
bool Note::eventFilter(QObject *object, QEvent *event)
{
    if(object == textEdit)
    {
        if(event->type() == QEvent::FocusIn)
        {
            QFocusEvent *focusEvent = static_cast <QFocusEvent *> (event);
            focusInEvent(focusEvent);
            textEdit->focusInEvent(focusEvent);

            return false;

        }else if(event->type() == QEvent::FocusOut)
        {
            QFocusEvent *focusEvent = static_cast <QFocusEvent *> (event);
            focusOutEvent(focusEvent);
            textEdit->focusOutEvent(focusEvent);

            return false;

        }else
            return false;
    }
}
void Note::resizeEvent(QResizeEvent *)
{
    if(!dataNote.minimize)
    {
        dataNote.width  = width();
        dataNote.height = height();

        // if formatingPanelText is create
        if(formatingPanelText)
            // formatingPanelText will move note
            formatingPanelText->move(pos().x() + width(),
                                     pos().y());

        savePosition();
        saveSize();
    }
}
void Note::moveEvent(QMoveEvent *)
{
    dataNote.positionX = x();
    dataNote.positionY = y();

    // if formatingPanelText is create
    if(formatingPanelText)
        formatingPanelText->move(pos().x() + width(),
                                 pos().y());
    if(dataNote.minimize)
        buttonOnOffAutocomplete->setVisible(false);

    savePosition();
}
void Note::mouseDoubleClickEvent(QMouseEvent * event)
{
    // если указатель мыши находился на заголовке
    if(event->pos().y() <= topBorder)
        Note::setMinimize(!dataNote.minimize);
}
void Note::changeEvent(QEvent *event)
{
    // изменилась раскладка
    if(event->type() == QEvent::KeyboardLayoutChange)
        testAutocompleterLanguage();
    else
        QWidget::changeEvent(event);
}
