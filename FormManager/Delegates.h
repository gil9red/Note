#ifndef DELEGATES_H
#define DELEGATES_H

#include <QtGui>
#include <QtCore>

#include "Globals.h"

class ReadDelegate: public QItemDelegate
{
public:
    inline QWidget *createEditor(QWidget*,
                                 const QStyleOptionViewItem&,
                                 const QModelIndex&) const
    {
        return 0;
    }
};

class ComboBoxDelegate: public QItemDelegate
{
public:
    inline QWidget * createEditor(QWidget *parent,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
    {
        QComboBox *comboBox = new QComboBox(parent);
        comboBox->addItem(QString("true"), true);
        comboBox->addItem(QString("false"), false);

        return comboBox;
    }
    inline void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QComboBox *comboBox = qobject_cast<QComboBox *>(editor);

        if (!comboBox)
            return;

        int pos = comboBox->findText(index.model()->data(index).toString(), Qt::MatchExactly);
        comboBox->setCurrentIndex(pos);
    }
    inline void setModelData(QWidget *editor, QAbstractItemModel *model,
                             const QModelIndex &index) const
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);

        model->setData(index, comboBox->currentText(), Qt::EditRole);
    }
};

#endif // DELEGATES_H
