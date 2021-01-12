#include "note.h"

const string Note::TAG = "Note";

Note::Note(uint32_t type, const string &name, Dumper *noteData):
    m_noteData(noteData)
{
    m_name = name;
    m_noteData = noteData;

    m_nhdr.n_type = type;
    m_nhdr.n_namesz = m_name.size() + 1;
    m_nhdr.n_descsz = m_noteData->size();
}

Note::~Note()
{
    delete m_noteData;
}

int Note::dump(fstream &out)
{
    Log::info(TAG, "dump offset: " + to_string(out.tellp()));
    out.write((char *)&m_nhdr, sizeof(m_nhdr));
    out.write(m_name.data(), m_name.size());
    out.write("\0", 1);
    dumpAlign(out, 4);
    m_noteData->dump(out);

    return 0;
}

int Note::size()
{
    int headerSize = sizeof(m_nhdr);
    int nameSize = m_name.size() + 1 + skipSize(m_name.size() + 1, 4); ///< add 1 to include '\0'
    int dataSize = m_noteData->size();

    return headerSize + nameSize + dataSize;
}
