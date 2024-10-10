#include "compiler.hpp"
#include "symbol.hpp"
#include "parser.hpp"
#include "lexer.hpp"

// Internal library variables
static ErrorHandler userHandler = nullptr;
static std::vector<std::string> errors;

static void ErrorCallback(const std::string& errMsg, const std::string& offLine, int line, int offset) {
    std::string msg = "error:" + std::to_string(line) + ":" + std::to_string(offset) + errMsg + "\n" + offLine + "\n";
    if (userHandler) userHandler(msg);
}

void SelenaSetErrorHandler(ErrorHandler handler) {
    userHandler = handler;
}

SelenaInfo SelenaCompileShaderSource(std::string& source) {
    SymbolTable table = {};
    errors.clear();

    Lexer lexer = Lexer(source);
    lexer.callback = ErrorCallback;
    std::vector<Token> tokens = lexer.Tokenize();

    Parser parser = Parser(tokens, table);
    parser.callback = ErrorCallback;

    std::optional<ParseNode> node = parser.Parse();
    if(!node) return {0, false, std::move(errors), nullptr};

    // TODO: Fix when ShBin CodeGen is available 
    return {0, true, std::move(errors), nullptr};
}