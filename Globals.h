#ifndef GLOBALS_H
#define GLOBALS_H

#include <QtGui>
#include <QtCore>

#ifdef Q_WS_WIN
#include <windows.h>
#endif

const QString versionProject                       = "0.2.2";
const QString nameOrganization                     = "Petrash Ilya";
const QString nameProject                          = "Note";
const QString dataBuildProject                     = "17.04.2013";

const QString ownerProgram                         = "Ilya Petrash";
const QString rangeOfDates                         = "2012 - 2013";

static QString textCodec                           = "windows-1251";
const QString suffixFilesNote                      = "FileNote";
const QString descriptionFilesNote                 = "Note the program file";
const QString suffixLanguageFiles                  = "notelng";
const QString suffixAutocompleteLanguageFiles      = "wordlist";
const QString suffixStyleFiles = "style";

const QString nameOfTheFolderLocationNotes         = "Notes";
const QString remoteLocationNameOfTheFolderNotes   = "Recycle Bin";
const QString nameOfTheFolderLocationLanguageFiles = "Language";
const QString nameOfTheFolderLocationLanguageToAutocomplete = "List Language Autocomplete";
const QString nameOfTheFolderStyles = "styles";
const QString nameOfTheFolderFonts = "fonts";
const QString nameOfTheFolderImageformats = "imageformats";

static QStringList listFolders()
{
    return QStringList()
            << nameOfTheFolderLocationNotes
            << remoteLocationNameOfTheFolderNotes
            << nameOfTheFolderLocationLanguageFiles
            << nameOfTheFolderLocationLanguageToAutocomplete
            << nameOfTheFolderStyles
            << nameOfTheFolderFonts
            << nameOfTheFolderImageformats;
}

static bool makeFolder(QString folder)
{
    return QDir(qApp->applicationDirPath()).mkdir(folder);
}

const QString nameExe                              = nameProject + ".exe";
const QString nameConfigFileProgram                = "config.ini";

const QString fileNameChangeHistory                = "history.txt";
const QString fileNameLicense                      = "license.txt";
const QString fileNameRussianLanguage              = "russian." + suffixLanguageFiles;

const QString pathToDefaultStyle = ":/resource/styles/Default/Default.style";

const QString messageFromQtSingleApplication_Show  = "Show!";

const QString noTitle = "(no_title)";

const QString formatDate = "dd/MM/yyyy";
const QString formatTime = "hh:mm:ss";

const QString separatorListFilesNote = "|?|";

const bool isMinimizeToTrayManagerAtClosing = false;

const int widthFormatingPanel = 200;
const int heightFormatingPanel = 240;

// HOTKEY
const QString separatorHotkey = "+";
const int hotkeyActionNewNote              = 0x0;
const int hotkeyActionNewNoteFromClipboard = 0x1;
const int hotkeyActionHideAllNote          = 0x2;
const int hotkeyActionShowAllNote          = 0x3;
const int hotkeyActionShowHideManager      = 0x4;
// HOTKEY

enum IndexPageManager
{
    PageNotes,
    PageRecycleBin,
    PageResultSearch,
    PageSettings,
    PageUnknown = -1
};

namespace UzPosition
{
const QString AlwaysOnBottom = "AlwaysOnBottom";
const QString AlwaysOnTop    = "AlwaysOnTop";
}

const QString defaultZPositionNote = UzPosition::AlwaysOnTop;

namespace USizeIcons
{
const QString str128x128 = "128x128";
const QString str96x96   = "96x96";
const QString str64x64   = "64x64";
const QString str48x48   = "48x48";
const QString str32x32   = "32x32";

const QSize size128x128(128, 128);
const QSize size96x96(96, 96);
const QSize size64x64(64, 64);
const QSize size48x48(48, 48);
const QSize size32x32(32, 32);
}

namespace UFormatsText
{
const QString TXT  = "TXT";
const QString ODT  = "ODT";
const QString RTF  = "RTF";
const QString HTML = "HTML";
const QString HTM  = "HTM";
const QString PDF  = "PDF";

namespace UDescriptionFilters
{
const QString TXT  = "Plain Text files (*.txt)";
const QString ODT  = "OpenDocument Format (*.odt)";
const QString RTF  = "Rich Text Format (*.rtf)";
const QString HTML = "HyperText Markup Language (*.html; *.htm)";
const QString HTM  = HTML;
const QString PDF  = "Portable Document Format (*.pdf)";
}
}

namespace UFormatsImage
{
const QString BMP  = "BMP";
const QString JPG  = "JPG";
const QString JPEG = "JPEG";
const QString PNG  = "PNG";
const QString TIFF = "TIFF";
const QString TIF  = "TIF";
const QString ICO  = "ICO";
const QString PPM  = "PPM";
const QString XBM  = "XBM";
const QString XPM  = "XPM";

namespace UDescriptionFormatImage
{
const QString BMP  = "Windows Bitmap";
const QString JPG  = "Joint Photographic Experts Group";
const QString JPEG = JPG;
const QString PNG  = "Portable Network Graphics";
const QString TIFF = "Tagged Image File Format";
const QString TIF  = TIFF;
const QString ICO  = "Windows icon";
const QString PPM  = "Portable Pixmap";
const QString XBM  = "X11 Bitmap";
const QString XPM  = "X11 Pixmap";
}

namespace UDescriptionFilters
{
const QString BMP  = "Windows Bitmap (*.bmp)";
const QString JPG  = "Joint Photographic Experts Group (*.jpg; *.jpeg)";
const QString JPEG = JPG;
const QString PNG  = "Portable Network Graphics (*.png)";
const QString TIFF = "Tagged Image File Format (*.tiff; *.tif)";
const QString TIF  = TIFF;
const QString ICO  = "Windows icon (*.ico)";
const QString PPM  = "Portable Pixmap (*.ppm)";
const QString XBM  = "X11 Bitmap (*.xbm)";
const QString XPM  = "X11 Pixmap (*.xpm)";
}

static QString getDescriptionFiltersImage(QString separator = "\n")
{
    QString formats = "";
    for(int i = 0; i < QImageWriter::supportedImageFormats().count(); i++)
    {
        QString format = QString(QImageWriter::supportedImageFormats().at(i)).toUpper();

        if(format == BMP)
            formats += UDescriptionFilters::BMP
                    + separator;

        else if(format == JPG)
            formats += UDescriptionFilters::JPG
                    + separator;

        else if(format == PNG)
            formats += UDescriptionFilters::PNG
                    + separator;

        else if(format == TIF)
            formats += UDescriptionFilters::TIF
                    + separator;

        else if(format == ICO)
            formats += UDescriptionFilters::ICO
                    + separator;

        else if(format == PPM)
            formats += UDescriptionFilters::PPM
                    + separator;

        else if(format == XBM)
            formats += UDescriptionFilters::XBM
                    + separator;

        else if(format == XPM)
            formats += UDescriptionFilters::XPM
                    + separator;
    }
    return formats;
}
}// namespace UFormatsImage

static QString getStringHexColorForStyleSheet(QColor color)
{
    QString hexColor = QString::number(color.rgb(), 16);

    return hexColor.remove(0, 2);
}
static QColor getColorFromHexColorForStyleSheet(QString hexColor)
{
    QByteArray rgb;
    rgb.append(hexColor);

    return QColor((QRgb)rgb.toInt(0, 16));
}

static QString absolutePathToTheFolderRemoteNotes()
{
    return QApplication::applicationDirPath()
            + QDir::separator()
            + remoteLocationNameOfTheFolderNotes;
}
static QString absolutePathToTheConfigurationProgram()
{
    return QApplication::applicationDirPath()
            + QDir::separator()
            + nameConfigFileProgram;
}
static QString absolutePathToTheFolderLanguages()
{
    return QApplication::applicationDirPath()
            + QDir::separator()
            + nameOfTheFolderLocationLanguageFiles;
}
static QString absolutePathToTheFolderNotes()
{
    return QApplication::applicationDirPath()
            + QDir::separator()
            + nameOfTheFolderLocationNotes;
}
static QString absolutePathToTheFolderLocationLanguageToAutocomplete()
{
    return QApplication::applicationDirPath()
            + QDir::separator()
            + nameOfTheFolderLocationLanguageToAutocomplete;
}
static QString absolutePathToTheFolderStyles()
{
    return QApplication::applicationDirPath()
            + QDir::separator()
            + nameOfTheFolderStyles;
}
static QString absolutePathToTheFolderFonts()
{
    return QApplication::applicationDirPath()
            + QDir::separator()
            + nameOfTheFolderFonts;
}

static qint32 intRandom(qint32 A, qint32 B)
{
    return qrand() % (B - A + 1) + A;
}

static QString currentDataTime(QString formatData = "d.MM.yy",
                               QString formatTime = "hh.mm.ss.zzz",
                               QString stringBetweenTheDateAndTime = " ")
{
    QDateTime dataTime = QDateTime::currentDateTime();

    return dataTime.date().toString(formatData)
            + stringBetweenTheDateAndTime
            + dataTime.time().toString(formatTime);
}

static int getIntKeyA_Z(QString key)
{
    if(key == "A")
        return Qt::Key_A;

    else if(key == "B")
        return Qt::Key_B;

    else if(key == "C")
        return Qt::Key_C;

    else if(key == "D")
        return Qt::Key_D;

    else if(key == "E")
        return Qt::Key_E;

    else if(key == "F")
        return Qt::Key_F;

    else if(key == "G")
        return Qt::Key_G;

    else if(key == "H")
        return Qt::Key_H;

    else if(key == "I")
        return Qt::Key_I;

    else if(key == "J")
        return Qt::Key_J;

    else if(key == "K")
        return Qt::Key_K;

    else if(key == "L")
        return Qt::Key_L;

    else if(key == "M")
        return Qt::Key_M;

    else if(key == "N")
        return Qt::Key_N;

    else if(key == "O")
        return Qt::Key_O;

    else if(key == "P")
        return Qt::Key_P;

    else if(key == "Q")
        return Qt::Key_Q;

    else if(key == "R")
        return Qt::Key_R;

    else if(key == "S")
        return Qt::Key_S;

    else if(key == "T")
        return Qt::Key_T;

    else if(key == "U")
        return Qt::Key_U;

    else if(key == "V")
        return Qt::Key_V;

    else if(key == "W")
        return Qt::Key_W;

    else if(key == "X")
        return Qt::Key_X;

    else if(key == "Y")
        return Qt::Key_Y;

    else if(key == "Z")
        return Qt::Key_Z;

    else
        return 0x0;
}

static QString getStrKeyF1_F12(int key)
{
    switch(key)
    {
    case Qt::Key_F1:
        return "F1";
    case Qt::Key_F2:
        return "F2";
    case Qt::Key_F3:
        return "F3";
    case Qt::Key_F4:
        return "F4";
    case Qt::Key_F5:
        return "F5";
    case Qt::Key_F6:
        return "F6";
    case Qt::Key_F7:
        return "F7";
    case Qt::Key_F8:
        return "F8";
    case Qt::Key_F9:
        return "F9";
    case Qt::Key_F10:
        return "F10";
    case Qt::Key_F11:
        return "F11";
    case Qt::Key_F12:
        return "F12";
    default:
        return "";
    }
}
static QString getStrKeyA_Z(int key)
{
    switch(key)
    {
    case Qt::Key_A:
        return "A";
    case Qt::Key_B:
        return "B";
    case Qt::Key_C:
        return "C";
    case Qt::Key_D:
        return "D";
    case Qt::Key_E:
        return "E";
    case Qt::Key_F:
        return "F";
    case Qt::Key_G:
        return "G";
    case Qt::Key_H:
        return "H";
    case Qt::Key_I:
        return "I";
    case Qt::Key_J:
        return "J";
    case Qt::Key_K:
        return "K";
    case Qt::Key_L:
        return "L";
    case Qt::Key_M:
        return "M";
    case Qt::Key_N:
        return "N";
    case Qt::Key_O:
        return "O";
    case Qt::Key_P:
        return "P";
    case Qt::Key_Q:
        return "Q";
    case Qt::Key_R:
        return "R";
    case Qt::Key_S:
        return "S";
    case Qt::Key_T:
        return "T";
    case Qt::Key_U:
        return "U";
    case Qt::Key_V:
        return "V";
    case Qt::Key_W:
        return "W";
    case Qt::Key_X:
        return "X";
    case Qt::Key_Y:
        return "Y";
    case Qt::Key_Z:
        return "Z";
    default:
        return "";
    }
}
static QString getStrOtherKeys(int key)
{
    switch(key)
    {
    case Qt::Key_Escape:
        return "Escape";
    case Qt::Key_Left:
        return "Left";
    case Qt::Key_Up:
        return "Up";
    case Qt::Key_Right:
        return "Right";
    case Qt::Key_Down:
        return "Down";
    case Qt::Key_Home:
        return "Home";
    case Qt::Key_End:
        return "End";
    case Qt::Key_Insert:
        return "Insert";
    case Qt::Key_Delete:
        return "Delete";
    case Qt::Key_PageUp:
        return "Page Up";
    case Qt::Key_PageDown:
        return "Page Down";
    case Qt::Key_Space:
        return "Space";
    case Qt::Key_Return:
    case Qt::Key_Enter:
        return "Enter";
    case Qt::Key_Backspace:
        return "Backspace";
    case Qt::Key_Clear:
        return "Clear";
    default:
        return "";
    }
}
static QString getStrDigitKeys(int key)
{
    switch(key)
    {
    case Qt::Key_0:
        return "0";
    case Qt::Key_1:
        return "1";
    case Qt::Key_2:
        return "2";
    case Qt::Key_3:
        return "3";
    case Qt::Key_4:
        return "4";
    case Qt::Key_5:
        return "5";
    case Qt::Key_6:
        return "6";
    case Qt::Key_7:
        return "7";
    case Qt::Key_8:
        return "8";
    case Qt::Key_9:
        return "9";
    default:
        return "";
    }
}
static QString getStrDefaultMathOperationKeys(int key)
{
    switch(key)
    {
    case Qt::Key_Asterisk:
        return "*";
    case Qt::Key_Plus:
        return "+";
    case Qt::Key_Minus:
        return "-";
    case Qt::Key_Slash:
        return "/";
    case Qt::Key_Equal:
        return "=";
    default:
        return "";
    };
}


// ABSTRACT NOTE
const int topBorder         = 31;
const int bottomBorder      = 12;
const int rightBorder       = 12;

const int sizeButtonOnTitle = 22;
// ABSTRACT NOTE

const int sizePushButton      = 25;
const int sizePButtonToExtendedContextMenu     = 75;

const int heightPButtonToSelectPageOnManager   = 45;
const int minWidthPButtonToSelectPageOnManager = 120;

const int minimalWidthNote  = 130;
const int minimalHeightNote = 160;

static int widthDesktop(){ return qApp->desktop()->width(); }
static int heightDesktop(){ return qApp->desktop()->height(); }

static int maximalWidthNote(){ return widthDesktop(); }
static int maximalHeightNote(){ return heightDesktop(); }

// ABOUT
const int maxWidthAbout = 518;
const int maxHeightAbout = 378;

const int minWidthAbout = 490;
const int minHeightAbout = 211;

const QSize maxSizeAbout(maxWidthAbout, maxHeightAbout);
const QSize minSizeAbout(minWidthAbout, minHeightAbout);
// ABOUT

/*
Параметр KLID   Описание
"00000407"      Немецкий (стандартный)
"00000409"      Английский (США)
"0000040C"      Французский (стандартный)
"0000040D"      Финский
"00000410"      Итальянский
"00000415"      Польский
"00000419"      Русский
"00000422"      Украинский
"00000423"      Белорусский
"00000425"      Эстонский
"00000426"      Латвийский
"00000427"      Литовский
*/

const int hexKeyLanguageRussian = 0x419;
const int hexKeyLanguageEnglish = 0x409;

// COMPLETER
static QAbstractItemModel *modelFromFile(const QString& fileName,
                                         QCompleter *completer)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly))
        return new QStringListModel(completer);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif

    QStringList words;

    if(!file.atEnd())
    {
        QByteArray codec = file.readLine();
        QTextCodec::setCodecForTr(QTextCodec::codecForName(codec));
    }

    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            words << QObject::tr(line.trimmed());
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    return new QStringListModel(words, completer);
}
static QString getTheCurrentLanguageKeyboardLayouts()
{
    QString name = "";

#ifdef Q_WS_WIN
    switch(LOWORD(GetKeyboardLayout(0)))
    {
    case hexKeyLanguageRussian:
        name = "Russian";
        break;
    case hexKeyLanguageEnglish:
        name = "English";
        break;
    }
#endif

    return name;
}
// COMPLETER


const short minimalOpacity  = 20;
const short maximalOpacity  = 100;

const int ksSelectAll        = Qt::CTRL + Qt::Key_A;

const int ksCut              = Qt::CTRL + Qt::Key_X;
const int ksCopy             = Qt::CTRL + Qt::Key_C;
const int ksPaste            = Qt::CTRL + Qt::Key_V;

const int ksSettings         = Qt::CTRL + Qt::Key_S;

const int ksHide             = Qt::CTRL + Qt::SHIFT + Qt::Key_H;
const int ksLock             = 0;
const int ksUnlock           = 0;

const int ksMoveToRecycleBin = Qt::CTRL + Qt::Key_R;
const int ksDuplicate        = Qt::CTRL + Qt::Key_D;

const int ksOpen             = Qt::CTRL + Qt::Key_O;
const int ksSaveAs           = Qt::CTRL + Qt::SHIFT + Qt::Key_S;

const int ksPrint            = Qt::CTRL + Qt::Key_P;
const int ksPreviewPrint     = Qt::CTRL + Qt::SHIFT + Qt::Key_P;
const int ksFind             = Qt::CTRL + Qt::Key_F;
const int ksReplace          = Qt::CTRL + Qt::Key_H;
const int ksTitle            = Qt::CTRL + Qt::SHIFT + Qt::Key_T;

enum
{
    indexColumnTitle,
    indexColumnVisible,
    indexColumnLock,
    indexColumnDateLastChange,
    indexColumnDateOfCreating,
    indexColumnDateOfLastRemoval,
    indexColumnDateOfLastRestore,
    indexColumnCountTextSymbols,
    indexColumnCountTextLines,

    countColumns,

    indexMainColumn = indexColumnTitle
};

const int pointerOnDataItemNote       = 32;
const int pointerOnDataColumnVisible  = 33;
const int pointerOnDataColumnLock     = 34;

const int expectedWidthOfSettingsNote = 398;

const int primaryKeyTableNotes = 10;
const int primaryKeyTableRecycleBin = 20;

const QString IconLogo36x36             = ":/Logo36x36Icon";
const QString IconLogo                  = ":/LogoIcon";

const QString IconCut                   = ":/CutIcon";
const QString IconCopy                  = ":/CopyIcon";
const QString IconPaste                 = ":/PasteIcon";
const QString IconSelectAll             = ":/SelectAllIcon";

const QString IconUnlock                = ":/UnlockIcon";
const QString IconUnlockAll             = ":/UnlockAllIcon";
const QString IconLock                  = ":/LockIcon";
const QString IconLockAll               = ":/LockAllIcon";
const QString IconDuplicate             = ":/DuplicateIcon";

const QString IconShow                  = ":/ShowIcon";
const QString IconHide                  = ":/HideIcon";
const QString IconShowAll               = ":/ShowAllIcon";
const QString IconHideAll               = ":/HideAllIcon";

const QString IconBold                  = ":/TextBoldIcon";
const QString IconItalic                = ":/TextItalicIcon";

const QString IconUnderline             = ":/TextUnderlineIcon";
const QString IconStrikeOut             = ":/TextStrikeIcon";
const QString IconOverline              = ":/TextOverlineIcon";

const QString IconAlignLeft             = ":/AlignmentLeftIcon";
const QString IconAlignCenter           = ":/AlignmentCenterIcon";
const QString IconAlignRight            = ":/AlignmentRightIcon";
const QString IconAlignJustify          = ":/AlignmentJustifyIcon";

const QString IconTextColor             = ":/TextColorTextIcon";
const QString IconBackgroundColor       = ":/TextBackgroundColorIcon";

const QString IconUpperCaseSensitive    = ":/UpperCaseSensitiveIcon";
const QString IconLowerCaseSensitive    = ":/LowerCaseSensitiveIcon";
const QString IconNegativeCaseSensitive = ":/NegativeCaseSensitiveIcon";

const QString IconEditor                = ":/EditorIcon";
const QString IconRecycleBin_Empty      = ":/RecycleBinEmptyIcon";
const QString IconRecycleBin_Full       = ":/RecycleBinFullIcon";

const QString IconSettings              = ":/SettingsIcon";
const QString IconSettingsProgram       = IconSettings;
const QString IconSettingsNote          = IconSettingsProgram;
const QString IconSettingsManager       = IconSettingsProgram;

const QString IconHelp                  = ":/HelpIcon";
const QString IconManager               = ":/ManagerIcon";
const QString IconQuit                  = ":/QuitIcon";

const QString IconAdd                   = ":/AddIcon";
const QString IconDelete                = ":/DeleteIcon";
const QString IconDeleteAll             = ":/DeleteAllIcon";
const QString IconRestore               = ":/RestoreIcon";
const QString IconRestoreAll            = ":/RestoreAllIcon";
const QString IconOpen                  = ":/OpenIcon";
const QString IconSaveAs                = ":/SaveAsIcon";
const QString IconClear                 = ":/ClearIcon";

const QString IconPassword              = ":/PasswordIcon";

const QString IconFigureStyleList       = ":/FigureListIcon";
const QString IconFigureStyleListDisc   = ":/DiscIcon";
const QString IconFigureStyleListCircle = ":/CircleIcon";
const QString IconFigureStyleListSquare = ":/SquareIcon";

const QString IconOrderedStyleList           = ":/OrderedListIcon";
const QString IconOrderedStyleListDecimal    = ":/DecimalIcon";
const QString IconOrderedStyleListAlphaLower = ":/AlphaLowerIcon";
const QString IconOrderedStyleListAlphaUpper = ":/AlphaUpperIcon";
const QString IconOrderedStyleListRomanLower = ":/RomanLowerIcon";
const QString IconOrderedStyleListRomanUpper = ":/RomanUpperIcon";

const QString IconAllNotes              = ":/AllNotesIcon";

const QString IconSplashIcon            = ":/SplashIcon";

const QString IconOpenDictionary        = ":/OpenDictionaryIcon";
const QString IconClosedDictionary      = ":/CloseDictionaryIcon";

const QString IconPreviewFilePrint      = ":/PreviewFilePrintIcon";
const QString IconFilePrint             = ":/FilePrintIcon";

const QString IconFind                  = ":/FindIcon";
const QString IconReplace               = ":/ReplaceIcon";
const QString IconResultSearch          = ":/ResultSearchIcon";

const QString IconGeneral               = ":/GeneralIcon";

const QString IconColor                 = ":/ColorIcon";
const QString IconTitle                 = ":/TitleIcon";
const QString IconOpacity               = ":/OpacityIcon";
const QString IconOpacity_100           = ":/Opacity_100Icon";
const QString IconOpacity_90            = ":/Opacity_90Icon";
const QString IconOpacity_80            = ":/Opacity_80Icon";
const QString IconOpacity_70            = ":/Opacity_70Icon";
const QString IconOpacity_60            = ":/Opacity_60Icon";
const QString IconOpacity_50            = ":/Opacity_50Icon";
const QString IconOpacity_40            = ":/Opacity_40Icon";
const QString IconOpacity_30            = ":/Opacity_30Icon";
const QString IconOpacity_20            = ":/Opacity_20Icon";

const QString IconSingleUnderline         = ":/SingleUnderlineIcon";
const QString IconWaveUnderline           = ":/WaveUnderlineIcon";
const QString IconDotLineUnderline        = ":/DotLineIcon";
const QString IconDashUnderline           = ":/DashUnderlineIcon";
const QString IconDashDotLineUnderline    = ":/DashDotLineIcon";
const QString IconDashDotDotLineUnderline = ":/DashDotDotLineIcon";

const QString IconSubScript               = ":/TextSubScriptIcon";
const QString IconSuperScript             = ":/TextSuperScriptIcon";

const QString IconClearFromFormatting     = ":/ClearFromFormattingIcon";

const QString IconIncreaseFontSize        = ":/IncreaseFontSizeIcon";
const QString IconDecreaseFontSize        = ":/DecreaseFontSizeIcon";

const QString IconClearTextEnter        = ":/ClearTextEnterIcon";
const QString IconClearTextLeave        = ":/ClearTextLeaveIcon";

const QString strDefaultLanguageFile      = ":/resource/russian.notelng";


enum UTypesOfCaseSensitive
{
    UpperCaseSensitive,
    LowerCaseSensitive,
    NegativeCaseSensitive
};

static void showForegroundWindow(QWidget *widget)
{
    widget->show();

#ifdef Q_WS_WIN
    // winapi:
    // Как утверждает документация, SetForegroundWindow работает как нам нужно,
    // только если вызывающий её процесс является активным.
    // А активен тот процесс, чей поток обрабатывает пользовательский ввод.
    // Оказывается, наш процесс может "прикинуться" активным, подключив свой
    // поток к обработке пользовательского ввода. Это осуществляется при
    // помощи функции AttachThreadInput. После вызова SetForegroundWindow
    // можно будет отключиться от чужого потока, используя ту же функцию.
    // http://www.rsdn.ru/article/qna/ui/wndsetfg.xml
    HWND hCurrWnd  = GetForegroundWindow();
    DWORD iMyTID   = GetCurrentThreadId();
    DWORD iCurrTID = GetWindowThreadProcessId(hCurrWnd, 0);

    AttachThreadInput(iMyTID,
                      iCurrTID,
                      true);

    SetForegroundWindow(widget->winId());

    AttachThreadInput(iMyTID,
                      iCurrTID,
                      false);
#endif
}

static bool qStringIsValue(QString string)
{
    return (!string.isEmpty()
            && !string.isNull());
}

#endif // GLOBALS_H
