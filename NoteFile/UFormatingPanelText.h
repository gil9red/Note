#ifndef UFORMATINGPANELTEXT_H
#define UFORMATINGPANELTEXT_H

#include <QtGui>
#include <QtCore>

#include "UOfficeWordStyleComboBox.h"
#include "UButtonsWithMenu/UButtonWithContextMenu.h"
#include "UButtonsWithMenu/UButtonWithExtendedContextMenu.h"
#include "UPushButtonSelectColor.h"
#include "USymbolTable.h"

#include "Globals.h"

class UFormatingPanelText: public QWidget
{
    Q_OBJECT

public:
    UFormatingPanelText(QWidget *parent = 0);

private:
    void addPushButtonChangeText();
    void addPushButtonUnderline();
    void addPushButtonAlign();    
    void addPushButtonSelectCaseSensitive();
    void addComboBox();
    void addComboBoxList();

private:    
    QFontComboBox *comboFont;
    UOfficeWordStyleComboBox *comboSize;

    QPushButton *pButtonIncreaseFontSize;
    QPushButton *pButtonDecreaseFontSize;

    // STYLE LIST    
    UButtonWithExtendedContextMenu *pButtonFigureStyle;
    QPushButton *pButtonSelectDisc;
    QPushButton *pButtonSelectCircle;
    QPushButton *pButtonSelectSquare;

    UButtonWithExtendedContextMenu *pButtonOrderedStyle;
    QPushButton *pButtonSelectDecimal;
    QPushButton *pButtonSelectAlphaLower;
    QPushButton *pButtonSelectAlphaUpper;
    QPushButton *pButtonSelectRomanLower;
    QPushButton *pButtonSelectRomanUpper;
    // STYLE LIST


    QPushButton *pButtonTextBold;
    QPushButton *pButtonTextItalic;

    QPushButton *pButtonTextSubScript;
    QPushButton *pButtonTextSuperScript;

    QPushButton *pButtonClearTheTextFromFormatting;

    QPushButton *pButtonInsertSymbol;

    QPushButton *pButtonTextUnderline;

    UButtonWithExtendedContextMenu *pButtonTextUnderlineStyle;
    QPushButton *pButtonSelectNoUnderline;
    QPushButton *pButtonSelectSingleUnderline;
    QPushButton *pButtonSelectDashUnderline;
    QPushButton *pButtonSelectDotLine;
    QPushButton *pButtonSelectDashDotLine;
    QPushButton *pButtonSelectDashDotDotLine;
    QPushButton *pButtonSelectWaveUnderline;

    QPushButton *pButtonTextStrikeOut;
    QPushButton *pButtonTextOverline;    

    QPushButton *pButtonAlignLeft;
    QPushButton *pButtonAlignCenter;
    QPushButton *pButtonAlignRight;
    QPushButton *pButtonAlignJustify;

    // COLOR TEXT
    UPushButtonSelectColor *pButtonTextColor;
    // COLOR TEXT

    QPushButton *pButtonConvertUpperCaseSensitive;
    QPushButton *pButtonConvertLowerCaseSensitive;

    QVBoxLayout *vBoxLayoutMain;

    QString currentLanguage;

    USymbolTable *symbolTable;


public slots:
    void clickPButtonAlignLeft();
    void clickPButtonAlignCenter();
    void clickPButtonAlignRight();
    void clickPButtonAlignJustify();

    void fontChanged(const QFont &font);
    void colorChanged(QColor color);
    void alignmentChanged(Qt::Alignment align);
    void verticalAlignmentChanged(QTextCharFormat::VerticalAlignment);

    void clickPButtonBold(){ emit textBold(pButtonTextBold->isChecked()); }
    void clickPButtonItalic(){ emit textItalic(pButtonTextItalic->isChecked()); }
    void clickPButtonClearTheTextFromFormatting(){ emit clearTheTextFromFormatting(); }

    void clickPButtonSubScript();
    void clickPButtonSuperScript();

    void clickPButtonUnderline(){ emit textUnderline(pButtonTextUnderline->isChecked()); }
    void clickPButtonStrikeOut(){ emit textStrikeOut(pButtonTextStrikeOut->isChecked()); }
    void clickPButtonOverline(){ emit textOverline(pButtonTextOverline->isChecked()); }
    void clickPButtonUpperCaseSensitive(){ emit textCaseSensitive(UpperCaseSensitive); }
    void clickPButtonLowerCaseSensitive(){ emit textCaseSensitive(LowerCaseSensitive); }

    void setLanguage(QString pathToTheFileLanguage);

private slots:    
    void clickPButtonIncreaseFontSize();
    void clickPButtonDecreaseFontSize();

    void clickPButtonSelectDisc()
    { emit styleSelectIndex(QTextListFormat::ListDisc); }

    void clickPButtonSelectCircle()
    { emit styleSelectIndex(QTextListFormat::ListCircle); }

    void clickPButtonSelectSquare()
    { emit styleSelectIndex(QTextListFormat::ListSquare); }

    void clickPButtonSelectDecimal()
    { emit styleSelectIndex(QTextListFormat::ListDecimal); }

    void clickPButtonSelectAlphaLower()
    { emit styleSelectIndex(QTextListFormat::ListLowerAlpha); }

    void clickPButtonSelectAlphaUpper()
    { emit styleSelectIndex(QTextListFormat::ListUpperAlpha); }

    void clickPButtonSelectRomanLower()
    { emit styleSelectIndex(QTextListFormat::ListLowerRoman); }

    void clickPButtonSelectRomanUpper()
    { emit styleSelectIndex(QTextListFormat::ListUpperRoman); }

    void clickInsertSymbol();

    void deleteSymbolTable();

signals:
    void textBold(bool checked);
    void textItalic(bool checked);

    void clearTheTextFromFormatting();

    void selectTextVerticalAlignment(QTextCharFormat::VerticalAlignment);

    void textUnderline(bool checked);
    void textStrikeOut(bool checked);
    void textOverline(bool checked);

    void textColor(QColor);
    void textCaseSensitive(UTypesOfCaseSensitive);
    void selectAlignment(Qt::Alignment);

    void styleSelectIndex(QTextListFormat::Style);
    void comboFamilySelectText(QString);
    void selectFontSize(QString);

    void insertSymbol(QString);

    void isHideMe();

    void focusIn();
    void focusOut();

    void languageIsChange(QString);

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // UFORMATINGPANELTEXT_H
