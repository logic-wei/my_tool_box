#ifndef VMAPROGRAMHEADER_H
#define VMAPROGRAMHEADER_H

#include <string>
#include <elf.h>

#include "programheader.h"

class VmaProgramHeader : public ProgramHeader
{
public:
    enum Flag {
        FLAG_R = 0x4,
        FLAG_W = 0x2,
        FLAG_X = 0x1
    };
            VmaProgramHeader(const string &name);
    void    setOffset(uint32_t offset);
    void    setVirtAddr(uint32_t addr);
    void    setFileSize(uint32_t size);
    void    setMemSize(uint32_t size);
    void    setFlags(uint32_t flags);
    string &getName() {return m_name;}

    uint32_t getOffset();
    uint32_t getVirtAddr();
    uint32_t getFileSize();
    uint32_t getMemSize();
    uint32_t getFlags();

private:
    string m_name;
};

#endif // VMAPROGRAMHEADER_H
