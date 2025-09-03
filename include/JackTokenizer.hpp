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
    /**
     * Creates a new JackTokenizer module to read tokens from the provided file.
     */
    JackTokenizer(const fs::path& infilePath);

    /**
     * Peeks the next token from input without advancing to it and processing the token.
     */
    const Token& nextToken() const;

    /**
     * Returns whether or not there are more tokens in input to read and process.
     */
    bool hasMoreTokens() const;

    /**
     * Reads the next token from input and processes it.
     */
    const Token& advance();

    /**
     * Peeks the second token from input without modifying the current state of the token stream.
     */
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
