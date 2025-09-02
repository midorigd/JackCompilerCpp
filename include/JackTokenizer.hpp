#ifndef JACKTOKENIZER_H
#define JACKTOKENIZER_H

#include "CompilerResources.hpp"

#include <deque>
#include <filesystem>
#include <regex>

namespace Compiler {

namespace fs = std::filesystem;

class JackTokenizer {
public:
    JackTokenizer(const fs::path& infilePath);
    const Token& nextToken() const;
    bool hasMoreTokens() const;
    const Token& advance();
    const Token& peekSecond() const;

private:
    static std::regex tokenPattern;

    std::string data;
    Token currToken;
    std::deque<Token> tokens;
    std::unordered_map<TokenType, std::function<void()>> tokenizeMap;
    
    static TokenType getTokenType(const std::string& tokenVal);
    static Token tokenize(const std::string& tokenVal);
    void matchTokens();
};

}

#endif
