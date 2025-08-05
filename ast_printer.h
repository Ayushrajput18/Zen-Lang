#pragma once
#include "ast.h"
#include <iostream>

inline void printAST(const ASTNode* node, int indent = 0) {
    if (!node) return;
    std::string pad(indent, ' ');

    if (auto var = dynamic_cast<const VarDeclNode*>(node)) {
        std::cout << pad << "VarDecl: " << var->name << std::endl;
        printAST(var->value.get(), indent + 2);
    } else if (auto print = dynamic_cast<const PrintNode*>(node)) {
        std::cout << pad << "Print" << std::endl;
        printAST(print->expr.get(), indent + 2);
    } else if (auto bin = dynamic_cast<const BinaryExprNode*>(node)) {
        std::cout << pad << "BinaryExpr: " << bin->op << std::endl;
        printAST(bin->left.get(), indent + 2);
        printAST(bin->right.get(), indent + 2);
    } else if (auto id = dynamic_cast<const IdentifierNode*>(node)) {
        std::cout << pad << "Identifier: " << id->name << std::endl;
    } else if (auto num = dynamic_cast<const NumberNode*>(node)) {
        std::cout << pad << "Number: " << num->value << std::endl;
    } else if (auto str = dynamic_cast<const StringNode*>(node)) {
        std::cout << pad << "String: " << str->value << std::endl;
    } else {
        std::cout << pad << "Unknown ASTNode" << std::endl;
    }
} 