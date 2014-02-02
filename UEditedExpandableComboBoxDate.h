#ifndef UEditedExpandableComboBoxDate_H
#define UEditedExpandableComboBoxDate_H

#include <QtGui>
#include <QtCore>

#include "ULineEditWithButton/ULineEditWithClearButton.h"

#include "Globals.h"

class UIndicatorDateTime: public QLabel
{
    Q_OBJECT

public:
    inline UIndicatorDateTime(QWidget *parent = 0):
        QLabel(parent)
    {
        timer = new QTimer();
        connect(timer, SIGNAL(timeout()),
                this, SLOT(timerTick()));
    }

private:
    QTimer *timer;
    QString formatDateTime;

public slots:
    inline void setFormatDateTime(QString format){ formatDateTime = format; }
    inline void setInterval(int msec){ timer->setInterval(msec); }
    inline int interval() const { return timer->interval(); }
    inline void start(){ timer->start(); }
    inline void stop(){ timer->stop();}

private slots:
    inline void timerTick()
    {
        QString text = QDateTime::currentDateTime().toString(formatDateTime);
        setText(text);
    }
};

class UDialogAddToListWidget: public QDialog
{
    Q_OBJECT

public:
    inline UDialogAddToListWidget(QWidget *parent = 0):
        QDialog(parent)
    {
        QTextCodec::setCodecForTr(QTextCodec::codecForName(textCodec.toUtf8()));

        setWindowFlags(Qt::Dialog
                       | Qt::WindowCloseButtonHint);

        createGUI();

        setSizeGripEnabled(true);
    }

    inline void setFormatDate(QString format)
    { lineEdit->setText(format); }

    inline int getDialogReturnCode(){ return dialogReturnCode; }
    inline QString getFormatDate(){ return lineEdit->text(); }

private:
    inline void createGUI()
    {
        QVBoxLayout *vBoxLayoutMain = new QVBoxLayout();

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

        vBoxLayoutMain->addWidget(lineEdit);

        indicatorDateTime = new UIndicatorDateTime();

        vBoxLayoutMain->addWidget(indicatorDateTime);

        connect(lineEdit, SIGNAL(textChanged(QString)),
                indicatorDateTime, SLOT(setFormatDateTime(QString)));

        indicatorDateTime->setInterval(10);
        indicatorDateTime->start();

        pButtonOk = new QPushButton();
        pButtonOk->setDefault(true);
        connect(pButtonOk, SIGNAL(clicked()),
                this, SLOT(clickOk()));

        pButtonCancel = new QPushButton();
        connect(pButtonCancel, SIGNAL(clicked()),
                this, SLOT(clickCancel()));

        QHBoxLayout *hBox = new QHBoxLayout();
        hBox->addStretch();
        hBox->addWidget(pButtonOk);
        hBox->addWidget(pButtonCancel);

        vBoxLayoutMain->addLayout(hBox);

        setLayout(vBoxLayoutMain);
    }

private:
    QPushButton *pButtonOk;
    QPushButton *pButtonCancel;

    ULineEditWithClearButton *lineEdit;

    UIndicatorDateTime *indicatorDateTime;

    int dialogReturnCode;

private slots:
    inline void clickOk()
    { dialogReturnCode = 1; close(); }
    inline void clickCancel()
    { dialogReturnCode = 0; close(); }

public slots:
    inline void setLanguage(QString pathToTheFileLanguage)
    {
        QSettings ini(pathToTheFileLanguage,
                           QSettings::IniFormat);
        ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

        ini.beginGroup("UDialogAddToListWidget");

        setWindowTitle(ini.value("Title").toString());
        pButtonOk->setText(ini.value("Ok").toString());
        pButtonCancel->setText(ini.value("Cancel").toString());
        lineEdit->setToolTip(ini.value("ToolTipEditedDateTime").toString());

        ini.endGroup();
    }
};

class UDialogChangesComboBoxDate: public QDialog
{
    Q_OBJECT

public:
    UDialogChangesComboBoxDate(QWidget *parent = 0);
    void setListItemsDate(QStringList *list);
    QStringList * getListItemDate(){ return & listComboBoxDate; }
    int getDialogReturnCode(){ return dialogReturnCode; }

private:
    void createGUI();

private:
    QListWidget *listWidget;

    QPushButton *pButtonAdd;
    QPushButton *pButtonDelete;

    QPushButton *pButtonOk;
    QPushButton *pButtonCancel;

    QStringList listComboBoxDate;

    UIndicatorDateTime *indicatorDateTimeListWidget;

    int dialogReturnCode;

    QString currentLanguage;

private slots:
    void clickAdd();
    void editedItem(QListWidgetItem *item);

    void clickDelete();

    void clickOk();
    void clickCancel();

    void selectedItemListWidget(QListWidgetItem *item)
    { indicatorDateTimeListWidget->setFormatDateTime(item->text()); }

public slots:
    void setLanguage(QString pathToTheFileLanguage);

signals:
    void languageIsChange(QString);
};

class UEditedExpandableComboBoxDate: public QWidget
{
    Q_OBJECT
    
public:
    UEditedExpandableComboBoxDate(QWidget *parent = 0);
    void setListItemsDate(QStringList *list);
    void setCurrentItemComboBoxDate(QString text);
    QStringList * getListDateTimeFormat();

private:
    void createGUI();

private:
    QComboBox *comboBox;
    QPushButton *pButton;

    UIndicatorDateTime *indicatorDateTime;

    UDialogChangesComboBoxDate *dialog;

    QGroupBox *groupBoxShowIndicatorDateTime;

    QString currentLanguage;

public slots:
    void setLanguage(QString pathToTheFileLanguage);

private slots:
    void clickButton();        

signals:
    void activatedItemComboBoxDate(QString);
    void saveListDateTimeFormat();

    void languageIsChange(QString);
};

#endif // UEditedExpandableComboBoxDate_H
