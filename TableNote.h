#ifndef TableNote_H
#define TableNote_H

#include <QTableWidget>
#include <QMenu>

#include <QTableWidget>
#include <QItemDelegate>
#include <QHeaderView>

#include <QComboBox>

#include <QDialog>
#include <QGroupBox>
#include <QCheckBox>
#include <QGridLayout>
#include <QTextCodec>
#include <QTabWidget>

#include <QList>

#include "NoteFile/DialogSettingsNote.h"
#include "SettingsTableNote.h"

class ReadDelegate: public QItemDelegate
{
public:
    inline ReadDelegate(QObject * parent = 0):
        QItemDelegate(parent)
    {

    }

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
    inline ComboBoxDelegate(QObject * parent = 0):
        QItemDelegate(parent)
    {

    }

    inline QWidget* createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
    {
        QComboBox *comboBox = new QComboBox(parent);
        comboBox->addItem(QString("true"));
        comboBox->addItem(QString("false"));

        return comboBox;
    }
    inline void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);

        model->setData(index, comboBox->currentText(), Qt::EditRole);
    }
};

class MyTableWidget: public QTableWidget
{
    Q_OBJECT
public:
    MyTableWidget(QWidget *parent = 0);
    QList <bool> & getListVisibleColumn();

public:
    QAction *actionDeleteItem;
    QAction *actionShowItem;
    QAction *actionHideItem;
    QAction *actionSettings;

    SettingsTableNote *settingsTable;

private:
    bool visibleContextMenu;
    QList <bool> listVisibleColumn;

private slots:
    void signalCellPressed(int row, int col);
    void clickActionDelete();
    void clickActionShow();
    void clickActionHide();    

public slots:
    void setVisibleColumnTable(int indexColumn,
                               bool visible);
    void setVisibleColumnTable(QList <bool> &list);
    void showSettings();
    void setLanguage(QString language);
    void showContextMenu(QPoint p);

signals:
    void clickActionDeleteItem();
    void clickActionShowItem();
    void clickActionHideItem();
    void clickActionSettingsItem();
    void languageIsChange(QString);
};

#endif // TableNote_H
