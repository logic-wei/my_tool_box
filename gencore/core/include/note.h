#ifndef NOTE_H
#define NOTE_H

#include <string>
#include <elf.h>

#include "dumper.h"
#include "log.h"

using namespace std;

class Note : public Dumper
{
public:
        Note(uint32_t type, const string &name, Dumper *noteData);
        ~Note() override;
    int dump(fstream &out) override;
    int size() override;
public:
    static const string TAG;

    Elf32_Nhdr  m_nhdr;
    string      m_name;
    Dumper      *m_noteData;
};

#endif // NOTE_H
