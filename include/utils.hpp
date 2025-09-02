#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Compiler {

bool isValidArguments(const int argc, const char* const argv[]);
void displayUsage();

void removeComments(std::string& text);
bool strIsDigit(const std::string& str);

}

#endif
