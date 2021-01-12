#ifndef LOG_H
#define LOG_H

#include <string>
#include <iostream>

using namespace std;

class Log {
public:
    static void info(const string &log) {
        if (!s_debug)
            return;
        cout << "\033[1mINFO: \033[0m" << log << endl;
    }

    static void info(const string &tag, const string &log) {
        if (!s_debug)
            return;
        info("[" + tag + "] " + log);
    }

    static void err(const string &log) {
        if (!s_debug)
            return;
        cout << "\033[31mERROR: \033[0m" << log << endl;
    }

    static void err(const string &tag, const string &log) {
        if (!s_debug)
            return;
        err("[" + tag + "] " + log);
    }

    static void debug(bool on) {
        s_debug = on;
    }

private:
    static bool s_debug;
};

#endif // LOG_H
