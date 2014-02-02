#include "UAnimatedSplashScreen.h"

/// PUBLIC
UAnimatedSplashScreen::UAnimatedSplashScreen(QWidget *parent,
                                             const QPixmap &pixmap,
                                             Qt::WindowFlags f):
    QSplashScreen(parent, pixmap, f)
{
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()),
            this, SLOT(nextPixmap()));

    setCursor(Qt::ArrowCursor);

    // костыль
    vBoxlayoutMain = new QVBoxLayout();
    labelMessage = new QLabel();

    vBoxlayoutMain->addSpacing(100);
    vBoxlayoutMain->addWidget(labelMessage,
                              0,
                              Qt::AlignCenter);

    setLayout(vBoxlayoutMain);
    // костыль
}

UAnimatedSplashScreen::UAnimatedSplashScreen(const QPixmap &pixmap,
                                             Qt::WindowFlags f):
    QSplashScreen(pixmap, f)
{
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()),
            this, SLOT(nextPixmap()));

    setCursor(Qt::ArrowCursor);

    // костыль
    vBoxlayoutMain = new QVBoxLayout();
    labelMessage = new QLabel();

    vBoxlayoutMain->addSpacing(60);
    vBoxlayoutMain->addWidget(labelMessage,
                              0,
                              Qt::AlignCenter);

    setLayout(vBoxlayoutMain);
    // костыль
}

void UAnimatedSplashScreen::setPixmap(const QList <QPixmap> &list)
{
    listPixmap = list;

    currentCountImage = 0;
    maxCountsImage    = listPixmap.size();
}

/// PUBLIC SLOTS
void UAnimatedSplashScreen::show()
{
    QSplashScreen::show();

    if(!listPixmap.isEmpty())
    {
        QPixmap pixmap = listPixmap.at(0);
        QSplashScreen::setPixmap(pixmap);

        timer->start();
    }else if(listPixmap.size() == 1)
    {
        QPixmap pixmap = listPixmap.at(0);
        QSplashScreen::setPixmap(pixmap);
    }
}

void UAnimatedSplashScreen::setMessage(QString message,
                                       QFont font,
                                       QColor color/* = Qt::black*/)
{
    qApp->processEvents();

    labelMessage->setText(message);
    labelMessage->setFont(font);

    //копируем имеющуюся палитру лейбла (вдруг нестандартная)
    QPalette palette(labelMessage->palette());
    palette.setColor(QPalette::WindowText,
                     color);
    labelMessage->setPalette(palette);
}

/// PRIVATE SLOTS
void UAnimatedSplashScreen::nextPixmap()
{
    if(!listPixmap.isEmpty())
    {
        currentCountImage++;

        if(currentCountImage == maxCountsImage)
            currentCountImage = 0;

        if(currentCountImage < maxCountsImage)
        {
            QPixmap pixmap = listPixmap.at(currentCountImage);
            QSplashScreen::setPixmap(pixmap);
        }
    }
}

/// PROTECTED
void UAnimatedSplashScreen::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        oldPosition = event->pos();
        buttonMousePress = true;
    }
}

void UAnimatedSplashScreen::mouseReleaseEvent(QMouseEvent *event)
{
    buttonMousePress = false;
}

void UAnimatedSplashScreen::mouseMoveEvent(QMouseEvent *event)
{
    if(buttonMousePress)
    {
        QPoint delta = event->pos() - oldPosition;
        move(pos() + delta);
    }
}
