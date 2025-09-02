#ifndef COMPILATIONENGINE_H
#define COMPILATIONENGINE_H

#include "CompilerResources.hpp"
#include "JackTokenizer.hpp"
#include "SymbolTable.hpp"
#include "VMWriter.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Compiler {

namespace fs = std::filesystem;

class CompilationEngine {
public:
    CompilationEngine(const fs::path& infile, const fs::path& outfile, std::ofstream* const debugFile);
    void compileClass();

private:
    static const std::string MATH_MULTIPLY;
    static const std::string MATH_DIVIDE;
    static const std::string MEMORY_ALLOC;
    static const std::string STRING_NEW;
    static const std::string STRING_APPENDCHAR;

    static const std::unordered_map<Symbol, Command> commandLookup;
    static const std::unordered_map<Symbol, std::string> mathLookup;

    JackTokenizer tokenizer;
    VMWriter writer;
    int labelCount;
    std::string currClassName;

    SymbolTable classSymbols;
    SymbolTable methodSymbols;

    static void handleInvalidToken(const Token& token, const TokenReq& req, const std::string* customReqName = nullptr);

    std::string getLabel();
    std::pair<std::string, std::string> getLabelPair();

    bool compareToken(const Token& token, const TokenReq& req) const;
    TokenVal process(const TokenReq& req);
    const TokenReq* compareTokens(const Token& token, const std::vector<TokenReq>& reqsList) const;
    bool nextTokenIs(const TokenReq& req) const;
    const TokenReq* nextTokenIsOneOf(const std::vector<TokenReq>& reqsList) const;

    Keyword processKeyword();
    Symbol processSymbol();
    int processIntConst();
    std::string processStringConst();
    std::string processIdentifier();

    TokenVal verifySet(const std::vector<TokenReq>& reqsList, const std::string& setName);
    TokenVal verifyVarType();
    TokenVal verifyReturnType();

    bool isClassVarDec() const;
    bool isSubroutineDec() const;
    bool isVarDec() const;
    bool isStatement() const;

    bool termIsSubroutineCall() const;
    bool termIsArrayExp() const;

    const SymbolTable* getVarScope(const std::string& name) const;

    void compileClassVarDec();
    void compileSubroutine();
    void compileParameterList();
    void compileSubroutineBody(const std::string& name, const Keyword& type);
    void compileVarDec();
    void compileFunctionHeader(const std::string& name, const Keyword& type);
    std::string compileVarName(const std::string& type, const Segment& segment, SymbolTable& symbolTable);
    const SymbolTable::Entry* compileVarName();
    std::string compileName();
    void compileSubroutineCall();
    void compileStatements();
    void compileLet();
    void compileIf();
    void compileWhile();
    void compileDo();
    void compileReturn();
    void compileExpression();
    void compileTerm();
    int compileExpressionList();
    void compileStrConstTerm();
    void compileKeywordConstTerm();
    void compileIdentifierTerm();
};

}

#endif
