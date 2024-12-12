#include "lexer.hpp"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <iostream>
#include "symbol.hpp"
#include "parser.hpp"
#include <cstring>
#include "ast.hpp"

#ifndef __3DS__

#define ANSI_COLOR_RED  "\x1b[0;31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

struct Options {
    bool verbose = false;
    bool assembly = false;
    std::string output;
    std::string input;
};

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

static inline void PrintToken(Token& t) {
    printf("%s:%d:%d\n", t.value.c_str(), t.line, t.offset);
}

static void PrintParseTree(ParseNode *node, int depth) {
    for(int i = 0; i < depth; i++) {
        printf("   ");
    }
    if(node->type != ParseNode::T) {
        switch (node->type) {
            case ParseNode::Declaration:
            printf("Declaration:%d:\n", depth);
            break;
            case ParseNode::FunctionDefinition:
            printf("Function Definition:%d:\n", depth);
            break;
            /*case ParseNode::FUNCTION_CALL:
            printf("FUNC_CALL%d\n", Depth);
            break;*/
            case ParseNode::TypeQualifier:
            printf("Type Qualifier:%d:\n", depth);
            break;
            case ParseNode::TypeSpecifier:
            printf("Type Specifier:%d:\n", depth);
            break;
            case ParseNode::PrecisionQualifier:
            printf("Precision Qualifier:%d:\n", depth);
            break;
            case ParseNode::LayoutQualifier:
            printf("Layout Qualifier:%d:\n", depth);
            break;
            case ParseNode::AssignmentExpression:
            printf("Assignment Expression:%d:\n", depth);
            break;
            case ParseNode::Expression:
            printf("Expression:%d:\n", depth);
            break;
            /*case ParseNode::CONDITIONAL_EXPR:
            printf("COND_EXPR%d:", Depth);
            PrintToken(&node->token);
            break;*/
            case ParseNode::PrimaryExpression:
            printf("Primary Expression:%d:\n", depth);
            break;
            case ParseNode::MultiplicativeExpression:
            printf("Multiplicative Expression:%d:\n", depth);
            break;
            case ParseNode::FunctionCall:
            printf("Function Call:%d:\n", depth);
            break;
            case ParseNode::E:
            printf("E%d:\n", depth);
            break;
        }

        for(size_t i = 0; i < node->children.size(); i++) {
            PrintParseTree(&node->children[i], depth + 1);
        }
    } else {
        printf("T%d: ", depth);
        PrintToken(node->token);
    }
}

void PrintAST(AstNode& root, int depth);

void PrintASTNode(AstNode& child, int depth) {
  switch (child.type) {
  case AstNode::Function:
    printf("function:%s:%lu\n", child.token.value.c_str(), child.children.size());
    PrintAST(child, depth + 1);
    break;
  case AstNode::FunctionCall:
    printf("call:%s:%lu\n", child.token.value.c_str(), child.children.size());
    PrintAST(child, depth + 1);
    break;
  case AstNode::Assigment:
    printf("assignment =\n");
    PrintAST(child, depth + 1);
    break;
  case AstNode::Multiply:
    printf("multiply *\n");
    PrintAST(child, depth + 1);
    break;
  case AstNode::Variable:
    printf("variable: %s\n", child.token.value.c_str());
    PrintAST(child, depth + 1);
    break;
  case AstNode::FloatLiteral:
    printf("float:%s\n", child.token.value.c_str());
    PrintAST(child, depth + 1);
    break;
  case AstNode::IntLiteral:
    printf("int:%s\n", child.token.value.c_str());
    PrintAST(child, depth + 1);
    break;
  case AstNode::Declaration:
    printf("declaration:%s:%lu\n", child.token.value.c_str(), child.children.size());
  case AstNode::None:
    printf("EMPTY\n");
    PrintAST(child, depth + 1);
    break;
  default:
    printf("unk:%d\n", child.type);
    break;
  }
}

void PrintAST(AstNode& root, int depth) {
    for(auto child : root.children) {
        for (int i = 0; i < depth; i++) {
            printf("  ");
        }
        printf("D%d ", depth);
        PrintASTNode(child, depth);
    }
}

static std::string SlurpFile(const std::string& path) {
    std::stringstream fileContents;
    std::fstream input(path, std::ios::in);
    if(!input.is_open()) {
        std::printf(ANSI_COLOR_RED "fatal error: " 
            ANSI_COLOR_RESET "%s: " 
            "no such file or directory" 
            "\n", path.c_str());
        return "";
    }
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
    std::printf("     -s, --assembly     | Generate and output picasso assembly\n");
}

static bool hasErrored = false;
void ErrorHandler(const std::string& errMsg, const std::string& offLine, int line, int offset) {
    printf(ANSI_COLOR_RED "error:" ANSI_COLOR_RESET "%d:%d: %s\n%s\n", line, offset, errMsg.c_str(), offLine.c_str());
    printf("%*c^\n", offset, ' ');
    hasErrored = true;
}

static Options ProcessArguments(int argc, char* argv[]) {
    Options op;
    for(int i = 1; i < argc; i++) {
        if(std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0) {
            PrintHelp(argv[0]);
            std::exit(EXIT_SUCCESS);
        } else if(std::strcmp(argv[i], "-o") == 0 || std::strcmp(argv[i], "--output") == 0) {
            op.output = argv[i++];
        } else if(std::strcmp(argv[i], "-s") == 0 || std::strcmp(argv[i], "--assembly") == 0) {
            op.assembly = true;
        } else if(std::strcmp(argv[i], "-v") == 0 || std::strcmp(argv[i], "--verbose") == 0) {
            op.verbose = true;
        } else {
            if(op.input.empty()) 
                op.input = argv[i];
        }
    }
    return op;
}

int main(int argc, char* argv[]) {
    // Process arguments
    Options op = ProcessArguments(argc, argv);

    // Read source code
    std::string source = SlurpFile(op.input);
    if(source.empty()) return 0;

    // Symbol Table
    SymbolTable table;

    // Lexical Analysis
    Lexer lexer(source, table);
    lexer.Callback = ErrorHandler;
    lexer.ReceiveLine = ReceiveLine;
    std::vector<Token> tokens = lexer.Tokenize();

    if(hasErrored) return EXIT_FAILURE;

    // Syntactic & Semantic Analysis
    Parser parser(tokens, table, source);
    parser.Callback = ErrorHandler;
    parser.ReceiveLine = ReceiveLine;
    std::optional<ParseNode> node = parser.ParseTranslationUnit();

    if(op.verbose)
        PrintParseTree(&node.value(), 0);

    if(hasErrored) return EXIT_FAILURE;

    AstBuilder builder = AstBuilder(node.value());
    AstNode root = builder.BuildTranslationUnit();

    if(op.verbose) {
        printf("\n");
        PrintAST(root, 0);
    }

    return 0;
}
#endif