#include "aligningdumper.h"

const string AligningDumper::TAG = "AligningDumper";

AligningDumper::AligningDumper(int align):
    m_align(align),
    m_size(0)
{

}

int AligningDumper::dump(fstream &out)
{
    Log::info(TAG, "dump offset: " + to_string(out.tellp()));

    m_size = dumpAlign(out, m_align);

    return 0;
}

int AligningDumper::size()
{
    return m_size; ///< can only be known after dump
}
