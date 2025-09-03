#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Compiler {

/**
 * Validates the command-line arguments and flags provided in the executable call.
 */
bool isValidArguments(const int argc, const char* const argv[]);

/**
 * Displays an error message showing the correct usage of the compiler.
 */
void displayUsage();

/**
 * Removes C++/Java-style single- and multi-line comments from the provided string in place.
 */
void removeComments(std::string& text);

/**
 * Returns whether or not every character in the provided string is a numeric character.
 */
bool strIsDigit(const std::string& str);

}

#endif
