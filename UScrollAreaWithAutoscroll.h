#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QScrollBar>
#include <QScrollArea>
#include <QTimer>
#include <QSlider>
#include <QEvent>

class UScrollAreaWithAutoscroll: public QWidget
{
    Q_OBJECT
    
public:
    UScrollAreaWithAutoscroll(QWidget *parent = 0);
    void setWidget(QWidget *widget){ scrollArea->setWidget(widget); }
    void setWidgetResizable(bool bValue){ scrollArea->setWidgetResizable(bValue); }

private:
    int getIntervalToPositionSlider(int pos);

private:
    QScrollArea *scrollArea;
    QSlider *slider;
    QHBoxLayout *hBoxLayoutMain;
    QTimer *timer;

private slots:
    void timerTick();
    void changeCurrentPositionSlider(int pos);

protected:
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // WIDGET_H
