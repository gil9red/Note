#include "UScrollAreaWithAutoscroll.h"

/// PUBLIC
UScrollAreaWithAutoscroll::UScrollAreaWithAutoscroll(QWidget *parent):
    QWidget(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(timerTick()));

    scrollArea = new QScrollArea();
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->verticalScrollBar()->installEventFilter(this);

    slider = new QSlider();
    slider->setRange(0, 2);
    slider->setSliderPosition(0);

    slider->setVisible(scrollArea->verticalScrollBar()->isVisible());

    connect(slider, SIGNAL(valueChanged(int)),
            this, SLOT(changeCurrentPositionSlider(int)));

    changeCurrentPositionSlider(slider->sliderPosition());

    QVBoxLayout *vBoxLayoutSlider = new QVBoxLayout();
    vBoxLayoutSlider->addStretch();
    vBoxLayoutSlider->addWidget(slider);
    vBoxLayoutSlider->addStretch();

    hBoxLayoutMain = new QHBoxLayout();
    hBoxLayoutMain->addWidget(scrollArea);
    hBoxLayoutMain->addLayout(vBoxLayoutSlider);

    setLayout(hBoxLayoutMain);
}

/// PRIVATE
int UScrollAreaWithAutoscroll::getIntervalToPositionSlider(int pos)
{
    if(pos == 0
            || pos == 2)
        return 35;

//    default return value
    return 35;
}

/// PRIVATE SLOTS
void UScrollAreaWithAutoscroll::timerTick()
{
    QScrollBar *scrollBar = scrollArea->verticalScrollBar();

    if(slider->sliderPosition() > slider->maximum() / 2
            && slider->sliderPosition() <= slider->maximum())
        scrollBar->setSliderPosition(scrollBar->sliderPosition() - 1);

    else if(slider->sliderPosition() >= slider->minimum()
             && slider->sliderPosition() < slider->maximum() / 2)    
        scrollBar->setSliderPosition(scrollBar->sliderPosition() + 1);    
}

void UScrollAreaWithAutoscroll::changeCurrentPositionSlider(int pos)
{
    if(pos == slider->maximum() / 2)    
        timer->stop();

    else if(!timer->isActive())
    {
        int interval = getIntervalToPositionSlider(pos);
        timer->start(interval);

    }else if(timer->isActive())
    {
        int interval = getIntervalToPositionSlider(pos);
        timer->setInterval(interval);
    }
}

/// PROTECTED
bool UScrollAreaWithAutoscroll::eventFilter(QObject *object, QEvent *event)
{
    // если событие пришло от вертикального скролла
    if(object == scrollArea->verticalScrollBar())
    {
        // если он показывается
        if(event->type() == QEvent::Show)
        {
            // покажем и ползунок
            slider->show();
            return false;

        // если он прячется
        }else if(event->type() == QEvent::Hide)
        {
            // прячем и ползунок
            slider->hide();
            return false;

            // если выше ни одно условие не сработало - душим событие
        }else        
            return false;        
    }

    // душим событие, нет смысла давать ему дальше расходиться
    return false;
}
