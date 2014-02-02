#ifndef UOFFICEWORDSTYLECOMBOBOX_H
#define UOFFICEWORDSTYLECOMBOBOX_H

#include <QtGui>
#include <QtCore>

class UOfficeWordStyleComboBox : public QComboBox
{
    Q_OBJECT

public:
    UOfficeWordStyleComboBox(QWidget *parent = 0):
        QComboBox(parent) { setEditable(false); }

protected:
    void showPopup()
    {
        qint32 width = 0;
        QFontMetrics fm(font());

        for(int i = 0; i < count(); i++)
            width = qMax(fm.width(itemText(i)), width);

        view()->setMinimumWidth(width + 10);

        QComboBox::showPopup();
    }
};

#endif // UOFFICEWORDSTYLECOMBOBOX_H
