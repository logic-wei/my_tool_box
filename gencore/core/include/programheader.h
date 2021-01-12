#ifndef PROGRAMHEADER_H
#define PROGRAMHEADER_H

#include <elf.h>
#include <fstream>
#include <cstring>

#include "dumper.h"
#include "log.h"

class ProgramHeader : public Dumper
{
public:
                ProgramHeader();
                ~ProgramHeader();
    int         dump(fstream &out) override;
    int         size() override;
    void        setOffset(uint32_t offset)  {m_phdr.p_offset = offset;}
    void        setFileSize(uint32_t size)  {m_phdr.p_filesz = size;}
    uint32_t    getOffset()                 {return m_phdr.p_offset;}
    uint32_t    getFileSize()               {return m_phdr.p_filesz;}
    Elf32_Phdr &getRaw()                    {return m_phdr;}
protected:
    Elf32_Phdr m_phdr;
private:
    static const string TAG;
};

#endif // PROGRAMHEADER_H
