#include "HelpBrowser.h"

/// PUBLIC
HelpBrowser::HelpBrowser(QString strPath,
                         QString strFileName,
                         QWidget *parent):
    QWidget(parent)
{
    pButtonBack    = new QPushButton("<<");
    pButtonHome    = new QPushButton("Home");
    pButtonForward = new QPushButton(">>");
    textBrowser = new QTextBrowser();

    connect(pButtonBack, SIGNAL(clicked()),
            textBrowser, SLOT(backward()));
    connect(pButtonHome, SIGNAL(clicked()),
            textBrowser, SLOT(home()));
    connect(pButtonForward, SIGNAL(clicked()),
            textBrowser, SLOT(forward()));
    connect(textBrowser, SIGNAL(backwardAvailable(bool)),
            pButtonBack, SLOT(setEnabled(bool)));
    connect(textBrowser, SIGNAL(forwardAvailable(bool)),
            pButtonForward, SLOT(setEnabled(bool)));

    textBrowser->setSource(QUrl::fromLocalFile(QString(strPath + strFileName)));

    hBoxLayoutCommandButton = new QHBoxLayout;

    hBoxLayoutCommandButton->addWidget(pButtonBack);
    hBoxLayoutCommandButton->addWidget(pButtonHome);
    hBoxLayoutCommandButton->addWidget(pButtonForward);

    vBoxLayoutMain = new QVBoxLayout;

    vBoxLayoutMain->addLayout(hBoxLayoutCommandButton);
    vBoxLayoutMain->addWidget(textBrowser);

    setLayout(vBoxLayoutMain);
}
