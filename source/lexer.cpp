#include "lexer.hpp"
#include <iostream>
#include "symbol.hpp"
//#include "preprocessor.hpp"

Lexer::Lexer(std::string& src, SymbolTable& t) : source(src), offset(0), index(0), line(0), table(t) {}

char Lexer::Consume(void) { offset++; return source.at(index++); } 

std::string Token::TokenToString(int t) { 
    for(auto k : Lexer::keywords)
        if(t == k.second)
            return k.first;
    return "";
}

std::map<std::string, int> Lexer::keywords = {
    {"=", Token::Equal},
    {"*", Token::Star},
    {",", Token::Comma},
    {"in", Token::Input},
    {"mat4", Token::Mat4},
    {"vec3", Token::Vec3},
    {"vec2", Token::Vec2},
    {"void", Token::Void},
    {";", Token::SemiColon},
    {"{", Token::OpenCurly},
    {"}", Token::CloseCurly},
    {"layout", Token::Layout},
    {"uniform", Token::Uniform},
    {"(", Token::OpenParenthese},
    {")", Token::CloseParenthese},
};

std::vector<Token> Lexer::Tokenize(void) {
    std::vector<Token> tokens;
    while(index < source.length()) {
        std::string buf = ReadString();
        if(buf.empty()) continue;

        Token token = Tokenize(buf);
        if(token.type == Token::Invalid) {
            Error("syntax error", token);
        }
        if(token.type == Token::Identifier) {
            if(token.value[0] == 'g' && token.value[1] == 'l' 
            && token.value[2] == '_' && !table.Lookup(token.value)) {
                Error("illegal usage of reserved keyword '" 
                + Token::TokenToString(token) + "'", token);
            }
            for(size_t i = 0; i < table.reserved.size(); i++) {
                if(token.value == table.reserved.at(i)) {
                    Error("illegal usage of reserved keyword"
                    + Token::TokenToString(token) + "'", token);
                    break;
                }
            }
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
            Consume();
            if(source[index] == '\n') {
                offset = 0;
                line++;
            }
            if(index >= source.length()) {
                Error("unclosed comment at the end of file", Token(line, offset, 0, ""));
                exit(EXIT_FAILURE); // force exit due structure of Tokenize() loop
            }
        }
        return "";
    } else if(source[index] == '\n') {
        index++;
        line++;
        offset = 0;
    } else if(std::isspace(source[index])) {
        Consume();
    } else {          
        while(!std::isspace(source[index]) &&  !std::isalnum(source[index])) {
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
        t = Token(line, offset, Token::Identifier, s);
    } else if(std::isdigit(s[0])) {
        for(size_t i = 0; i < s.length(); i++) {
            if(s[i] == '.') {
                t = Token(line, offset, Token::FloatLit, s);
                return t;
            }
        }
        t = Token(line, offset, Token::IntegerLit, s);
    } else t = Token(line, offset, Token::Invalid, s);
    return t;
}

void Lexer::Error(const std::string& s, Token t) {
    std::string line = ReceiveLine(source, t.line);
    if(callback) 
        callback(s, line, t.line, t.offset);
}