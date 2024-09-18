#include "ast.h"

void AstNode::AddChildNode(AstNode node) {
    children.push_back(node);
}

AstNode* Ast::GetRoot() { return root; }