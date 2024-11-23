#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include <optional>

struct ParseNode {
    enum {
        E,
        T,
        Expression,
        Declaration,
        FunctionCall,
        TypeQualifier,
        TypeSpecifier,
        LayoutQualifier,
        PrecisionQualifier,
        FunctionDefinition,
        FunctionDeclaration,
        UnaryExpression,
        PrimaryExpression,
        PostfixExpression,
        AssignmentExpression,
        AdditiveExpression,
        MultiplicativeExpression
    };

    Token token;
    int type;
    std::vector<ParseNode> children = {};

    ParseNode(int t) : type(t) {}

    ParseNode() : token(Token()), type(E) {}

    ParseNode(Token tok, int t = T) : token(tok), type(t) {}

    void Append(const ParseNode node) {
        children.insert(children.end(), node.children.begin(), node.children.end());
    }

    bool Empty() { return children.size() == 0 && type == E; }
};

class SymbolTable;
struct TableEntry;

class Parser {
    private:
    struct OperatorInfo {
        std::string symbol;
        bool leftAssoc;
        int precedence = 0;
        int exprNodeType;
    };

    struct ParserState {
        Token token;
        size_t index;
    };

    // Symbol Table
    SymbolTable& table;

    // Token checking
    void Match(int t);

    // Lookahead functionality
    void PopState();
    void PushState();
    void RestoreState();
    void DisableErrors();
    void EnableErrors();
    bool useErrors = false;

    // A dynamic list of parser states
    std::vector<ParserState> stateStack;

    // Current Parser State
    Token token;
    size_t index = 0;
    std::vector<Token>& tokens;
    
    #ifndef __3DS__
    friend int main(int argc, char* argv[]);
    #endif

    // Helper function for generating an error
    void Error(const std::string& s, Token t);

    // Check if the token adhere's to a group
    static bool IsJumpStatement(int t);
    static bool IsTypeSpecifier(int t);
    static bool IsTypeQualifier(int t);
    static bool IsUnaryOperator(int t);
    static bool IsBinaryOperator(int t);
    static bool IsPostfixOperator(int t);
    static bool IsSelectionStatement(int t);
    static bool IsIterationStatement(int t);
    static bool IsAssignmentOperator(int t);
    static bool IsPrecisionQualifier(int t);
    static bool IsParameterQualifier(int t);
    static bool IsExpressionSeperator(int t);
    static bool IsConstructorIdentifier(int t);

    // Helper functions for interfacing with the symbol table
    void InsertDeclaration(ParseNode& node, int type);
    TableEntry* LookupFunctionDeclaration(const std::string& id);

    // Helper functions for declaration parsing
    std::optional<ParseNode> ParseDeclaration();
    std::optional<ParseNode> ParseExternalDeclaration();
    std::optional<ParseNode> ParseSingleDeclaration();

    // Helper functions for type specifier/qualifier parsing
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
    std::optional<ParseNode> ParseSimpleStatement();
    std::optional<ParseNode> ParseStatementScope();
    std::optional<ParseNode> ParseJumpStatement() {return std::nullopt;}
    std::optional<ParseNode> ParseDeclarationStatement() {return std::nullopt;}
    std::optional<ParseNode> ParseExpressionStatement();
    std::optional<ParseNode> ParseIterationStatement() {return std::nullopt;}
    std::optional<ParseNode> ParseSelectionStatement() {return std::nullopt;}

    // Helper functions for expression parsing
    std::optional<ParseNode> ParseFunctionCall();
    std::optional<ParseNode> ParseUnaryExpression();
    std::optional<ParseNode> ParsePostfixExpression();
    std::optional<ParseNode> ParsePrimaryExpression();
    std::optional<ParseNode> ParseExpression(int minPrec);

    // TODO: Helper functions for struct parsing

    // Turn tokens into a parse tree
    std::optional<ParseNode> ParseTranslationUnit(void);

    // Get the tokens, symbol table and source code string
    Parser(std::vector<Token>& token, SymbolTable& t, std::string& s);

    // Error handling
    std::string& source;
    std::string(*ReceiveLine)(std::string& source, int line);
    void(*Callback)(const std::string&, const std::string&, int, int);

    // Operator Information for parsing expressions with precedence
    static std::array<OperatorInfo, 5> operatorInformation;
    std::optional<OperatorInfo> GetOperatorInfoFromString(std::string& s);

    // Internal support for the compiler library
    friend SelenaInfo SelenaCompileShaderSource(std::string& source);
};
#endif /* PARSER_HPP */