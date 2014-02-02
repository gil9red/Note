#ifndef UPUSHBUTTONSELECTCOLOR_H
#define UPUSHBUTTONSELECTCOLOR_H

#include <QtGui>
#include <QtCore>

#include "Globals.h"

class UPushButtonSelectColor: public QPushButton
{
    Q_OBJECT

public:    
    UPushButtonSelectColor(QWidget *parent = 0)
        : QPushButton(parent)
    { init(); }

    UPushButtonSelectColor(const QString &text,
                           QWidget *parent = 0)
        : QPushButton(text, parent)
    { init(); }

    UPushButtonSelectColor(const QIcon &icon,
                           const QString &text,
                           QWidget *parent = 0)
        : QPushButton(icon, text, parent)
    { init(); }

    QMenu * menuColors()
    { return popurMenuTextColor; }

    QList <QAction *> actionsColors()
    { return popurMenuTextColor->actions(); }

private:
    void init();
    void createActionsSelectColor();

private:
    QMenu *popurMenuTextColor;
    QAction *actionTextColor_White;
    QAction *actionTextColor_Black;

    QAction *actionTextColor_Red;
    QAction *actionTextColor_DarkRed;

    QAction *actionTextColor_Green;
    QAction *actionTextColor_DarkGreen;

    QAction *actionTextColor_Blue;
    QAction *actionTextColor_DarkBlue;

    QAction *actionTextColor_Cyan;
    QAction *actionTextColor_DarkCyan;

    QAction *actionTextColor_Magenta;
    QAction *actionTextColor_DarkMagenta;

    QAction *actionTextColor_Yellow;
    QAction *actionTextColor_DarkYellow;

    QAction *actionTextColor_Gray;
    QAction *actionTextColor_DarkGray;
    QAction *actionTextColor_LightGray;

    QAction *actionTextColor_Other;

    QColor color;

public slots:
    void setLanguage(QString pathToTheFileLanguage);

    void black()
    { selectColor(Qt::black); }

    void white()
    { selectColor(Qt::white); }

    void gray()
    { selectColor(Qt::gray); }

    void red()
    { selectColor(Qt::red); }

    void green()
    { selectColor(Qt::green); }

    void cyan()
    { selectColor(Qt::cyan); }

    void blue()
    { selectColor(Qt::blue); }

    void magenta()
    { selectColor(Qt::magenta); }

    void yellow()
    { selectColor(Qt::yellow); }

    void darkGray()
    { selectColor(Qt::darkGray); }

    void darkRed()
    { selectColor(Qt::darkRed); }

    void darkGreen()
    { selectColor(Qt::darkGreen); }

    void darkCyan()
    { selectColor(Qt::darkCyan); }

    void darkBlue()
    { selectColor(Qt::darkBlue); }

    void darkMagenta()
    { selectColor(Qt::darkMagenta); }

    void darkYellow()
    { selectColor(Qt::darkYellow); }

    void lightGray()
    { selectColor(Qt::lightGray); }

    void colorDialog();

    void setColor(QColor color)
    { this->color = color; }

private slots:
    void showMenu()
    { popurMenuTextColor->exec(mapToGlobal(QPoint(frameSize().width(), 0))); }

    void selectColor(QColor color);

signals:
    void selectedColor(QColor);
};

#endif // UPUSHBUTTONSELECTCOLOR_H
