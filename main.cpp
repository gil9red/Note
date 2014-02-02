#include <QtGui>
#include <QtCore>

#include "ManagerNote.h"
#include "qtsingleapplication/qtsingleapplication.h"
#include "UAnimatedSplashScreen.h"
#include "UFileAssociationManager/UFileAssociationManager.h"

int main(int argc, char *argv[])
{
    QApplication::setColorSpec(QApplication::ManyColor);

    QtSingleApplication *application = new QtSingleApplication(argc, argv);

    qApp->setApplicationName(nameOrganization);
    qApp->setApplicationVersion(versionProject);
    qApp->setOrganizationName(nameOrganization);

    // создаем все нужные папки
    for(int i = 0; i < listFolders().count(); i++)
    {
        QString name = listFolders().at(i);
        makeFolder(name);
    }

    QLibrary libraryIcons;
    libraryIcons.setFileName(qApp->applicationDirPath()
                             + QDir::separator()
                             + "icons.dll");
    libraryIcons.load();

    QDir *dirFonts = new QDir(absolutePathToTheFolderFonts());
    dirFonts->setFilter(QDir::Files);

#ifdef Q_OS_WIN
    extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
#endif

#ifdef Q_OS_WIN
    qt_ntfs_permission_lookup++;
#endif

    QFileInfoList listFonts = dirFonts->entryInfoList();

#ifdef Q_OS_WIN
    qt_ntfs_permission_lookup--;
#endif

    for(int i = 0; i < listFonts.count(); i++)
        QFontDatabase::addApplicationFont(listFonts.at(i).absoluteFilePath());

    delete dirFonts;

    UFileAssociationManager::makeApplicationDefaultForExtension(suffixFilesNote);

    QTextCodec::setCodecForTr(QTextCodec::codecForName(textCodec.toUtf8()));

    QString flatListArguments = "";
    QStringList listArguments;

    // выводим список аргументов (параметров) ехе
    // первый аргумент (индекс 0) - путь к ехе,
    // поэтому смотрим с второго аргумента
    for(int i = 1; i < argc; i++)
    {
        QString argument = QObject::tr(argv[i]);
        QString suffix = QFileInfo(argument).suffix();

        // если аргумент - путь к файлу и его формат - формат заметок
        if(QFileInfo(argument).isFile()
                && suffix == suffixFilesNote)
        {
            flatListArguments += argument;
            listArguments.append(argument);

            // если текущий аргумент не последний
            if(i < argc - 1)
                flatListArguments += separatorListFilesNote;
        }
    }

    // если копия уже запущена
    if(application->isRunning())
    {
        application->sendMessage(messageFromQtSingleApplication_Show);

        if(!flatListArguments.isEmpty())
            application->sendMessage(flatListArguments);

        return 0;
    }

    // инициализируем генератор псевдослучайных чисел
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    /// _
    QSettings *ini = new QSettings(absolutePathToTheConfigurationProgram(),
                                   QSettings::IniFormat);
    ini->setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    QString language = ini->value("Language").toString();

    // если папка с языками пуста
    if(QDir(absolutePathToTheFolderLanguages())
            .entryInfoList(QDir::Files).isEmpty())
    {
        QFile::copy(strDefaultLanguageFile,
                    absolutePathToTheFolderLanguages()
                    + QDir::separator()
                    + fileNameRussianLanguage);

        language = fileNameRussianLanguage;
    }

    QString pathToLanguage = absolutePathToTheFolderLanguages()
            + QDir::separator()
            + language;

    // если нет файла языка, сохраненного в последний раз
    // как текущий язык
    if(!QFileInfo(pathToLanguage).isFile())
    {
        // то новым текущим будет первый из списка языков
        QDir dir = QDir(absolutePathToTheFolderLanguages());
        dir.setFilter(QDir::Files);
        dir.setSorting(QDir::Name);

        QStringList filters;
        filters << QString("*." + suffixLanguageFiles);
        dir.setNameFilters(filters);

        QStringList list = dir.entryList();

        if(!list.isEmpty())
            language = list.first();

        pathToLanguage = absolutePathToTheFolderLanguages()
                + QDir::separator()
                + language;
    }

    delete ini;

    ini = new QSettings(pathToLanguage, QSettings::IniFormat);
    ini->setIniCodec(QTextCodec::codecForName(textCodec.toUtf8()));

    QStringList list = ini->value("File/LoadingInfo").toStringList();
    delete ini;
    /// _

    //---
    QFont font;
    font.setFamily("Arial");
    font.setPointSize(8);
    font.setWeight(QFont::Bold);

    QPixmap *pixmap = new QPixmap(IconSplashIcon);

    UAnimatedSplashScreen *animatedSplash = new UAnimatedSplashScreen();
    animatedSplash->setWindowFlags(animatedSplash->windowFlags()
                                   | Qt::WindowStaysOnTopHint);
    animatedSplash->setPixmap(*pixmap);
    animatedSplash->show();

    ManagerNote *managerNote = new ManagerNote();

    const int numberPhase = 8;
    for(int i = 0; i < numberPhase + 1; i++)
    {
        if(!list.isEmpty())
            animatedSplash->setMessage(list.takeFirst(), font);

        application->processEvents();
        managerNote->startThePreparatoryPhase(i + 1);
    }

    animatedSplash->finish(managerNote);

    // добавим и откроем файлы заметок, которые были указаны в
    // аргументах ехе
    for(int i = 0; i < listArguments.count(); i++)
        managerNote->openFileNote(listArguments.at(i));

    delete pixmap;
    delete animatedSplash;

    application->setQuitOnLastWindowClosed(false);

    QObject::connect(application, SIGNAL(messageReceived(QString)),
                     managerNote, SLOT(messageToApp(QString)));

    return application->exec();
}
