#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include <optional>

struct ParseNode {
    enum {
        E,
        T,
        Declaration,
        TypeQualifier,
        TypeSpecifier,
        LayoutQualifier,
        PrecisionQualifier,
        FunctionDefinition,
        FunctionDeclaration,
        PrimaryExpression,
        AssigmentExpression,
        MultiplicativeExpression,
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
    struct ParserState {
        Token token;
        size_t index;
    };

    Lexer& lexer;

    // Token checking
    void Match(int t);

    // Lookahead functionality
    void PopState();
    void PushState();
    void RestoreState();
    void DisableErrors();
    void EnableErrors();

    // Current Parser State
    Token token;
    size_t index = 0;
    std::vector<Token>& tokens;

    // Function to call upon error
    bool useErrors = false;
    
    friend int main(int argc, char* argv[]);

    // A dynamic list of parser states
    std::vector<ParserState> stateStack;

    // Turn tokens into a parse tree
    std::optional<ParseNode> Parse(void);

    // Helper function for generating an error
    void Error(const std::string& s, Token t);

    // Get the tokens
    Parser(std::vector<Token>& token, Lexer& l);

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
    std::optional<ParseNode> ParseLayoutQualifier();

    // Helper functions for function parsing
    std::optional<ParseNode> ParseFunctionHeader();
    std::optional<ParseNode> ParseFunctionParameters();
    std::optional<ParseNode> ParseFunctionParameter();
    std::optional<ParseNode> ParseFunctionPrototype();
    std::optional<ParseNode> ParseFunctionDefinition();

    // Helper functions for statement parsing
    std::optional<ParseNode> ParseStatementList();

    // Helper functions for expression parsing
    std::optional<ParseNode> ParsePrimaryExpression();
    std::optional<ParseNode> ParseAssigmentExpression();
    std::optional<ParseNode> ParseMultiplicativeExpression();

    // TODO: Helper functions for struct parsing

    void(*callback)(const std::string&, const std::string&, int, int);

    // Internal support for the compiler library
    friend SelenaInfo SelenaCompileShaderSource(std::string& source);
};
#endif /* PARSER_HPP */