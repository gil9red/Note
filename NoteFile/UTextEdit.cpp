#include "UTextEdit.h"

/// PUBLIC
UTextEdit::UTextEdit(QWidget *parent):
    QTextEdit(parent),
    c(0),
    autocomplete(true)
{
    setContextMenuPolicy(Qt::NoContextMenu);

    connect(this, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SLOT(currentCharFormatChanged(QTextCharFormat)));
    connect(this, SIGNAL(cursorPositionChanged()),
            this, SLOT(cursorPositionChanged()));
}

void UTextEdit::setConnectWithFormatingPanelText(UFormatingPanelText *formatingPanelText)
{
    // if formatingPanelText is create
    if(formatingPanelText)
    {
        connect(formatingPanelText, SIGNAL(textBold(bool)),
                this, SLOT(textBold(bool)));
        connect(formatingPanelText, SIGNAL(textItalic(bool)),
                this, SLOT(textItalic(bool)));

        connect(formatingPanelText, SIGNAL(clearTheTextFromFormatting()),
                this, SLOT(clearTheTextFromFormatting()));

        connect(formatingPanelText, SIGNAL(selectTextVerticalAlignment(QTextCharFormat::VerticalAlignment)),
                this, SLOT(textVerticalAlignment(QTextCharFormat::VerticalAlignment)));

        connect(formatingPanelText, SIGNAL(textUnderline(bool)),
                this, SLOT(textUnderline(bool)));
        connect(formatingPanelText, SIGNAL(textStrikeOut(bool)),
                this, SLOT(textStrikeOut(bool)));
        connect(formatingPanelText, SIGNAL(textOverline(bool)),
                this, SLOT(textOverline(bool)));

        connect(formatingPanelText, SIGNAL(textColor(QColor)),
                this, SLOT(textColorEdit(QColor)));

        connect(formatingPanelText, SIGNAL(textCaseSensitive(UTypesOfCaseSensitive)),
                this, SLOT(textCaseSensitiveEdit(UTypesOfCaseSensitive)));

        connect(formatingPanelText, SIGNAL(comboFamilySelectText(QString)),
                this, SLOT(textFamily(QString)));
        connect(formatingPanelText, SIGNAL(selectFontSize(QString)),
                this, SLOT(textSize(QString)));
        connect(formatingPanelText, SIGNAL(styleSelectIndex(QTextListFormat::Style)),
                this, SLOT(textStyle(QTextListFormat::Style)));

        connect(formatingPanelText, SIGNAL(selectAlignment(Qt::Alignment)),
                this, SLOT(textAlign(Qt::Alignment)));

        connect(formatingPanelText, SIGNAL(insertSymbol(QString)),
                this, SLOT(insertSymbol(QString)));

        // signals from text edit
        connect(this, SIGNAL(fontChanged(QFont)),
                formatingPanelText, SLOT(fontChanged(QFont)));
        connect(this, SIGNAL(colorChanged(QColor)),
                formatingPanelText, SLOT(colorChanged(QColor)));
        connect(this, SIGNAL(alignmentChanged(Qt::Alignment)),
                formatingPanelText, SLOT(alignmentChanged(Qt::Alignment)));

        connect(this, SIGNAL(verticalAlignmentChanged(QTextCharFormat::VerticalAlignment)),
                formatingPanelText, SLOT(verticalAlignmentChanged(QTextCharFormat::VerticalAlignment)));

        emit fontChanged(font());
        emit colorChanged(textColor());
        emit alignmentChanged(alignment());
        emit verticalAlignmentChanged(currentCharFormat().verticalAlignment());
    }
}

void UTextEdit::setCompleter(QCompleter *completer)
{
    if(c)
        QObject::disconnect(c, 0, this, 0);

    c = completer;

    if(!c)
        return;

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(c, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));
}

/// PRIVATE
QString UTextEdit::makeCaseSensitiveNegative(QString string)
{
    for(int i = 0; i < string.size(); i++)
        if(string[i].isLower())
            string[i] = string[i].toUpper();
        else
            string[i] = string[i].toLower();

    return string;
}

void UTextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textCursor();

    if(!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);    

    cursor.mergeCharFormat(format);
    mergeCurrentCharFormat(format);
}

QString UTextEdit::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);

    return tc.selectedText();
}

/// PUBLIC SLOTS
void UTextEdit::setVisibleScrollBar(bool visible)
{
    if(visible)
    {
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }else
    {
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}

/// PRIVATE SLOTS
void UTextEdit::textBold(bool checked)
{
    QTextCharFormat fmt;

    QFont::Weight weight;

    if(checked)
        weight = QFont::Bold;
    else
        weight = QFont::Normal;

    fmt.setFontWeight(weight);

    mergeFormatOnWordOrSelection(fmt);
}

void UTextEdit::textItalic(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontItalic(checked);
    mergeFormatOnWordOrSelection(fmt);
}

void UTextEdit::clearTheTextFromFormatting()
{
    // получаем не форматированный текст
    QString plainText = textCursor().selectedText();

    // заменяем форматированный
    textCursor().insertHtml(plainText);
}

void UTextEdit::textVerticalAlignment(QTextCharFormat::VerticalAlignment vAlign)
{
    QTextCharFormat fmt;

    fmt.setVerticalAlignment(vAlign);

    mergeFormatOnWordOrSelection(fmt);
}

void UTextEdit::textUnderline(bool checked)
{
    QTextCharFormat fmt;

    QTextCharFormat::UnderlineStyle selectStyle;

    if(checked)
        selectStyle = QTextCharFormat::SingleUnderline;
    else
        selectStyle = QTextCharFormat::NoUnderline;

    fmt.setUnderlineStyle(selectStyle);

    mergeFormatOnWordOrSelection(fmt);
}

void UTextEdit::textStrikeOut(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontStrikeOut(checked);
    mergeFormatOnWordOrSelection(fmt);
}
void UTextEdit::textOverline(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontOverline(checked);
    mergeFormatOnWordOrSelection(fmt);
}

void UTextEdit::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void UTextEdit::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();

    if(p.toFloat() > 0)
    {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}

void UTextEdit::textStyle(QTextListFormat::Style style)
{
    QTextCursor cursor = textCursor();

    if(style != 0)
    {        
        cursor.beginEditBlock();

//! код ниже является кодом из примера
        QTextBlockFormat blockFmt = cursor.blockFormat();

        QTextListFormat listFmt;

        if(cursor.currentList())        
            listFmt = cursor.currentList()->format();
        else
        {
            listFmt.setIndent(blockFmt.indent() + 1);
            blockFmt.setIndent(0);
            cursor.setBlockFormat(blockFmt);
        }

        listFmt.setStyle(style);        

        cursor.createList(listFmt);        

        cursor.endEditBlock();
    }else
    {
        QTextBlockFormat bfmt;
        bfmt.setObjectIndex(-1);
        cursor.mergeBlockFormat(bfmt);
    }
}


void UTextEdit::textColorEdit(QColor color)
{
    QTextCharFormat fmt;
    fmt.setForeground(color);
    mergeFormatOnWordOrSelection(fmt);    
}

void UTextEdit::textCaseSensitiveEdit(UTypesOfCaseSensitive typeOfCaseSensitive)
{
    QTextCursor cursor = textCursor();

    int startPosition = cursor.selectionStart();
    int endPosition   = cursor.selectionEnd();
    int cursorStart   = 0;
    int cursorEnd     = 0;
    //определяем блок по позиции курсора
    QTextBlock block = document()->findBlock(startPosition);

    while(true)
    {
        //если блок не существует
        if(!block.isValid())
            break;
        //если нач позиция первого символа > endPosition
        if(block.position() > endPosition)
            break;

        QTextBlock::iterator it;

        //перебираем фрагменты - это кусок текста с одинаковым форматированием
        for(it = block.begin(); !(it.atEnd()); ++it)
        {
            QTextFragment currentFragment = it.fragment();
            if (!currentFragment.isValid())
                continue;
            //длина фрагменты - количество символов
            int fragmentLength = currentFragment.length();
            //начальная позиция
            int fragmentStart  = currentFragment.position();
            //конечная
            int fragmentEnd    = fragmentStart + fragmentLength;
            if (endPosition < fragmentStart || startPosition > fragmentEnd)
                continue;
            //получаем курсор на документ
            //QTextEdit - это представление. Данные/цвет/позиции курсора/шрифт,
            //всё это хранится в QDocument - модели.
            QTextCursor temp(document());
            //определяем позиции
            if(startPosition < fragmentStart)
                cursorStart = fragmentStart;
            else
                cursorStart = startPosition;

            if(endPosition < fragmentEnd)
                cursorEnd = endPosition;
            else
                cursorEnd = fragmentEnd;

            //устанавливаем курсор
            temp.setPosition(cursorStart);
            //сдвигаем курсор и выделяем символы
            temp.movePosition(QTextCursor::Right,
                              QTextCursor::KeepAnchor,
                              cursorEnd - cursorStart);
            //получаем выделенные символы
            QString text = temp.selectedText();
            //взависимости от выбора, меняем регистр у текущего
            //фрагмента
            if(typeOfCaseSensitive == UpperCaseSensitive)
                text = makeCaseSensitiveUpper(text);

            else if(typeOfCaseSensitive == LowerCaseSensitive)
                text = makeCaseSensitiveLower(text);

            else if(typeOfCaseSensitive == NegativeCaseSensitive)
                text = makeCaseSensitiveNegative(text);

            temp.beginEditBlock();
            //вставка заменит выделенные текст на тот
            //который вставляем
            temp.insertText(text);

            temp.endEditBlock();
        }
        //смотрим следующий блок
        block = block.next();
    }

    // восстанавливаем выделение текста
    QTextCursor tc = textCursor();
    tc.setPosition(startPosition);
    tc.movePosition(QTextCursor::Right,
                    QTextCursor::KeepAnchor,
                    endPosition - startPosition);
    setTextCursor(tc);
}

void UTextEdit::currentCharFormatChanged(const QTextCharFormat format)
{
    emit fontChanged(format.font());
    emit colorChanged(format.foreground().color());
    emit colorBackgroundChanged(format.background().color());
    emit verticalAlignmentChanged(format.verticalAlignment());
}

void UTextEdit::insertCompletion(const QString& completion)
{
    if (c->widget() != this)
        return;

    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();

    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);

    tc.insertText(completion.right(extra));

    setTextCursor(tc);
}

/// PROTECTED
void UTextEdit::wheelEvent(QWheelEvent *event)
{
    QTextEdit::wheelEvent(event);

    if (event->modifiers() == Qt::ALT)
    {
        int numDegrees = event->delta() / 8;
        int numSteps = numDegrees / 15;

        if(numSteps)
            zoomIn(numSteps);
        else
            zoomOut(numSteps);
    }   
}

void UTextEdit::focusInEvent(QFocusEvent *e)
{
    if (c)
        c->setWidget(this);

    QTextEdit::focusInEvent(e);
}

void UTextEdit::keyPressEvent(QKeyEvent *e)
{
    if (c && c->popup()->isVisible())
    {
        // The following keys are forwarded by the completer to the widget
        switch (e->key())
        {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            e->ignore();
            // let the completer do default behavior
            return;

        default:
            break;
        }
    }

    // CTRL+SPACE
    bool isShortcut = ((e->modifiers() & Qt::ControlModifier)
                       && e->key() == Qt::Key_Space);
    // do not process the shortcut when we have a completer

    if(!c || !isShortcut)
        QTextEdit::keyPressEvent(e);

    // если автодополнение разрешено
    if(autocomplete)
    {
        const bool ctrlOrShift = e->modifiers()
                & (Qt::ControlModifier | Qt::ShiftModifier);

        if(!c || (ctrlOrShift && e->text().isEmpty()))
            return;

        // end of word
        static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-=");

        bool hasModifier = (e->modifiers() != Qt::NoModifier)
                && !ctrlOrShift;

        QString completionPrefix = textUnderCursor();

        if(!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
                           || eow.contains(e->text().right(1))))
        {
            c->popup()->hide();
            return;
        }

        if(completionPrefix != c->completionPrefix())
        {
            c->setCompletionPrefix(completionPrefix);
            c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
        }
        QRect cr = cursorRect();
        cr.setWidth(c->popup()->sizeHintForColumn(0)
                    + c->popup()->verticalScrollBar()->sizeHint().width());
        // popup it up!
        c->complete(cr);
    }
}
