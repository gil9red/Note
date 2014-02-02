#ifndef UABOUTPROGRAM_H
#define UABOUTPROGRAM_H

#include <QtGui>
#include <QtCore>

#include "Globals.h"

class UAboutProgram: public QDialog
{
    Q_OBJECT

public:
    UAboutProgram(QWidget *parent = 0);
    void addTranslations(QList <QString> *list);

private:
    void createGUI();

    void addSectionAuthors(QBoxLayout *layout);
    void addAuthors(QWidget *name);
    void addAuthors(QWidget *name, QWidget *contact);
    void addAuthors(QList <QWidget*> *list);

    void addSectionProgrammers(QBoxLayout *layout);
    void addProgrammers(QWidget *name);
    void addProgrammers(QWidget *name, QWidget *contact);
    void addProgrammers(QList <QWidget*> *list);

    void addSectionDesigners(QBoxLayout *layout);
    void addDesigners(QWidget *name);
    void addDesigners(QWidget *name, QWidget *contact);
    void addDesigners(QList <QWidget*> *list);

    void addSectionTesters(QBoxLayout *layout);
    void addTesters(QWidget *name);
    void addTesters(QWidget *name, QWidget *contact);
    void addTesters(QList <QWidget*> *list);    

    void addSectionTranslations(QBoxLayout *layout);

    void addPass(QBoxLayout *layout);

    void addLogo(QBoxLayout *layout, QWidget *logo);

private:
    QPushButton *pButtonClose;

    QLabel *labelImageLogo;

    QLabel *labelSectionAuthors;
    QLabel *labelSectionProgrammers;
    QLabel *labelSectionDesigners;
    QLabel *labelSectionTesters;

    QVBoxLayout *vBoxMainLayoutAuthors;
    QVBoxLayout *vBoxMainLayoutProgrammers;
    QVBoxLayout *vBoxMainLayoutDesigners;
    QVBoxLayout *vBoxMainLayoutTesters;

    QVBoxLayout *vBoxMainLayoutTranslations;

    QVBoxLayout *vBoxLayoutMain;

    QLabel *labelAuthor1, *labelContaktAuthor1;
    QLabel *labelProgrammer1;
    QLabel *labelDesigner1, *labelContaktDesigner1;
    QLabel *labelTester1, *labelContaktTester1;

    QLabel *labelSectionTranslations;

    QWidget *mainWidgetToScrollArea;

    QScrollArea *scrollAreaWithAutoscroll;

    QPushButton *pButtonOpenFileHistory;
    QPushButton *pButtonOpenFileLicense;
    QPushButton *pButtonShowHideDevelopers;

    QLabel *labelAllRightsReserved;
    QLabel *labelDescription;
    QLabel *labelWordFromTheOwner;

public slots:
    void setLanguage(QString pathToTheFileLanguage);
    void clickPButtonOpenFileHistory();
    void clickPButtonOpenFileLicense();
    void clickPButtonShowHideDevelopers();

signals:
    void iClose();

protected:
    void closeEvent(QCloseEvent *);
};

#endif // UABOUTPROGRAM_H
