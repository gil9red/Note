#include "UFindReplaceTextDialog.h"

/// PUBLIC
UFindReplaceTextDialog::UFindReplaceTextDialog(QWidget *parent):
    QWidget(parent),
    colorSelection(Qt::green),
    deleteWhenClose(true)
{
    setWindowFlags(Qt::Tool);
    createForm();
}

void UFindReplaceTextDialog::setTextEdit(QTextEdit *textEdit)
{
    this->textEdit = textEdit;

    lineEditFindText->setText(this->textEdit->textCursor().selectedText());
}

void UFindReplaceTextDialog::deselect()
{
    // если выделения есть
    if(!textEdit->extraSelections().isEmpty())
    {
        textEdit->setExtraSelections(QList<QTextEdit::ExtraSelection>());
        textEdit->setTextCursor(textCursor);
    }
}

/// PRIVATE
void UFindReplaceTextDialog::createForm()
{
    ///
    labelFind = new QLabel();
    labelReplaceTo = new QLabel();

    QPixmap pixmap_enter(IconClearTextEnter);
    QPixmap pixmap_leave(IconClearTextLeave);

    int widthIcon = 20;
    int heightIcon = 20;
    pixmap_enter = pixmap_enter.scaled(widthIcon, heightIcon, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pixmap_leave = pixmap_leave.scaled(widthIcon, heightIcon, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    lineEditFindText = new ULineEditWithClearButton();
    lineEditFindText->setIconButton(pixmap_leave);
    lineEditFindText->setIconButton(ULineEditWithClearButton::EnterEvent,
                            pixmap_enter);
    lineEditFindText->setIconButton(ULineEditWithClearButton::LeaveEvent,
                            pixmap_leave);

    lineEditReplaceText = new ULineEditWithClearButton();
    lineEditReplaceText->setIconButton(pixmap_leave);
    lineEditReplaceText->setIconButton(ULineEditWithClearButton::EnterEvent,
                            pixmap_enter);
    lineEditReplaceText->setIconButton(ULineEditWithClearButton::LeaveEvent,
                            pixmap_leave);


    QGridLayout *gLayout1 = new QGridLayout();

    gLayout1->addWidget(labelFind,           0, 0);
    gLayout1->addWidget(lineEditFindText,    0, 1);
    gLayout1->addWidget(labelReplaceTo,      1, 0);
    gLayout1->addWidget(lineEditReplaceText, 1, 1);
    ///

    ///
    labelStatus = new QLabel();
    ///

    ///
    rButtonUp = new QRadioButton();
    rButtonDown = new QRadioButton();
    rButtonDown->setChecked(true);

    QVBoxLayout *vBoxLayoutGBoxDirection = new QVBoxLayout();
    vBoxLayoutGBoxDirection->addWidget(rButtonDown);

    groupBoxDirection = new QGroupBox();
    groupBoxDirection->setLayout(vBoxLayoutGBoxDirection);

    checkBoxWholeWords = new QCheckBox();
    checkBoxCaseSensitive = new QCheckBox();
    checkBoxRegExp = new QCheckBox();


    QVBoxLayout *vBoxLayoutGBoxSettings = new QVBoxLayout();
    vBoxLayoutGBoxSettings->addWidget(checkBoxWholeWords);
    vBoxLayoutGBoxSettings->addWidget(checkBoxCaseSensitive);
    vBoxLayoutGBoxSettings->addWidget(checkBoxRegExp);

    groupBoxSettings = new QGroupBox();
    groupBoxSettings->setLayout(vBoxLayoutGBoxSettings);

    pButtonSelectColor = new UPushButtonSelectColor();
    selectColor(colorSelection);
    connect(pButtonSelectColor, SIGNAL(selectedColor(QColor)),
            this, SLOT(selectColor(QColor)));

    QHBoxLayout *hBoxLayoutGroupBoxs = new QHBoxLayout();
    hBoxLayoutGroupBoxs->addWidget(groupBoxDirection);
    hBoxLayoutGroupBoxs->addWidget(groupBoxSettings);
    ///


    QVBoxLayout *vBoxLayoutMain1 = new QVBoxLayout();
    vBoxLayoutMain1->addLayout(gLayout1);
    vBoxLayoutMain1->addWidget(labelStatus);
    vBoxLayoutMain1->addLayout(hBoxLayoutGroupBoxs);
    vBoxLayoutMain1->addWidget(pButtonSelectColor);

    ///
    pButtonFind       = new QPushButton();
    pButtonReplace    = new QPushButton();
    pButtonReplaceAll = new QPushButton();
    pButtonClose      = new QPushButton();

    connect(pButtonFind, SIGNAL(clicked()),
            this, SLOT(find()));
    connect(pButtonReplace, SIGNAL(clicked()),
            this, SLOT(replace()));
    connect(pButtonReplaceAll, SIGNAL(clicked()),
            this, SLOT(replaceAll()));
    connect(pButtonClose, SIGNAL(clicked()),
            this, SLOT(close()));

    QVBoxLayout *vBoxLayoutMain2 = new QVBoxLayout();
    vBoxLayoutMain2->addWidget(pButtonFind);
    vBoxLayoutMain2->addWidget(pButtonReplace);
    vBoxLayoutMain2->addWidget(pButtonReplaceAll);
    vBoxLayoutMain2->addWidget(pButtonClose);
    vBoxLayoutMain2->addStretch();
    ///

    QHBoxLayout *hBoxLayoutMain = new QHBoxLayout();
    hBoxLayoutMain->addLayout(vBoxLayoutMain1);
    hBoxLayoutMain->addLayout(vBoxLayoutMain2);

    setLayout(hBoxLayoutMain);
}

/// PUBLIC SLOTS
void UFindReplaceTextDialog::setLanguage(QString pathToTheFileLanguage)
{
    emit languageIsChange(pathToTheFileLanguage);

    pButtonSelectColor->setLanguage(pathToTheFileLanguage);

    QSettings ini(pathToTheFileLanguage,
                       QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("FindReplaceTextDialog");

    labelFind->setText(ini.value("Find").toString());
    labelReplaceTo->setText(ini.value("ReplaceTo").toString());

    groupBoxDirection->setTitle(ini.value("DirectionFind").toString());
    rButtonUp->setText(ini.value("Up").toString());
    rButtonDown->setText(ini.value("Down").toString());

    groupBoxSettings->setTitle(ini.value("Settings").toString());
    checkBoxWholeWords->setText(ini.value("WholeWords").toString());
    checkBoxCaseSensitive->setText(ini.value("CaseSensitive").toString());
    checkBoxRegExp->setText(ini.value("RegExp").toString());

    pButtonFind->setText(ini.value("ButtonFind").toString());
    pButtonReplace->setText(ini.value("Replace").toString());
    pButtonReplaceAll->setText(ini.value("ReplaceAll").toString());
    pButtonClose->setText(ini.value("Close").toString());

    pButtonSelectColor->setText(ini.value("SelectColor").toString());

    strNoMatchFound = ini.value("NoMatchFound").toString();
    strReplaced = ini.value("Replaced").toString();

    ini.endGroup();
}

void UFindReplaceTextDialog::find(bool next)
{
    if(!textEdit)
        return;

    // backward search
    bool up = !next;

    const QString toSearch = lineEditFindText->text();

    bool result = false;

    QTextDocument::FindFlags flags;

    bool caseSensitive      = checkBoxCaseSensitive->isChecked();
    bool wholeWords         = checkBoxWholeWords->isChecked();
    bool regularExpressions = checkBoxRegExp->isChecked();

    if(up)
        flags |= QTextDocument::FindBackward;

    if(caseSensitive)
        flags |= QTextDocument::FindCaseSensitively;

    if(wholeWords)
        flags |= QTextDocument::FindWholeWords;

    QTextCursor _textCursor = textEdit->textCursor();

    if(regularExpressions)
    {
        QRegExp reg(toSearch,
                    caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive);

        _textCursor = textEdit->document()->find(reg, _textCursor, flags);
        result = !_textCursor.isNull();

    }else
        result = textEdit->find(toSearch, flags);

    if(result)
        showError("");

    else
    {
        showError(strNoMatchFound);

        // move to the beginning of the document for the next find
        _textCursor.setPosition(0);
        textEdit->setTextCursor(_textCursor);
    }

    if(!regularExpressions)
        _textCursor = textEdit->textCursor();

    textCursor = _textCursor;

    _textCursor.clearSelection();
    textEdit->setTextCursor(_textCursor);

    if(textCursor.hasSelection())
    {
        QTextEdit::ExtraSelection selection;
        QTextCharFormat format;
        format.setBackground(colorSelection);
        selection.format = format;
        selection.cursor = textCursor;
        textEdit->setExtraSelections(QList <QTextEdit::ExtraSelection>()
                                     << selection);
    }
}

void UFindReplaceTextDialog::replace()
{
    if(!textCursor.hasSelection())
        find();

    else
    {
        textEdit->setTextCursor(textCursor);
        textCursor = textEdit->textCursor();
        textCursor.insertText(lineEditReplaceText->text());
        find();
    }
}

void UFindReplaceTextDialog::replaceAll()
{
    if(!textCursor.hasSelection())
        find();

    else
    {
        int i = 0;
        while(textCursor.hasSelection())
        {
            textCursor.insertText(lineEditReplaceText->text());
            find();
            i++;
        }
        showMessage(QString(strReplaced).arg(i));
    }
}

/// PRIVATE SLOTS
void UFindReplaceTextDialog::showError(const QString &error)
{
    if(error == "")
        labelStatus->setText("");
    else    
        labelStatus->setText("<span style=\" font-weight:600; color:#ff0000;\">"
                             + error
                             + "</spam>");
}

void UFindReplaceTextDialog::showMessage(const QString &message)
{
    if(message == "")
        labelStatus->setText("");

    else
        labelStatus->setText("<span style=\" font-weight:600; color:green;\">"
                             + message
                             + "</spam>");
}

void UFindReplaceTextDialog::selectColor(QColor color)
{
    QPixmap pixmap(50, 50);
    pixmap.fill(color);
    pButtonSelectColor->setIcon(QIcon(pixmap));
    colorSelection = color;
}

/// PROTECTED
void UFindReplaceTextDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
    hide();

    deselect();

    emit iClose();

    if(deleteWhenClose)
        delete this;
}

void UFindReplaceTextDialog::hideReplaceWidgets()
{
    labelReplaceTo->setVisible(false);
    lineEditReplaceText->setVisible(false);
    pButtonReplace->setVisible(false);
    pButtonReplaceAll->setVisible(false);
}

//! UUnitedFindReplaceTextDialog
/// PUBLIC
UUnitedFindReplaceTextDialog::UUnitedFindReplaceTextDialog(QWidget *parent):
    QWidget(parent)
{
    setWindowFlags(Qt::Tool);

    findReplaceTextDialog = new UFindReplaceTextDialog();
    findTextDialog = new UFindTextDialog();

    findReplaceTextDialog->setDeleteWhenClose(false);
    findTextDialog->setDeleteWhenClose(false);

    connect(findReplaceTextDialog, SIGNAL(iClose()),
            this, SLOT(close()));
    connect(findTextDialog, SIGNAL(iClose()),
            this, SLOT(close()));

    tabWidget = new QTabWidget();

    tabWidget->addTab(findTextDialog, tr(""));
    tabWidget->addTab(findReplaceTextDialog, tr(""));

    connect(tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(selectTab(int)));

    QVBoxLayout *vBoxLayoutMain = new QVBoxLayout();
    vBoxLayoutMain->addWidget(tabWidget);

    setLayout(vBoxLayoutMain);
}

void UUnitedFindReplaceTextDialog::setTextEdit(QTextEdit *textEdit)
{
    findReplaceTextDialog->setTextEdit(textEdit);
    findTextDialog->setTextEdit(textEdit);
}

/// PUBLIC SLOTS
void UUnitedFindReplaceTextDialog::setLanguage(QString pathToTheFileLanguage)
{
    emit languageIsChange(pathToTheFileLanguage);

    findReplaceTextDialog->setLanguage(pathToTheFileLanguage);
    findTextDialog->setLanguage(pathToTheFileLanguage);

    QSettings ini(pathToTheFileLanguage,
                       QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("FindReplaceTextDialog");

    tabWidget->setTabText(0, ini.value("FindTitle").toString());
    tabWidget->setTabText(1, ini.value("FindReplaceTitle").toString());

    ini.endGroup();

    selectTab(tabWidget->currentIndex());
}

/// PROTECTED
void UUnitedFindReplaceTextDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
    hide();

    // если выделения есть
    if(!findTextDialog->textEdit->extraSelections().isEmpty())
    {
        findTextDialog->textEdit->setExtraSelections(QList<QTextEdit::ExtraSelection>());
        findTextDialog->textEdit->setTextCursor(findTextDialog->textCursor);
    }
    emit iClose();
}
//! UUnitedFindReplaceTextDialog
