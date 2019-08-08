// Little Endian byte order assumed for the target platform

#include <exception>
#include <stdexcept>
#include <string>
#include <vector>

#include "help.hpp"
#include "pack.hpp"
#include "version.hpp"

struct Args {
    bool printHelp = false;
    bool printVersion = false;
    std::vector<std::string> inputFiles;
    std::string outputFile;
};

class InvalidArgsException : public std::runtime_error {
public:
    InvalidArgsException() : std::runtime_error("") {}
};

static Args parseArgs(int argc, char** argv) {
    Args args;
    std::vector<std::string> argList;

    for (int i = 1; i < argc; i++) {
        argList.push_back(argv[i]);
    }

    for (auto& i : argList) {
        if (!i.empty() && i.at(0) == '-') {
            // Option
            if (i == "-h" || i == "--help") {
                args.printHelp = true;
            } else if (i == "-v" || i == "--version") {
                args.printVersion = true;
            }
        } else {
            // File
            args.inputFiles.push_back(i);
        }
    }

    // Last file is the output file. Rest are input files
    if (!args.inputFiles.empty()) {
        args.outputFile = args.inputFiles.back();
        args.inputFiles.pop_back();
    }

    if ((args.printHelp || args.printVersion) ||
        (!args.inputFiles.empty() && !args.outputFile.empty()))
    {
        return args;
    }

    throw InvalidArgsException();
}

int main(int argc, char** argv) {
    try {
        auto args = parseArgs(argc, argv);
        if (args.printHelp) {
            printf("%s", HelpText.c_str());
        } else if (args.printVersion) {
            printf("Resource packer v" VersionString "\n");
        } else {
            pack(args.inputFiles, args.outputFile);
        }
        return 0;
    } catch (InvalidArgsException& e) {
        fprintf(stderr, "Invalid arguments\n\n%s", HelpText.c_str());
        return 2;
    } catch (std::exception& e) {
        fprintf(stderr, "Error: %s\n", e.what());
    } catch (...) {
        fprintf(stderr, "Error: Unknown exception\n");
    }

    return 1;
}
