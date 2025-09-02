#include "JackTokenizer.hpp"
#include "CompilerResources.hpp"
#include "utils.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace Compiler {

namespace fs = std::filesystem;

std::regex JackTokenizer::tokenPattern = std::regex(R"(\d+|".*?"|[{}()\[\].,;+\-*/&|<>=~]|[a-zA-Z_]\w*)");

JackTokenizer::JackTokenizer(const fs::path& infilePath) {
    std::ifstream infile(infilePath);
    if (!infile) {
        std::cerr << "Input file not opened\n";
        exit(2);
    }

    std::stringstream buffer;
    buffer << infile.rdbuf();
    data = buffer.str();
    infile.close();

    matchTokens();
}

const Token& JackTokenizer::nextToken() const {
    return tokens.front();
}

bool JackTokenizer::hasMoreTokens() const {
    return !tokens.empty();
}

const Token& JackTokenizer::advance() {
    currToken = tokens.front();
    tokens.pop_front();
    return currToken;
}

const Token& JackTokenizer::peekSecond() const {
    return tokens[1];
}

TokenType JackTokenizer::getTokenType(const std::string& tokenVal) {
    if (strToKeyword.find(tokenVal) != strToKeyword.end()) {
        return TokenType::KEYWORD;
    } else if (strToSymbol.find(tokenVal) != strToSymbol.end()) {
        return TokenType::SYMBOL;
    } else if (strIsDigit(tokenVal)) {
        return TokenType::INT_CONST;
    } else if (tokenVal[0] == '"' && tokenVal[tokenVal.length() - 1] == '"') {
        return TokenType::STRING_CONST;
    } else {
        return TokenType::IDENTIFIER;
    }
}

Token JackTokenizer::tokenize(const std::string &tokenVal) {
    Token token;
    TokenType tokenType = getTokenType(tokenVal);
    token.type = tokenType;

    switch (tokenType) {
    case TokenType::KEYWORD:
        token.val = strToKeyword.at(tokenVal);
        break;
    case TokenType::SYMBOL:
        token.val = strToSymbol.at(tokenVal);
        break;
    case TokenType::INT_CONST:
        token.val = std::stoi(tokenVal);
        break;
    case TokenType::STRING_CONST:
        token.val = tokenVal.substr(1, tokenVal.length() - 2);
        break;
    case TokenType::IDENTIFIER:
        token.val = tokenVal;
        break;
    }

    return token;
}

void JackTokenizer::matchTokens() {
    removeComments(data);
    for (std::sregex_iterator it(data.begin(), data.end(), tokenPattern), end; it != end; ++it) {
        std::smatch match = *it;
        tokens.push_back(tokenize(match.str())); // tokenize return value out of scope??
    }
}

}
