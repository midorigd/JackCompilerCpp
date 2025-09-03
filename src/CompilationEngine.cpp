#include "CompilationEngine.hpp"
#include "CompilerResources.hpp"
#include "JackTokenizer.hpp"
#include "SymbolTable.hpp"
#include "VMWriter.hpp"

namespace Compiler {

namespace fs = std::filesystem;

const std::string CompilationEngine::MATH_MULTIPLY = "Math.multiply";
const std::string CompilationEngine::MATH_DIVIDE = "Math.divide";
const std::string CompilationEngine::MEMORY_ALLOC = "Memory.alloc";
const std::string CompilationEngine::STRING_NEW = "String.new";
const std::string CompilationEngine::STRING_APPENDCHAR = "String.appendChar";

const std::unordered_map<Symbol, Command> CompilationEngine::commandLookup {
    {Symbol::PLUS, Command::ADD},
    {Symbol::MINUS, Command::SUB},
    {Symbol::EQUAL, Command::EQ},
    {Symbol::GREATER_THAN, Command::GT},
    {Symbol::LESS_THAN, Command::LT},
    {Symbol::AMPERSAND, Command::AND},
    {Symbol::VERTICAL_BAR, Command::OR}
};

const std::unordered_map<Symbol, std::string> CompilationEngine::mathLookup {
    {Symbol::STAR, CompilationEngine::MATH_MULTIPLY},
    {Symbol::SLASH, CompilationEngine::MATH_DIVIDE}
};

CompilationEngine::CompilationEngine(const fs::path& infile, const fs::path& outfile, std::ofstream* const debugFile) :
    tokenizer(infile),
    writer(outfile),
    labelCount(0),
    classSymbols(debugFile),
    methodSymbols(debugFile) { compileClass(); }

// 'class' className '{' classVarDec* subroutineDec* '}'
void CompilationEngine::compileClass() {
    process(Keyword::CLASS);
    currClassName = compileName();
    process(Symbol::CURLBRACE_L);
    while (isClassVarDec()) { compileClassVarDec(); }
    while (isSubroutineDec()) { compileSubroutine(); }
    process(Symbol::CURLBRACE_R);

    classSymbols.dumpTable(currClassName + " class");
}

void CompilationEngine::handleInvalidToken(const Token& token, const TokenReq& req, const std::string* customReqName) {
    std::string reqName { customReqName ? *customReqName : reqToString(req) };

    if (std::holds_alternative<TokenVal>(req)) {
        throw TokenError(reqToString(token.val), reqName);
    } else {
        throw WildcardTokenError(reqToString(token.type), reqName);
    }
}

std::string CompilationEngine::getLabel() {
    return "L" + std::to_string(labelCount++);
}

std::pair<std::string, std::string> CompilationEngine::getLabelPair() {
    return {getLabel(), getLabel()};
}

bool CompilationEngine::compareToken(const Token& token, const TokenReq& req) const {
    if (std::holds_alternative<TokenVal>(req)) {
        return token.val == std::get<TokenVal>(req);
    } else {
        return token.type == std::get<TokenType>(req);
    }
}

TokenVal CompilationEngine::process(const TokenReq& req) {
    Token token { tokenizer.advance() };
    if (compareToken(token, req)) {
        return token.val;
    }

    handleInvalidToken(token, req);
}

const TokenReq* CompilationEngine::compareTokens(const Token& token, const std::vector<TokenReq>& reqsList) const {
    for (const TokenReq& req : reqsList) {
        if (compareToken(token, req)) {
            return &req;
        }
    }

    return nullptr;
}

bool CompilationEngine::nextTokenIs(const TokenReq& req) const {
    return compareToken(tokenizer.nextToken(), req);
}

const TokenReq* CompilationEngine::nextTokenIsOneOf(const std::vector<TokenReq>& reqsList) const {
    return compareTokens(tokenizer.nextToken(), reqsList);
}

Keyword CompilationEngine::processKeyword() {
    return std::get<Keyword>( process(TokenType::KEYWORD) );
}

Symbol CompilationEngine::processSymbol() {
    return std::get<Symbol>( process(TokenType::SYMBOL) );
}

int CompilationEngine::processIntConst() {
    return std::get<int>( process(TokenType::INT_CONST) );
}

std::string CompilationEngine::processStringConst() {
    return std::get<std::string>( process(TokenType::STRING_CONST) );
}

std::string CompilationEngine::processIdentifier() {
    return std::get<std::string>( process(TokenType::IDENTIFIER) );
}

TokenVal CompilationEngine::verifySet(const std::vector<TokenReq>& reqsList, const std::string& setName) {
    if (const TokenReq* reqPtr = nextTokenIsOneOf(reqsList)) {
        return process(*reqPtr);
    }

    handleInvalidToken(tokenizer.nextToken(), reqsList[0], &setName);
}

TokenVal CompilationEngine::verifyVarType() {
    return verifySet(TokenSet::DATA_TYPES, "var type");
}

TokenVal CompilationEngine::verifyReturnType() {
    return verifySet(TokenSet::RETURN_TYPES, "return type");
}

bool CompilationEngine::isClassVarDec() const {
    return nextTokenIsOneOf(TokenSet::CLASS_VAR_DEC);
}

bool CompilationEngine::isSubroutineDec() const {
    return nextTokenIsOneOf(TokenSet::SUBROUTINE_DEC);
}

bool CompilationEngine::isVarDec() const {
    return nextTokenIs(Keyword::VAR);
}

bool CompilationEngine::isStatement() const {
    return nextTokenIsOneOf(TokenSet::STATEMENTS);
}

bool CompilationEngine::termIsSubroutineCall() const {
    return compareTokens(tokenizer.peekSecond(), TokenSet::SUBROUTINE_CALL);
}

bool CompilationEngine::termIsArrayExp() const {
    return compareToken(tokenizer.nextToken(), Symbol::SQRBRACK_L);
}

const SymbolTable* CompilationEngine::getVarScope(const std::string& name) const {
    if (methodSymbols.contains(name)) {
        return &methodSymbols;
    } else if (classSymbols.contains(name)) {
        return &classSymbols;
    } else {
        return nullptr;
    }
}

// ( 'static' | 'field' ) type varName ( ',' varName )* ';'
void CompilationEngine::compileClassVarDec() {
    Segment symbolSegment { keywordToSegment( processKeyword() ) };
    std::string symbolType { reqToString( verifyVarType() ) };

    while (true) {
        compileVarName(symbolType, symbolSegment, classSymbols);
        if (!nextTokenIs(Symbol::COMMA)) { break; }
        process(Symbol::COMMA);
    }
    process(Symbol::SEMICOLON);
}

// ( 'constructor' | 'function' | 'method' ) ( 'void' | type ) subroutineName '(' parameterList ')' subroutineBody
void CompilationEngine::compileSubroutine() {
    Keyword subroutineType { processKeyword() };
    verifyReturnType();
    std::string subroutineName { compileName() };

    methodSymbols.reset();
    // methods take this object as implicit first argument
    if (subroutineType == Keyword::METHOD) { methodSymbols.defineThisObject(currClassName); }

    process(Symbol::PAREN_L);
    compileParameterList();
    process(Symbol::PAREN_R);
    compileSubroutineBody(subroutineName, subroutineType);

    methodSymbols.dumpTable(subroutineName + " method");
}

// ( ( type varName ) ( ',' type varName )* )?
void CompilationEngine::compileParameterList() {
    if (nextTokenIs(Symbol::PAREN_R)) { return; }
    while (true) {
        std::string symbolType { reqToString( verifyVarType() ) };
        compileVarName(symbolType, Segment::ARG, methodSymbols);
        if (!nextTokenIs(Symbol::COMMA)) { break; }
        process(Symbol::COMMA);
    }
}

// '{' varDec* statements '}'
void CompilationEngine::compileSubroutineBody(const std::string& name, const Keyword& type) {
    process(Symbol::CURLBRACE_L);
    while (isVarDec()) { compileVarDec(); }
    compileFunctionHeader(name, type);
    compileStatements();
    process(Symbol::CURLBRACE_R);
}

// 'var' type varName ( ',' varName )* ';'
void CompilationEngine::compileVarDec() {
    process(Keyword::VAR);
    std::string symbolType { reqToString( verifyVarType() ) };
    while (true) {
        compileVarName(symbolType, Segment::LOCAL, methodSymbols);
        if (!nextTokenIs(Symbol::COMMA)) { break; }
        process(Symbol::COMMA);
    }
    process(Symbol::SEMICOLON);
}

/*
constructor: push # field vars, alloc, then pop address to THIS ptr
method: pop this address (first arg) to THIS ptr
function: no extra setup
*/
void CompilationEngine::compileFunctionHeader(const std::string& name, const Keyword& type) {
    std::string functionName { currClassName + '.' + name };
    int nVars { methodSymbols.varCount(Segment::LOCAL) };
    writer.writeFunction(functionName, nVars);

    if (type == Keyword::CONSTRUCTOR) {
        int nFields { classSymbols.varCount(Segment::THIS) };
        writer.writeConstant(nFields);
        writer.writeCall(MEMORY_ALLOC, 1);
        writer.writePopThisPtr();
    } else if (type == Keyword::METHOD) {
        const SymbolTable::Entry* entryPtr { methodSymbols.getEntry(+Keyword::THIS) };
        Segment thisSegment { entryPtr->segment };
        int thisIndex { entryPtr->index };

        writer.writePush(thisSegment, thisIndex);
        writer.writePopThisPtr();
    }
}

std::string CompilationEngine::compileVarName(const std::string& type, const Segment& segment, SymbolTable& symbolTable) {
    std::string name { processIdentifier() };
    symbolTable.define(name, type, segment);
    return name;
}

const SymbolTable::Entry* CompilationEngine::compileVarName() {
    std::string name { processIdentifier() };
    if (const SymbolTable* symbolTable = getVarScope(name)) {
        return symbolTable->getEntry(name);
    }

    throw SymbolError(name);
}

std::string CompilationEngine::compileName() {
    return processIdentifier();
}

// ( className | varName ) '.' subroutineName '(' expressionList ')'
//                           | subroutineName '(' expressionList ')'
void CompilationEngine::compileSubroutineCall() {
    /*
    internal method (no dot):   className is current class; push this to stack as first arg
    external method (varName):  className is type(varName); push var to stack as first arg
    external func (className):  className is provided; no extra setup
    */

    std::string className { currClassName };
    int nArgs { 1 };

    if (compareToken(tokenizer.peekSecond(), Symbol::DOT)) {
        std::string symbolName { std::get<std::string>( tokenizer.nextToken().val ) };

        if (getVarScope(symbolName)) {
            const SymbolTable::Entry* entryPtr { compileVarName() };
            writer.writePush(entryPtr->segment, entryPtr->index);
            className = entryPtr->type;
        } else {
            className = compileName();
            nArgs = 0;
        }

        process(Symbol::DOT);

    } else {
        writer.writePushThisPtr();
    }

    std::string subroutineName { compileName() };
    process(Symbol::PAREN_L);
    nArgs += compileExpressionList();
    process(Symbol::PAREN_R);

    std::string functionName { className + '.' + subroutineName };
    writer.writeCall(functionName, nArgs);
}

// ( letStatement | ifStatement | whileStatement | doStatement | returnStatement )*
void CompilationEngine::compileStatements() {
    while (isStatement()) {
        switch (std::get<Keyword>( tokenizer.nextToken().val )) {
            case Keyword::LET:    compileLet();    break;
            case Keyword::IF:     compileIf();     break;
            case Keyword::WHILE:  compileWhile();  break;
            case Keyword::DO:     compileDo();     break;
            case Keyword::RETURN: compileReturn(); break;
        }
    }
}

// 'let' varName ( '[' expression ']' )? '=' expression ';'
void CompilationEngine::compileLet() {
    process(Keyword::LET);
    const SymbolTable::Entry* entryPtr { compileVarName() };

    if (nextTokenIs(Symbol::SQRBRACK_L)) {
        writer.writePush(entryPtr->segment, entryPtr->index);

        process(Symbol::SQRBRACK_L);
        compileExpression();
        process(Symbol::SQRBRACK_R);

        writer.writeArithmetic(Command::ADD);

        process(Symbol::EQUAL);
        compileExpression();
        process(Symbol::SEMICOLON);

        writer.writePop(Segment::TEMP, 0);
        writer.writePopThatPtr();
        writer.writePush(Segment::TEMP, 0);
        writer.writePop(Segment::THAT, 0);
    } else {
        process(Symbol::EQUAL);
        compileExpression();
        process(Symbol::SEMICOLON);

        writer.writePop(entryPtr->segment, entryPtr->index);
    }
}

// 'if' '(' expression ')' '{' statements '}' ( 'else' '{' statements '}' )?
void CompilationEngine::compileIf() {
    auto [ifLabel, gotoLabel] { getLabelPair() };

    process(Keyword::IF);
    process(Symbol::PAREN_L);
    compileExpression();
    process(Symbol::PAREN_R);

    writer.writeArithmetic(Command::NOT);
    writer.writeIf(ifLabel);

    process(Symbol::CURLBRACE_L);
    compileStatements();
    process(Symbol::CURLBRACE_R);

    writer.writeGoto(gotoLabel);
    writer.writeLabel(ifLabel);

    if (nextTokenIs(Keyword::ELSE)) {
        process(Keyword::ELSE);
        process(Symbol::CURLBRACE_L);
        compileStatements();
        process(Symbol::CURLBRACE_R);
    }

    writer.writeLabel(gotoLabel);
}

// 'while' '(' expression ')' '{' statements '}'
void CompilationEngine::compileWhile() {
    auto [loopLabel, exitLabel] { getLabelPair() };

    process(Keyword::WHILE);

    writer.writeLabel(loopLabel);

    process(Symbol::PAREN_L);
    compileExpression();
    process(Symbol::PAREN_R);

    writer.writeArithmetic(Command::NOT);
    writer.writeIf(exitLabel);

    process(Symbol::CURLBRACE_L);
    compileStatements();
    process(Symbol::CURLBRACE_R);

    writer.writeGoto(loopLabel);
    writer.writeLabel(exitLabel);
}

// 'do' subroutineCall ';'
void CompilationEngine::compileDo() {
    process(Keyword::DO);
    compileSubroutineCall();
    process(Symbol::SEMICOLON);

    writer.writePop(Segment::TEMP, 0);
}

// 'return' expression? ';'
void CompilationEngine::compileReturn() {
    process(Keyword::RETURN);

    if (nextTokenIs(Symbol::SEMICOLON)) {
        writer.writeConstant(0); // push dummy value; will be thrown away by caller
    } else {
        compileExpression();
    }

    process(Symbol::SEMICOLON);
    writer.writeReturn();
}

// term ( op term )*
void CompilationEngine::compileExpression() {
    compileTerm();

    while (nextTokenIsOneOf(TokenSet::OPERATORS)) {
        Symbol op { processSymbol() };
        compileTerm();

        if (commandLookup.find(op) != commandLookup.end()) {
            writer.writeArithmetic(commandLookup.at(op));
        } else {
            writer.writeCall(mathLookup.at(op), 2);
        }
    }
}

// intConst | stringConst | keywordConst | varName | varName '[' expression ']'
// | subroutineCall | '(' expression ')' | unaryOp term
void CompilationEngine::compileTerm() {
    // refactor this to use pointer returned by nextTokenIsOneOf
    if (nextTokenIs(TokenType::INT_CONST)) {
        writer.writeConstant( processIntConst() );
    } else if (nextTokenIs(TokenType::STRING_CONST)) {
        compileStrConstTerm();
    } else if (nextTokenIsOneOf(TokenSet::KEYWORD_CONSTANTS)) {
        compileKeywordConstTerm();
    } else if (nextTokenIs(TokenType::IDENTIFIER)) {
        compileIdentifierTerm();
    } else if (nextTokenIs(Symbol::PAREN_L)) {
        process(Symbol::PAREN_L);
        compileExpression();
        process(Symbol::PAREN_R);
    } else if (nextTokenIsOneOf(TokenSet::UNARY_OPS)) {
        Command op { processSymbol() == Symbol::MINUS ? Command::NEG : Command::NOT };
        compileTerm();
        writer.writeArithmetic(op);
    } else {
        std::string reqName { "term" };
        handleInvalidToken(tokenizer.nextToken(), TokenType::IDENTIFIER, &reqName);
    }
}

// ( expression ( ',' expression )* )?
int CompilationEngine::compileExpressionList() {
    int count { 0 };

    if (!nextTokenIs(Symbol::PAREN_R)) {
        while (true) {
            compileExpression();
            ++count;
            if (!nextTokenIs(Symbol::COMMA)) { break; }
            process(Symbol::COMMA);
        }
    }

    return count;
}

void CompilationEngine::compileStrConstTerm() {
    std::string str { processStringConst() };

    writer.writeConstant(str.length());
    writer.writeCall(STRING_NEW, 1);
    for (const char& chr : str) {
        writer.writeConstant(static_cast<int>(chr));
        writer.writeCall(STRING_APPENDCHAR, 2);
    }
}

// 'true' | 'false' | 'null' | 'this'
void CompilationEngine::compileKeywordConstTerm() {
    switch (processKeyword()) {
        case Keyword::TRUE:
            writer.writeConstant(1);
            writer.writeArithmetic(Command::NEG);
            break;
        case Keyword::THIS:
            writer.writePushThisPtr();
            break;
        default:
            writer.writeConstant(0);
            break;
    }
}

// varName | varName '[' expression ']' | subroutineCall
void CompilationEngine::compileIdentifierTerm() {
    if (termIsSubroutineCall()) {
        compileSubroutineCall();
    } else {
        const SymbolTable::Entry* entryPtr { compileVarName() };
        writer.writePush(entryPtr->segment, entryPtr->index);

        if (termIsArrayExp()) {
            process(Symbol::SQRBRACK_L);
            compileExpression();
            process(Symbol::SQRBRACK_R);

            writer.writeArithmetic(Command::ADD);
            writer.writePopThatPtr();
            writer.writePush(Segment::THAT, 0);
        }
    }
}

}
