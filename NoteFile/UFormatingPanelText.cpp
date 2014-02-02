#include "UFormatingPanelText.h"

/// PUBLIC
UFormatingPanelText::UFormatingPanelText(QWidget *parent):
    QWidget(parent),
    symbolTable(0)
{
    setWindowFlags(Qt::Tool);

    QTextCodec::setCodecForTr(QTextCodec::codecForName(textCodec.toUtf8()));

    vBoxLayoutMain = new QVBoxLayout();

    addComboBox();
    addPushButtonChangeText();
    addPushButtonUnderline();
    addPushButtonAlign();
    addComboBoxList();
    addPushButtonSelectCaseSensitive();

    setLayout(vBoxLayoutMain);

    setFixedSize(widthFormatingPanel,
                 heightFormatingPanel);
}

/// PRIVATE
void UFormatingPanelText::addPushButtonChangeText()
{
    QHBoxLayout *hBoxLayoutSizeFont = new QHBoxLayout();

    comboSize = new UOfficeWordStyleComboBox();
    comboSize->setFixedHeight(sizePushButton);
    comboSize->setMinimumWidth(50);
    comboSize->setEditable(true);


    QList <int> listFontSize = QFontDatabase::standardSizes();

    for(int i = 0; i < listFontSize.count(); i++)
    {
        int size = listFontSize.at(i);
        comboSize->addItem(QString::number(size));
    }

    connect(comboSize, SIGNAL(activated(QString)),
            this, SIGNAL(selectFontSize(QString)));

    int pointSizeFont = QApplication::font().pointSize();
    int currentIndexComboSize = comboSize->findText(QString::number(pointSizeFont));
    comboSize->setCurrentIndex(currentIndexComboSize);

    hBoxLayoutSizeFont->addWidget(comboSize);

    pButtonIncreaseFontSize = new QPushButton();
    pButtonIncreaseFontSize->setIcon(QIcon(IconIncreaseFontSize));
    pButtonIncreaseFontSize->setFixedSize(sizePushButton,
                                          sizePushButton);
    connect(pButtonIncreaseFontSize, SIGNAL(clicked()),
            this, SLOT(clickPButtonIncreaseFontSize()));

    pButtonDecreaseFontSize = new QPushButton();
    pButtonDecreaseFontSize->setIcon(QIcon(IconDecreaseFontSize));
    pButtonDecreaseFontSize->setFixedSize(sizePushButton,
                                          sizePushButton);
    connect(pButtonDecreaseFontSize, SIGNAL(clicked()),
            this, SLOT(clickPButtonDecreaseFontSize()));

    hBoxLayoutSizeFont->addWidget(pButtonIncreaseFontSize);
    hBoxLayoutSizeFont->addWidget(pButtonDecreaseFontSize);
    hBoxLayoutSizeFont->addStretch();
    hBoxLayoutSizeFont->setSpacing(1);

    vBoxLayoutMain->addLayout(hBoxLayoutSizeFont);

    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    pButtonTextBold = new QPushButton(QIcon(IconBold),
                                      tr(""),
                                      this);
    pButtonTextBold->setCheckable(true);
    pButtonTextBold->setFixedSize(sizePushButton, sizePushButton);
    connect(pButtonTextBold, SIGNAL(clicked()),
            this, SLOT(clickPButtonBold()));

    pButtonTextItalic = new QPushButton(QIcon(IconItalic),
                                        tr(""),
                                        this);
    pButtonTextItalic->setCheckable(true);
    pButtonTextItalic->setFixedSize(sizePushButton, sizePushButton);
    connect(pButtonTextItalic, SIGNAL(clicked()),
            this, SLOT(clickPButtonItalic()));

    pButtonClearTheTextFromFormatting = new QPushButton(QIcon(IconClearFromFormatting),
                                                        tr(""),
                                                        this);
    pButtonClearTheTextFromFormatting->setFixedSize(sizePushButton, sizePushButton);
    QSize iconSize = pButtonClearTheTextFromFormatting->size();
    iconSize.setWidth(iconSize.width() - 5);
    iconSize.setHeight(iconSize.height() - 5);
    pButtonClearTheTextFromFormatting->setIconSize(iconSize);

    connect(pButtonClearTheTextFromFormatting, SIGNAL(clicked()),
            this, SLOT(clickPButtonClearTheTextFromFormatting()));


    pButtonInsertSymbol = new QPushButton("Symbol");
    connect(pButtonInsertSymbol, SIGNAL(clicked()),
            this, SLOT(clickInsertSymbol()));
//    hBoxLayout->addWidget(pButtonInsertSymbol);


    QPixmap pix(IconTextColor);
    QPainter *painter = new QPainter();
    painter->begin(&pix);
    painter->setBrush(QColor(Qt::black));
    painter->setPen(QColor(Qt::black));
    painter->drawRect(0, pix.height() - 25,
                      pix.width(), pix.height() - 25);
    painter->end();

    pButtonTextColor = new UPushButtonSelectColor(pix, "", this);
    pButtonTextColor->setFixedSize(sizePushButton, sizePushButton);
    connect(pButtonTextColor, SIGNAL(selectedColor(QColor)),
            this, SIGNAL(textColor(QColor)));
    connect(pButtonTextColor, SIGNAL(selectedColor(QColor)),
            this, SLOT(colorChanged(QColor)));

    hBoxLayout->addWidget(pButtonTextBold);
    hBoxLayout->addWidget(pButtonTextItalic);
    hBoxLayout->addWidget(pButtonTextColor);
    hBoxLayout->addWidget(pButtonClearTheTextFromFormatting);
    hBoxLayout->addStretch();

    hBoxLayout->setSpacing(1);

    vBoxLayoutMain->addLayout(hBoxLayout);
}
void UFormatingPanelText::addPushButtonUnderline()
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout();

    pButtonTextSubScript = new QPushButton(QIcon(IconSubScript),
                                           tr(""),
                                           this);
    pButtonTextSubScript->setCheckable(true);
    pButtonTextSubScript->setFixedSize(sizePushButton, sizePushButton);
    QSize iconSize = pButtonTextSubScript->size();
    iconSize.setWidth(iconSize.width() - 5);
    iconSize.setHeight(iconSize.height() - 5);
    pButtonTextSubScript->setIconSize(iconSize);

    connect(pButtonTextSubScript, SIGNAL(clicked()),
            this, SLOT(clickPButtonSubScript()));

    hBoxLayout->addWidget(pButtonTextSubScript);


    pButtonTextSuperScript = new QPushButton(QIcon(IconSuperScript),
                                             tr(""),
                                             this);
    pButtonTextSuperScript->setCheckable(true);
    pButtonTextSuperScript->setFixedSize(sizePushButton, sizePushButton);
    iconSize = pButtonTextSuperScript->size();
    iconSize.setWidth(iconSize.width() - 5);
    iconSize.setHeight(iconSize.height() - 5);
    pButtonTextSuperScript->setIconSize(iconSize);

    connect(pButtonTextSuperScript, SIGNAL(clicked()),
            this, SLOT(clickPButtonSuperScript()));

    hBoxLayout->addWidget(pButtonTextSuperScript);


    pButtonTextUnderline = new QPushButton(QIcon(IconUnderline),
                                           tr(""),
                                           this);
    pButtonTextUnderline->setCheckable(true);
    pButtonTextUnderline->setFixedSize(sizePushButton, sizePushButton);
    connect(pButtonTextUnderline, SIGNAL(clicked()),
            this, SLOT(clickPButtonUnderline()));

    pButtonTextStrikeOut = new QPushButton(QIcon(IconStrikeOut),
                                           tr(""),
                                           this);
    pButtonTextStrikeOut->setCheckable(true);
    pButtonTextStrikeOut->setFixedSize(sizePushButton, sizePushButton);
    iconSize = pButtonTextStrikeOut->size();
    iconSize.setWidth(iconSize.width() - 5);
    iconSize.setHeight(iconSize.height() - 5);
    pButtonTextStrikeOut->setIconSize(iconSize);
    connect(pButtonTextStrikeOut, SIGNAL(clicked()),
            this, SLOT(clickPButtonStrikeOut()));


    pButtonTextOverline = new QPushButton(QIcon(IconOverline),
                                          tr(""),
                                          this);
    pButtonTextOverline->setCheckable(true);
    pButtonTextOverline->setFixedSize(sizePushButton, sizePushButton);
    iconSize = pButtonTextOverline->size();
    iconSize.setWidth(iconSize.width() - 5);
    iconSize.setHeight(iconSize.height() - 5);
    pButtonTextOverline->setIconSize(iconSize);
    connect(pButtonTextOverline, SIGNAL(clicked()),
            this, SLOT(clickPButtonOverline()));


    pButtonSelectNoUnderline = new QPushButton(tr("Убрать подчеркивание"));
    pButtonSelectSingleUnderline = new QPushButton(/*tr("Подчеркивание")*/);
    pButtonSelectDashUnderline = new QPushButton(/*tr("Пунктирное подчеркивание")*/);
    pButtonSelectDotLine = new QPushButton(/*tr("Штриховое подчеркивание")*/);
    pButtonSelectDashDotLine = new QPushButton(/*tr("Штрихпунктирное подчеркивание")*/);
    pButtonSelectDashDotDotLine = new QPushButton(/*tr("Штрихпунктирное подчеркивание с двумя точками")*/);
    pButtonSelectWaveUnderline = new QPushButton(/*tr("Подчеркивание волнистой линией")*/);


    pButtonSelectSingleUnderline->setIcon(QIcon(IconSingleUnderline));
    pButtonSelectDashUnderline->setIcon(QIcon(IconDashUnderline));
    pButtonSelectDotLine->setIcon(QIcon(IconDotLineUnderline));
    pButtonSelectDashDotLine->setIcon(QIcon(IconDashDotLineUnderline));
    pButtonSelectDashDotDotLine->setIcon(QIcon(IconDashDotDotLineUnderline));
    pButtonSelectWaveUnderline->setIcon(QIcon(IconWaveUnderline));

    pButtonSelectSingleUnderline->setFixedHeight(sizePushButton);
    pButtonSelectSingleUnderline->setFixedWidth(140);
    pButtonSelectSingleUnderline->setIconSize(QSize(150, sizePushButton + 5));

    pButtonSelectDashUnderline->setFixedHeight(sizePushButton);
    pButtonSelectDashUnderline->setFixedWidth(140);
    pButtonSelectDashUnderline->setIconSize(QSize(150, sizePushButton + 5));

    pButtonSelectDotLine->setFixedHeight(sizePushButton);
    pButtonSelectDotLine->setFixedWidth(140);
    pButtonSelectDotLine->setIconSize(QSize(150, sizePushButton + 5));

    pButtonSelectDashDotLine->setFixedHeight(sizePushButton);
    pButtonSelectDashDotLine->setFixedWidth(140);
    pButtonSelectDashDotLine->setIconSize(QSize(150, sizePushButton + 5));

    pButtonSelectDashDotDotLine->setFixedHeight(sizePushButton);
    pButtonSelectDashDotDotLine->setFixedWidth(140);
    pButtonSelectDashDotDotLine->setIconSize(QSize(150, sizePushButton + 5));

    pButtonSelectWaveUnderline->setFixedHeight(sizePushButton);
    pButtonSelectWaveUnderline->setFixedWidth(140);
    pButtonSelectWaveUnderline->setIconSize(QSize(150, sizePushButton + 5));

    pButtonTextUnderlineStyle = new UButtonWithExtendedContextMenu();
    pButtonTextUnderlineStyle->setIcon(QIcon(IconUnderline));
    //    pButtonTextUnderlineStyle->setCheckable(true);
    pButtonTextUnderlineStyle->setFixedSize(sizePushButton,
                                            sizePushButton);

    pButtonTextUnderlineStyle->addWidgetToExtendedContextMenu(pButtonSelectSingleUnderline,
                                                              0, 0);
    pButtonTextUnderlineStyle->addWidgetToExtendedContextMenu(pButtonSelectDashUnderline,
                                                              1, 0);
    pButtonTextUnderlineStyle->addWidgetToExtendedContextMenu(pButtonSelectDotLine,
                                                              2, 0);
    pButtonTextUnderlineStyle->addWidgetToExtendedContextMenu(pButtonSelectDashDotLine,
                                                              3, 0);
    pButtonTextUnderlineStyle->addWidgetToExtendedContextMenu(pButtonSelectDashDotDotLine,
                                                              4, 0);
    pButtonTextUnderlineStyle->addWidgetToExtendedContextMenu(pButtonSelectWaveUnderline,
                                                              5, 0);
    pButtonTextUnderlineStyle->addWidgetToExtendedContextMenu(pButtonSelectNoUnderline,
                                                              6, 0);


    hBoxLayout->addWidget(pButtonTextUnderline);
    hBoxLayout->addWidget(pButtonTextStrikeOut);
    hBoxLayout->addWidget(pButtonTextOverline);
    //    hBoxLayout->addWidget(pButtonTextUnderlineStyle);
    hBoxLayout->addStretch();

    hBoxLayout->setSpacing(1);

    vBoxLayoutMain->addLayout(hBoxLayout);
}
void UFormatingPanelText::addPushButtonAlign()
{
    QHBoxLayout *hBoxLayout = new QHBoxLayout();

    //Make sure the alignLeft  is always left of the alignRight
    pButtonAlignLeft = new QPushButton(QIcon(IconAlignLeft),
                                       tr(""),
                                       this);
    pButtonAlignLeft->setCheckable(true);
    pButtonAlignLeft->setFixedSize(sizePushButton, sizePushButton);
    connect(pButtonAlignLeft, SIGNAL(clicked()),
            this, SLOT(clickPButtonAlignLeft()));


    pButtonAlignCenter = new QPushButton(QIcon(IconAlignCenter),
                                         tr(""),
                                         this);
    pButtonAlignCenter->setCheckable(true);
    pButtonAlignCenter->setFixedSize(sizePushButton, sizePushButton);
    connect(pButtonAlignCenter, SIGNAL(clicked()),
            this, SLOT(clickPButtonAlignCenter()));


    pButtonAlignRight = new QPushButton(QIcon(IconAlignRight),
                                        tr(""),
                                        this);
    pButtonAlignRight->setCheckable(true);
    pButtonAlignRight->setFixedSize(sizePushButton, sizePushButton);
    connect(pButtonAlignRight, SIGNAL(clicked()),
            this, SLOT(clickPButtonAlignRight()));


    pButtonAlignJustify = new QPushButton(QIcon(IconAlignJustify),
                                          tr(""),
                                          this);
    pButtonAlignJustify->setCheckable(true);
    pButtonAlignJustify->setFixedSize(sizePushButton, sizePushButton);
    connect(pButtonAlignJustify, SIGNAL(clicked()),
            this, SLOT(clickPButtonAlignJustify()));

    hBoxLayout->addWidget(pButtonAlignLeft);
    hBoxLayout->addWidget(pButtonAlignCenter);
    hBoxLayout->addWidget(pButtonAlignRight);
    hBoxLayout->addWidget(pButtonAlignJustify);

    hBoxLayout->setSpacing(1);
    hBoxLayout->addStretch();

    vBoxLayoutMain->addLayout(hBoxLayout);
}

void UFormatingPanelText::addPushButtonSelectCaseSensitive()
{   
    pButtonConvertUpperCaseSensitive    = new QPushButton();
    pButtonConvertLowerCaseSensitive    = new QPushButton();

    pButtonConvertUpperCaseSensitive->setFixedSize(sizePushButton * 2 + 10,
                                                   sizePushButton);
    pButtonConvertLowerCaseSensitive->setFixedSize(sizePushButton * 2 + 10,
                                                   sizePushButton);

    pButtonConvertUpperCaseSensitive->setIcon(QIcon(IconUpperCaseSensitive));
    pButtonConvertLowerCaseSensitive->setIcon(QIcon(IconLowerCaseSensitive));

    QSize iconSize = pButtonConvertUpperCaseSensitive->size();
    iconSize.setWidth(iconSize.width() - 2);
    iconSize.setHeight(iconSize.height() - 2);
    pButtonConvertUpperCaseSensitive->setIconSize(iconSize);

    iconSize = pButtonConvertLowerCaseSensitive->size();
    iconSize.setWidth(iconSize.width() - 2);
    iconSize.setHeight(iconSize.height() - 2);
    pButtonConvertLowerCaseSensitive->setIconSize(iconSize);

    connect(pButtonConvertUpperCaseSensitive, SIGNAL(clicked()),
            this, SLOT(clickPButtonUpperCaseSensitive()));
    connect(pButtonConvertLowerCaseSensitive, SIGNAL(clicked()),
            this, SLOT(clickPButtonLowerCaseSensitive()));

    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(pButtonConvertUpperCaseSensitive);
    hBoxLayout->addWidget(pButtonConvertLowerCaseSensitive);
    hBoxLayout->addStretch();
    hBoxLayout->addSpacing(0);

    vBoxLayoutMain->addLayout(hBoxLayout);
}
void UFormatingPanelText::addComboBox()
{
    comboFont = new QFontComboBox();

    connect(comboFont, SIGNAL(activated(QString)),
            this, SIGNAL(comboFamilySelectText(QString)));

    QHBoxLayout *hBoxLayout = new QHBoxLayout();

    hBoxLayout->addWidget(comboFont);
    hBoxLayout->addStretch();

    vBoxLayoutMain->addLayout(hBoxLayout);
}
void UFormatingPanelText::addComboBoxList()
{
    QSize iconSize;

    pButtonSelectDisc   = new QPushButton(this);
    pButtonSelectCircle = new QPushButton(this);
    pButtonSelectSquare = new QPushButton(this);

    //
    pButtonSelectDisc->setFixedSize(sizePButtonToExtendedContextMenu + 5,
                                    sizePButtonToExtendedContextMenu + 5);
    iconSize = pButtonSelectDisc->size();
    iconSize.setWidth(iconSize.width() - 7);
    iconSize.setHeight(iconSize.height() - 7);
    pButtonSelectDisc->setIconSize(iconSize);
    pButtonSelectDisc->setIcon(QIcon(IconFigureStyleListDisc));
    //
    pButtonSelectCircle->setFixedSize(sizePButtonToExtendedContextMenu + 5,
                                      sizePButtonToExtendedContextMenu + 5);
    iconSize = pButtonSelectCircle->size();
    iconSize.setWidth(iconSize.width() - 7);
    iconSize.setHeight(iconSize.height() - 7);
    pButtonSelectCircle->setIconSize(iconSize);
    pButtonSelectCircle->setIcon(QIcon(IconFigureStyleListCircle));

    //
    pButtonSelectSquare->setFixedSize(sizePButtonToExtendedContextMenu + 5,
                                      sizePButtonToExtendedContextMenu + 5);
    iconSize = pButtonSelectSquare->size();
    iconSize.setWidth(iconSize.width() - 7);
    iconSize.setHeight(iconSize.height() - 7);
    pButtonSelectSquare->setIconSize(iconSize);
    pButtonSelectSquare->setIcon(QIcon(IconFigureStyleListSquare));

    connect(pButtonSelectDisc, SIGNAL(clicked()),
            this, SLOT(clickPButtonSelectDisc()));
    connect(pButtonSelectCircle, SIGNAL(clicked()),
            this, SLOT(clickPButtonSelectCircle()));
    connect(pButtonSelectSquare, SIGNAL(clicked()),
            this, SLOT(clickPButtonSelectSquare()));


    // ORDERED
    pButtonSelectDecimal    = new QPushButton(this);
    pButtonSelectAlphaLower = new QPushButton(this);
    pButtonSelectAlphaUpper = new QPushButton(this);
    pButtonSelectRomanLower = new QPushButton(this);
    pButtonSelectRomanUpper = new QPushButton(this);

    //
    pButtonSelectDecimal->setFixedSize(sizePButtonToExtendedContextMenu + 5,
                                       sizePButtonToExtendedContextMenu + 5);
    iconSize = pButtonSelectDecimal->size();
    iconSize.setWidth(iconSize.width() - 7);
    iconSize.setHeight(iconSize.height() - 7);
    pButtonSelectDecimal->setIconSize(iconSize);
    pButtonSelectDecimal->setIcon(QIcon(IconOrderedStyleListDecimal));

    //
    pButtonSelectAlphaLower->setFixedSize(sizePButtonToExtendedContextMenu + 5,
                                          sizePButtonToExtendedContextMenu + 5);
    iconSize = pButtonSelectAlphaLower->size();
    iconSize.setWidth(iconSize.width() - 7);
    iconSize.setHeight(iconSize.height() - 7);
    pButtonSelectAlphaLower->setIconSize(iconSize);
    pButtonSelectAlphaLower->setIcon(QIcon(IconOrderedStyleListAlphaLower));

    //
    pButtonSelectAlphaUpper->setFixedSize(sizePButtonToExtendedContextMenu + 5,
                                          sizePButtonToExtendedContextMenu + 5);
    iconSize = pButtonSelectAlphaUpper->size();
    iconSize.setWidth(iconSize.width() - 7);
    iconSize.setHeight(iconSize.height() - 7);
    pButtonSelectAlphaUpper->setIconSize(iconSize);
    pButtonSelectAlphaUpper->setIcon(QIcon(IconOrderedStyleListAlphaUpper));

    //
    pButtonSelectRomanLower->setFixedSize(sizePButtonToExtendedContextMenu + 5,
                                          sizePButtonToExtendedContextMenu + 5);
    iconSize = pButtonSelectRomanLower->size();
    iconSize.setWidth(iconSize.width() - 7);
    iconSize.setHeight(iconSize.height() - 7);
    pButtonSelectRomanLower->setIconSize(iconSize);
    pButtonSelectRomanLower->setIcon(QIcon(IconOrderedStyleListRomanLower));

    //
    pButtonSelectRomanUpper->setFixedSize(sizePButtonToExtendedContextMenu + 5,
                                          sizePButtonToExtendedContextMenu + 5);
    iconSize = pButtonSelectRomanUpper->size();
    iconSize.setWidth(iconSize.width() - 7);
    iconSize.setHeight(iconSize.height() - 7);
    pButtonSelectRomanUpper->setIconSize(iconSize);
    pButtonSelectRomanUpper->setIcon(QIcon(IconOrderedStyleListRomanUpper));

    connect(pButtonSelectDecimal, SIGNAL(clicked()),
            this, SLOT(clickPButtonSelectDecimal()));
    connect(pButtonSelectAlphaLower, SIGNAL(clicked()),
            this, SLOT(clickPButtonSelectAlphaLower()));
    connect(pButtonSelectAlphaUpper, SIGNAL(clicked()),
            this, SLOT(clickPButtonSelectAlphaUpper()));
    connect(pButtonSelectRomanLower, SIGNAL(clicked()),
            this, SLOT(clickPButtonSelectRomanLower()));
    connect(pButtonSelectRomanUpper, SIGNAL(clicked()),
            this, SLOT(clickPButtonSelectRomanUpper()));


    pButtonFigureStyle  = new UButtonWithExtendedContextMenu();
    pButtonFigureStyle->setFixedSize(sizePushButton + 5,
                                     sizePushButton + 5);
    iconSize = pButtonFigureStyle->size();
    iconSize.setWidth(iconSize.width() - 7);
    iconSize.setHeight(iconSize.height() - 7);
    pButtonFigureStyle->setIconSize(iconSize);
    pButtonFigureStyle->setIcon(QIcon(IconFigureStyleList));

    pButtonFigureStyle->addWidgetToExtendedContextMenu(pButtonSelectDisc,
                                                       0, 0);
    pButtonFigureStyle->addWidgetToExtendedContextMenu(pButtonSelectCircle,
                                                       0, 1);
    pButtonFigureStyle->addWidgetToExtendedContextMenu(pButtonSelectSquare,
                                                       0, 2);

    pButtonOrderedStyle = new UButtonWithExtendedContextMenu();
    pButtonOrderedStyle->setFixedSize(sizePushButton + 5,
                                      sizePushButton + 5);
    iconSize = pButtonOrderedStyle->size();
    iconSize.setWidth(iconSize.width() - 7);
    iconSize.setHeight(iconSize.height() - 7);
    pButtonOrderedStyle->setIconSize(iconSize);
    pButtonOrderedStyle->setIcon(QIcon(IconOrderedStyleList));

    pButtonOrderedStyle->addWidgetToExtendedContextMenu(pButtonSelectDecimal,
                                                        0, 0);
    pButtonOrderedStyle->addWidgetToExtendedContextMenu(pButtonSelectAlphaLower,
                                                        1, 0);
    pButtonOrderedStyle->addWidgetToExtendedContextMenu(pButtonSelectAlphaUpper,
                                                        1, 1);
    pButtonOrderedStyle->addWidgetToExtendedContextMenu(pButtonSelectRomanLower,
                                                        2, 0);
    pButtonOrderedStyle->addWidgetToExtendedContextMenu(pButtonSelectRomanUpper,
                                                        2, 1);

    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(pButtonFigureStyle);
    hBoxLayout->addWidget(pButtonOrderedStyle);
    hBoxLayout->addStretch();

    vBoxLayoutMain->addLayout(hBoxLayout);
}

/// PUBLIC SLOTS
void UFormatingPanelText::clickPButtonAlignLeft()
{
    pButtonAlignLeft->setChecked(true);
    pButtonAlignCenter->setChecked(false);
    pButtonAlignRight->setChecked(false);
    pButtonAlignJustify->setChecked(false);
    emit selectAlignment(Qt::AlignLeft
                         | Qt::AlignAbsolute);
}
void UFormatingPanelText::clickPButtonAlignCenter()
{
    pButtonAlignLeft->setChecked(false);
    pButtonAlignCenter->setChecked(true);
    pButtonAlignRight->setChecked(false);
    pButtonAlignJustify->setChecked(false);
    emit selectAlignment(Qt::AlignHCenter);
}
void UFormatingPanelText::clickPButtonAlignRight()
{
    pButtonAlignLeft->setChecked(false);
    pButtonAlignCenter->setChecked(false);
    pButtonAlignRight->setChecked(true);
    pButtonAlignJustify->setChecked(false);
    emit selectAlignment(Qt::AlignRight
                         | Qt::AlignAbsolute);
}
void UFormatingPanelText::clickPButtonAlignJustify()
{
    pButtonAlignLeft->setChecked(false);
    pButtonAlignCenter->setChecked(false);
    pButtonAlignRight->setChecked(false);
    pButtonAlignJustify->setChecked(true);
    emit selectAlignment(Qt::AlignJustify);
}
void UFormatingPanelText::fontChanged(const QFont &font)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(font)
                                                   .family()));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(font
                                                                   .pointSize())));

    pButtonTextBold->setChecked(font.bold());
    pButtonTextItalic->setChecked(font.italic());

    pButtonTextUnderline->setChecked(font.underline());
    pButtonTextStrikeOut->setChecked(font.strikeOut());
    pButtonTextOverline->setChecked(font.overline());
}
void UFormatingPanelText::colorChanged(QColor color)
{
    QPixmap pix(IconTextColor);

    QPainter *painter = new QPainter();

    painter->begin(&pix);
    painter->setBrush(color);
    painter->setPen(color);
    painter->drawRect(0,
                      pix.height() - 25,
                      pix.width(),
                      pix.height() - 25);

    painter->end();
    pButtonTextColor->setIcon(pix);
    pButtonTextColor->setColor(color);
}
void UFormatingPanelText::alignmentChanged(Qt::Alignment align)
{
    pButtonAlignLeft->setChecked(false);
    pButtonAlignCenter->setChecked(false);
    pButtonAlignRight->setChecked(false);
    pButtonAlignJustify->setChecked(false);

    if(align & Qt::AlignLeft)
        pButtonAlignLeft->setChecked(true);

    else if(align & Qt::AlignHCenter)
        pButtonAlignCenter->setChecked(true);

    else if(align & Qt::AlignRight)
        pButtonAlignRight->setChecked(true);

    else if(align & Qt::AlignJustify)
        pButtonAlignJustify->setChecked(true);
}
void UFormatingPanelText::verticalAlignmentChanged(QTextCharFormat::VerticalAlignment vAlign)
{
    pButtonTextSubScript->setChecked(false);
    pButtonTextSuperScript->setChecked(false);

    if(vAlign & QTextCharFormat::AlignSubScript)
        pButtonTextSubScript->setChecked(true);

    else if(vAlign & QTextCharFormat::AlignSuperScript)
        pButtonTextSuperScript->setChecked(true);
}

void UFormatingPanelText::clickPButtonSubScript()
{
    QTextCharFormat::VerticalAlignment vAlign;

    if(pButtonTextSubScript->isChecked())
        vAlign = QTextCharFormat::AlignSubScript;
    else
        vAlign = QTextCharFormat::AlignNormal;

    emit selectTextVerticalAlignment(vAlign);
}
void UFormatingPanelText::clickPButtonSuperScript()
{
    QTextCharFormat::VerticalAlignment vAlign;

    if(pButtonTextSuperScript->isChecked())
        vAlign = QTextCharFormat::AlignSuperScript;
    else
        vAlign = QTextCharFormat::AlignNormal;

    emit selectTextVerticalAlignment(vAlign);
}

void UFormatingPanelText::setLanguage(QString pathToTheFileLanguage)
{
    currentLanguage = pathToTheFileLanguage;
    emit languageIsChange(currentLanguage);

    QSettings ini(pathToTheFileLanguage,
                       QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("FormatingPanel");

    setWindowTitle(ini.value("Title").toString());

    pButtonTextColor->setLanguage(pathToTheFileLanguage);

    // TOOL TIP
    pButtonTextBold->setToolTip(ini.value("ToolTipTextBold").toString());
    pButtonTextItalic->setToolTip(ini.value("ToolTipTextItalic").toString());

    pButtonClearTheTextFromFormatting->setToolTip(ini.value("ToolTipClearTheTextFromFormatting").toString());

    pButtonTextSubScript->setToolTip(ini.value("ToolTipTextSubScript").toString());
    pButtonTextSuperScript->setToolTip(ini.value("ToolTipTextSuperScript").toString());

    pButtonTextUnderline->setToolTip(ini.value("ToolTipTextUnderline").toString());
    pButtonTextOverline->setToolTip(ini.value("ToolTipTextOverline").toString());
    pButtonTextStrikeOut->setToolTip(ini.value("ToolTipTextStrikeOut").toString());

    pButtonAlignLeft->setToolTip(ini.value("ToolTipAlignLeft").toString());
    pButtonAlignCenter->setToolTip(ini.value("ToolTipAlignCenter").toString());
    pButtonAlignRight->setToolTip(ini.value("ToolTipAlignRight").toString());
    pButtonAlignJustify->setToolTip(ini.value("ToolTipAlignJustify").toString());

    pButtonTextColor->setToolTip(ini.value("ToolTipTextColor").toString());

    pButtonConvertUpperCaseSensitive->setToolTip(ini.value("ToolTipConvertUpperCaseSensitive").toString());
    pButtonConvertLowerCaseSensitive->setToolTip(ini.value("ToolTipConvertLowerCaseSensitive").toString());

    pButtonFigureStyle->setToolTip(ini.value("ToolTipFigureStyle").toString());
    pButtonOrderedStyle->setToolTip(ini.value("ToolTipOrderedStyle").toString());

    pButtonSelectDisc->setToolTip(ini.value("ToolTipSelectDisc").toString());
    pButtonSelectCircle->setToolTip(ini.value("ToolTipSelectCircle").toString());
    pButtonSelectSquare->setToolTip(ini.value("ToolTipSelectSquare").toString());

    pButtonSelectDecimal->setToolTip(ini.value("ToolTipSelectDecimal").toString());
    pButtonSelectAlphaLower->setToolTip(ini.value("ToolTipSelectAlphaLower").toString());
    pButtonSelectAlphaUpper->setToolTip(ini.value("ToolTipSelectAlphaUpper").toString());
    pButtonSelectRomanLower->setToolTip(ini.value("ToolTipSelectRomanLower").toString());
    pButtonSelectRomanUpper->setToolTip(ini.value("ToolTipSelectRomanUpper").toString());

    comboFont->setToolTip(ini.value("ToolTipFont").toString());

    comboSize->setToolTip(ini.value("ToolTipSize").toString());

    pButtonIncreaseFontSize->setToolTip(ini.value("ToolTipIncreaseFontSize").toString());
    pButtonDecreaseFontSize->setToolTip(ini.value("ToolTipDecreaseFontSize").toString());

    ini.endGroup();
}

/// PRIVATE SLOTS
void UFormatingPanelText::clickPButtonIncreaseFontSize()
{
    int indexLastSize = comboSize->count() - 1;

    int indexCurrentSize = comboSize->currentIndex();

    // если текущий индекс последний (самый большой),
    // то не сможем его плюсануть
    if(indexCurrentSize == indexLastSize)
        return;

    indexCurrentSize++;

    // получаем его размер
    QString size = comboSize->itemText(indexCurrentSize);

    emit selectFontSize(size);
}
void UFormatingPanelText::clickPButtonDecreaseFontSize()
{
    int indexFirstSize = 0;

    int indexCurrentSize = comboSize->currentIndex();

    // если текущий индекс первый (самый маленький),
    // то не сможем его минусануть
    if(indexCurrentSize == indexFirstSize)
        return;

    indexCurrentSize--;

    // получаем его размер
    QString size = comboSize->itemText(indexCurrentSize);

    emit selectFontSize(size);
}

void UFormatingPanelText::clickInsertSymbol()
{
//    if(!symbolTable)
//    {
//        symbolTable = new USymbolTable();
//        symbolTable->setLanguage(currentLanguage);

//        connect(this, SIGNAL(languageIsChange(QString)),
//                symbolTable, SLOT(setLanguage(QString)));
//        connect(symbolTable, SIGNAL(symbolIsSelected(QString)),
//                this, SIGNAL(insertSymbol(QString)));
//        connect(symbolTable, SIGNAL(iClose()),
//                this, SLOT(deleteSymbolTable()));
//        connect(this, SIGNAL(isHideMe()),
//                this, SLOT(deleteSymbolTable()));

//        symbolTable->resize(350, 350);
//        symbolTable->show();

//    }else
//        showForegroundWindow(symbolTable);
}

void UFormatingPanelText::deleteSymbolTable()
{
    if(symbolTable)
    {
        delete symbolTable;
        symbolTable = 0;
    }
}

/// PROTECTED
void UFormatingPanelText::closeEvent(QCloseEvent *event)
{
    event->ignore();
    hide();
    emit isHideMe();

    if(symbolTable)
        symbolTable->close();
}
