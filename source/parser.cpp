#include "parser.hpp"

Parser::Parser(std::vector<Token>& token) : tokens(token), index(0) {}

void Parser::Match(int t) {
    if(token.type == t)
        token = tokens[index++];
    else
        Error("expected" + TokenToString(t) + "before" 
        + TokenToString(token.type), token);
}

void Parser::Error(const std::string& s, Token t) {
    if(callback)
        callback(s, "", t.line, t.offset);
}

bool Parser::IsTypeSpecifier(int t) {
    switch(t) {
        case Token::Int:
        case Token::Bool:
        case Token::Void:
        case Token::Float:
        case Token::Struct:
        case Token::Mat4:
        case Token::Mat3:
        case Token::Mat2:
        case Token::Vec4:
        case Token::Vec3:
        case Token::Vec2:
        case Token::IVec4:
        case Token::IVec3:
        case Token::IVec2:
        case Token::BVec4:
        case Token::BVec3:
        case Token::BVec2:
        return true;
    }
    return false;
}

bool Parser::IsTypeQualifier(int t) {
    switch(t) {
        case Token::Attribute:
        case Token::Varying:
        case Token::Input:
        case Token::Inout:
        case Token::Const:
        case Token::Output:
        case Token::Uniform:
        return true;
    }
    return false;
}

bool Parser::IsPrecisionQualifier(int t) {
    switch(t) {
        case Token::Lowp:
        case Token::Highp:
        case Token::Mediump:
        return true;
    }
    return false;
}

bool Parser::IsParameterQualifier(int t) {
    switch(t) {
        case Token::Input:
        case Token::Inout:
        case Token::Output:
        return true;
    }
    return false;
}

bool Parser::IsConstructorIdentifier(int t) {
    switch(t) {
        case Token::Int:
        case Token::Bool:
        case Token::Float:
        case Token::Mat4:
        case Token::Mat3:
        case Token::Mat2:
        case Token::Vec4:
        case Token::Vec3:
        case Token::Vec2:
        case Token::IVec4:
        case Token::IVec3:
        case Token::IVec2:
        case Token::BVec4:
        case Token::BVec3:
        case Token::BVec2:
        return true;
    }
    return false;
}

std::optional<ParseNode> Parser::ParseFunctionHeader() {
    ParseNode node;
    if(!IsTypeQualifier(token.type)) {
        node.Append(ParseSpecifiedType().value());
        node.children.push_back(token);
        Match(Token::Identifer);
        node.children.push_back(token);
        Match(Token::OpenParenthese);
        return node;
    }
    return std::nullopt;
}

std::optional<ParseNode> Parser::ParseFunctionParameter() {
    ParseNode node = ParseNode(ParseNode::Declaration);
    if(IsParameterQualifier(token.type)) {
        node.children.push_back(token);
        Match(token.type);
        node.children.push_back(ParseTypeSpecifier().value());
        Match(token.type);
        node.children.push_back(token);
        Match(Token::Identifer);
        return node;
    } else {
        node.Append(ParseTypeSpecifier().value());
        Match(token.type);
        node.children.push_back(token);
        Match(Token::Identifer);
        return node;
    }
    return std::nullopt;
}

std::optional<ParseNode> Parser::ParseFunctionParameters() {
    ParseNode node = ParseFunctionHeader().value();
    if(token.type != Token::CloseParenthese) {
        node.children.push_back(ParseFunctionParameter().value());
        while(token.type == Token::Comma) {
            node.children.push_back(token);
            Match(Token::Comma);
            node.children.push_back(ParseFunctionParameter().value());
        }
    }
    return node;
}

std::optional<ParseNode> Parser::ParseFunctionPrototype() {
    ParseNode node;
    node.Append(ParseFunctionParameters().value());
    node.children.push_back(token);
    Match(Token::CloseParenthese);
    return node;
}

std::optional<ParseNode> Parser::ParsePrecisionQualifier() {
    ParseNode node;
    if(IsPrecisionQualifier(token.type)) {
        node = ParseNode(token, ParseNode::PrecisionQualifier);
        Match(token.type);
        return node;
    }
    return std::nullopt;
}

std::optional<ParseNode> Parser::ParseSpecifiedType() {
    ParseNode node;
    if(IsTypeQualifier(token.type)) {
        node.Append(ParseTypeQualifier().value());
    }
    node.Append(ParseTypeSpecifier().value());
    return node;
}

std::optional<ParseNode> Parser::ParseTypeSpecifier() {
    ParseNode node;
    if(IsTypeSpecifier(token.type)) {
        node = ParseNode(token, ParseNode::TypeSpecifier);
        Match(token.type);
        return node;
    }
    return std::nullopt;
}

std::optional<ParseNode> Parser::ParseTypeQualifier() {
    ParseNode node;
    if(IsTypeQualifier(token.type)) {
        node.children.push_back(ParseNode(token, ParseNode::TypeQualifier));
        Match(token.type);
        return node;
    }
    return std::nullopt;
}

std::optional<ParseNode> Parser::ParseSingleDeclaration() {
    ParseNode node;
    node.Append(ParseSpecifiedType().value());
    node.children.push_back(token);
    Match(Token::Identifer);
    return node;
}

std::optional<ParseNode> Parser::ParseExternalDeclaration() {
    ParseNode node;
    ParseSingleDeclaration();
    if(token.type == Token::OpenParenthese) {
        node = ParseNode(ParseNode::FunctionDeclaration);
        ParseFunctionPrototype();
        if(token.type == Token::OpenCurly) {
            node = ParseNode(ParseNode::FunctionDefinition);
            return ParseFunctionDefinition();
        }
    }
    Match(Token::SemiColon);
    return node;
}

std::optional<ParseNode> Parser::Parse(void) {
    ParseNode node;
    while(index < tokens.size()) {
        if(auto decl = ParseExternalDeclaration())
            node.children.push_back(decl.value());
        else return std::nullopt;
    }
    return node;
}