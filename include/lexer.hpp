#ifndef LEXER_H
#define LEXER_H

#include <map>
#include <string>
#include <vector>
#include <optional>

struct Token {
    enum {
        None,

        Void,
        True,
        False,
        Int,
        Bool,
        Float,

        Mat4,
        Mat3,
        Mat2,

        Vec4,
        Vec3,
        Vec2,

        IVec4,
        IVec3,
        IVec2,

        BVec4,
        BVec3,
        BVec2,

        Const,
        Input,
        Inout,
        Struct,
        Output,
        Layout,
        Uniform,
        Location,
        Attribute,
        Varying,

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
    
        FloatLit,
        IntegerLit,

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

        Assembly,

        Invalid,
    };

    int type;
    int line = 0;
    int offset = 0;
    std::string value;

    Token(int line, int offset, int t, std::string val) :
    line(line), offset(offset), type(t), value(val) {}

    Token(void) {}
};

std::string TokenToString(int t) {}

class SymbolTable;

class Lexer {
    private:
    // Source Code + Index
    std::string& source;
    size_t offset = 0;
    size_t index = 0;
    size_t line = 0;

    // Symbol Table
    SymbolTable* table;

    // Consumes a char 
    char Consume(void);

    // Gets contents of a line
    std::string GetLine(int line);
    public:
    // Turn source code to tokens
    std::vector<Token> Tokenize(void);

    // Get the source code & symbol table
    Lexer(std::string& src, SymbolTable* t);

    private:
    // Helper functions for lexing the source code
    Token Tokenize(std::string s);
    std::string ReadString(void);

    // A dictionary of valid symbols & keywords
    static std::map<std::string, int> keywords;
};
#endif /* LEXER_H */