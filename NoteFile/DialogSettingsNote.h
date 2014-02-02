#ifndef DIALOGSETTINGSNOTE_H
#define DIALOGSETTINGSNOTE_H

#include <QtGui>
#include <QtCore>

#include "NoteFile/Password/USettingsPassword.h"
#include "UPushButtonSelectColor.h"
#include "ULineEditWithButton/ULineEditWithClearButton.h"
#include "Globals.h"

struct DataNote
{
    QString text;
    QString title;
    bool lock;

    int width;
    int height;
    int positionX;
    int positionY;

    bool visible;

    QColor backgroundColor;

    double opacity;

    QString zPosition;

    QString dateOfCreating;
    QString dateLastChange;
    QString dateOfLastRemoval;
    QString dateOfLastRestore;

    int countTextSymbols;
    int countTextLines;    

    QString password;
    bool protectionActive;

    bool autocomplete;

    bool minimize;
};

class DialogSettingsNote: public QWidget
{
    Q_OBJECT

public:
    DialogSettingsNote(QWidget *parent = 0);

private:
    void addSelectTitle(QBoxLayout *layout);
    void addSelectOpacity(QBoxLayout *layout);
    void addSelectColor(QBoxLayout *layout);
    void addSelectZPosition(QBoxLayout *layout);
    void addCommandButtons(QBoxLayout *layout);

    void setDataNote(DataNote *data);

public:
    QWidget *widgetSelectPage;
    QPushButton *pButtonGeneralSettings;
    QPushButton *pButtonSettingsPassword;

    QWidget *widgetPages;

    // 1
    QPushButton *pButtonOk;

    QGroupBox *groupBoxSelectOpacity;
    QLabel *labelSelectOpacity;
    QSlider *sliderSelectOpacity;

    QGroupBox *groupBoxSelectZPosition;
    QLabel *labelSelectZPosition;
    QRadioButton *radioButtonWindowStaysOnBottomHint;
    QRadioButton *radioButtonWindowStaysOnTopHint;

    QGroupBox *groupBoxSelectTitle;
    QLabel *labelSelectTitle;
    ULineEditWithClearButton *lineEditSelectTitle;

    QGroupBox *groupBoxSelectColor;
    QLabel *labelSelectColor;
    UPushButtonSelectColor *pButtonSelectColor;

    QWidget *widgetGeneral;
    // 1

    // 2
    USettingsPassword *settingsPassword;
    // 2

    QSplitter *mainSplitter;

public slots:
    void showDialog(DataNote *data);
    void showDialog(DataNote *data, QPoint pos);
    void setLanguage(QString pathToTheFileLanguage);

private slots:
    void clickButtonOk()
    { close(); }

    void clickButtonSelectColor(QColor color);
    void setColorToButtonSelectColor(QColor color);
    void clickPButtonGeneralSettings();
    void clickPButtonSettingsPassword();
    void clickRadioButtonChangeZPosition();

    void sliderChangePosition(int pos)
    { emit changeOpacity((qreal)pos / 100); }

    void lineEditTitleChangeText(QString text)
    { emit changeTitle(text); }

    void newPasswordIsSelected();
    void changeProtectionEnable();

signals:
    void changeColor(QColor);
    void changeTitle(QString);
    void changeOpacity(qreal);
    void changeZPosition(QString);
    void changePassword(QString, bool);

    void iClose();

protected:
    void closeEvent(QCloseEvent *event);
};

class UMiniDialogSetTitle: public QWidget
{
    Q_OBJECT
public:
    inline UMiniDialogSetTitle(QWidget *parent = 0):
        QWidget(parent)
    {
        setWindowFlags(Qt::Tool);

        pButtonOk = new QPushButton("Ok");
        connect(pButtonOk, SIGNAL(clicked()),
                this, SLOT(close()));

        QPixmap pixmap_enter(IconClearTextEnter);
        QPixmap pixmap_leave(IconClearTextLeave);

        int widthIcon = 20;
        int heightIcon = 20;
        pixmap_enter = pixmap_enter.scaled(widthIcon, heightIcon, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        pixmap_leave = pixmap_leave.scaled(widthIcon, heightIcon, Qt::KeepAspectRatio, Qt::SmoothTransformation);


        lineEdit = new ULineEditWithClearButton();
        lineEdit->setIconButton(pixmap_leave);
        lineEdit->setIconButton(ULineEditWithClearButton::EnterEvent,
                                pixmap_enter);
        lineEdit->setIconButton(ULineEditWithClearButton::LeaveEvent,
                                pixmap_leave);

        connect(lineEdit, SIGNAL(textEdited(QString)),
                this, SIGNAL(changeTitle(QString)));

        QVBoxLayout *vBoxLayoutMain = new QVBoxLayout();
        vBoxLayoutMain->addWidget(lineEdit);
        vBoxLayoutMain->addWidget(pButtonOk, 0, Qt::AlignRight);

        setLayout(vBoxLayoutMain);
    }

public:
    QPushButton *pButtonOk;
    ULineEditWithClearButton *lineEdit;

public slots:
    void setLanguage(QString pathToTheFileLanguage)
    {
        QSettings langFile(pathToTheFileLanguage,
                           QSettings::IniFormat);
        langFile.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
        setWindowTitle(langFile.value("NOTE/Title").toString());
    }

signals:
    void changeTitle(QString);
    void iClose();

protected:
    void closeEvent(QCloseEvent *event)
    {
        event->ignore();
        hide();
        emit iClose();
    }
    void keyReleaseEvent(QKeyEvent *event)
    {
        if(event->key() == Qt::Key_Escape)
            close();
    }
};

class UMiniDialogSetOpacity: public QWidget
{
    Q_OBJECT
public:
    inline UMiniDialogSetOpacity(QWidget *parent = 0):
        QWidget(parent)
    {
        setWindowFlags(Qt::Tool);

        pButtonOk = new QPushButton("Ok");
        connect(pButtonOk, SIGNAL(clicked()),
                this, SLOT(close()));

        slider = new QSlider();
        slider->setOrientation(Qt::Horizontal);
        slider->setRange(minimalOpacity, maximalOpacity);
        slider->setSliderPosition(maximalOpacity);
        connect(slider, SIGNAL(valueChanged(int)),
                this, SLOT(sliderChangePosition(int)));

        QVBoxLayout *vBoxLayoutMain = new QVBoxLayout();
        vBoxLayoutMain->addWidget(slider);
        vBoxLayoutMain->addWidget(pButtonOk,
                                  0,
                                  Qt::AlignRight);

        setLayout(vBoxLayoutMain);
    }

public:
    QPushButton *pButtonOk;
    QSlider *slider;

public slots:
    void setLanguage(QString pathToTheFileLanguage)
    {
        QSettings langFile(pathToTheFileLanguage,
                           QSettings::IniFormat);
        langFile.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));
        setWindowTitle(langFile.value("NOTE/Opacity").toString());
    }

private slots:
    void sliderChangePosition(int pos)
    {
        emit changeOpacity((qreal)pos / 100);
    }

signals:
    void changeOpacity(qreal);
    void iClose();

protected:
    void closeEvent(QCloseEvent *event)
    {
        event->ignore();
        hide();
        emit iClose();
    }
    void keyReleaseEvent(QKeyEvent *event)
    {
        if(event->key() == Qt::Key_Escape ||
                event->key() == Qt::Key_Return)
            close();
    }
};

#endif // DIALOGSETTINGSNOTE_H
