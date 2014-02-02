#ifndef USETTINGSPASSWORD_H
#define USETTINGSPASSWORD_H

#include <QtGui>
#include <QtCore>

#include "ULineEditWithButton/ULineEditWithClearButton.h"

#include "Globals.h"

class USettingsPassword: public QWidget
{
    Q_OBJECT
    
public:
    USettingsPassword(QWidget *parent = 0);

    void setPassword(QString str);

    QString getPassword() const { return password; }
    bool protectionIsEnabled(){ return checkBoxPasswordProtect->isChecked(); }
    void setProtectionEnabled(bool enabled){ checkBoxPasswordProtect->setChecked(enabled); }

private:
    bool testInputData();
    void clearLineEdit();

public:
    QCheckBox   *checkBoxPasswordProtect;
    QCheckBox   *checkBoxHideTheInputData;

    QGroupBox *groupBoxMain;
    QVBoxLayout *vBoxLayoutGroupBoxMain;

    QGroupBox   *groupBoxNewPassword;
    QVBoxLayout *vBoxLayoutGroupBoxNewPassword;
    QLabel      *labelNewPassword;
    ULineEditWithClearButton *lineEditNewPassword;
    QLabel      *labelConfirmNewPassword;
    ULineEditWithClearButton *lineEditConfirmNewPassword;

    QLabel      *labelOldPassword;
    ULineEditWithClearButton *lineEditOldPassword;

    QPushButton *pButtonSaveChange;

    QVBoxLayout *vBoxLayoutMain;

    QString password;

    QString strInformation;
    QString strWarning;
    QString strAcceptedNewPassword;
    QString strErrorWhenOldPasswordChange;

public slots:
    void setLanguage(QString pathToTheFileLanguage);
    void setHideTheInputData(bool hide);
    void clickPButtonSaveChange();

signals:
    void newPasswordIsSelected();
};

#endif // USETTINGSPASSWORD_H
