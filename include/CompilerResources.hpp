#ifndef COMPILERRESOURCES_H
#define COMPILERRESOURCES_H

#include <exception>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace Compiler {

/**
 * Base class for all compiler-related errors.
 */
class JackCompilerError : public std::exception {
public:
    JackCompilerError(const std::string& msg);
    const char* what() const noexcept override;

private:
    std::string message;
};

/**
 * Indicates a compiler has received a token whose value does not match any standard Jack grammar patterns.
 */
class TokenError : public JackCompilerError {
    public:
    TokenError(const std::string& tokenVal, const std::string& reqVal);
};

/**
 * Indicates a compiler has received a token whose value does not match any standard Jack grammar patterns.
 */
class WildcardTokenError : public JackCompilerError {
public:
    WildcardTokenError(const std::string& tokenType, const std::string& reqType);
};

/**
 * Indicates a compiler has received a token whose symbol is undefined.
 */
class SymbolError : public JackCompilerError {
public:
    SymbolError(const std::string& symbol);
};

/**
 * Enums for each Jack grammar token type.
 */
enum class TokenType {
    KEYWORD,
    SYMBOL,
    INT_CONST,
    STRING_CONST,
    IDENTIFIER
};

/**
 * Enums for each keyword in the Jack language.
 */
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

/**
 * Enums for each symbol in the Jack language.
 */
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

/**
 * Enums for each memory segment that can be accessed in VM commands.
 */
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

/**
 * Enums for each arithmetic operation in the Jack language.
 */
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

/**
 * Token value can be any integer, string constant, or keyword/symbol enum.
 */
using TokenVal = std::variant<Keyword, Symbol, int, std::string>;

/**
 * Required token can be either a token type (any value that matches the type permitted) or token value.
 */
using TokenReq = std::variant<TokenType, TokenVal>;

/**
 * Predefined token sets to match/validate certain conditions during the compilation process.
 */
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

/**
 * Writes a string representation of a memory segment enum to output.
 */
std::ostream& operator<<(std::ostream& os, const Segment& segment);

/**
 * Writes a string representation of a command enum to output.
 */
std::ostream& operator<<(std::ostream& os, const Command& command);

/**
 * Converts and returns the string representation of a keyword enum.
 */
std::string operator+(const Keyword& keyword);

/**
 * Converts and returns the string representation of any requirement token.
 */
std::string reqToString(const TokenReq& req);

/**
 * Converts and returns the segment enum equivalent of any memory-related keyword enum.
 */
Segment keywordToSegment(const Keyword& keyword);

/**
 * Represents a token in the input file with a type and value.
 */
struct Token {
    TokenType type;
    TokenVal val;

    Token() {}
    Token(TokenType t, TokenVal v) : type(t), val(v) {}
};

}

#endif
