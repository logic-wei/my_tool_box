#include "programheader.h"

const string ProgramHeader::TAG = "ProgramHeader";

ProgramHeader::ProgramHeader()
{
    memset((void *) &m_phdr, 0, sizeof(m_phdr));
}

ProgramHeader::~ProgramHeader()
{

}

int ProgramHeader::dump(fstream &out)
{
    Log::info(TAG, "dump offset: " + to_string(out.tellp()));
    out.write((char *) &m_phdr, sizeof(m_phdr));

    return 0;
}

int ProgramHeader::size()
{
    return sizeof(m_phdr);
}
