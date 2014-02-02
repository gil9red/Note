#include "ULineEditWithButtons.h"

/// PUBLIC
ULineEditWithButtons::ULineEditWithButtons(QWidget *parent /*= 0*/)
    : QLineEdit(parent),
      d_spacing(4)
{
    createGUI();
}

void ULineEditWithButtons::addButton(QAbstractButton *button)
{
    button->setParent(this);
    button->setCursor(Qt::ArrowCursor);
    button->setStyleSheet("QToolButton { border: none; padding: 0px; }");

    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);

    listButtons.append(button);

    int padding = frameWidth + 1;

    for(int i = 0; i < listButtons.count(); i++)
        padding += listButtons.at(i)->sizeHint().width();

    setPadding(padding);
}


/// PRIVATE:
void ULineEditWithButtons::createGUI()
{
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    int height = 16 + frameWidth * 2 + 2;

    setMinimumSize(qMax(minimumSizeHint().width(),
                        height),

                   qMax(minimumSizeHint().height(),
                        height));
}

/// PUBLIC SLOTS
void ULineEditWithButtons::refreshMetrics()
{
    if(listButtons.isEmpty())
        return;

    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);

    int lastWidth = listButtons[0]->sizeHint().width();
    int indent = rect().right() - frameWidth;

    for(int i = 0; i < listButtons.count(); i++)
    {
        // не учитываем кнопку при метрике, если она невидима
        if(listButtons[i]->isHidden())
            continue;

        int x = indent - lastWidth;
        int y = (rect().bottom() + 1 - listButtons[i]->sizeHint().height()) / 2;

        listButtons[i]->move(x, y);

        lastWidth = listButtons[i]->sizeHint().width();
        indent -= lastWidth - d_spacing;
    }
}

/// PROTECTED
void ULineEditWithButtons::resizeEvent(QResizeEvent *)
{
    refreshMetrics();
}

