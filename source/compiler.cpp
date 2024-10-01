#include "compiler.hpp"
#include "symbol.hpp"
#include "parser.hpp"
#include "lexer.hpp"

// Internal library variables
static ErrorHandler userHandler = nullptr;
static std::vector<std::string> errors;

void SelenaSetErrorHandler(ErrorHandler handler) {
    userHandler = handler;
}

SelenaInfo SelenaCompileShaderSource(std::string& source) {
    SelenaInfo info = {};
    SymbolTable table = {};
    errors.clear();

    Lexer lexer = Lexer(source, &table);
    std::vector<Token> tokens = lexer.Tokenize();

    Parser parser = Parser(tokens);
    std::optional<ParseNode> node = parser.Parse();

    info.errors = std::move(errors);
    return info;
}