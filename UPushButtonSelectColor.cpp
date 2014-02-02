#include "UPushButtonSelectColor.h"

/// PRIVATE
void UPushButtonSelectColor::init()
{
    createActionsSelectColor();

    connect(this, SIGNAL(clicked()),
            this, SLOT(showMenu()));
}

void UPushButtonSelectColor::createActionsSelectColor()
{
    actionTextColor_White       = new QAction(this);
    actionTextColor_Black       = new QAction(this);
    actionTextColor_Red         = new QAction(this);
    actionTextColor_DarkRed     = new QAction(this);
    actionTextColor_Green       = new QAction(this);
    actionTextColor_DarkGreen   = new QAction(this);
    actionTextColor_Blue        = new QAction(this);
    actionTextColor_DarkBlue    = new QAction(this);
    actionTextColor_Cyan        = new QAction(this);
    actionTextColor_DarkCyan    = new QAction(this);
    actionTextColor_Magenta     = new QAction(this);
    actionTextColor_DarkMagenta = new QAction(this);
    actionTextColor_Yellow      = new QAction(this);
    actionTextColor_DarkYellow  = new QAction(this);
    actionTextColor_Gray        = new QAction(this);
    actionTextColor_DarkGray    = new QAction(this);
    actionTextColor_LightGray   = new QAction(this);
    actionTextColor_Other       = new QAction(this);

    QPixmap pixmap(10, 10);

    pixmap.fill(Qt::white);
    actionTextColor_White->setIcon(pixmap);

    pixmap.fill(Qt::black);
    actionTextColor_Black->setIcon(pixmap);

    pixmap.fill(Qt::red);
    actionTextColor_Red->setIcon(pixmap);

    pixmap.fill(Qt::darkRed);
    actionTextColor_DarkRed->setIcon(pixmap);

    pixmap.fill(Qt::green);
    actionTextColor_Green->setIcon(pixmap);

    pixmap.fill(Qt::darkGreen);
    actionTextColor_DarkGreen->setIcon(pixmap);

    pixmap.fill(Qt::blue);
    actionTextColor_Blue->setIcon(pixmap);

    pixmap.fill(Qt::darkBlue);
    actionTextColor_DarkBlue->setIcon(pixmap);

    pixmap.fill(Qt::cyan);
    actionTextColor_Cyan->setIcon(pixmap);

    pixmap.fill(Qt::darkCyan);
    actionTextColor_DarkCyan->setIcon(pixmap);

    pixmap.fill(Qt::magenta);
    actionTextColor_Magenta->setIcon(pixmap);

    pixmap.fill(Qt::darkMagenta);
    actionTextColor_DarkMagenta->setIcon(pixmap);

    pixmap.fill(Qt::yellow);
    actionTextColor_Yellow->setIcon(pixmap);

    pixmap.fill(Qt::darkYellow);
    actionTextColor_DarkYellow->setIcon(pixmap);

    pixmap.fill(Qt::gray);
    actionTextColor_Gray->setIcon(pixmap);

    pixmap.fill(Qt::darkGray);
    actionTextColor_DarkGray->setIcon(pixmap);

    pixmap.fill(Qt::lightGray);
    actionTextColor_LightGray->setIcon(pixmap);


    connect(actionTextColor_White, SIGNAL(triggered()),
            this, SLOT(white()));
    connect(actionTextColor_Black, SIGNAL(triggered()),
            this, SLOT(black()));
    connect(actionTextColor_Red, SIGNAL(triggered()),
            this, SLOT(red()));
    connect(actionTextColor_DarkRed, SIGNAL(triggered()),
            this, SLOT(darkRed()));
    connect(actionTextColor_Green, SIGNAL(triggered()),
            this, SLOT(green()));
    connect(actionTextColor_DarkGreen, SIGNAL(triggered()),
            this, SLOT(darkGreen()));
    connect(actionTextColor_Blue, SIGNAL(triggered()),
            this, SLOT(blue()));
    connect(actionTextColor_DarkBlue, SIGNAL(triggered()),
            this, SLOT(darkBlue()));
    connect(actionTextColor_Cyan, SIGNAL(triggered()),
            this, SLOT(cyan()));
    connect(actionTextColor_DarkCyan, SIGNAL(triggered()),
            this, SLOT(darkCyan()));
    connect(actionTextColor_Magenta, SIGNAL(triggered()),
            this, SLOT(magenta()));
    connect(actionTextColor_DarkMagenta, SIGNAL(triggered()),
            this, SLOT(darkMagenta()));
    connect(actionTextColor_Yellow, SIGNAL(triggered()),
            this, SLOT(yellow()));
    connect(actionTextColor_DarkYellow, SIGNAL(triggered()),
            this, SLOT(darkYellow()));
    connect(actionTextColor_Gray, SIGNAL(triggered()),
            this, SLOT(gray()));
    connect(actionTextColor_DarkGray, SIGNAL(triggered()),
            this, SLOT(darkGray()));
    connect(actionTextColor_LightGray, SIGNAL(triggered()),
            this, SLOT(lightGray()));
    connect(actionTextColor_Other, SIGNAL(triggered()),
            this, SLOT(colorDialog()));

    popurMenuTextColor = new QMenu();
    popurMenuTextColor->addAction(actionTextColor_White);
    popurMenuTextColor->addAction(actionTextColor_Black);
    popurMenuTextColor->addAction(actionTextColor_Red);
    popurMenuTextColor->addAction(actionTextColor_DarkRed);
    popurMenuTextColor->addAction(actionTextColor_Green);
    popurMenuTextColor->addAction(actionTextColor_DarkGreen);
    popurMenuTextColor->addAction(actionTextColor_Blue);
    popurMenuTextColor->addAction(actionTextColor_DarkBlue);
    popurMenuTextColor->addAction(actionTextColor_Cyan);
    popurMenuTextColor->addAction(actionTextColor_DarkCyan);
    popurMenuTextColor->addAction(actionTextColor_Magenta);
    popurMenuTextColor->addAction(actionTextColor_DarkMagenta);
    popurMenuTextColor->addAction(actionTextColor_Yellow);
    popurMenuTextColor->addAction(actionTextColor_DarkYellow);
    popurMenuTextColor->addAction(actionTextColor_Gray);
    popurMenuTextColor->addAction(actionTextColor_DarkGray);
    popurMenuTextColor->addAction(actionTextColor_LightGray);
    popurMenuTextColor->addAction(actionTextColor_Other);
}

/// PUBLIC SLOTS
void UPushButtonSelectColor::setLanguage(QString pathToTheFileLanguage)
{
    QSettings ini(pathToTheFileLanguage,
                  QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("Color");

    actionTextColor_White->setText(ini.value("White").toString());
    actionTextColor_Black->setText(ini.value("Black").toString());
    actionTextColor_Red->setText(ini.value("Red").toString());
    actionTextColor_DarkRed->setText(ini.value("DarkRed").toString());
    actionTextColor_Green->setText(ini.value("Green").toString());
    actionTextColor_DarkGreen->setText(ini.value("DarkGreen").toString());
    actionTextColor_Blue->setText(ini.value("Blue").toString());
    actionTextColor_DarkBlue->setText(ini.value("DarkBlue").toString());
    actionTextColor_Cyan->setText(ini.value("Cyan").toString());
    actionTextColor_DarkCyan->setText(ini.value("DarkCyan").toString());
    actionTextColor_Magenta->setText(ini.value("Magenta").toString());
    actionTextColor_DarkMagenta->setText(ini.value("DarkMagenta").toString());
    actionTextColor_Yellow->setText(ini.value("Yellow").toString());
    actionTextColor_DarkYellow->setText(ini.value("DarkYellow").toString());
    actionTextColor_Gray->setText(ini.value("Gray").toString());
    actionTextColor_DarkGray->setText(ini.value("DarkGray").toString());
    actionTextColor_LightGray->setText(ini.value("LightGray").toString());
    actionTextColor_Other->setText(ini.value("Other").toString());

    ini.endGroup();
}

void UPushButtonSelectColor::colorDialog()
{
    QColorDialog *colorDialog = new QColorDialog(color, this);

    colorDialog->exec();

    QColor col = colorDialog->selectedColor();

    delete colorDialog;

    if(!col.isValid())
        return;

    selectColor(col);
}

/// PRIVATE SLOTS
void UPushButtonSelectColor::selectColor(QColor color)
{
    setColor(color);
    emit selectedColor(color);
}
