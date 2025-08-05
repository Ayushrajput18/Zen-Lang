#pragma once
#include <string>
#include <vector>
#include <memory>

// Base AST node
class ASTNode {
public:
    virtual ~ASTNode() = default;
};

// Expression node
class ExprNode : public ASTNode {
public:
    virtual ~ExprNode() = default;
};

// Variable declaration: let x = expr;
class VarDeclNode : public ASTNode {
public:
    std::string name;
    std::unique_ptr<ExprNode> value;
    VarDeclNode(const std::string& n, std::unique_ptr<ExprNode> v)
        : name(n), value(std::move(v)) {}
};

// Print statement: print(expr);
class PrintNode : public ASTNode {
public:
    std::unique_ptr<ExprNode> expr;
    PrintNode(std::unique_ptr<ExprNode> e) : expr(std::move(e)) {}
};

// If statement
class IfNode : public ASTNode {
public:
    std::unique_ptr<ExprNode> condition;
    std::vector<std::unique_ptr<ASTNode>> thenBranch;
    std::vector<std::unique_ptr<ASTNode>> elseBranch;
    IfNode(std::unique_ptr<ExprNode> cond) : condition(std::move(cond)) {}
};

// While loop
class WhileNode : public ASTNode {
public:
    std::unique_ptr<ExprNode> condition;
    std::vector<std::unique_ptr<ASTNode>> body;
    WhileNode(std::unique_ptr<ExprNode> cond) : condition(std::move(cond)) {}
};

// For loop
class ForNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> init;
    std::unique_ptr<ExprNode> condition;
    std::unique_ptr<ASTNode> increment;
    std::vector<std::unique_ptr<ASTNode>> body;
    ForNode(std::unique_ptr<ASTNode> i, std::unique_ptr<ExprNode> c, std::unique_ptr<ASTNode> inc)
        : init(std::move(i)), condition(std::move(c)), increment(std::move(inc)) {}
};

// Switch statement
class SwitchNode : public ASTNode {
public:
    std::unique_ptr<ExprNode> expr;
    // Add case/branch structure as needed
    SwitchNode(std::unique_ptr<ExprNode> e) : expr(std::move(e)) {}
};

// Array node (for array literals)
class ArrayNode : public ExprNode {
public:
    std::vector<std::unique_ptr<ExprNode>> elements;
    ArrayNode(std::vector<std::unique_ptr<ExprNode>> elems) : elements(std::move(elems)) {}
};

// Pointer node (for pointer expressions)
class PointerNode : public ExprNode {
public:
    std::unique_ptr<ExprNode> pointee;
    PointerNode(std::unique_ptr<ExprNode> p) : pointee(std::move(p)) {}
};

// Binary expression (e.g., x + y)
class BinaryExprNode : public ExprNode {
public:
    std::string op;
    std::unique_ptr<ExprNode> left, right;
    BinaryExprNode(const std::string& o, std::unique_ptr<ExprNode> l, std::unique_ptr<ExprNode> r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
};

// Identifier
class IdentifierNode : public ExprNode {
public:
    std::string name;
    IdentifierNode(const std::string& n) : name(n) {}
};

// Number literal
class NumberNode : public ExprNode {
public:
    std::string value;
    NumberNode(const std::string& v) : value(v) {}
};

// String literal
class StringNode : public ExprNode {
public:
    std::string value;
    StringNode(const std::string& v) : value(v) {}
};

// Array indexing: array[index]
class IndexNode : public ExprNode {
public:
    std::unique_ptr<ExprNode> array;
    std::unique_ptr<ExprNode> index;
    IndexNode(std::unique_ptr<ExprNode> arr, std::unique_ptr<ExprNode> idx)
        : array(std::move(arr)), index(std::move(idx)) {}
};

// Function call: len(array)
class CallNode : public ExprNode {
public:
    std::string func;
    std::vector<std::unique_ptr<ExprNode>> args;
    CallNode(const std::string& f, std::vector<std::unique_ptr<ExprNode>> a)
        : func(f), args(std::move(a)) {}
};

// Function definition
class FunctionNode : public ASTNode {
public:
    std::string name;
    std::vector<std::string> params;
    std::vector<std::unique_ptr<ASTNode>> body;
    FunctionNode(const std::string& n, std::vector<std::string> p)
        : name(n), params(std::move(p)) {}
};

// Return statement
class ReturnNode : public ASTNode {
public:
    std::unique_ptr<ExprNode> value;
    ReturnNode(std::unique_ptr<ExprNode> v) : value(std::move(v)) {}
}; 