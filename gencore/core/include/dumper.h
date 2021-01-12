#ifndef _DUMPER_H
#define _DUMPER_H

#include <fstream>
#include <iostream>

using namespace std;

class Dumper
{
public:
    /**
     * @brief ~Dumper Must be implemented.
     */
    virtual ~Dumper() {};
    /**
     * @brief dump Dump myself and keep aligned.
     * @param out
     */
    virtual int dump(fstream &out) = 0;
    /**
     * @brief size Size of myself with aligned data.
     * @return
     */
    virtual int size() = 0;
    /**
     * @brief dumpAlign Auto align.
     * @param out
     * @param align
     */
    int dumpAlign(fstream &out, int align);
    /**
     * @brief skipSize Calculate how many bytes to skip for aligning.
     * @param size
     * @param align
     * @return
     */
    static int skipSize(int size, int align);
};


#endif
