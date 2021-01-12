#include "dumper.h"

int Dumper::dumpAlign(fstream &out, int align)
{
    unsigned int mod = out.tellp() & (align - 1);
    int cout = 0;
    if (align & (align - 1))
        return 0;
    for (unsigned int i = 0; i < align - mod && mod != 0; i += 1) {
        char zero = 0;
        out.write(&zero, 1);
        cout += 1;
    }
    return cout;
}

int Dumper::skipSize(int size, int align)
{
    unsigned int mod = size & (align - 1);
    if (align & (align - 1) || mod == 0)
        return 0;
    return align - mod;
}

