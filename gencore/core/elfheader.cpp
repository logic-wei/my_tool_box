#include "elfheader.h"

const string ElfHeader::TAG = "ElfHeader";

ElfHeader::ElfHeader()
{
    memset((void *) &m_ehdr, 0x0, sizeof(m_ehdr));

    m_ehdr.e_ident[EI_MAG0]     = ELFMAG0;
    m_ehdr.e_ident[EI_MAG1]     = ELFMAG1;
    m_ehdr.e_ident[EI_MAG2]     = ELFMAG2;
    m_ehdr.e_ident[EI_MAG3]     = ELFMAG3;
    m_ehdr.e_ident[EI_CLASS]    = ELFCLASS32;
    m_ehdr.e_ident[EI_DATA]     = ELFDATA2LSB;
    m_ehdr.e_ident[EI_VERSION]  = EV_CURRENT;
    m_ehdr.e_ident[EI_OSABI]    = ELFOSABI_NONE;

    m_ehdr.e_type       = ET_CORE;
    m_ehdr.e_machine    = EM_ARM;
    m_ehdr.e_version    = EV_CURRENT;
    m_ehdr.e_phoff      = sizeof(m_ehdr);
    m_ehdr.e_flags      = 0;
    m_ehdr.e_ehsize     = sizeof(m_ehdr);
}

ElfHeader::~ElfHeader()
{

}

int ElfHeader::dump(fstream &out)
{
    Log::info(TAG, "dump offset: " + to_string(out.tellp()));
    out.write((char *)&m_ehdr, sizeof(m_ehdr));
    return 0;
}

int ElfHeader::size()
{
    return sizeof(m_ehdr);
}
