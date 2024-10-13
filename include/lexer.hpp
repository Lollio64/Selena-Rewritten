#ifndef LEXER_HPP
#define LEXER_HPP

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

    static std::string TokenToString(int t) { return ""; }

    Token(void) {}
};

// Forward declaration
struct SelenaInfo;
class SymbolTable;

class Lexer {
    private:
    // Keep everything internal for library
    #ifndef __3DS__
    friend int main(int argc, char* argv[]);
    #endif
    friend class Parser;

    // Source Code + Index
    std::string& source;
    size_t offset = 0;
    size_t index = 0;
    size_t line = 0;

    // Reserved keywords
    SymbolTable& table;

    // Consumes a char 
    char Consume(void);

    // Turn source code to tokens
    std::vector<Token> Tokenize(void);

    // Helper functions for lexing the source code
    Token Tokenize(std::string s);
    std::string ReadString(void);

    // Get the source code & symbol table
    Lexer(std::string& src, SymbolTable& t);

    // Helper function for generating an error
    void Error(const std::string& s, Token t);

    // A dictionary of valid symbols & keywords
    static std::map<std::string, int> keywords;

    // Error handling
    std::string(*ReceiveLine)(std::string& source, int line);
    void(*callback)(const std::string&, const std::string&, int, int);

    // Internal support for the compiler library
    friend SelenaInfo SelenaCompileShaderSource(std::string& source);
};
#endif /* LEXER_HPP */