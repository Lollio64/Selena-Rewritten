#include "lexer.h"

Lexer::Lexer(std::string& src) : source(src), index(0) {}

std::optional<char> Lexer::Peek(size_t off) {
    if(index + off >= source.length()) {
        return std::nullopt;
    }
    return source.at(index + off);
}

char Lexer::Consume(void) {
    return source.at(index++);
}

std::vector<Token> Lexer::Tokenize(void) {
    // Buffer of code tokens
    std::vector<Token> tokens;

    // Token Information
    int currentLine = 1;
    std::string tokenBuffer;

    while(Peek().has_value()) {
        // Check if character is alphabetic
        if(std::isalpha(Peek().value()) || Peek().value() == '#') {
            // Read all the characters 
            tokenBuffer += Consume();
            while(Peek().has_value() && std::isalnum(Peek().value()) || Peek().value() != ';' && !std::isspace(Peek().value()))
                tokenBuffer += Consume();
           
            // Check if token is valid or an identifier
            if(tokenBuffer == "void") {
                tokens.push_back(Token(currentLine, Void, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "mat4") {
                tokens.push_back(Token(currentLine, Mat4, tokenBuffer));
                tokenBuffer.clear(); 
            } else if(tokenBuffer == "vec4") {
                tokens.push_back(Token(currentLine, Vec4, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "vec3") {
                tokens.push_back(Token(currentLine, Vec3, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "vec2") {
                tokens.push_back(Token(currentLine, Vec2, tokenBuffer));
                tokenBuffer.clear(); 
            } else if(tokenBuffer == "in") {
                tokens.push_back(Token(currentLine, Input, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "out") {
                tokens.push_back(Token(currentLine, Output, tokenBuffer));
                tokenBuffer.clear(); 
            } else if(tokenBuffer == "uniform") {
                tokens.push_back(Token(currentLine, Uniform, tokenBuffer));
                tokenBuffer.clear(); 
            } else if(tokenBuffer == "const") {
                tokens.push_back(Token(currentLine, Const, tokenBuffer));
                tokenBuffer.clear(); 
            } else if(tokenBuffer == "mat2") {
                tokens.push_back(Token(currentLine, Mat2, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "mat3") {
                tokens.push_back(Token(currentLine, Mat3, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "if") {
                tokens.push_back(Token(currentLine, If, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "else") {
                tokens.push_back(Token(currentLine, Else, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "bool") {
                tokens.push_back(Token(currentLine, BoolVar, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "int") {
                tokens.push_back(Token(currentLine, IntVar, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "float") {
                tokens.push_back(Token(currentLine, FloatVar, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "layout") {
                tokens.push_back(Token(currentLine, Layout, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "location") {
                tokens.push_back(Token(currentLine, Location, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "inout") {
                tokens.push_back(Token(currentLine, Inout, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "return") {
                tokens.push_back(Token(currentLine, Return, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "struct") {
                tokens.push_back(Token(currentLine, Struct, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "precision") {
                tokens.push_back(Token(currentLine, Precision, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "mediump") {
                tokens.push_back(Token(currentLine, Mediump, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "highp") {
                tokens.push_back(Token(currentLine, Highp, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "lowp") {
                tokens.push_back(Token(currentLine, Lowp, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "true") {
                tokens.push_back(Token(currentLine, True, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "false") {
                tokens.push_back(Token(currentLine, False, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "#version") {
                tokenBuffer.clear();
                while(Peek().value() != '\n')
                    Consume();
            } else if(tokenBuffer == "#define") {
                tokens.push_back(Token(currentLine, Macro, tokenBuffer));
                tokenBuffer.clear();
            } else if(tokenBuffer == "asm") {
                tokens.push_back(Token(currentLine, Assembly, tokenBuffer));
                tokenBuffer.clear();
            } else {
                tokens.push_back(Token(currentLine, Identifer, tokenBuffer));
                tokenBuffer.clear();
            }
        } else if(std::isdigit(Peek().value())) {
            tokenBuffer += Consume();
            while(std::isdigit(Peek().value()))
                tokenBuffer += Consume();
            if(Peek().value() == '.') {
                tokenBuffer += Consume();
                while(std::isdigit(Peek().value()))
                    tokenBuffer += Consume();
                tokens.push_back(Token(currentLine, Float, tokenBuffer));
                tokenBuffer.clear();
                continue;
            }
            tokens.push_back(Token(currentLine, Integer, tokenBuffer));
            tokenBuffer.clear();
        } else if(Peek().value() == '/' && Peek(1).value() == '/') {
            Consume();
            Consume();
            while(Peek().value() != '\n')
                Consume();
        } else if(Peek().value() == '/' && Peek(1).value() == '*') {
            Consume();
            Consume();
            while(Peek().value() != '*' && Peek(1).value() != '/') {
                if(Peek().value() == '\n')
                    currentLine++;
                Consume();
            }
            Consume();
            Consume();
        } else if(Peek().value() == '(') {
            tokenBuffer += Consume();
            tokens.push_back(Token(currentLine, OpenCurly, tokenBuffer));
            tokenBuffer.clear();
        } else if(Peek().value() == ')') {
            tokenBuffer += Consume();
            tokens.push_back(Token(currentLine, CloseCurly, tokenBuffer));
            tokenBuffer.clear();
        } else if(Peek().value() == '{') {
            tokenBuffer += Consume();
            tokens.push_back(Token(currentLine, OpenParenthese, tokenBuffer));
            tokenBuffer.clear();
        } else if(Peek().value() == '}') {
            tokenBuffer += Consume();
            tokens.push_back(Token(currentLine, OpenParenthese, tokenBuffer));
            tokenBuffer.clear();
        } else if(Peek().value() == ';') {
            tokenBuffer += Consume();
            tokens.push_back(Token(currentLine, SemiColon, tokenBuffer));
            tokenBuffer.clear();
        } else if(Peek().value() == '=') {
            tokenBuffer += Consume();
            tokens.push_back(Token(currentLine, Equal, tokenBuffer));
            tokenBuffer.clear();
        } else if(Peek().value() == '+') {
            tokenBuffer += Consume();
            tokens.push_back(Token(currentLine, Add, tokenBuffer));
            tokenBuffer.clear();
        } else if(Peek().value() == '-') {
            tokenBuffer += Consume();
            tokens.push_back(Token(currentLine, Subtract, tokenBuffer));
            tokenBuffer.clear();
        } else if(Peek().value() == '*') {
            tokenBuffer += Consume();
            tokens.push_back(Token(currentLine, Multiply, tokenBuffer));
            tokenBuffer.clear();
        } else if(Peek().value() == '/') {
            tokenBuffer += Consume();
            tokens.push_back(Token(currentLine, Divide, tokenBuffer));
            tokenBuffer.clear();
        } else if(Peek().value() == '.') {
            tokenBuffer += Consume();
            tokens.push_back(Token(currentLine, Dot, tokenBuffer));
            tokenBuffer.clear();
        } else if(Peek().value() == '+' && Peek(1).value() == '=') {
            tokenBuffer += Consume();
            tokenBuffer += Consume();
            tokens.push_back(Token(currentLine, PlusEqual, tokenBuffer));
            tokenBuffer.clear();    
        } else if(Peek().value() == '-' && Peek(1).value() == '=') {
            tokenBuffer += Consume();
            tokenBuffer += Consume();
            tokens.push_back(Token(currentLine, MinusEqual, tokenBuffer));
            tokenBuffer.clear();
        } else if(Peek().value() == '*' && Peek(1).value() == '=') {
            tokenBuffer += Consume();
            tokenBuffer += Consume();
            tokens.push_back(Token(currentLine, TimesEqual, tokenBuffer));
            tokenBuffer.clear();
        } else if(Peek().value() == '/' && Peek(1).value() == '=') {
            tokenBuffer += Consume();
            tokenBuffer += Consume();
            tokens.push_back(Token(currentLine, DivideEqual, tokenBuffer));
            tokenBuffer.clear();
        } else if(Peek().value() == ',') {
            tokenBuffer += Consume();
            tokens.push_back(Token(currentLine, Comma, tokenBuffer));
            tokenBuffer.clear();
        } else if(Peek().value() == '\n') {
            Consume();
            currentLine++;
        } else if(std::isspace(Peek().value())) {
            Consume();
        } else {
            while(!std::isspace(Peek().value()))
                tokenBuffer += Consume();
            printf("Error: At line %i, there is a unidentifiable token: '%s'.", currentLine, tokenBuffer.c_str());
            exit(EXIT_FAILURE);
        }
    }

    index = 0;
    return tokens;
}