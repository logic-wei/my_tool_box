#include "coregenerator.h"

const string CoreGenerator::TAG = "CoreGenerator";

CoreGenerator::CoreGenerator(const string &logPath, const string &corePath, Format format):
    m_format(format),
    m_logPath(logPath),
    m_corePath(corePath)
{

}

CoreGenerator::~CoreGenerator()
{

}


int CoreGenerator::generate()
{
    int                             offset = 0;
    int                             offsetIndex = 0;
    ElfHeader                       *elfHeader;
    NoteProgramHeader               *noteProHeader;
    vector<Note *>                  noteList;
    vector<VmaProgramHeader *>      vmaHeaderList;
    map<string, Vma *>              vmaMap;
    map<string, int>                vmaOffsetMap;
    AligningDumper                  *align4k = new AligningDumper(4096);

    if (parseLog() != 0) {
        Log::err(TAG, "Failed to parse log.");
        return -1;
    }
    if (parseMeta() != 0) {
        Log::err(TAG, "Failed to parse meta.");
        return -1;
    }
    m_coreFile.open(m_corePath, ios::binary|ios::out);

    ///< create headers and etc...
    elfHeader       = new ElfHeader();
    noteProHeader   = new NoteProgramHeader();

    for (auto task: m_metaRoot["tasks"]) {
        Prstatus *prstatus  = new Prstatus();
        Note *note          = new Note(NT_PRSTATUS, "CORE", prstatus);

        prstatus->getRaw().pr_pid           = task["id"].asUInt();
        prstatus->getRaw().pr_pgrp          = 0;
        prstatus->getRaw().pr_info.si_signo = 11;
        //prstatus->getRaw().pr_info.si_errno = 11;
        prstatus->getRaw().pr_reg.uregs[4]  = task["reg_r4"].asUInt();
        prstatus->getRaw().pr_reg.uregs[5]  = task["reg_r5"].asUInt();
        prstatus->getRaw().pr_reg.uregs[6]  = task["reg_r6"].asUInt();
        prstatus->getRaw().pr_reg.uregs[7]  = task["reg_r7"].asUInt();
        prstatus->getRaw().pr_reg.uregs[8]  = task["reg_r8"].asUInt();
        prstatus->getRaw().pr_reg.uregs[9]  = task["reg_r9"].asUInt();
        prstatus->getRaw().pr_reg.uregs[10] = task["reg_r10"].asUInt();
        prstatus->getRaw().pr_reg.uregs[11] = task["reg_fp"].asUInt();
        prstatus->getRaw().pr_reg.uregs[13] = task["reg_sp"].asUInt();
        prstatus->getRaw().pr_reg.uregs[14] = task.isMember("reg_lr") ? task["reg_lr"].asUInt() : 0;
        prstatus->getRaw().pr_reg.uregs[15] = task.isMember("reg_pc") ? task["reg_pc"].asUInt() : 0;
        prstatus->getRaw().pr_reg.uregs[16] = task.isMember("reg_cpsr") ? task["reg_cpsr"].asUInt() : 0;
        noteList.push_back(note);
    }

    for (auto vmaMeta: m_metaRoot["vma"]) {
        string              vmaName     = vmaMeta["name"].asString();
        uint32_t            vmaAddr     = vmaMeta["addr"].asUInt();
        uint32_t            vmaSize     = vmaMeta["size"].asUInt();
        VmaProgramHeader    *vmaHeader  = new VmaProgramHeader(vmaName);
        Vma                 *vma        = new Vma(m_memZBase64Map[vmaName], vmaAddr, vmaSize, 0, vmaName);

        vmaHeaderList.push_back(vmaHeader);
        vmaMap[vmaHeader->getName()] = vma;
        vmaHeader->setFlags(VmaProgramHeader::FLAG_R|VmaProgramHeader::FLAG_X);
        vmaHeader->setVirtAddr(vma->getVaddr());
        vmaHeader->setMemSize(vma->size());
        vmaHeader->setFileSize(vma->size());
        vmaHeader->getRaw().p_align = 4096;
    }

    ///< collect all the date and set some parameters...
    Log::info(TAG, "offset_" + to_string(offsetIndex++) + ": " + to_string(offset));

    m_dumpers.push_back(elfHeader);
    offset += elfHeader->size();
    Log::info(TAG, "offset_" + to_string(offsetIndex++) + ": " + to_string(offset));

    m_dumpers.push_back(noteProHeader);
    offset += noteProHeader->size();
    Log::info(TAG, "offset_" + to_string(offsetIndex++) + ": " + to_string(offset));

    for (auto vmaHeader: vmaHeaderList) {
        m_dumpers.push_back(vmaHeader);
        offset += vmaHeader->size();
        Log::info(TAG, "offset_" + to_string(offsetIndex++) + ": " + to_string(offset));
    }

    noteProHeader->getRaw().p_offset    = offset;
    noteProHeader->getRaw().p_filesz    = calcDumpersSize(noteList);
    noteProHeader->getRaw().p_align     = 0;

    for (auto note: noteList) {
        m_dumpers.push_back(note);
        offset += note->size();
        Log::info(TAG, "offset_" + to_string(offsetIndex++) + ": " + to_string(offset));
    }

    m_dumpers.push_back(align4k);
    offset += Dumper::skipSize(calcDumpersSize(m_dumpers), 4096);
    Log::info(TAG, "offset_" + to_string(offsetIndex++) + ": " + to_string(offset));
    for (auto vmaHeader: vmaHeaderList) {
        auto vma = vmaMap[vmaHeader->getName()];

        m_dumpers.push_back(vma);
        vmaOffsetMap[vmaHeader->getName()] = offset;
        offset += vma->size();
        Log::info(TAG, "offset_" + to_string(offsetIndex++) + ": " + to_string(offset));
    }

    ///< set some values
    elfHeader->setProgramHeaderEntrySize(sizeof(Elf32_Phdr));
    elfHeader->setProgrameHeaderNum(1 + vmaMap.size());

    for (auto vmaHeader: vmaHeaderList) {
        vmaHeader->setOffset(vmaOffsetMap[vmaHeader->getName()]);
    }

    ///< dump all the data
    for (auto dumper: m_dumpers) {
        dumper->dump(m_coreFile);
    }

    ///< release resource
    for (auto dumper: m_dumpers) {
        delete dumper;
    }
    m_dumpers.clear();
    m_coreFile.close();

    return 0;
}

int CoreGenerator::parseLog()
{
    regex   regCoreBegin("^\\*\\*\\* Beginning of core dump \\*\\*\\*$");
    regex   regCoreEnd("^\\*\\*\\* End of core dump \\*\\*\\*$");
    regex   regMetaBegin("^\\*\\*\\* Beginning of meta \\*\\*\\*$");
    regex   regMetaEnd("^\\*\\*\\* End of meta \\*\\*\\*$");
    regex   regMemBegin("^\\*\\*\\* Beginning of mem dump:(.*) \\*\\*\\*$");
    regex   regMemEnd("^\\*\\*\\* End of mem dump:(.*) \\*\\*\\*$");
    regex   regMemFailed("^\\*\\*\\* Failed to dump mem:(.*) \\*\\*\\*$");
    string  line;
    State   state   = StateStart;
    int     ret     = 0;
    string  memName;

    m_logFile.open(m_logPath, ios::in);

    while (!m_logFile.eof()) {
        smatch match;
        getline(m_logFile, line);

        switch (state) {
        case StateStart:
            if (regex_match(line, match, regCoreBegin)) {
                state = StateCoreDump;
            } else if (regex_match(line, match, regCoreEnd)) {
                state = StateEnd;
            }
            break;
        case StateCoreDump:
            if (regex_match(line, match, regMetaBegin)) {
                state = StateMetaDump;
            } else if (regex_match(line, match, regMemBegin)) {
                state = StateMemDump;
                memName = match[1].str();
                m_memZBase64Map[memName] = "";
            }
            break;
        case StateMetaDump:
            if (regex_match(line, match, regMetaEnd)) {
                state = StateCoreDump;
            } else {
                m_metaJson.append(line).append("\n");
            }
            break;
        case StateMemDump:
            if (regex_match(line, match, regMemEnd)) {
                trim(m_memZBase64Map[memName]);
                state = StateCoreDump;
            } else if (regex_match(line, match, regMemFailed)) {
                state = StateFailed;
            } else {
                m_memZBase64Map[memName].append(line);
            }
            break;
        default:
            break;
        }
        if (state == StateFailed) {
            ret = -1;
            break;
        }
    }

    m_logFile.close();

    return ret;
}

void CoreGenerator::trim(string &s)
{
    if (!s.empty()) {
        s.erase(0, s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
    }
}

int CoreGenerator::parseMeta()
{
    Json::Reader reader;

    if (m_metaJson.size() == 0) {
        Log::err(TAG, "No meta found!");
        return -1;
    }

    reader.parse(m_metaJson, m_metaRoot);

    return 0;
}

template<class T>
uint32_t CoreGenerator::calcDumpersSize(const vector<T *> &dumpers)
{
    uint32_t total = 0;

    for (auto dumper: dumpers) {
        total += dumper->size();
    }

    return total;
}
