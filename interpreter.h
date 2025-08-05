#pragma once
#include "ast.h"
#include <unordered_map>
#include <string>
#include <variant>
#include <vector>
#include <memory>

using Value = std::variant<double, std::string, std::vector<std::shared_ptr<Value>>>;

class Interpreter {
public:
    Interpreter();
    void interpret(const std::vector<std::unique_ptr<ASTNode>>& ast);
private:
    std::unordered_map<std::string, Value> variables;
    std::unordered_map<std::string, const FunctionNode*> functions;
    std::vector<std::unordered_map<std::string, Value>> callStack;
    bool hasReturn = false;
    Value returnValue;
    void exec(const ASTNode* node);
    Value eval(const ExprNode* expr);
    void pushScope();
    void popScope();
    void setVar(const std::string& name, const Value& value);
    Value getVar(const std::string& name);
}; 