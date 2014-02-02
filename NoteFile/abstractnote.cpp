#include "abstractnote.h"

/// PUBLIC
AbstractNote::AbstractNote(QWidget *parent):
    QWidget(parent)
{    
    setWindowFlags(Qt::Tool
                   | Qt::FramelessWindowHint);

    setMinimumSize(minimalWidthNote,
                   minimalHeightNote);

    setMouseTracking(true);

    flagButtonMousePress = false;
    minimizeAbstractNote = false;

    countMinimize = 0;

    colorBackground = Qt::gray;
    setColorBackground(colorBackground);

#ifdef Q_WS_WIN
    // ���� � ���� =)
    SetClassLong(winId(),
                 GCL_STYLE,
                 GetClassLong(winId(), GCL_STYLE)
                 | 0x00020000);
#endif
}

/// PUBLIC SLOTS
void AbstractNote::setZPosition(QString zPosition)
{
    if(zPosition == UzPosition::AlwaysOnBottom)
    {
        setWindowFlags(Qt::Tool
                       | Qt::FramelessWindowHint
                       | Qt::WindowStaysOnBottomHint);
        show();

    }else
    {
        setWindowFlags(Qt::Tool
                       | Qt::FramelessWindowHint
                       | Qt::WindowStaysOnTopHint);
        show();
    }
}

void AbstractNote::setMinimize(bool bValue)
{
    minimizeAbstractNote = bValue;

    if(minimizeAbstractNote)
    {
        countMinimize++;
        // ���������� ��������
        oldHeight = height();
        // ��������� ������
        setFixedHeight(topBorder);

    }else if(!minimizeAbstractNote
             && countMinimize)
    {
        // ������� ��������
        setFixedHeight(QWIDGETSIZE_MAX);

        setMinimumSize(minimalWidthNote,
                       minimalHeightNote);
        // ���������� ������ ������
        resize(width(),
               oldHeight);
    }
}

void AbstractNote::setColorBackground(QColor color)
{
    colorBackground = color;
    update();
}

void AbstractNote::setImageBackground(QImage image)
{
    imageBackground = image;
    update();
}

/// PROTECTED
void AbstractNote::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        oldPosition = event->pos();
        flagButtonMousePress = true;

        if(event->pos().x() >= width() - rightBorder
                && event->pos().y() >= height() - bottomBorder)
        {
            borderWindow = BottomRight;
            setCursor(Qt::SizeAllCursor);

        }else if(event->pos().y() <= topBorder)
        {
            borderWindow = Top;
            setCursor(Qt::ArrowCursor);

        }else
        {
            borderWindow = None;
            setCursor(Qt::ArrowCursor);
        }
        repaint();
    }
}

void AbstractNote::mouseReleaseEvent(QMouseEvent *event)
{
    flagButtonMousePress = false;

    setFocus();
}

void AbstractNote::mouseMoveEvent(QMouseEvent *event)
{
    if(event->pos().y() <= topBorder
            && !minimizeAbstractNote)    
        setCursor(Qt::ArrowCursor);

    else if(event->pos().x() >= width() - rightBorder
             && event->pos().y() >= height() - bottomBorder)

        setCursor(Qt::SizeAllCursor);
    else
        setCursor(Qt::ArrowCursor);    

    if(flagButtonMousePress)
    {
        QRect R = geometry();
        QPoint delta = event->pos() - oldPosition;

        if(borderWindow == Top)        
            move(pos() + delta);

        if(borderWindow == BottomRight)
        {
            R.setBottom(R.bottom() + delta.y());
            R.setRight(R.right() + delta.x());
            setGeometry(R);
            oldPosition = event->pos();
        }
    }
}

void AbstractNote::paintEvent(QPaintEvent *)
{
    QPainter painter;

    painter.begin(this);

    // ���� ����������� �������
    painter.setBrush(colorBackground);
    painter.setPen(colorBackground);
    painter.drawRect(rect());
    // ���� ����������� �������


    // ���� ��������� ����������� �������    
    int h, s, v;
    colorBackground.getHsv(&h, &s, &v);
    v -= 20;
    if(v < 0)
        v = 0;

    QColor colorTitle = colorBackground;
    colorTitle.setHsv(h, s, v);

    painter.setBrush(colorTitle);
    painter.setPen(colorTitle);
    painter.drawRect(0,
                     0,
                     width(),
                     topBorder - 2);
    // ���� ��������� ����������� �������

    // ���� ���� ��������� �������
    // �������� ����������� � ��������� ������
    QImage image = QImage(rightBorder - 1,
                          bottomBorder - 1,
                          QImage::Format_ARGB32);

    QColor colorAngle;

    if(minimizeAbstractNote)
        colorAngle = colorTitle;
    else
        colorAngle = colorBackground;

    // �������� �� �����������,
    // ����: ���� ����������� �������
    QPainter painterImage;
    painterImage.begin(&image);
    painterImage.setBrush(colorAngle);
    painterImage.setPen(QPen(colorAngle));

    painterImage.drawRect(image.rect());

    // ����������� ������� (������ ������ ����� � �.�.)
    image.invertPixels(QImage::InvertRgb);

    // �������� ������������ �����
    painterImage.setPen(QPen(QBrush(colorAngle),
                             1.4));
    for(int i = rightBorder / 4; i < rightBorder / 2; i++)
        painterImage.drawLine(0,
                              i * 4,
                              i * 4,
                              0);
    painterImage.end();

    // ����������� ������� (������ ������ ����� � �.�.)
    image.invertPixels(QImage::InvertRgb);

    // �������� �������� �� ����������� �������
    painter.drawImage(width() - rightBorder,
                      height() - bottomBorder,
                      image);
    // ���� ���� ��������� �������

    painter.end();
}
