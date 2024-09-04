#ifndef AST_H
#define AST_H

#include "lexer.h"
#include <vector>

enum AstNodeType {
    Root,
    //Multiply,
    IntValue,
    FloatValue,
    Variable,
    Function,
    Assignment,
    UniformDecl,
    Mat4Decl,
    Mat3Decl,
    Mat2Decl,
    Vec4Decl,
    Vec3Decl,
    Vec2Decl,
    IntDecl,
    FloatDecl,
    BoolDecl,
    InDecl,
    FunctionDecl,
};

struct AstNode {
    Token value;
    AstNodeType type;

    std::vector<AstNode> children;
    void AddChildNode(AstNode node);

    AstNode(Token value, AstNodeType type) :
    value(value), type(type) {}
};

class Ast {
    private:
    AstNode* root = NULL;
    public:
    Ast(void) { 
        root = new AstNode(Token(0, None, ""), Root);
    }
    ~Ast(void) { if(root) delete root; }
    AstNode* GetRoot();
};
#endif /* AST_H */