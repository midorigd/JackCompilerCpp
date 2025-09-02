#include "utils.hpp"

#include <iostream>
#include <regex>

namespace Compiler {

bool isValidArguments(const int argc, const char* const argv[]) {
    return argc == 2 || (argc == 3 && std::string(argv[2]) == "-d");
}

void displayUsage() {
    std::cerr << "Usage: bin/JackCompiler <dirname OR filename.vm> [-d]\n";
    std::cerr << "   -d: Enables symbol table debug file\n";
}

void removeComments(std::string& text) {
    std::regex singleLinePattern("//.*");
    std::regex multiLinePattern(R"(/\*[\s\S]*?\*/)");

    text = std::regex_replace(text, singleLinePattern, "");
    text = std::regex_replace(text, multiLinePattern, "");
}

bool strIsDigit(const std::string& str) {
    for (size_t i = 0; i < str.length(); ++i) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

}
