#include "lexer.hpp"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <iostream>
#include "symbol.hpp"
//#include "parser.hpp"

#define ANSI_COLOR_YELLOW  "\x1b[0;31m"
#define ANSI_COLOR_RESET   "\x1b[0m"


/*void PrintTokens(std::vector<Token> tokens) {
    for(Token t : tokens) {
        printf("Line: %i Value: %s Type: %s ValueType: %s\n", t.line, t.value.c_str(), 
        t.type == Identifer ? "Identifier" : "Keyword or Symbol", 
        t.type == Integer ? "Integer" : t.type == Float ? "Float" : "");
    }
}*/

/*void PrintAstNode(AstNode node, int ident) {
    std::string iBuf = "";
    for(int i = 0; i < ident; i++)
        iBuf.append(" ");
    printf("%s", iBuf.c_str());
    printf("Line: %i Value: %s Type: %s ValueType: %s\n", node.value.line, node.value.value.c_str(), 
    node.value.type == Identifer ? "Identifier" : "Keyword or Symbol", 
    node.value.type == Integer ? "Integer" : node.value.type == Float ? "Float" : "");

    if(!node.children.empty())
        for(AstNode child : node.children)
            PrintAstNode(child, ident + 4);
}*/

void ErrorHandler(const std::string&, const std::string&, int, int) {}

int main(int argc, char* argv[]) {
    // Read source code
    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    // Symbol Table
    SymbolTable table;

    // Lexical Analysis
    Lexer lexer(contents, &table);
    lexer.callback = ErrorHandler;
    std::vector<Token> tokens = lexer.Tokenize();

    for(Token t : tokens) {
        std::printf( ANSI_COLOR_YELLOW "info:" ANSI_COLOR_RESET "%i:%i: found token" "\n%s\n ^\n", t.line, t.offset, t.value.c_str());
    }

    // Semantic Analysis
    //Parser parser(tokens);
    //std::optional<ParseNode> node = parser.Parse();

    #if defined AstDebug
    printf("Ast Root: Node Count: %i\n", ast.value().GetRoot()->children.size());
    PrintAstNode(*ast.value().GetRoot(), 4);
    #endif

    return 0;
}