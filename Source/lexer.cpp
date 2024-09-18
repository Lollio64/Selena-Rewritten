#include "../include/lexer.hpp"
#include <iostream>
//#include "preprocessor.hpp"

Lexer::Lexer(std::string& src) : source(src), index(0), offset(0), line(0) {}

char Lexer::Consume(void) { offset++; return source.at(index++); }

std::map<std::string, Token::TokenType> Lexer::keywords = {
    {"layout", Token::Layout},
    {"location", Token::Location},
    {"uniform", Token::Uniform},
    {"mat4", Token::Mat4},
    {"vec3", Token::Vec3},
    {"vec2", Token::Vec2},
    {"*", Token::Multiply},
    {"void", Token::Void},
    {"(", Token::OpenParenthese},
    {")", Token::CloseParenthese},
    {"=", Token::Equal},
    {";", Token::SemiColon},
    {"{", Token::OpenCurly},
    {"}", Token::CloseCurly},
    {",", Token::Comma}
};

std::vector<Token> Lexer::Tokenize(void) {
    std::vector<Token> tokens;
    while(index < source.length()) {
        std::string buf = ReadString();
        if(buf.empty()) continue;

        Token token = Tokenize(buf);
        if(token.type == Token::Invalid) {
            std::cout << line << ":" << offset
            << ": error: syntax error '" 
            << token.value << "'\n";
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
        index++;
        offset++;
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