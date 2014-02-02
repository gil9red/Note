#ifndef TEXTEDIT_H
#define UTEXTEDIT_H

#include <QtGui>
#include <QtCore>

#include "UFormatingPanelText.h"

class UTextEdit: public QTextEdit
{
    Q_OBJECT

public:
    UTextEdit(QWidget *parent = 0);
    int getCountTextSymbols(){ return toPlainText().count() - document()->lineCount() + 1; }
    int getCountTextLines(){ return document()->lineCount(); }
    void setConnectWithFormatingPanelText(UFormatingPanelText *formatingPanelText);

    // completer
    void setCompleter(QCompleter *c);
    QCompleter * completer() const { return c; }
    // completer

private:
    QCompleter *c;
    bool autocomplete;

private:
    QString makeCaseSensitiveUpper(QString string){ return string.toUpper(); }
    QString makeCaseSensitiveLower(QString string){ return string.toLower(); }
    QString makeCaseSensitiveNegative(QString string);
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);

    // completer
    QString textUnderCursor() const;
    // completer

public slots:
    void setAutocomplete(bool bValue){ autocomplete = bValue; }
    void setVisibleScrollBar(bool visible);

private slots:
    void textBold(bool checked);
    void textItalic(bool checked);

    void clearTheTextFromFormatting();

    void insertSymbol(QString symbol){ textCursor().insertHtml(symbol); }

    void textVerticalAlignment(QTextCharFormat::VerticalAlignment vAlign);

    void textUnderline(bool checked);
    void textStrikeOut(bool checked);
    void textOverline(bool checked);

    void textFamily(const QString &f);
    void textSize(const QString &p);
    void textStyle(QTextListFormat::Style style);
    void textColorEdit(QColor color);
    void textCaseSensitiveEdit(UTypesOfCaseSensitive typeOfCaseSensitive);
    void textAlign(Qt::Alignment align){ setAlignment(align); }

    void currentCharFormatChanged(const QTextCharFormat format);
    void cursorPositionChanged(){ emit alignmentChanged(alignment()); }

    // completer
    void insertCompletion(const QString &completion);
    // completer

signals:
    void fontChanged(const QFont&);
    void colorChanged(QColor);
    void colorBackgroundChanged(const QColor &);
    void alignmentChanged(Qt::Alignment);
    void verticalAlignmentChanged(QTextCharFormat::VerticalAlignment);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *){}
    void wheelEvent(QWheelEvent *event);

    // completer
    void keyPressEvent(QKeyEvent *e);

public:
    void focusInEvent(QFocusEvent *e);
    // completer

    void focusOutEvent(QFocusEvent *e){ QTextEdit::focusOutEvent(e); }
};

#endif
