#include "ureadertextfiles.h"

bool UReaderTextFiles::readUsingDialogIn(QTextEdit *textEdit,
                                         QString fileFilters)
{
    QString fileName = QFileDialog::getOpenFileName(0,
                                                    QString(),
                                                    QString(),
                                                    fileFilters);

    if(!fileName.isEmpty())
    {
        QFile file(fileName);

        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            QString textInFile = in.readAll();

            QString fileSuffix = QFileInfo(fileName).suffix();

            using namespace UFormatsText;

            if(fileSuffix.contains(HTML, Qt::CaseInsensitive)
                    || fileSuffix.contains(HTM, Qt::CaseInsensitive))
            {                                
                QMessageBox *msgBox = new QMessageBox(textEdit->parentWidget());
                msgBox->setIcon(QMessageBox::Question);
                msgBox->setWindowTitle(titleMessageBox);
                msgBox->setText(textMessageBox);

                msgBox->addButton(strOk, QMessageBox::AcceptRole);
                msgBox->addButton(strNo, QMessageBox::NoRole);
                msgBox->addButton(strCancel, QMessageBox::RejectRole);

                int selectUser = msgBox->exec();

                delete msgBox;

                if(selectUser == 0) // Yes                
                    textEdit->setPlainText(textInFile);

                else if(selectUser == 1) // No
                    textEdit->setHtml(textInFile);
            }else            
                // показываем внутреннее содержимое файла
                textEdit->setPlainText(textInFile);

        }
    }else    
        return false;    

    return true;
}

bool UReaderTextFiles::readIn(QTextEdit *textEdit,
                              QString fileName)
{
    if(!fileName.isEmpty())
    {
        QFile file(fileName);

        if(file.open(QIODevice::ReadOnly))
        {
            QString fileSuffix = QFileInfo(fileName).suffix();

            // пока не будем обрабатывать свои файлы =)
            if(fileSuffix == suffixFilesNote)
                return false;

            QTextStream in(&file);
            QString textInFile = in.readAll();            

            // если textEdit пуст, тогда нет смысла
            // предлагать куда вставить текст
            if(textEdit->toPlainText().isEmpty())
            {
                using namespace UFormatsText;
                if(fileSuffix.contains(HTML, Qt::CaseInsensitive)
                        || fileSuffix.contains(HTM, Qt::CaseInsensitive))
                {
                    QMessageBox *msgBox = new QMessageBox(textEdit->parentWidget());
                    msgBox->setIcon(QMessageBox::Question);
                    msgBox->setWindowTitle(titleMessageBox);
                    msgBox->setText(textMessageBox);

                    msgBox->addButton(strOk, QMessageBox::AcceptRole);
                    msgBox->addButton(strNo, QMessageBox::NoRole);
                    msgBox->addButton(strCancel, QMessageBox::RejectRole);

                    int selectUser = msgBox->exec();

                    delete msgBox;

                    if(selectUser == 0) // Yes                    
                        textEdit->setPlainText(textInFile);

                    else if(selectUser == 1) // No
                        textEdit->setHtml(textInFile);
                }else                
                    textEdit->setPlainText(textInFile);

            }else
            {
                dialogInsertMode = new UDialogInsertMode(textEdit->parentWidget());
                dialogInsertMode->setLanguage(currentLanguage);
                dialogInsertMode->showDialog();                

                int variantInsertText = dialogInsertMode->getResultExecuteDialog();

                delete dialogInsertMode;
                dialogInsertMode = 0;

                if(variantInsertText == UDialogInsertMode::Top)
                {
                    QTextCursor cursor = textEdit->textCursor();
                    cursor.movePosition(QTextCursor::Start);

                    textEdit->setTextCursor(cursor);
                    textEdit->textCursor().insertText(textInFile);

                }else if(variantInsertText == UDialogInsertMode::End)
                {
                    QTextCursor cursor = textEdit->textCursor();
                    cursor.movePosition(QTextCursor::End);

                    textEdit->setTextCursor(cursor);
                    textEdit->textCursor().insertText(textInFile);

                }else if(variantInsertText == UDialogInsertMode::Middle)
                {
                    QTextCursor cursor = textEdit->textCursor();
                    cursor.setPosition(textEdit->toPlainText().size() / 2);

                    textEdit->setTextCursor(cursor);
                    textEdit->textCursor().insertText(textInFile);

                }else if(variantInsertText == UDialogInsertMode::PositionCursor)                
                    textEdit->textCursor().insertText(textInFile);
            }
        }
    }else    
        return false;

    return true;
}

/// PUBLIC SLOTS
void UReaderTextFiles::setLanguage(QString pathToTheFileLanguage)
{
    currentLanguage = pathToTheFileLanguage;

    QSettings ini(pathToTheFileLanguage,
                       QSettings::IniFormat);
    ini.setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    ini.beginGroup("MessageReaderTextFiles");

    titleMessageBox = ini.value("Title").toString();
    textMessageBox = ini.value("Text").toString();
    strOk = ini.value("Ok").toString();
    strNo = ini.value("No").toString();
    strCancel = ini.value("Cancel").toString();

    ini.endGroup();
}
