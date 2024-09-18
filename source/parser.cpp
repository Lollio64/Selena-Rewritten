#include "parser.hpp"

Parser::Parser(std::vector<Token>& token) : tokens(token), index(0) {}

std::optional<Token> Parser::Peek(size_t offset) {
    if(index + offset >= tokens.size()) {
        return std::nullopt;
    }
    return tokens.at(index + offset);
}

Token Parser::Consume(void) {
    return tokens.at(index++);
}

bool Parser::IsTypeSpecifier(TokenType t) {
    switch(t) {
        case Int:
        case Bool:
        case Void:
        case Float:
        case Struct:
        case Mat4:
        case Mat3:
        case Mat2:
        case Vec4:
        case Vec3:
        case Vec2:
        case IVec4:
        case IVec3:
        case IVec2:
        case BVec4:
        case BVec3:
        case BVec2:
        return true;
    }
    return false;
}

bool Parser::IsTypeQualifier(TokenType t) {
    switch(t) {
        case Attribute:
        case Varying:
        case Input:
        case Inout:
        case Const:
        case Output:
        case Uniform:
        return true;
    }
    return false;
}

bool Parser::IsPrecisionQualifier(TokenType t) {
    switch(t) {
        case Lowp:
        case Highp:
        case Mediump:
        return true;
    }
    return false;
}

bool Parser::IsConstructorIdentifier(TokenType t) {
    switch(t) {
        case Int:
        case Bool:
        case Float:
        case Mat4:
        case Mat3:
        case Mat2:
        case Vec4:
        case Vec3:
        case Vec2:
        case IVec4:
        case IVec3:
        case IVec2:
        case BVec4:
        case BVec3:
        case BVec2:
        return true;
    }
    return false;
}

std::optional<ParseNode> Parser::ParseSpecifiedType() {
    ParseNode node;
    if(IsTypeQualifier(Peek().value().type)) {
        node.children.push_back(ParseNode(Peek().value()));
        Consume();
    }
    if(IsTypeSpecifier(Peek().value().type)) {
        node.children.push_back(ParseNode(Peek().value()));
        Consume();
        if(Peek().value().type == Identifer) {
            node.children.push_back(ParseNode(Peek().value()));
            Consume();
        } else printf("expected identifier after type declaration");
        return node;
    }
    return std::nullopt;
}

std::optional<ParseNode> Parser::ParseDeclaration() {
    ParseNode node;
    if(IsTypeQualifier(Peek().value().type) || IsTypeSpecifier(Peek().value().type) || IsPrecisionQualifier(Peek().value().type)) {
        if(auto type = ParseSpecifiedType())
            if(type.has_value()) node = type.value();
    }
    return std::nullopt;
}

std::optional<ParseNode> Parser::ParseStatement() {
    return std::nullopt;
}

std::optional<ParseNode> Parser::Parse(void) {
    ParseNode node;
    while(Peek().has_value()) {
        node.children.push_back(ParseStatement().value());
    }
    return node;
}