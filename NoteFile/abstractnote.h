#ifndef ABSTRACTNOTE_H
#define ABSTRACTNOTE_H

#include <QtGui>
#include <QtCore>

#ifdef Q_WS_WIN
#include <windows.h>
#endif

#include "Globals.h"

class AbstractNote: public QWidget
{
    Q_OBJECT

public:
    AbstractNote(QWidget *parent = 0);

public:    
    QColor colorBackground;

private:
    enum Border
    {
        None,
        Top,
        BottomRight
    } borderWindow;

    QPoint oldPosition;

    bool flagButtonMousePress;

    QImage imageBackground;

    // minimize
    bool minimizeAbstractNote;
    int oldHeight;
    int countMinimize;
    // minimize

public slots:    
    void setZPosition(QString zPosition);
    void setMinimize(bool bValue);
    void setColorBackground(QColor color);
    void setImageBackground(QImage image);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);
};

#endif // ABSTRACTNOTE_H
