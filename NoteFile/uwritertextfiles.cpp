#include "uwritertextfiles.h"

/// PUBLIC
bool UWriterTextFiles::writeOut(Note *usedNote,
                                QString fileName)
{
    using namespace UFormatsText;

    if(!fileName.isEmpty())
    {
        QString suffix = QFileInfo(fileName).suffix();

        // ��� �������� pdf ������ ������������ ������ �����
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

            // ������
            textDocumentWriter.setFormat(suffix.toLocal8Bit());

            // ��������� �����
            textDocumentWriter.setCodec(QTextCodec::codecForName(textCodec.toUtf8()));

            // �������, ������ ������� �������� ��������
            // rtf �����
            if(suffix.contains(RTF, Qt::CaseInsensitive))
                textDocumentWriter.setFormat(HTML.toLocal8Bit());

            // ���� ����� � odt, ������ ���������,
            // �� ������, �� ��� QtextCodec.toUtf8()�� ���� �����
            // ����������� � �������
            if(suffix.contains(ODT, Qt::CaseInsensitive))
                textDocumentWriter.setCodec(QTextCodec::codecForName("utf-8"));

            // ���� � �����
            textDocumentWriter.setFileName(fileName);

            // ������
            return textDocumentWriter.write(usedNote->textEdit->document());
        }

    }else    
        return false;

    return true;
}
