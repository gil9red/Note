#include "uwritertextfiles.h"

/// PUBLIC
bool UWriterTextFiles::writeOut(Note *usedNote,
                                QString fileName)
{
    using namespace UFormatsText;

    if(!fileName.isEmpty())
    {
        QString suffix = QFileInfo(fileName).suffix();

        // для создания pdf файлов используется другой класс
        if(suffix.contains(PDF, Qt::CaseInsensitive))
        {
            QPrinter printer;
            printer.setOutputFormat(QPrinter::PdfFormat);

            printer.setOutputFileName(fileName);

            usedNote->textEdit->print(&printer);

            return true;

        }else if(suffix == suffixFilesNote)
        {
            QString oldFileName = usedNote->getPathInFileNote();
            QString newFileName = fileName;

            QFile::copy(oldFileName, newFileName);
        }else
        {
            QTextDocumentWriter textDocumentWriter;

            // формат
            textDocumentWriter.setFormat(suffix.toLocal8Bit());

            // текстовый кодек
            textDocumentWriter.setCodec(QTextCodec::codecForName(textCodec.toUtf8()));

            // Костыль, кривой костыль имитации создания
            // rtf файла
            if(suffix.contains(RTF, Qt::CaseInsensitive))
                textDocumentWriter.setFormat(HTML.toLocal8Bit());

            // если пишем в odt, меняем кодировку,
            // хз почему, но при QtextCodec.toUtf8()ой файл будет
            // открываться с ошибкой
            if(suffix.contains(ODT, Qt::CaseInsensitive))
                textDocumentWriter.setCodec(QTextCodec::codecForName("utf-8"));

            // путь к файлу
            textDocumentWriter.setFileName(fileName);

            // запись
            return textDocumentWriter.write(usedNote->textEdit->document());
        }

    }else    
        return false;

    return true;
}
