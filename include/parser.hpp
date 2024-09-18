#ifndef PARSER_H
#define PARSER_H

#include "ast.hpp"
#include "lexer.hpp"
#include <optional>

enum NodeType {
    E,
    T,
    DECLARATION,
    FUNCTION_DEFINITION,
    FUNCTION_CALL,
    TYPE_QUALIFIER,
    TYPE_SPECIFIER,
    PRECISION_QUALIFER,
    ASSIGNMENT_EXPR,
    CONDITIONAL_EXPR,
    PRIMARY_EXPRESSION,
    EXPRESSION,
    LOGOR_EXPR,
    MULTIPLY_EXPR
};

struct ParseNode {
    std::vector<ParseNode> children;
    NodeType type;
    Token token;

    ParseNode() : type(E) {}

    ParseNode(NodeType t) : type(t) {}

    ParseNode(Token tok, NodeType t = T) : token(tok), type(t) {}

    void Append(const ParseNode &node) {
        children.insert(children.end(), node.children.begin(), node.children.end());
    }

    //bool Empty() { return children.size() == 0 && type == E; }
};

class Parser {
    private:
    // Tokens + Index
    std::vector<Token>& tokens;
    size_t index = 0;

    // Consumes a token
    Token Consume(void);
    public:
    // Get the tokens
    Parser(std::vector<Token>& token);

    // Turn tokens into a parse tree
    std::optional<ParseNode> Parse(void);
    private:
    // Token group checking
    bool IsTypeSpecifier(TokenType t);
    bool IsTypeQualifier(TokenType t);
    bool IsPrecisionQualifier(TokenType t);
    bool IsConstructorIdentifier(TokenType t);

    // Parse a statement/declaration
    std::optional<ParseNode> ParseStatement();
    std::optional<ParseNode> ParseDeclaration();
    std::optional<ParseNode> ParseSpecifiedType();
  
    // Peek a few tokens ahead
    std::optional<Token> Peek(size_t offset = 0);
};
#endif /* PARSER_H */