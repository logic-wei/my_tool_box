#include <iostream>
#include <string>
#include <vector>

#include "cmdline.h"
#include "coregenerator.h"

using namespace std;

#define CONFIG_VERSION "v1.0"
#define CONFIG_AUTHOR "weipeng"


int main(int argc, char **argv) {
    cmdline::parser parser;
    string output;

    parser.footer("filename");
    parser.add<string>("output", 'o', "Specify the path to save core file.", false, "./core");
    parser.add("debug", 'd', "Enable debug mode.");
    parser.add("version", 'v', "Print the version.");

    parser.parse_check(argc, argv);

    if (parser.exist("version")) {
        cout << CONFIG_VERSION << " Written by " << CONFIG_AUTHOR << endl;
        cout << "BUILD TIME:" << __DATE__ << " " << __TIME__ << endl;
        return 0;
    }

    if (parser.exist("debug")) {
        Log::debug(true);
    }

    if (parser.rest().size() == 0) {
        cout << "No input file." << endl;
        return 0;
    }
    output = parser.get<string>("output");

    CoreGenerator coreGen(parser.rest()[0], output);

    if (coreGen.generate() == 0) {
        cout << "\033[32mFinished.\033[0m \nCoredump path: \033[4m" << output << "\033[0m" << endl;
    } else {
        cout << "\033[31mFailed to generate coredump.\033[0m" << endl;
    }

    return 0;
}
