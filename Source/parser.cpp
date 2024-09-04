#include "parser.h"

Parser::Parser(std::vector<Token>& token) : tokens(token), index(0) {}

std::optional<Token> Parser::Peek(size_t off) {
    if(index + off >= tokens.size()) {
        return std::nullopt;
    }
    return tokens.at(index + off);
}

Token Parser::Consume(void) {
    return tokens.at(index++);
}

static AstNodeType TypeToDecl(TokenType t) {
    switch(t) {
        case Mat2:
        return Mat2Decl;
        break;

        case Mat3:
        return Mat3Decl;
        break;

        case Mat4:
        return Mat4Decl;
        break;

        case Vec2:
        return Vec2Decl;
        break;

        case Vec3:
        return Vec3Decl;
        break;

        case Vec4:
        return Vec4Decl;
        break;

        case IntVar:
        return IntDecl;
        break;

        case FloatVar:
        return FloatDecl;
        break;

        case BoolVar:
        return BoolDecl;
        break;

        default:
        return Root;
        break;
    }
}

std::optional<AstNode> Parser::ParseStatement() {
    if(Peek().has_value() && Peek().value().type == Uniform) {
        AstNode uniform(Peek().value(), UniformDecl);
        Consume();
        if(Peek().has_value()) {
            auto type = TypeToDecl(Peek().value().type);
            if(type == Root) {
                printf("At line %i, there's an invalid type declaration:'%s'.", Peek().value().line, Peek().value().value.c_str());
                exit(EXIT_FAILURE);
            }
            AstNode unifType(Peek().value(), type);
            uniform.AddChildNode(unifType);
            Consume();

            if(Peek().value().type == Identifer) {
                AstNode unifIdent(Peek().value(), Variable);
                uniform.AddChildNode(unifIdent);
                Consume();
            } else {
                printf("At line %i, there's a missing or invalid identifier:'%s'.", Peek().value().line, Peek().value().value.c_str());
                exit(EXIT_FAILURE);
            }

            if(!Peek().value().type == SemiColon) {     
                printf("At line %i, there's a missing semicolon:'%s'.", Peek().value().line, Peek().value().value.c_str());
                exit(EXIT_FAILURE);
            } else Consume();
            return uniform;
        }
    } else if(Peek().has_value() && Peek().value().type == Input) {
        AstNode input(Peek().value(), InDecl);
        Consume();
        if(Peek().has_value()) {
            auto type = TypeToDecl(Peek().value().type);
            if(type == Root) {
                printf("At line %i, there's an invalid type declaration:'%s'.", Peek().value().line, Peek().value().value.c_str());
                exit(EXIT_FAILURE);
            }
            AstNode inType(Peek().value(), type);
            input.AddChildNode(inType);
            Consume();

            if(Peek().value().type == Identifer) {
                AstNode inIdent(Peek().value(), Variable);
                input.AddChildNode(inIdent);
                Consume();
            } else {
                printf("At line %i, there's a missing or invalid identifier:'%s'.", Peek().value().line, Peek().value().value.c_str());
                exit(EXIT_FAILURE);
            }

            if(!Peek().value().type == SemiColon) {
                printf("At line %i, there's a missing semicolon:'%s'.", Peek().value().line, Peek().value().value.c_str());
                exit(EXIT_FAILURE);
            } else Consume();
            return input;
        }
    } /*else if(Peek().has_value() && TypeToDecl(Peek().value().type) != Root) {
        AstNode type(Peek().value(), TypeToDecl(Peek().value().type));
        if(Peek(1).value().type != Identifer) {
            printf("At line %i, there's a missing or invalid identifier:'%s'.", Peek(1).value().line, Peek(1).value().value.c_str());
            exit(EXIT_FAILURE);
        } else Consume();
        
        AstNode ident(Peek().value(), Variable);
        Consume();

        if(Peek().value().type == OpenCurly) {
            Consume();
            AstNode funcdecl(Token(type.value.line, None, ""), FunctionDecl);
            funcdecl.AddChildNode(type);
            funcdecl.AddChildNode(ident);
            while(Peek().value().type != CloseCurly) {}
            Consume();
        }
    }*/
    return std::nullopt;
}

std::optional<Ast> Parser::Parse(void) {
    Ast ast;
    while(Peek().has_value()) {
        if(auto stmt = ParseStatement())
            ast.GetRoot()->AddChildNode(stmt.value());
        else Consume();
    }
    return ast;
}
