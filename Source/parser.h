#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"
#include <optional>

class Parser {
    private:
    // Tokens + Index
    std::vector<Token>& tokens;
    size_t index = 0;

    // Consumes a token
    Token Consume(void);
    public:
    // Turn tokens into ast
    std::optional<Ast> Parse(void);

    // Get the tokens
    Parser(std::vector<Token>& token);
    private:
    // Parse a statement
    std::optional<AstNode> ParseStatement();

    // Peek a few tokens ahead
    std::optional<Token> Peek(size_t off = 0);
};
#endif /* PARSER_H */