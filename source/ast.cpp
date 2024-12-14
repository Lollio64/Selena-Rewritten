#include "ast.hpp"
#include "parser.hpp"

AstBuilder::AstBuilder(ParseNode& root) : root(root) {}

int AstBuilder::OperatorToInstruction(Token t) {
    switch (t.type) {
        case Token::Star:
        return AstNode::Multiply;
        case Token::Equal:
        return AstNode::Assigment;
        break;
    }
    return AstNode::None;
}

AstNode AstBuilder::BuildPrimaryExpression(ParseNode& p) {
    AstNode node;
    switch(p.children[0].token.type) {
        case Token::IntegerLit:
        node = AstNode(AstNode::IntLiteral, p.children[0].token);
        break;
        case Token::FloatLit:
        node = AstNode(AstNode::FloatLiteral, p.children[0].token);
        break;
        case Token::Identifier:
        node = AstNode(AstNode::Variable, p.children[0].token);
        break;
    }
    return node;
}

AstNode AstBuilder::BuildAssignmentExpression(ParseNode& p) {
    AstNode node = AstNode(AstNode::Assigment, p.token);
    if(p.token.type == Token::Equal)
        return node;
    return node;
}

AstNode AstBuilder::BuildExpression(ParseNode& p) {
    AstNode node;
    ParseNode expr = p.children[0].children[0];
    node.children.push_back(BuildPrimaryExpression(expr.children[0]));
    for(int i = 0; i < expr.children.size() - 1; i++) {
        if(expr.children[i].token.type == Token::SemiColon)
            break;

        if(Parser::IsAssignmentOperator(expr.children[i].token.type)) {
            node.children.push_back(BuildAssignmentExpression(expr.children[i]));
            node.type = AstNode::Assigment;
        } else if(Parser::IsAssignmentOperator(expr.children[i + 1].token.type)) {
            node.children.push_back(BuildPrimaryExpression(expr.children[i]));
            node.children.push_back(BuildAssignmentExpression(expr.children[i]));
            node.type = AstNode::Assigment;
        } else if(Parser::IsBinaryOperator(expr.children[i].token.type) || expr.children[i].type == ParseNode::Expression) {
            node.children.push_back(BuildExpression(expr));
            node.type = OperatorToInstruction(expr.children[i].token);
        }
    }
    return node;
}

AstNode AstBuilder::BuildDeclaration(ParseNode& p) {
    AstNode node = AstNode(AstNode::Declaration);
    for(int i = 0; i < p.children.size(); i++) {
        if(p.children[i].token.type == Token::Identifier) {
            node.children.push_back(AstNode(AstNode::Variable, p.children[i].token));
        } else if(p.children[i].token.type == Token::SemiColon) {
            break;
        }
    }
    return node;
}

AstNode AstBuilder::BuildFunctionDefintion(ParseNode& p) {
    AstNode node = AstNode(AstNode::Function);
    for(int i = 0; i < p.children.size(); i++) {
        if(p.children[i].token.type == Token::Identifier) {
            node.token = p.children[i].token;
        } else if(p.children[i].token.type == Token::OpenCurly) {
            node.children.push_back(BuildStatementScope(p));
        }
    }
    return node;
}

AstNode AstBuilder::BuildStatement(ParseNode& p) {
    AstNode node = AstNode(AstNode::None);
    switch(p.type) {
        case ParseNode::Expression:
        return BuildExpression(p);
        case ParseNode::Declaration:
        return BuildDeclaration(p);
    }
    return node;
}

AstNode AstBuilder::BuildStatementScope(ParseNode& p) {
    AstNode node = AstNode(AstNode::None);
    for(ParseNode& child : p.children) {
        node.children.push_back(BuildStatement(child));
    }
    return node;
}

AstNode AstBuilder::BuildTranslationUnit(void) {
    AstNode node;
    for(ParseNode& child : root.children) {
        if(child.type == ParseNode::Declaration)
            node.children.push_back(BuildDeclaration(child));
        else if(child.type == ParseNode::FunctionDefinition)
            node.children.push_back(BuildFunctionDefintion(child));
    }
    return node;
}