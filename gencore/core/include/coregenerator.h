#ifndef COREGENERATOR_H
#define COREGENERATOR_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <map>

#include "json/json.h"
#include "dumper.h"
#include "elfheader.h"
#include "programheader.h"
#include "noteprogramheader.h"
#include "note.h"
#include "prstatus.h"
#include "vmaprogramheader.h"
#include "vma.h"
#include "aligningdumper.h"
#include "log.h"

using namespace std;

class CoreGenerator
{
public:
    enum Format {
        FormatBase64, FormatZBase64
    };
            CoreGenerator(const string &logPath, const string &corePath, Format format=FormatZBase64);
            ~CoreGenerator();
    int     generate();

private:
    int     parseLog();
    void    trim(string &s);
    int    parseMeta();
    template<class T>
    uint32_t calcDumpersSize(const vector<T *> &dumpers);

private:
    enum State {
        StateStart, StateCoreDump, StateMetaDump, StateMemDump, StateEnd, StateFailed
    };
    static const string TAG;

    Format              m_format;
    string              m_logPath;
    string              m_corePath;
    ifstream            m_logFile;
    fstream             m_coreFile;
    string              m_metaJson;
    map<string, string> m_memZBase64Map;

    Json::Value         m_metaRoot;
    vector<Dumper *>    m_dumpers;
};

#endif // COREGENERATOR_H
