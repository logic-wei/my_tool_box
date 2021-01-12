#include "noteprogramheader.h"

NoteProgramHeader::NoteProgramHeader():
    ProgramHeader()
{
    m_phdr.p_type = PT_NOTE;
}
