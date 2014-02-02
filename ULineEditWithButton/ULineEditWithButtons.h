#ifndef ULINEEDITWITHBUTTONS_H
#define ULINEEDITWITHBUTTONS_H

#include <QtGui>
#include <QtCore>

class ULineEditWithButtons: public QLineEdit
{
    Q_OBJECT

public:
    ULineEditWithButtons(QWidget *parent = 0);

    void setSpacing(int spacing){ d_spacing = spacing; }
    int spacing(){ return d_spacing; }

    void addButton(QAbstractButton *button);

    QList <QAbstractButton *> buttons()
    { return listButtons; }

    bool removeButton(QAbstractButton *button)
    { return listButtons.removeOne(button); }        


private:
    void createGUI();    

private:
    QHBoxLayout *hBoxLayout;
    int d_spacing;

public slots:
    void refreshMetrics();

protected:
    QList <QAbstractButton *> listButtons;

protected:
    void setPadding(int padding)
    {
        setStyleSheet(QString("QLineEdit { padding-right: %1px; } ")
                      .arg(padding));
    }
    void resizeEvent(QResizeEvent *);
};

#endif // ULINEEDITWITHBUTTONS_H
