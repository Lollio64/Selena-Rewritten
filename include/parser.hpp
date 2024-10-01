#ifndef PARSER_H
#define PARSER_H

#include "ast.hpp"
#include "lexer.hpp"
#include <optional>

using ErrorCallback = void(*)(const std::string&, const std::string&, int, int);

struct ParseNode {
    enum {
        E,
        T,
        Declaration,
        TypeQualifier,
        TypeSpecifier,
        PrecisionQualifier,
        FunctionDefinition,
        FunctionDeclaration
    };

    int type;
    Token token;
    std::vector<ParseNode> children;

    ParseNode() : type(E) {}

    ParseNode(int t) : type(t) {}

    ParseNode(Token tok, int t = T) : token(tok), type(t) {}

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

    // Function to call upon error
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

    // Helper functions for declaration parsing
    std::optional<ParseNode> ParseDeclaration();
    std::optional<ParseNode> ParseExternalDeclaration();
    std::optional<ParseNode> ParseSingleDeclaration();

    // Helper functions for type parsing
    std::optional<ParseNode> ParseSpecifiedType();
    std::optional<ParseNode> ParseTypeSpecifier();
    std::optional<ParseNode> ParseTypeQualifier();
    std::optional<ParseNode> ParsePrecisionQualifier();

    // Helper functions for function parsing
    std::optional<ParseNode> ParseFunctionHeader();
    std::optional<ParseNode> ParseFunctionParameters();
    std::optional<ParseNode> ParseFunctionParameter();
    std::optional<ParseNode> ParseFunctionPrototype();
    std::optional<ParseNode> ParseFunctionDefinition();

    // TODO: Helper functions for statement parsing

    // TODO: Helper functions for expression parsing
};
#endif /* PARSER_H */