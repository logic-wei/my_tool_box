#ifndef ALIGNINGDUMPER_H
#define ALIGNINGDUMPER_H

#include "dumper.h"
#include "log.h"

class AligningDumper : public Dumper
{
public:
        AligningDumper(int align);
        ~AligningDumper() {};
    int dump(fstream &out) override;
    /**
     * @brief size Can only be known after dump.
     * @return How many bytes were writen into file after dump.
     */
    int size() override;

private:
    static const string TAG;
    int m_align;
    int m_size;
};

#endif // ALIGNINGDUMPER_H
