#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <optional>

enum TokenType {
    None,

    Void,
    True,
    False,
    IntVar,
    BoolVar,
    FloatVar,

    Mat4,
    Mat3,
    Mat2,

    Vec4,
    Vec3,
    Vec2,

    Const,
    Input,
    Inout,
    Struct,
    Output,
    Layout,
    Uniform,
    Location,

    Add,
    Subtract,
    Multiply,
    Divide,

    Equal,
    PlusEqual,
    MinusEqual,
    TimesEqual,
    DivideEqual,

    If,
    Else,
    
    Float,
    Integer,

    Precision,
    Mediump,
    Highp,
    Lowp,

    Dot,
    Comma,
    OpenCurly,
    CloseCurly,
    OpenParenthese,
    CloseParenthese,
    SemiColon,
    Identifer,
    Return,
    Macro,

    Assembly
};

struct Token {
    int line = 0;
    TokenType type;
    std::string value;

    Token(int l, TokenType t, std::string v) :
    line(l), type(t), value(v) {}
};

class Lexer {
    private:
    // Source Code + Index
    std::string& source;
    size_t index = 0;

    // Consumes a char 
    char Consume(void);
    public:
    // Get the source code
    Lexer(std::string& src);

    // Turn source code to tokens
    std::vector<Token> Tokenize(void);

    private:
    // Peek a few characters ahead
    std::optional<char> Peek(size_t off = 0);
};
#endif /* LEXER_H */