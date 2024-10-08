#include "lexer.hpp"
#include <iostream>
//#include "preprocessor.hpp"

Lexer::Lexer(std::string& src) : source(src), index(0), offset(0), line(0) {}

char Lexer::Consume(void) { offset++; return source.at(index++); }

std::map<std::string, int> Lexer::keywords = {
    {"=", Token::Equal},
    {",", Token::Comma},
    {"mat4", Token::Mat4},
    {"vec3", Token::Vec3},
    {"vec2", Token::Vec2},
    {"void", Token::Void},
    {"*", Token::Multiply},
    {";", Token::SemiColon},
    {"{", Token::OpenCurly},
    {"}", Token::CloseCurly},
    {"layout", Token::Layout},
    {"uniform", Token::Uniform},
    {"(", Token::OpenParenthese},
    {")", Token::CloseParenthese},
};

std::string Lexer::GetLine(int line) {
    std::string ret = "";
    int currentLine = 1;
    for(size_t i = 0; i < source.length(); i++) {
        if(source[i] == '\n') continue;
        if(currentLine == line - 1) {
            ret.push_back(source[i]);
            while(source[i] != '\n') {
                ret.push_back(source[i++]);
            }
            break;
        }
    }
    return ret;
}

std::vector<Token> Lexer::Tokenize(void) {
    std::vector<Token> tokens;
    while(index < source.length()) {
        std::string buf = ReadString();
        if(buf.empty()) continue;

        Token token = Tokenize(buf);
        if(token.type == Token::Invalid) {
            Error("syntax error", token);
        }
        tokens.push_back(token);
    }
    return tokens;
}

std::string Lexer::ReadString(void) {
    std::string ret;
    if(std::isalpha(source[index])) {
        while(std::isalnum(source[index]) || source[index] == '_')
            ret.push_back(Consume());
    } else if(std::isdigit(source[index])) {
        while(std::isdigit(source[index]))
            ret.push_back(Consume());
        if(source[index] == '.') {
            ret.push_back(Consume());
            while(std::isdigit(source[index]))
                ret.push_back(Consume());
        }
    } else if(source[index] == '/' && source[index + 1] == '/') {
        while(source[index] != '\n')
            index++;
        line++;
        offset = 0;
        return "";
    } else if(source[index] == '/' && source[index + 1] == '*') {
        while(source[index] != '*' && source[index + 1] != '/') {
            index++;
            offset++;
            if(source[index] == '\n') {
                offset = 0;
                line++;
            }
            if(index > source.length())
                break;
        }
        return "";
    } else if(source[index] == '\n') {
        index++;
        line++;
        offset = 0;
    } else if(std::isspace(source[index])) {
        Consume();
    } else {          
        while(!std::isspace(source[index]) && !std::isalnum(source[index])) {
            ret.push_back(Consume());
            if(index >= source.length() || source[index] != '=')
                break;
        }
    }
    return ret;
}

Token Lexer::Tokenize(std::string s) {
    Token t;
    if(keywords.find(s) != keywords.end()) {
        t = Token(line, offset, keywords.find(s)->second, s);
    } else if(std::isalpha(s[0])) {
        t = Token(line, offset, Token::Identifer, s);
    } else if(std::isdigit(s[0])) {
        for(int i = 0; i < s.length(); i++) {
            if(s[i] == '.') {
                t = Token(line, offset, Token::FloatLit, s);
                return t;
            }
            t = Token(line, offset, Token::IntegerLit, s);
        }
    } else t = Token(line, offset, Token::Invalid, s);
    return t;
}

void Lexer::Error(const std::string& s, Token t) {
    std::string line = GetLine(t.line);
    if(callback) 
        callback(s, line, t.line, t.offset);
}