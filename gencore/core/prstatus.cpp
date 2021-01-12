#include "prstatus.h"

const string Prstatus::TAG = "Prstatus";

Prstatus::Prstatus()
{
    memset((void *) &m_prstatus, 0, sizeof(m_prstatus));
}

Prstatus::~Prstatus()
{

}

int Prstatus::dump(fstream &out)
{
    Log::info(TAG, "dump offset: " + to_string(out.tellp()));
   
    out.write((char *)&m_prstatus, sizeof(m_prstatus));
    dumpAlign(out, 4);

    return 0;
}

int Prstatus::size()
{
    return sizeof(m_prstatus) + skipSize(sizeof(m_prstatus), 4);
}
