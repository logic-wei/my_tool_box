#ifndef NOTEPROGRAMHEADER_H
#define NOTEPROGRAMHEADER_H

#include <cstring>
#include <elf.h>

#include "programheader.h"

class NoteProgramHeader : public ProgramHeader
{
public:
    NoteProgramHeader();
};

#endif // NOTEPROGRAMHEADER_H
