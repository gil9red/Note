#ifndef USETTINGS_H
#define USETTINGS_H

#include <QtGui>
#include <QtCore>

#include "Globals.h"

class USettings: public QDialog
{
    Q_OBJECT

public:
    USettings(QWidget *parent = 0);
    void setAutorun(bool bValue);
    bool isAutorun();   

public:
    QGroupBox *groupBoxProgram;
    QVBoxLayout *vBoxLayoutGroupBoxProgram;
    QCheckBox *checkBoxRunWithWindows;
    QCheckBox *checkBoxShowTheManagerWhenTheProgramStarts;

    QGroupBox *groupBoxSelectLanguage;
    QVBoxLayout *vBoxLayoutGroupBoxSelectLanguage;
    QComboBox *comboBoxSelectLanguage;

    QGroupBox *groupBoxWhenClosingManager;
    QVBoxLayout *vBoxLayoutGroupBoxWhenClosingManager;
    QRadioButton *rButtonMinimizeToTrayWhenClosingManager;
    QRadioButton *rButtonCloseTheProgramWhenClosingManager;

    QGroupBox *groupBoxWhenMinimizeManager;
    QVBoxLayout *vBoxLayoutGroupBoxWhenMinimizeManager;
    QRadioButton *rButtonMinimizeToTrayWhenMinimizeManager;
    QRadioButton *rButtonMinimizeToTaskbarMinimizeManager;

private:
    void createGUI();
    void loadLanguageToComboBox();

public slots:
    void setLanguage(QString pathToTheFileLanguage);
    void minimizeToTrayWhenClosingManager();
    void minimizeToTrayWhenMinimizeManager();

private slots:
    void clickCheckBoxRunWithWindows(){ setAutorun(checkBoxRunWithWindows->isChecked()); }
    void comboBoxSelectLanguageItemIsActivated(int indexItem);
    void setCheckedShowManagerAtStartProgram(bool bValue);

signals:
    void languageIsChange(QString);
};

#endif // USETTINGS_H
