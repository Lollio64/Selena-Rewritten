#include "parser.hpp"
#include "symbol.hpp"

Parser::Parser(std::vector<Token>& token, SymbolTable& t, std::string& s) : table(t), index(0), tokens(token), source(s) {}

void Parser::Match(int t) {
    if(token.type == t) {
        index++;
        if(index < tokens.size())
            token = tokens[index];
    } else {
        Error("expected '" + Token::TokenToString(t) + "' before '" 
        + Token::TokenToString(token) + "'", token);
    }
}

void Parser::Error(const std::string& s, Token t) {
    std::string line = ReceiveLine(source, t.line);
    if(callback) {
        if(useErrors) {
            callback(s, line, t.line, t.offset);
        }
    }
}

void Parser::PopState() {
    ParserState state = stateStack.back();
    token = state.token;
    index = state.index;
    stateStack.pop_back();
}

void Parser::PushState() {
    stateStack.push_back((ParserState){token, index});
}

void Parser::RestoreState() {
    ParserState state = stateStack.back();
    token = state.token;
    index = state.index;
}

void Parser::DisableErrors() { useErrors = false; }
    
void Parser::EnableErrors() { useErrors = true; }

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

bool Parser::IsSelectionStatement(int t) {
    switch (t) {
        case Token::If:
        case Token::Switch:
        return true;
    }
    return false;
}

bool Parser::IsIterationStatement(int t) {
    switch(t) {
        case Token::Do:
        case Token::For:
        case Token::While:
        return true;
    }
    return false;
}

bool Parser::IsJumpStatement(int t) {
    switch(t) {
        case Token::Break:
        case Token::Return:
        case Token::Continue:
        return true;
    }
    return false;
}

bool Parser::IsAssignmentOperator(int t) {
    switch(t) {
        case Token::Equal:
        case Token::AddAssign:
        case Token::SubAssign:
        case Token::MulAssign:
        case Token::DivAssign:
        return true;    
    }
    return false;
}

bool Parser::IsUnaryOperator(int t) {
    switch(t) {
        case Token::Plus:
        case Token::Minus:
        case Token::Increment:
        case Token::Decrement:
        return true;
    }
    return false;
}

bool Parser::IsBinaryOperator(int t) {
    switch(t) {
        case Token::Plus:
        case Token::Minus:
        case Token::Star:
        case Token::Slash:
        return true;
    }
    return false;
}

bool Parser::IsPostfixOperator(int t) {
    switch(t) {
        case Token::Dot:
        case Token::OpenBracket:
        case Token::Increment:
        case Token::Decrement:
        case Token::OpenParenthese:
        return true;
    }
    return false;
}

bool Parser::IsExpressionSeperator(int t) {
    switch(t) {
        case Token::Comma:
        case Token::SemiColon:
        case Token::CloseBracket:
        case Token::CloseParenthese:
        return true;
    }
    return false;
}

void Parser::InsertDeclaration(ParseNode& node, int type) {
    TableEntry* entry = nullptr;
    if(IsTypeSpecifier(node.children[0].token.type)) {
        if(!table.Lookup(node.children[1].token.value)) {
            entry = table.Insert(node.children[1].token.value, type);
            entry->typeSpecifier = node.children[1].token.type;
        }
    } else if(IsTypeQualifier(node.children[0].token.type)) {
        if(!table.Lookup(node.children[2].token.value)) {
            entry = table.Insert(node.children[2].token.value, type);
            entry->typeQualifier = node.children[0].token.type;
            entry->typeSpecifier = node.children[1].token.type;
        }
    }
}

TableEntry* Parser::LookupFunctionDeclaration(const std::string& id) {
    return table.Lookup(id);
}

std::optional<ParseNode> Parser::ParseSimpleStatement() {
    if(IsJumpStatement(token.type))
        return ParseJumpStatement();
    else if(IsSelectionStatement(token.type)) 
        return ParseSelectionStatement();
    else if(IsIterationStatement(token.type)) 
        return ParseIterationStatement();
    else if(IsTypeQualifier(token.type) || IsTypeSpecifier(token.type) 
        || token.type == Token::Precision || token.type == Token::Layout)
        return ParseDeclarationStatement();
    return ParseExpressionStatement();
}

std::optional<ParseNode> Parser::ParseStatementScope() {
    ParseNode node;
    node.children.push_back(token);
    Match(Token::OpenCurly);
    while(token.type != Token::CloseCurly) {
        node.Append(ParseSimpleStatement().value_or(ParseNode()));
    }
    node.children.push_back(token);
    Match(Token::CloseCurly);
    return node;
}

std::optional<ParseNode> Parser::ParseExpressionStatement() {
    ParseNode node;
    if(token.type != Token::SemiColon) {
        node.children.push_back(ParseExpression().value_or(ParseNode()));
    }
    node.children.push_back(token);
    Match(Token::SemiColon);
    return node;
}

std::optional<ParseNode> Parser::ParseFunctionCall() {}

std::optional<ParseNode> Parser::ParsePrimaryExpression() {
    ParseNode node = ParseNode(ParseNode::PrimaryExpression);
    if(token.type == Token::OpenParenthese) {
        node.children.push_back(token);
        Match(Token::OpenParenthese);
        node.children.push_back(ParseExpression().value_or(ParseNode()));
        node.children.push_back(token);
        Match(Token::CloseParenthese);
        return node;
    }
    switch(token.type) {
        case Token::True:
        case Token::False:
        case Token::FloatLit:
        case Token::Identifier:
        case Token::IntegerLit:
        node.children.push_back(token);
        Match(token.type);
        return node;
        default:
        Error("unexpected '" + Token::TokenToString(token)
        + "' in primary expression", token);
        break;
    }
    return std::nullopt;
}

std::optional<ParseNode> Parser::ParseExpression() {
    if(IsUnaryOperator(token.type)) {
        return ParseUnaryExpression();
    } else if(token.type == Token::Identifier 
           || IsTypeSpecifier(token.type)) {
        PushState();
        DisableErrors();
        Match(Token::Identifier);
        if(IsPostfixOperator(token.type)) {
            PopState();
            EnableErrors();
            return ParsePostfixExpression();
        }
        PopState();
    }
    PushState();
    ParsePrimaryExpression();
    if(IsBinaryOperator(token.type)) {
        PopState();
        EnableErrors();
        return ParseBinaryExpression();
    } else if(IsAssignmentOperator(token.type)) {
        PopState();
        EnableErrors();
        return ParseAssignmentExpression();
    }
    PopState();
    EnableErrors();
    return ParsePrimaryExpression();
}

std::optional<ParseNode> Parser::ParsePostfixExpression() {
    ParseNode node;
    if(IsTypeSpecifier(token.type)) {
        PushState();
        DisableErrors();
        if(token.type == Token::OpenParenthese) {
            return ParseFunctionCall();
        }
        PopState();
        EnableErrors();
        Error("expected '(' after type cast specifier", token);
        return std::nullopt;
    }
    return std::nullopt;
}

std::optional<ParseNode> Parser::ParseBinaryExpression() {
    PushState();
    DisableErrors();
    ParsePrimaryExpression();
    switch(token.type) {
        case Token::Plus:
        PopState();
        EnableErrors();
        return ParseAdditiveExpression();
        case Token::Minus:
        PopState();
        EnableErrors();
        return ParseSubtractiveExpression();
        case Token::Slash:
        PopState();
        EnableErrors();
        return ParseDivisiveExpression();
        case Token::Star:
        PopState();
        EnableErrors();
        return ParseMultiplicativeExpression();
    }
    PopState();
    EnableErrors();
    Error("expected binary operator before '" 
    + Token::TokenToString(token) + "'", token);
    return std::nullopt;
}

std::optional<ParseNode> Parser::ParseAssignmentExpression() {
    ParseNode node = ParseNode(ParseNode::AssignmentExpression);
    node.children.push_back(token);
    Match(Token::Identifier);
    if(IsAssignmentOperator(token.type)) {
        node.children.push_back(token);
        Match(token.type);
        while(!IsExpressionSeperator(token.type)) {
            node.Append(ParseExpression().value_or(ParseNode()));
        }
        return node;
    }
    Error("expected assigment operator before '" + Token::TokenToString(token) + "'", token);
    return std::nullopt;
}

std::optional<ParseNode> Parser::ParseMultiplicativeExpression() {
    ParseNode node = ParseNode(ParseNode::MultiplicativeExpression);
    DisableErrors();
    if(auto expr = ParsePrimaryExpression()) {
        node.children.push_back(expr.value());
    }
    EnableErrors();
    if(token.type == Token::Star) {
        node.children.push_back(token);
        Match(Token::Star);
        while(!IsExpressionSeperator(token.type)) {
            node.Append(ParseExpression().value_or(ParseNode()));
        }
        return node;
    }
    Error("expected operator before '" + Token::TokenToString(token) + "'", token);
    return std::nullopt;
}

std::optional<ParseNode> Parser::ParseFunctionHeader() {
    ParseNode node;
    if(!IsTypeQualifier(token.type)) {
        node.Append(ParseSpecifiedType().value_or(ParseNode()));
        node.children.push_back(token);
        Match(Token::Identifier);
        node.children.push_back(token);
        Match(Token::OpenParenthese);
        return node;
    }
    return std::nullopt;
}

std::optional<ParseNode> Parser::ParseFunctionParameter() {
    ParseNode node = ParseNode(ParseNode::Declaration);
    if(token.type == Token::Const) {
        PushState();
        DisableErrors();
        Match(Token::Const);
        if(IsParameterQualifier(token.type) && token.type != Token::Input) {
            Error("output parameter qualifier cannot be marked as const", token);
            return std::nullopt;
        }
        PopState();
        EnableErrors();
        node.children.push_back(token);
        Match(Token::Const);
    }  
    if(IsParameterQualifier(token.type)) {
        node.children.push_back(token);
        Match(token.type);
        node.children.push_back(ParseTypeSpecifier().value_or(ParseNode()));
        Match(token.type);
        node.children.push_back(token);
        Match(Token::Identifier);
        return node;
    }
    node.Append(ParseTypeSpecifier().value_or(ParseNode()));
    Match(token.type);
    node.children.push_back(token);
    Match(Token::Identifier);
    InsertDeclaration(node, TableEntry::Variable);
    return node;
}

std::optional<ParseNode> Parser::ParseFunctionParameters() {
    ParseNode node;
    if(token.type != Token::CloseParenthese) {
        node.children.push_back(ParseFunctionParameter().value_or(ParseNode()));
        while(token.type == Token::Comma) {
            node.children.push_back(token);
            Match(Token::Comma);
            node.children.push_back(ParseFunctionParameter().value_or(ParseNode()));
        }
    }
    return node;
}

std::optional<ParseNode> Parser::ParseFunctionPrototype() {
    ParseNode node = ParseNode(ParseNode::FunctionDeclaration);
    node.Append(ParseFunctionHeader().value_or(ParseNode()));
    node.Append(ParseFunctionParameters().value_or(ParseNode()));
    node.children.push_back(token);
    Match(Token::CloseParenthese);
    return node;
}

std::optional<ParseNode> Parser::ParseFunctionDefinition() {
    ParseNode node = ParseNode(ParseNode::FunctionDefinition);
    node.Append(ParseFunctionPrototype().value_or(ParseNode()));
    node.Append(ParseStatementScope().value_or(ParseNode()));
    return node;
}

std::optional<ParseNode> Parser::ParsePrecisionQualifier() {
    ParseNode node;
    node.children.push_back(token);
    Match(Token::Precision);
    if(IsPrecisionQualifier(token.type)) {
        node = ParseNode(token, ParseNode::PrecisionQualifier);
        Match(token.type);
        return node;
    }
    Error("expected precision qualifier before " 
    + Token::TokenToString(token), token);
    return std::nullopt;
}

std::optional<ParseNode> Parser::ParseLayoutQualifier() {
    ParseNode node = ParseNode(ParseNode::LayoutQualifier);
    node.children.push_back(token);
    Match(Token::Layout);
    if(token.type == Token::OpenParenthese) {
        PushState();
        DisableErrors();
        Match(Token::OpenParenthese);
        Match(Token::Identifier);
        if(token.type == Token::Equal) {
            PopState();
            EnableErrors();
            node.children.push_back(ParsePrimaryExpression().value_or(ParseNode()));
            return node;
        }
    }
    PopState();
    EnableErrors();
    Error("expected parenthesized assignment expression", token);
    return std::nullopt;
}

std::optional<ParseNode> Parser::ParseSpecifiedType() {
    ParseNode node;
    if(IsTypeQualifier(token.type)) {
        node.children.push_back(ParseTypeQualifier().value_or(ParseNode()));
    }
    node.children.push_back(ParseTypeSpecifier().value_or(ParseNode()));
    return node;
}

std::optional<ParseNode> Parser::ParseTypeSpecifier() {
    ParseNode node = ParseNode(ParseNode::TypeSpecifier);
    if(IsTypeSpecifier(token.type)) {
        node.children.push_back(token);
        Match(token.type);
        return node;
    }
    Error("expected type specifier before '" + Token::TokenToString(token) + "'", token);
    return std::nullopt;
}

std::optional<ParseNode> Parser::ParseTypeQualifier() {
    ParseNode node = ParseNode(ParseNode::TypeQualifier);
    if(IsTypeQualifier(token.type)) {
        node.children.push_back(token);
        Match(token.type);
        return node;
    }
    return std::nullopt;
}

std::optional<ParseNode> Parser::ParseSingleDeclaration() {
    ParseNode node;
    node.Append(ParseSpecifiedType().value_or(ParseNode()));
    node.children.push_back(token);
    Match(Token::Identifier);
    return node;
}

std::optional<ParseNode> Parser::ParseDeclaration() {
    ParseNode node;
    if(token.type == Token::Precision) {
        node.children.push_back(ParsePrecisionQualifier().value_or(ParseNode()));
        node.children.push_back(ParseTypeSpecifier().value_or(ParseNode()));
        node.children.push_back(token);
        Match(Token::SemiColon);
        return node;
    }
    if(token.type == Token::Layout) {
        node = ParseNode(ParseNode::Declaration);
        node.children.push_back(ParseLayoutQualifier().value_or(ParseNode()));
        node.children.push_back(ParseSingleDeclaration().value_or(ParseNode()));
        node.children.push_back(token);
        Match(Token::SemiColon);
        return node;
    }
    PushState();
    DisableErrors();
    if(!IsTypeQualifier(token.type)) {
        ParseSpecifiedType();
        Match(Token::Identifier);
        if(token.type == Token::OpenParenthese) {
            PopState();
            EnableErrors();
            node = ParseFunctionPrototype().value_or(ParseNode());
            if(token.type != Token::SemiColon) {
                Error("expected ';' after function declaration", token);
            }
            return node;
        }
    }
    PopState();
    EnableErrors();
    node = ParseNode(ParseNode::Declaration);
    //N.Append(ParseInitDeclaratorList()); TODO: Multiple declarations on the same line
    node.Append(ParseSingleDeclaration().value_or(ParseNode()));
    node.children.push_back(token);
    Match(Token::SemiColon);
    //InsertDeclaration(node, TableEntry::Variable);
    return node;
}

std::optional<ParseNode> Parser::ParseExternalDeclaration() {
    PushState();
    DisableErrors();
    ParseSingleDeclaration();
    if(token.type == Token::OpenParenthese) {
        RestoreState();
        ParseFunctionPrototype();
        if(token.type == Token::OpenCurly) {
            PopState();
            EnableErrors();
            return ParseFunctionDefinition();
        }
    }
    PopState();
    EnableErrors();
    return ParseDeclaration();
}

std::optional<ParseNode> Parser::ParseTranslationUnit(void) {
    ParseNode node;
    token = tokens[index];
    while(index < tokens.size()) {
        if(auto stmt = ParseExternalDeclaration())
            node.children.push_back(stmt.value());
        else exit(EXIT_FAILURE);
    }
    return node;
}