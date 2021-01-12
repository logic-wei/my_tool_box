#include "vma.h"

const string Vma::TAG = "Vma";

Vma::Vma(string &zbase64, uint32_t vaddr, uint32_t size, uint32_t flags, const string &name):
    m_name(name),
    m_zbase64(zbase64),
    m_vaddr(vaddr),
    m_size(size),
    m_flags(flags)
{

}

Vma::~Vma()
{

}

int Vma::dump(fstream &out)
{
    int ret = 0;
    string zbin = base64_decode(m_zbase64);
    unsigned char uzBuff[CHUNK];
    z_stream stream;

    Log::info(TAG, "[" + m_name + "] dump offset: " + to_string(out.tellp()));

    stream.zalloc   = Z_NULL;
    stream.zfree    = Z_NULL;
    stream.opaque   = Z_NULL;
    stream.avail_in = 0;
    stream.next_in  = Z_NULL;

    ret = inflateInit(&stream);
    if (ret != Z_OK) {
        Log::err(TAG, "inflateInit error.");
        return -1;
    }

    stream.avail_in = zbin.size();
    stream.next_in = (unsigned char *) zbin.c_str();
   
    do {
        stream.avail_out = CHUNK;
        stream.next_out = uzBuff;
        ret = inflate(&stream, Z_NO_FLUSH);
        switch (ret) {
        case Z_NEED_DICT:
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
        case Z_STREAM_ERROR:
            inflateEnd(&stream);
            Log::err(TAG, "inflate error. ret=" + to_string(ret));
            return -1;
        }
        out.write((char *) uzBuff, CHUNK - stream.avail_out);

    } while (stream.avail_out == 0 && ret != Z_STREAM_END);

    inflateEnd(&stream);
    dumpAlign(out, 4096);

    return 0;
}

int Vma::size()
{
    return m_size + skipSize(m_size, 4096);
}
