#ifndef UANIMATEDSPLASHSCREEN_H
#define UANIMATEDSPLASHSCREEN_H

#include <QtGui>
#include <QtCore>

class UAnimatedSplashScreen: public QSplashScreen
{
    Q_OBJECT

public:
    UAnimatedSplashScreen(QWidget *parent,
                          const QPixmap &pixmap = QPixmap(),
                          Qt::WindowFlags f = 0);
    UAnimatedSplashScreen(const QPixmap &pixmap = QPixmap(),
                          Qt::WindowFlags f = 0);

    void setPixmap(const QList <QPixmap> &list);
    void setPixmap(QPixmap &pixmap)
    { QSplashScreen::setPixmap(pixmap); }

private:
    QList <QPixmap> listPixmap;
    QTimer *timer;
    int currentCountImage;
    int maxCountsImage;

    QVBoxLayout *vBoxlayoutMain;
    QLabel *labelMessage;

    QPoint oldPosition;
    bool buttonMousePress;

public slots:
    void show();
    void setIntervalAnimation(int msec)
    { timer->setInterval(msec); }

    // костыль
    void setMessage(QString message,
                    QFont font,
                    QColor color = Qt::black);

private slots:
    void nextPixmap();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // UANIMATEDSPLASHSCREEN_H
