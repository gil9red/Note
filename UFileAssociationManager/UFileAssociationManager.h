#ifndef UFILEASSOCIATIONMANAGER_H
#define UFILEASSOCIATIONMANAGER_H

#include <QtGlobal>
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QCoreApplication>
#include <QDebug>

#if defined(NTDDI_VISTA) || defined(_WIN32_WINNT_VISTA)
#    define WIN_VISTA_OR_LATER
#    if defined(NTDDI_WIN7) || defined(_WIN32_WINNT_WIN7)
#        define WIN_7_OR_LATER
#    endif
#endif

#include <Shlobj.h>
#ifdef WIN_VISTA_OR_LATER
#   include <Shobjidl.h>
#endif

#include "Globals.h"

const QString kOpenWithFormat("%1/shell/open/command/.");

class UFileAssociationManager
{
public:
    // extension may or may not have preceding dot: 'avi' and '.mp3' are both valid to pass as a parameter
    static bool isApplicationDefaultForExtension(const QString &extension);
    static void makeApplicationDefaultForExtension(const QString &extension);

    static QString progIdForExtension(const QString &extension);

private:
    // helpers
    static QString extensionWithDotFromExtension(const QString &extension);
    static QString executablePath();
    static QString executablePathWithParam();

    // editing registry
    static void registerProgID(const QString &extensionWithDot);
    static void registerApplication(const QString &extensionWithDot);
    static bool isRegisteredApplicationOverridenInFileExts(const QString &extensionWithDot, bool removeIfExists);
};

#endif // UFILEASSOCIATIONMANAGER_H
