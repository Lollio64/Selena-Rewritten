#ifndef PARSER_H
#define PARSER_H

#include "ast.hpp"
#include "lexer.hpp"
#include <optional>

using ErrorCallback = void(const std::string&, const std::string&, int, int);

struct ParseNode {
    enum NodeType {
        E,
        T,
        Declaration,
        TypeQualifier,
        TypeSpecifier,
        PrecisionQualifier,
        FunctionDefinition,
        FunctionDeclaration
    };

    std::vector<ParseNode> children;
    NodeType type;
    Token token;

    ParseNode() : type(E) {}

    ParseNode(NodeType t) : type(t) {}

    ParseNode(Token tok, NodeType t = T) : token(tok), type(t) {}

    void Append(const ParseNode node) {
        children.insert(children.end(), node.children.begin(), node.children.end());
    }

    //bool Empty() { return children.size() == 0 && type == E; }
};

class Parser {
    private:
    // Tokens + Index
    std::vector<Token>& tokens;
    size_t index = 0;
    Token token;

    // Function to call upon eror
    ErrorCallback callback;

    // Check for correct token
    void Match(int t);
    public:
    // Get the tokens
    Parser(std::vector<Token>& token);

    // Turn tokens into a parse tree
    std::optional<ParseNode> Parse(void);
    private:
    // Helper function for generating an error
    void Error(const std::string& s, Token t);

    // Token group checking
    static bool IsTypeSpecifier(int t);
    static bool IsTypeQualifier(int t);
    static bool IsPrecisionQualifier(int t);
    static bool IsParameterQualifier(int t);
    static bool IsConstructorIdentifier(int t);

    // Helper functions for semantic analysis
    std::optional<ParseNode> ParseDeclaration();
    std::optional<ParseNode> ParseExternalDeclaration();
    std::optional<ParseNode> ParseSingleDeclaration();

    // Helper functions for type parsing
    std::optional<ParseNode> ParseSpecifiedType();
    std::optional<ParseNode> ParseTypeSpecifier();
    std::optional<ParseNode> ParseTypeQualifier();
    std::optional<ParseNode> ParsePrecisionQualifier();

    std::optional<ParseNode> ParseFunctionHeader();
    std::optional<ParseNode> ParseFunctionDeclarator();
    std::optional<ParseNode> ParseParameterDeclaration();
    std::optional<ParseNode> ParseFunctionPrototype();
    std::optional<ParseNode> ParseFunctionDefinition();
};
#endif /* PARSER_H */