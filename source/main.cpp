#include "lexer.hpp"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <iostream>
#include "symbol.hpp"
#include "parser.hpp"
#include <cstring>

#define ANSI_COLOR_RED  "\x1b[0;31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/* A bunch of helper functions */

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

static std::string SlurpFile(const std::string& path) {
    std::stringstream fileContents;
    std::fstream input(path, std::ios::in);
    if(!input.is_open())
        std::printf(ANSI_COLOR_RED "fatal error: " 
            ANSI_COLOR_RESET "%s: " 
            "no such file or directory" 
            "\n", path.c_str());
    fileContents << input.rdbuf();
    input.close(); // Good practice, I guess
    return fileContents.str();
}

static void PrintHelp(const std::string& title) {
    std::printf("Usage: %s <input_shader> [options]\n", title.c_str());
    std::printf("Options:\n");
    std::printf("     -o, --output       | Select output file\n");
    std::printf("     -h, --help         | Show this help message\n");
    std::printf("     -v, --verbose      | Print parse and abstract syntax tree structures\n");
    std::printf("     -s, --assembly     | Output picasso assembly\n");
}

static bool hasErrored = false;
void ErrorHandler(const std::string& errMsg, const std::string& offLine, int line, int offset) {
    printf(ANSI_COLOR_RED "error:" ANSI_COLOR_RESET "%d:%d: %s\n%s\n", line, offset, errMsg.c_str(), offLine.c_str());
    printf("%*c^\n", offset, ' ');
}

static std::string output = "";
void ProcessArguments(int argc, char* argv[]) {
    for(int i = 2; i < argc; i++) {
        if(std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help")) {
            PrintHelp(argv[0]);
            std::exit(EXIT_SUCCESS);
        } else if(std::strcmp(argv[i], "-o") == 0 || std::strcmp(argv[i], "--output")) {
            output = argv[i++];
            continue;
        }
    }
}

int main(int argc, char* argv[]) {
    // Read source code
    std::string source = SlurpFile(argv[1]);
    if(source.empty()) return 0;

    // Process arguments
    //ProcessArguments(argc, argv);

    // Symbol Table
    SymbolTable table;

    // Lexical Analysis
    Lexer lexer(source, table);
    lexer.callback = ErrorHandler;
    lexer.ReceiveLine = ReceiveLine;
    std::vector<Token> tokens = lexer.Tokenize();

    // Syntactic & Semantic Analysis
    Parser parser(tokens, table, source);
    parser.callback = ErrorHandler;
    parser.ReceiveLine = ReceiveLine;
    //std::optional<ParseNode> node = parser.ParseTranslationUnit();

    return 0;
}