#ifndef PRSTATUS_H
#define PRSTATUS_H

#include <elf.h>
#include <cstring>

#include "dumper.h"
#include "elfcore.h"
#include "log.h"

class Prstatus : public Dumper
{
public:
                    Prstatus();
                    ~Prstatus();
    int             dump(fstream &out) override;
    int             size() override;
    elf_prstatus &  getRaw() {return m_prstatus;}

private:
    static const string TAG;

    elf_prstatus m_prstatus;
};

#endif // PRSTATUS_H
