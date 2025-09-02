#include "CompilerResources.hpp"

namespace Compiler {

JackCompilerError::JackCompilerError(const std::string& msg) :
    message(msg) {}

const char* JackCompilerError::what() const noexcept {
    return message.c_str();
}

TokenError::TokenError(const std::string& tokenVal, const std::string& reqVal) :
    JackCompilerError("Expected token: " + reqVal + ". Got: " + tokenVal + '\n') {}

WildcardTokenError::WildcardTokenError(const std::string& tokenType, const std::string& reqType) :
    JackCompilerError("Expected token of type: " + reqType + ". Got: " + tokenType + '\n') {}

SymbolError::SymbolError(const std::string& symbol) :
    JackCompilerError("Undefined symbol: " + symbol) {}


namespace TokenSet {
    std::vector<TokenReq> DATA_TYPES;
    std::vector<TokenReq> RETURN_TYPES;
    std::vector<TokenReq> CLASS_VAR_DEC;
    std::vector<TokenReq> SUBROUTINE_DEC;
    std::vector<TokenReq> STATEMENTS;
    std::vector<TokenReq> UNARY_OPS;
    std::vector<TokenReq> OPERATORS;
    std::vector<TokenReq> KEYWORD_CONSTANTS;
    std::vector<TokenReq> TERMS;
    std::vector<TokenReq> SUBROUTINE_CALL;

    void init() {
        CLASS_VAR_DEC = { Keyword::STATIC, Keyword::FIELD };
        SUBROUTINE_DEC = { Keyword::CONSTRUCTOR, Keyword::FUNCTION, Keyword::METHOD };
        SUBROUTINE_CALL = { Symbol::PAREN_L, Symbol::DOT };
        UNARY_OPS = { Symbol::MINUS, Symbol::SQUIGGLE };

        DATA_TYPES = {
            Keyword::INT,
            Keyword::CHAR,
            Keyword::BOOLEAN,
            TokenType::IDENTIFIER
        };

        RETURN_TYPES = { Keyword::VOID };
        RETURN_TYPES.insert(RETURN_TYPES.end(), DATA_TYPES.begin(), DATA_TYPES.end());

        STATEMENTS = {
            Keyword::LET,
            Keyword::IF,
            Keyword::WHILE,
            Keyword::DO,
            Keyword::RETURN
        };

        OPERATORS = {
            Symbol::PLUS,
            Symbol::MINUS,
            Symbol::STAR,
            Symbol::SLASH,
            Symbol::AMPERSAND,
            Symbol::VERTICAL_BAR,
            Symbol::LESS_THAN,
            Symbol::GREATER_THAN,
            Symbol::EQUAL
        };

        KEYWORD_CONSTANTS = {
            Keyword::TRUE,
            Keyword::FALSE,
            Keyword::NULL_KW,
            Keyword::THIS
        };

        TERMS = {
            TokenType::INT_CONST,
            TokenType::STRING_CONST,
            TokenType::IDENTIFIER,
            Symbol::PAREN_L
        };
        TERMS.insert(TERMS.end(), KEYWORD_CONSTANTS.begin(), KEYWORD_CONSTANTS.end());
        TERMS.insert(TERMS.end(), UNARY_OPS.begin(), UNARY_OPS.end());
    }
}


const std::unordered_map<std::string, Keyword> strToKeyword {
    {"class", Keyword::CLASS},
    {"constructor", Keyword::CONSTRUCTOR},
    {"function", Keyword::FUNCTION},
    {"method", Keyword::METHOD},
    {"field", Keyword::FIELD},
    {"static", Keyword::STATIC},
    {"var", Keyword::VAR},

    {"int", Keyword::INT},
    {"char", Keyword::CHAR},
    {"boolean", Keyword::BOOLEAN},
    {"void", Keyword::VOID},

    {"true", Keyword::TRUE},
    {"false", Keyword::FALSE},
    {"null", Keyword::NULL_KW},
    {"this", Keyword::THIS},

    {"let", Keyword::LET},
    {"do", Keyword::DO},
    {"if", Keyword::IF},
    {"else", Keyword::ELSE},
    {"while", Keyword::WHILE},
    {"return", Keyword::RETURN}
};

const std::unordered_map<std::string, Symbol> strToSymbol {
    {"{", Symbol::CURLBRACE_L},
    {"}", Symbol::CURLBRACE_R},
    {"(", Symbol::PAREN_L},
    {")", Symbol::PAREN_R},
    {"[", Symbol::SQRBRACK_L},
    {"]", Symbol::SQRBRACK_R},
    {".", Symbol::DOT},
    {",", Symbol::COMMA},
    {";", Symbol::SEMICOLON},
    {"+", Symbol::PLUS},
    {"-", Symbol::MINUS},
    {"*", Symbol::STAR},
    {"/", Symbol::SLASH},
    {"&", Symbol::AMPERSAND},
    {"|", Symbol::VERTICAL_BAR},
    {">", Symbol::GREATER_THAN},
    {"<", Symbol::LESS_THAN},
    {"=", Symbol::EQUAL},
    {"~", Symbol::SQUIGGLE}
};

const std::unordered_map<std::string, Segment> strToSegment {
    {"field", Segment::FIELD},
    {"this", Segment::THIS},
    {"static", Segment::STATIC},
    {"argument", Segment::ARG},
    {"local", Segment::LOCAL},

    {"constant", Segment::CONST},
    {"that", Segment::THAT},
    {"pointer", Segment::POINTER},
    {"temp", Segment::TEMP}
};

const std::unordered_map<TokenType, std::string> typeToStr {
    {TokenType::KEYWORD, "keyword"},
    {TokenType::SYMBOL, "symbol"},
    {TokenType::INT_CONST, "int const"},
    {TokenType::STRING_CONST, "string const"},
    {TokenType::IDENTIFIER, "identifier"}
};

const std::unordered_map<Keyword, std::string> keywordToStr {
    {Keyword::CLASS, "class"},
    {Keyword::CONSTRUCTOR, "constructor"},
    {Keyword::FUNCTION, "function"},
    {Keyword::METHOD, "method"},
    {Keyword::FIELD, "field"},
    {Keyword::STATIC, "static"},
    {Keyword::VAR, "var"},
    
    {Keyword::INT, "int"},
    {Keyword::CHAR, "char"},
    {Keyword::BOOLEAN, "boolean"},
    {Keyword::VOID, "void"},
    
    {Keyword::TRUE, "true"},
    {Keyword::FALSE, "false"},
    {Keyword::NULL_KW, "null"},
    {Keyword::THIS, "this"},
    
    {Keyword::LET, "let"},
    {Keyword::DO, "do"},
    {Keyword::IF, "if"},
    {Keyword::ELSE, "else"},
    {Keyword::WHILE, "while"},
    {Keyword::RETURN, "return"}
};

const std::unordered_map<Symbol, std::string> symbolToStr {
    {Symbol::CURLBRACE_L, "{"},
    {Symbol::CURLBRACE_R, "}"},
    {Symbol::PAREN_L, "("},
    {Symbol::PAREN_R, ")"},
    {Symbol::SQRBRACK_L, "["},
    {Symbol::SQRBRACK_R, "]"},
    {Symbol::DOT, "."},
    {Symbol::COMMA, ","},
    {Symbol::SEMICOLON, ";"},
    {Symbol::PLUS, "+"},
    {Symbol::MINUS, "-"},
    {Symbol::STAR, "*"},
    {Symbol::SLASH, "/"},
    {Symbol::AMPERSAND, "&"},
    {Symbol::VERTICAL_BAR, "|"},
    {Symbol::GREATER_THAN, ">"},
    {Symbol::LESS_THAN, "<"},
    {Symbol::EQUAL, "="},
    {Symbol::SQUIGGLE, "~"}
};

const std::unordered_map<Command, std::string> commandToStr {
    {Command::ADD, "add"},
    {Command::SUB, "sub"},
    {Command::NEG, "neg"},
    {Command::EQ, "eq"},
    {Command::GT, "gt"},
    {Command::LT, "lt"},
    {Command::AND, "and"},
    {Command::OR, "or"},
    {Command::NOT, "not"}
};

const std::unordered_map<Segment, std::string> segmentToStr {
    {Segment::FIELD, "field"},
    {Segment::THIS, "this"},
    {Segment::STATIC, "static"},
    {Segment::ARG, "argument"},
    {Segment::LOCAL, "local"},

    {Segment::CONST, "constant"},
    {Segment::THAT, "that"},
    {Segment::POINTER, "pointer"},
    {Segment::TEMP, "temp"}
};

std::ostream& operator<<(std::ostream& os, const Segment& segment) {
    os << segmentToStr.at(segment);
    return os;
}

std::ostream& operator<<(std::ostream& os, const Command& command) {
    os << commandToStr.at(command);
    return os;
}

std::string operator+(const Keyword& keyword) {
    return keywordToStr.at(keyword);
}

std::string reqToString(const TokenReq& req) {
    if (std::holds_alternative<TokenType>(req)) {
        return typeToStr.at(std::get<TokenType>(req));
    }

    TokenVal val = std::get<TokenVal>(req);
    if (std::holds_alternative<Keyword>(val)) {
        return keywordToStr.at(std::get<Keyword>(val));
    } else if (std::holds_alternative<Symbol>(val)) {
        return symbolToStr.at(std::get<Symbol>(val));
    } else if (std::holds_alternative<int>(val)) {
        return std::to_string(std::get<int>(val));
    } else {
        return std::get<std::string>(val);
    }
}

Segment keywordToSegment(const Keyword& keyword) {
    return strToSegment.at(+keyword);
}

}
