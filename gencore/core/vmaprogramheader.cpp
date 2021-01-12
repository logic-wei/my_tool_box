#include "vmaprogramheader.h"

VmaProgramHeader::VmaProgramHeader(const string &name):
    m_name(name)
{
    m_phdr.p_type = PT_LOAD;
    m_phdr.p_paddr = 0;
    m_phdr.p_align = 0;
}

void VmaProgramHeader::setOffset(uint32_t offset)
{
    m_phdr.p_offset = offset;
}

void VmaProgramHeader::setVirtAddr(uint32_t addr)
{
    m_phdr.p_vaddr = addr;
}

void VmaProgramHeader::setFileSize(uint32_t size)
{
    m_phdr.p_filesz = size;
}
void VmaProgramHeader::setMemSize(uint32_t size)
{
    m_phdr.p_memsz = size;
}

void VmaProgramHeader::setFlags(uint32_t flags)
{
    m_phdr.p_flags = flags;
}

uint32_t VmaProgramHeader::getOffset()
{
    return m_phdr.p_offset;
}

uint32_t VmaProgramHeader::getVirtAddr()
{
    return m_phdr.p_vaddr;
}

uint32_t VmaProgramHeader::getFileSize()
{
    return m_phdr.p_filesz;
}

uint32_t VmaProgramHeader::getMemSize()
{
    return m_phdr.p_memsz;
}

uint32_t VmaProgramHeader::getFlags()
{
    return m_phdr.p_flags;
}
