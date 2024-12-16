#ifndef AST_HPP
#define AST_HPP

#include <vector>
#include "lexer.hpp"

struct SelenaInfo;
struct ParseNode;

struct AstNode {
    enum {
        None,
        Function,
        Variable,
        Multiply,
        Assigment,
        Declaration,
        IntLiteral,
        FloatLiteral,
        FunctionCall,
    };

    Token token;
    int type;
    std::vector<AstNode> children = {};

    AstNode(int type = None) : type(type) {}

    AstNode(Token token) : token(token) {}

    AstNode(int type, Token token) : type(type), token(token) {}

    void Append(const AstNode node) {
        children.insert(children.end(), node.children.begin(), node.children.end());
    }
};

class AstBuilder {
    private:
    ParseNode& root;

    AstBuilder(ParseNode& root);
    
    #ifndef __3DS__
    friend int main(int argc, char* argv[]);
    #endif

    int OperatorToInstruction(Token t);

    AstNode BuildFunctionDefintion(ParseNode& p);
    AstNode BuildFunctionCall(ParseNode& p);
    AstNode BuildStatement(ParseNode& p);
    AstNode BuildExpression(ParseNode& p);
    AstNode BuildDeclaration(ParseNode& p);
    AstNode BuildPrimaryExpression(ParseNode& p);
    AstNode BuildStatementScope(ParseNode& p);
    AstNode BuildAssignmentExpression(ParseNode& p);

    AstNode BuildTranslationUnit(void);

    friend SelenaInfo SelenaCompileShaderSource(std::string& source);
};
#endif /* AST_HPP */