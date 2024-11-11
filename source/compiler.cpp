#include "compiler.hpp"
#include "symbol.hpp"
#include "parser.hpp"
#include "lexer.hpp"

// Internal library variables
static ErrorHandler userHandler = nullptr;
static std::vector<std::string> errors;

static std::string ReceiveLine(std::string& source, int line) {
    std::string ret = "";
    int currentLine = 1;
    for(size_t i = 0; i < source.length(); i++) {
        if(source[i] == '\n') { 
            currentLine++;
            continue; 
        }
        if(currentLine == line) {
            ret.push_back(source[i]);
            while(source[i] != '\n') {
                ret.push_back(source[i++]);
            }
            break;
        }
    }
    return ret;
}

static void ErrorCallback(const std::string& errMsg, const std::string& offLine, int line, int offset) {
    std::string msg = "error:" + std::to_string(line) + ":" + std::to_string(offset) + errMsg + "\n" + offLine + "\n";
    errors.push_back(msg);
    if(userHandler) userHandler(msg);
}

void SelenaSetErrorHandler(ErrorHandler handler) {
    userHandler = handler;
}

SelenaInfo SelenaCompileShaderSource(std::string& source) {
    SymbolTable table = {};
    errors.clear();

    Lexer lexer = Lexer(source, table);
    lexer.Callback = ErrorCallback;
    lexer.ReceiveLine = ReceiveLine;
    std::vector<Token> tokens = lexer.Tokenize();

    if(!errors.empty()) return {0, false, std::move(errors), nullptr};

    Parser parser = Parser(tokens, table, source);
    parser.Callback = ErrorCallback;
    parser.ReceiveLine = ReceiveLine;
    std::optional<ParseNode> node = parser.ParseTranslationUnit();

    if(!errors.empty()) return {0, false, std::move(errors), nullptr};

    // TODO: Fix when ShBin CodeGen is available 
    return {0, true, std::move(errors), nullptr};
}