#ifndef _ELF_HEADER_H
#define _ELF_HEADER_H

#include <fstream>
#include <elf.h>
#include <cstring>

#include "dumper.h"
#include "log.h"

using namespace std;

class ElfHeader : public Dumper {
public:
            ElfHeader();
            ~ElfHeader();
    int     dump(fstream &out) override;
    int     size() override;
    void    setProgramHeaderEntrySize(uint32_t size)   {m_ehdr.e_phentsize = size;}
    void    setProgrameHeaderNum(uint16_t num)         {m_ehdr.e_phnum = num;}
    void    setSectionHeaderEntrySize(uint16_t size)   {m_ehdr.e_shentsize = size;}
    void    setSectionHeaderNum(uint16_t num)          {m_ehdr.e_shnum = num;}

    uint32_t getProgramHeaderEntrySize()    {return m_ehdr.e_phentsize;}
    uint16_t getProgrameHeaderNum()         {return m_ehdr.e_phnum;}
    uint16_t getSectionHeaderEntrySize()    {return m_ehdr.e_shentsize;}
    uint16_t getSectionHeaderNum()          {return m_ehdr.e_shnum;}
public:
    Elf32_Ehdr m_ehdr;
private:
    static const string TAG;
};

#endif
