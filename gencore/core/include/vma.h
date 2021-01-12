#ifndef VMA_H
#define VMA_H

#include <string>
#include <fstream>
#include <zlib.h>

#include "dumper.h"
#include "base64.h"
#include "log.h"


class Vma : public Dumper
{
public:
    enum {
        CHUNK=16384
    };
                Vma(string &zbase64, uint32_t vaddr, uint32_t size, uint32_t flags,
                    const string &name="defaut");
                ~Vma();
    int         dump(fstream &out) override;
    int         size() override;

    uint32_t    getVaddr() {return m_vaddr;}
    uint32_t    getFlags() {return m_flags;}
    string &    getName()  {return m_name;}
private:
    static const string TAG;

    string      m_name;
    string      m_zbase64;
    uint32_t    m_vaddr;
    uint32_t    m_size;
    uint32_t    m_flags;
};

#endif // VMA_H
