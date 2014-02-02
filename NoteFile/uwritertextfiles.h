#ifndef UWRITERTEXTFILES_H
#define UWRITERTEXTFILES_H

#include <QtGui>
#include <QtCore>

#include "Globals.h"

#include "Note.h"

class UWriterTextFiles
{
public:
    bool writeOut(Note *usedNote,
                  QString fileName);
};

#endif // UWRITERTEXTFILES_H
