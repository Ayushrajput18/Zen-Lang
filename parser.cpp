#include "parser.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens), pos(0) {}

const Token& Parser::peek(int offset) const {
    size_t idx = pos + offset;
    if (idx >= tokens.size()) return tokens.back();
    return tokens[idx];
}

const Token& Parser::advance() {
    if (!isAtEnd()) pos++;
    return peek(-1);
}

bool Parser::match(int type) {
    if (!isAtEnd() && static_cast<int>(peek().type) == type) {
        advance();
        return true;
    }
    return false;
}

bool Parser::isAtEnd() const {
    return pos >= tokens.size() || peek().type == TokenType::EndOfFile;
}

std::vector<std::unique_ptr<ASTNode>> Parser::parse() {
    std::vector<std::unique_ptr<ASTNode>> nodes;
    while (!isAtEnd()) {
        auto stmt = parseStatement();
        if (stmt) nodes.push_back(std::move(stmt));
        else advance(); // Skip invalid token
    }
    return nodes;
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    // Function definition
    if (peek().type == TokenType::Keyword && peek().value == "func") {
        return parseFunction();
    }
    // Return statement
    if (peek().type == TokenType::Keyword && peek().value == "return") {
        return parseReturn();
    }
    // Variable declaration: let x = expr;
    if (peek().type == TokenType::Keyword && peek().value == "let") {
        return parseVarDecl();
    }
    // Assignment: x = expr;
    if (peek().type == TokenType::Identifier && peek(1).type == TokenType::Assign) {
        std::string name = peek().value;
        advance(); // consume identifier
        advance(); // consume '='
        auto value = parseExpression();
        if (peek().type == TokenType::Operator && peek().value == ";") advance();
        else throw std::runtime_error("Expected ';' after assignment");
        return std::make_unique<VarDeclNode>(name, std::move(value));
    }
    // Array assignment: array[index] = expr;
    if (peek().type == TokenType::Identifier && peek(1).type == TokenType::LBracket) {
        // Parse array access as left-hand side
        auto arrayExpr = parsePrimary();
        if (peek().type == TokenType::Assign) {
            advance(); // consume '='
            auto value = parseExpression();
            if (peek().type == TokenType::Operator && peek().value == ";") advance();
            else throw std::runtime_error("Expected ';' after array assignment");
            // Use VarDeclNode for assignment, with IndexNode as name
            return std::make_unique<VarDeclNode>("", std::make_unique<BinaryExprNode>("[]=", std::move(arrayExpr), std::move(value)));
        }
    }
    // Print statement: print(expr);
    if (peek().type == TokenType::Keyword && peek().value == "print") {
        return parsePrint();
    }
    // If statement
    if (peek().type == TokenType::Keyword && peek().value == "if") {
        return parseIf();
    }
    // While statement
    if (peek().type == TokenType::Keyword && peek().value == "while") {
        return parseWhile();
    }
    // For statement
    if (peek().type == TokenType::Keyword && peek().value == "for") {
        return parseFor();
    }
    // TODO: Add more statement types
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::parseVarDecl() {
    // let x = expr;
    advance(); // consume 'let'
    if (peek().type != TokenType::Identifier) throw std::runtime_error("Expected identifier after 'let'");
    std::string name = peek().value;
    advance(); // consume identifier
    if (peek().type != TokenType::Assign) throw std::runtime_error("Expected '=' after identifier");
    advance(); // consume '='
    auto value = parseExpression();
    if (peek().type != TokenType::Operator || peek().value != ";") {
        // Accept both explicit semicolon or end of line as statement end
        if (peek().type != TokenType::EndOfFile) advance();
    } else {
        advance(); // consume ';'
    }
    return std::make_unique<VarDeclNode>(name, std::move(value));
}

std::unique_ptr<ASTNode> Parser::parsePrint() {
    advance(); // consume 'print'
    if (peek().type != TokenType::LParen) throw std::runtime_error("Expected '(' after 'print'");
    advance(); // consume '('
    auto expr = parseExpression();
    if (peek().type != TokenType::RParen) throw std::runtime_error("Expected ')' after print expression");
    advance(); // consume ')'
    if (peek().type == TokenType::Operator && peek().value == ";") advance(); // optional semicolon
    return std::make_unique<PrintNode>(std::move(expr));
}

std::unique_ptr<ExprNode> Parser::parseExpression() {
    return parseBinary();
}

std::unique_ptr<ExprNode> Parser::parsePrimary() {
    if (peek().type == TokenType::LBracket) {
        // Array literal
        advance(); // consume '['
        std::vector<std::unique_ptr<ExprNode>> elements;
        if (peek().type != TokenType::RBracket) {
            while (true) {
                elements.push_back(parseExpression());
                if (peek().type == TokenType::Comma) advance();
                else break;
            }
        }
        if (peek().type != TokenType::RBracket) throw std::runtime_error("Expected ']' in array literal");
        advance(); // consume ']'
        return std::make_unique<ArrayNode>(std::move(elements));
    }
    // Function call: len(expr)
    if (peek().type == TokenType::Keyword && peek().value == "len") {
        std::string func = peek().value;
        advance();
        if (peek().type != TokenType::LParen) throw std::runtime_error("Expected '(' after function name");
        advance(); // consume '('
        std::vector<std::unique_ptr<ExprNode>> args;
        if (peek().type != TokenType::RParen) {
            args.push_back(parseExpression());
        }
        if (peek().type != TokenType::RParen) throw std::runtime_error("Expected ')' after function argument");
        advance(); // consume ')'
        return std::make_unique<CallNode>(func, std::move(args));
    }
    // Array access: expr[expr]
    auto primary = [&]() -> std::unique_ptr<ExprNode> {
        if (peek().type == TokenType::Identifier) {
            std::string name = peek().value;
            advance();
            return std::make_unique<IdentifierNode>(name);
        }
        if (peek().type == TokenType::Number || peek().type == TokenType::Decimal) {
            std::string value = peek().value;
            advance();
            return std::make_unique<NumberNode>(value);
        }
        if (peek().type == TokenType::String) {
            std::string value = peek().value;
            advance();
            return std::make_unique<StringNode>(value);
        }
        if (peek().type == TokenType::LParen) {
            advance(); // consume '('
            auto expr = parseExpression();
            if (peek().type != TokenType::RParen) throw std::runtime_error("Expected ')' after expression");
            advance(); // consume ')'
            return expr;
        }
        if (peek().type == TokenType::Keyword && (peek().value == "true" || peek().value == "false")) {
            std::string value = (peek().value == "true") ? "1" : "0";
            advance();
            return std::make_unique<NumberNode>(value);
        }
        throw std::runtime_error("Unexpected token in expression");
    };
    auto expr = primary();
    while (peek().type == TokenType::LBracket) {
        advance(); // consume '['
        auto index = parseExpression();
        if (peek().type != TokenType::RBracket) throw std::runtime_error("Expected ']' after array index");
        advance(); // consume ']'
        expr = std::make_unique<IndexNode>(std::move(expr), std::move(index));
    }
    return expr;
}

// Simple binary expression parser (handles +, -, *, /, ==, !=, <, >, <=, >=, &&, ||)
int getPrecedence(const std::string& op) {
    if (op == "||") return 1;
    if (op == "&&") return 2;
    if (op == "==" || op == "!=" ) return 3;
    if (op == "<" || op == ">" || op == "<=" || op == ">=") return 4;
    if (op == "+" || op == "-") return 5;
    if (op == "*" || op == "/") return 6;
    return 0;
}

std::unique_ptr<ExprNode> Parser::parseBinary(int precedence) {
    auto left = parsePrimary();
    while (peek().type == TokenType::Operator && getPrecedence(peek().value) >= precedence) {
        std::string op = peek().value;
        int opPrec = getPrecedence(op);
        advance(); // consume operator
        auto right = parseBinary(opPrec + 1);
        left = std::make_unique<BinaryExprNode>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseIf() {
    advance(); // consume 'if'
    if (peek().type != TokenType::LParen) throw std::runtime_error("Expected '(' after 'if'");
    advance(); // consume '('
    auto condition = parseExpression();
    if (peek().type != TokenType::RParen) throw std::runtime_error("Expected ')' after if condition");
    advance(); // consume ')'
    if (peek().type != TokenType::LBrace) throw std::runtime_error("Expected '{' after if condition");
    advance(); // consume '{'
    std::vector<std::unique_ptr<ASTNode>> thenBranch;
    while (!isAtEnd() && peek().type != TokenType::RBrace) {
        auto stmt = parseStatement();
        if (stmt) thenBranch.push_back(std::move(stmt));
        else advance();
    }
    if (peek().type != TokenType::RBrace) throw std::runtime_error("Expected '}' after if block");
    advance(); // consume '}'
    std::vector<std::unique_ptr<ASTNode>> elseBranch;
    if (!isAtEnd() && peek().type == TokenType::Keyword && peek().value == "else") {
        advance(); // consume 'else'
        if (peek().type != TokenType::LBrace) throw std::runtime_error("Expected '{' after else");
        advance(); // consume '{'
        while (!isAtEnd() && peek().type != TokenType::RBrace) {
            auto stmt = parseStatement();
            if (stmt) elseBranch.push_back(std::move(stmt));
            else advance();
        }
        if (peek().type != TokenType::RBrace) throw std::runtime_error("Expected '}' after else block");
        advance(); // consume '}'
    }
    auto ifNode = std::make_unique<IfNode>(std::move(condition));
    ifNode->thenBranch = std::move(thenBranch);
    ifNode->elseBranch = std::move(elseBranch);
    return ifNode;
}

std::unique_ptr<ASTNode> Parser::parseWhile() {
    advance(); // consume 'while'
    if (peek().type != TokenType::LParen) throw std::runtime_error("Expected '(' after 'while'");
    advance(); // consume '('
    auto condition = parseExpression();
    if (peek().type != TokenType::RParen) throw std::runtime_error("Expected ')' after while condition");
    advance(); // consume ')'
    if (peek().type != TokenType::LBrace) throw std::runtime_error("Expected '{' after while condition");
    advance(); // consume '{'
    std::vector<std::unique_ptr<ASTNode>> body;
    while (!isAtEnd() && peek().type != TokenType::RBrace) {
        auto stmt = parseStatement();
        if (stmt) {
            body.push_back(std::move(stmt));
            // Require semicolon after each statement in the block
            if (peek().type == TokenType::Operator && peek().value == ";") advance();
            else throw std::runtime_error("Expected ';' after statement in while block");
        } else {
            advance();
        }
    }
    if (peek().type != TokenType::RBrace) throw std::runtime_error("Expected '}' after while block");
    advance(); // consume '}'
    auto whileNode = std::make_unique<WhileNode>(std::move(condition));
    whileNode->body = std::move(body);
    return whileNode;
}

std::unique_ptr<ASTNode> Parser::parseFor() {
    advance(); // consume 'for'
    if (peek().type != TokenType::Identifier) throw std::runtime_error("Expected loop variable after 'for'");
    std::string varName = peek().value;
    advance(); // consume variable name
    if (peek().type != TokenType::Assign) throw std::runtime_error("Expected '=' after loop variable");
    advance(); // consume '='
    auto startExpr = parseExpression();
    if (peek().type != TokenType::Keyword || peek().value != "to") throw std::runtime_error("Expected 'to' after for loop start value");
    advance(); // consume 'to'
    auto endExpr = parseExpression();
    std::unique_ptr<ExprNode> stepExpr;
    if (peek().type == TokenType::Keyword && peek().value == "step") {
        advance(); // consume 'step'
        stepExpr = parseExpression();
    }
    if (peek().type != TokenType::LBrace) throw std::runtime_error("Expected '{' after for loop header");
    advance(); // consume '{'
    std::vector<std::unique_ptr<ASTNode>> body;
    while (!isAtEnd() && peek().type != TokenType::RBrace) {
        auto stmt = parseStatement();
        if (stmt) {
            body.push_back(std::move(stmt));
            if (peek().type == TokenType::Operator && peek().value == ";") advance();
            else throw std::runtime_error("Expected ';' after statement in for block");
        } else {
            advance();
        }
    }
    if (peek().type != TokenType::RBrace) throw std::runtime_error("Expected '}' after for block");
    advance(); // consume '}'
    auto forNode = std::make_unique<ForNode>(nullptr, nullptr, nullptr);
    // We'll use init as the variable name, condition as start, increment as end, and step as step
    // Store info in a custom way for this simple for loop
    forNode->init = std::make_unique<IdentifierNode>(varName);
    forNode->condition = std::move(startExpr);
    forNode->increment = std::move(endExpr);
    forNode->body = std::move(body);
    if (stepExpr) {
        // Use step as a special case: attach as a NumberNode to the increment field (not ideal, but works for now)
        forNode->increment = std::make_unique<BinaryExprNode>(
            "step",
            std::unique_ptr<ExprNode>(static_cast<ExprNode*>(forNode->increment.release())),
            std::move(stepExpr)
        );
    }
    return forNode;
}

std::unique_ptr<ASTNode> Parser::parseFunction() {
    advance(); // consume 'func'
    if (peek().type != TokenType::Identifier) throw std::runtime_error("Expected function name after 'func'");
    std::string name = peek().value;
    advance(); // consume function name
    if (peek().type != TokenType::LParen) throw std::runtime_error("Expected '(' after function name");
    advance(); // consume '('
    std::vector<std::string> params;
    if (peek().type != TokenType::RParen) {
        while (true) {
            if (peek().type != TokenType::Identifier) throw std::runtime_error("Expected parameter name");
            params.push_back(peek().value);
            advance();
            if (peek().type == TokenType::Comma) advance();
            else break;
        }
    }
    if (peek().type != TokenType::RParen) throw std::runtime_error("Expected ')' after parameter list");
    advance(); // consume ')'
    if (peek().type != TokenType::LBrace) throw std::runtime_error("Expected '{' after parameter list");
    advance(); // consume '{'
    std::vector<std::unique_ptr<ASTNode>> body;
    while (!isAtEnd() && peek().type != TokenType::RBrace) {
        auto stmt = parseStatement();
        if (stmt) {
            body.push_back(std::move(stmt));
            if (peek().type == TokenType::Operator && peek().value == ";") advance();
        } else {
            advance();
        }
    }
    if (peek().type != TokenType::RBrace) throw std::runtime_error("Expected '}' after function body");
    advance(); // consume '}'
    auto funcNode = std::make_unique<FunctionNode>(name, params);
    funcNode->body = std::move(body);
    return funcNode;
}

std::unique_ptr<ASTNode> Parser::parseReturn() {
    advance(); // consume 'return'
    auto value = parseExpression();
    if (peek().type == TokenType::Operator && peek().value == ";") advance();
    else throw std::runtime_error("Expected ';' after return statement");
    return std::make_unique<ReturnNode>(std::move(value));
}

// Stub methods
std::unique_ptr<ASTNode> Parser::parseSwitch() { return nullptr; } 