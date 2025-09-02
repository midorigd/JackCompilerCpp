#include "JackCompiler.hpp"
#include "utils.hpp"

#include <string>

int main(int argc, char* argv[]) {
    if (!Compiler::isValidArguments(argc, argv)) {
        Compiler::displayUsage();
        exit(1);
    }

    std::string sourceFile { argv[1] };
    bool debugMode { argc == 3 };

    Compiler::JackCompiler compiler;
    compiler.compile(sourceFile, debugMode);

    return 0;
}

/**
 * Exit codes:
 * 1: Incorrect argv usage
 * 2: File not opened
 */
