#ifndef AST_HPP
#define AST_HPP

struct ParseNode;

struct AstNode {
    enum {
        Function,
        Variable,
        Multiply,
        Assigment
    };
};

class AstBuilder {
    private:
    ParseNode& root;

    AstBuilder(ParseNode& root);

    AstNode BuildExpression();

    AstNode BuildTranslationUnit(void);
};
#endif /* AST_HPP */