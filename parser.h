#pragma once
#include "lexer.h"
#include "ast.h"
#include <vector>
#include <memory>

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::vector<std::unique_ptr<ASTNode>> parse();
private:
    const std::vector<Token>& tokens;
    size_t pos;
    const Token& peek(int offset = 0) const;
    const Token& advance();
    bool match(int type);
    bool isAtEnd() const;
    // Parsing methods
    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<ASTNode> parseVarDecl();
    std::unique_ptr<ASTNode> parsePrint();
    std::unique_ptr<ASTNode> parseIf();
    std::unique_ptr<ASTNode> parseWhile();
    std::unique_ptr<ASTNode> parseFor();
    std::unique_ptr<ASTNode> parseSwitch();
    std::unique_ptr<ExprNode> parseExpression();
    std::unique_ptr<ExprNode> parsePrimary();
    std::unique_ptr<ExprNode> parseBinary(int precedence = 0);
    std::unique_ptr<ASTNode> parseFunction();
    std::unique_ptr<ASTNode> parseReturn();
}; 