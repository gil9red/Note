#ifndef _HelpBrowser_h_
#define _HelpBrowser_h_

#include <QtGui>
#include <QtCore>
#include <QtWebKit>

class HelpBrowser: public QWidget
{
    Q_OBJECT

public:
    HelpBrowser(QString strPath,
                QString strFileName,
                QWidget *parent = 0);

private:
    QPushButton*  pButtonBack;
    QPushButton*  pButtonHome;
    QPushButton*  pButtonForward;

    QTextBrowser* textBrowser;

    QVBoxLayout* vBoxLayoutMain;
    QHBoxLayout* hBoxLayoutCommandButton;

protected:
    void closeEvent(QCloseEvent *)
    { delete this; }
};

#endif  //_HelpBrowser_h_
