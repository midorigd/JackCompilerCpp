#ifndef COMPILERRESOURCES_H
#define COMPILERRESOURCES_H

#include <exception>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace Compiler {

class JackCompilerError : public std::exception {
public:
    JackCompilerError(const std::string& msg);
    const char* what() const noexcept override;

private:
    std::string message;
};

class TokenError : public JackCompilerError {
public:
    TokenError(const std::string& tokenVal, const std::string& reqVal);
};

class WildcardTokenError : public JackCompilerError {
public:
    WildcardTokenError(const std::string& tokenType, const std::string& reqType);
};

class SymbolError : public JackCompilerError {
public:
    SymbolError(const std::string& symbol);
};


enum class TokenType {
    KEYWORD,
    SYMBOL,
    INT_CONST,
    STRING_CONST,
    IDENTIFIER
};

enum class Keyword {
    CLASS,
    CONSTRUCTOR,
    FUNCTION,
    METHOD,
    FIELD,
    STATIC,
    VAR,

    INT,
    CHAR,
    BOOLEAN,
    VOID,

    TRUE,
    FALSE,
    NULL_KW,
    THIS,

    LET,
    DO,
    IF,
    ELSE,
    WHILE,
    RETURN
};

enum class Symbol {
    CURLBRACE_L,
    CURLBRACE_R,
    PAREN_L,
    PAREN_R,
    SQRBRACK_L,
    SQRBRACK_R,
    DOT,
    COMMA,
    SEMICOLON,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    AMPERSAND,
    VERTICAL_BAR,
    LESS_THAN,
    GREATER_THAN,
    EQUAL,
    SQUIGGLE
};

enum class Segment {
    FIELD,
    THIS,
    STATIC,
    ARG,
    LOCAL,

    CONST,
    THAT,
    POINTER,
    TEMP
};

enum class Command {
    ADD,
    SUB,
    NEG,
    EQ,
    GT,
    LT,
    AND,
    OR,
    NOT
};

using TokenVal = std::variant<Keyword, Symbol, int, std::string>;

using TokenReq = std::variant<TokenType, TokenVal>;

namespace TokenSet {
    extern std::vector<TokenReq> DATA_TYPES;
    extern std::vector<TokenReq> RETURN_TYPES;
    extern std::vector<TokenReq> CLASS_VAR_DEC;
    extern std::vector<TokenReq> SUBROUTINE_DEC;
    extern std::vector<TokenReq> STATEMENTS;
    extern std::vector<TokenReq> UNARY_OPS;
    extern std::vector<TokenReq> OPERATORS;
    extern std::vector<TokenReq> KEYWORD_CONSTANTS;
    extern std::vector<TokenReq> TERMS;
    extern std::vector<TokenReq> SUBROUTINE_CALL;

    void init();
}

extern const std::unordered_map<std::string, Keyword> strToKeyword;
extern const std::unordered_map<std::string, Symbol> strToSymbol;
extern const std::unordered_map<std::string, Segment> strToSegment;

extern const std::unordered_map<TokenType, std::string> typeToStr;
extern const std::unordered_map<Keyword, std::string> keywordToStr;
extern const std::unordered_map<Symbol, std::string> symbolToStr;
extern const std::unordered_map<Segment, std::string> segmentToStr;
extern const std::unordered_map<Command, std::string> commandToStr;

std::ostream& operator<<(std::ostream& os, const Segment& segment);
std::ostream& operator<<(std::ostream& os, const Command& command);
std::string operator+(const Keyword& keyword);
std::string reqToString(const TokenReq& req);
Segment keywordToSegment(const Keyword& keyword);

struct Token {
    TokenType type;
    TokenVal val;

    Token() {}
    Token(TokenType t, TokenVal v) : type(t), val(v) {}
};

}

#endif
