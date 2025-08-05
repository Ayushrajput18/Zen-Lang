#include "interpreter.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>

Interpreter::Interpreter() {}

void Interpreter::pushScope() {
    callStack.push_back(variables);
}
void Interpreter::popScope() {
    variables = callStack.back();
    callStack.pop_back();
}
void Interpreter::setVar(const std::string& name, const Value& value) {
    variables[name] = value;
}
Interpreter::Value Interpreter::getVar(const std::string& name) {
    auto it = variables.find(name);
    if (it == variables.end()) throw std::runtime_error("Undefined variable: " + name);
    return it->second;
}

void Interpreter::interpret(const std::vector<std::unique_ptr<ASTNode>>& ast) {
    // Register all functions first
    for (const auto& node : ast) {
        if (auto func = dynamic_cast<const FunctionNode*>(node.get())) {
            functions[func->name] = func;
        }
    }
    // Execute all top-level statements (skip function definitions)
    for (const auto& node : ast) {
        if (!dynamic_cast<const FunctionNode*>(node.get())) {
            exec(node.get());
            if (hasReturn) break;
        }
    }
}

void Interpreter::exec(const ASTNode* node) {
    if (auto var = dynamic_cast<const VarDeclNode*>(node)) {
        if (variables.count(var->name)) {
            variables[var->name] = eval(var->value.get());
        } else {
            variables[var->name] = eval(var->value.get());
        }
    } else if (auto print = dynamic_cast<const PrintNode*>(node)) {
        auto value = eval(print->expr.get());
        if (std::holds_alternative<double>(value)) {
            std::cout << std::get<double>(value) << std::endl;
        } else if (std::holds_alternative<std::string>(value)) {
            std::cout << std::get<std::string>(value) << std::endl;
        }
    } else if (auto ifNode = dynamic_cast<const IfNode*>(node)) {
        auto cond = eval(ifNode->condition.get());
        bool condTrue = false;
        if (std::holds_alternative<double>(cond)) {
            condTrue = std::get<double>(cond) != 0.0;
        } else if (std::holds_alternative<std::string>(cond)) {
            condTrue = !std::get<std::string>(cond).empty();
        }
        if (condTrue) {
            for (const auto& stmt : ifNode->thenBranch) exec(stmt.get());
        } else {
            for (const auto& stmt : ifNode->elseBranch) exec(stmt.get());
        }
    } else if (auto whileNode = dynamic_cast<const WhileNode*>(node)) {
        while (true) {
            auto cond = eval(whileNode->condition.get());
            bool condTrue = false;
            if (std::holds_alternative<double>(cond)) condTrue = std::get<double>(cond) != 0.0;
            if (std::holds_alternative<std::string>(cond)) condTrue = !std::get<std::string>(cond).empty();
            if (!condTrue) break;
            for (const auto& stmt : whileNode->body) exec(stmt.get());
            if (hasReturn) return;
        }
    } else if (auto forNode = dynamic_cast<const ForNode*>(node)) {
        std::string varName = dynamic_cast<const IdentifierNode*>(forNode->init.get())->name;
        double start = std::get<double>(eval(forNode->condition.get()));
        double end = 0;
        double step = 1;
        if (auto bin = dynamic_cast<const BinaryExprNode*>(forNode->increment.get())) {
            if (bin->op == "step") {
                end = std::get<double>(eval(bin->left.get()));
                step = std::get<double>(eval(bin->right.get()));
            }
        } else {
            end = std::get<double>(eval(forNode->increment.get()));
        }
        for (double i = start; (step > 0 ? i <= end : i >= end); i += step) {
            variables[varName] = i;
            for (const auto& stmt : forNode->body) exec(stmt.get());
            if (hasReturn) return;
        }
    } else if (auto func = dynamic_cast<const FunctionNode*>(node)) {
        // Already registered
    } else if (auto ret = dynamic_cast<const ReturnNode*>(node)) {
        hasReturn = true;
        returnValue = eval(ret->value.get());
    } else if (auto var = dynamic_cast<const VarDeclNode*>(node)) {
        variables[var->name] = eval(var->value.get());
    } else {
        // Other node types not yet implemented
    }
}

Interpreter::Value Interpreter::eval(const ExprNode* expr) {
    if (auto call = dynamic_cast<const CallNode*>(expr)) {
        // User-defined function call
        auto it = functions.find(call->func);
        if (it != functions.end()) {
            const FunctionNode* func = it->second;
            if (call->args.size() != func->params.size()) throw std::runtime_error("Argument count mismatch in call to " + func->name);
            pushScope();
            for (size_t i = 0; i < func->params.size(); ++i) {
                setVar(func->params[i], eval(call->args[i].get()));
            }
            hasReturn = false;
            for (const auto& stmt : func->body) {
                exec(stmt.get());
                if (hasReturn) break;
            }
            Value ret = hasReturn ? returnValue : 0.0;
            hasReturn = false;
            popScope();
            return ret;
        }
        // Built-in functions (len already handled above)
        if (call->func == "len") {
            if (call->args.size() != 1) throw std::runtime_error("len() takes one argument");
            auto arrVal = eval(call->args[0].get());
            if (!std::holds_alternative<std::vector<std::shared_ptr<Value>>>(arrVal)) throw std::runtime_error("len() expects array");
            return static_cast<double>(std::get<std::vector<std::shared_ptr<Value>>>(arrVal).size());
        }
        throw std::runtime_error("Unknown function: " + call->func);
    } else if (auto num = dynamic_cast<const NumberNode*>(expr)) {
        return std::stod(num->value);
    } else if (auto str = dynamic_cast<const StringNode*>(expr)) {
        return str->value;
    } else if (auto id = dynamic_cast<const IdentifierNode*>(expr)) {
        auto it = variables.find(id->name);
        if (it == variables.end()) throw std::runtime_error("Undefined variable: " + id->name);
        return it->second;
    } else if (auto arr = dynamic_cast<const ArrayNode*>(expr)) {
        std::vector<std::shared_ptr<Value>> elements;
        for (const auto& el : arr->elements) {
            elements.push_back(std::make_shared<Value>(eval(el.get())));
        }
        return elements;
    } else if (auto idx = dynamic_cast<const IndexNode*>(expr)) {
        auto arrVal = eval(idx->array.get());
        auto idxVal = eval(idx->index.get());
        if (!std::holds_alternative<std::vector<std::shared_ptr<Value>>>(arrVal)) throw std::runtime_error("Indexing non-array");
        auto& vec = std::get<std::vector<std::shared_ptr<Value>>>(arrVal);
        int i = static_cast<int>(std::get<double>(idxVal));
        if (i < 0 || i >= (int)vec.size()) throw std::runtime_error("Array index out of bounds");
        return *vec[i];
    } else if (auto bin = dynamic_cast<const BinaryExprNode*>(expr)) {
        if (bin->op == "[]=") {
            // Array assignment: left is IndexNode, right is value
            auto idxNode = dynamic_cast<const IndexNode*>(bin->left.get());
            if (!idxNode) throw std::runtime_error("Invalid array assignment");
            auto arrId = dynamic_cast<const IdentifierNode*>(idxNode->array.get());
            if (!arrId) throw std::runtime_error("Array assignment must be to a variable");
            auto arrIt = variables.find(arrId->name);
            if (arrIt == variables.end()) throw std::runtime_error("Undefined array: " + arrId->name);
            if (!std::holds_alternative<std::vector<std::shared_ptr<Value>>>(arrIt->second)) throw std::runtime_error("Variable is not an array");
            auto& vec = std::get<std::vector<std::shared_ptr<Value>>>(arrIt->second);
            int i = static_cast<int>(std::get<double>(eval(idxNode->index.get())));
            if (i < 0 || i >= (int)vec.size()) throw std::runtime_error("Array index out of bounds");
            vec[i] = std::make_shared<Value>(eval(bin->right.get()));
            return *vec[i];
        }
        auto left = eval(bin->left.get());
        auto right = eval(bin->right.get());
        if (bin->op == "+") {
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return std::get<double>(left) + std::get<double>(right);
            } else if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                return std::get<std::string>(left) + std::get<std::string>(right);
            } else if (std::holds_alternative<std::string>(left) && std::holds_alternative<double>(right)) {
                return std::get<std::string>(left) + std::to_string(std::get<double>(right));
            } else if (std::holds_alternative<double>(left) && std::holds_alternative<std::string>(right)) {
                return std::to_string(std::get<double>(left)) + std::get<std::string>(right);
            }
        } else if (bin->op == "-") {
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return std::get<double>(left) - std::get<double>(right);
            }
        } else if (bin->op == "*") {
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return std::get<double>(left) * std::get<double>(right);
            }
        } else if (bin->op == "/") {
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return std::get<double>(left) / std::get<double>(right);
            }
        } else if (bin->op == "==") {
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return std::get<double>(left) == std::get<double>(right) ? 1.0 : 0.0;
            } else if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                return std::get<std::string>(left) == std::get<std::string>(right) ? 1.0 : 0.0;
            }
        } else if (bin->op == "!=") {
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return std::get<double>(left) != std::get<double>(right) ? 1.0 : 0.0;
            } else if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                return std::get<std::string>(left) != std::get<std::string>(right) ? 1.0 : 0.0;
            }
        } else if (bin->op == "<") {
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return std::get<double>(left) < std::get<double>(right) ? 1.0 : 0.0;
            }
        } else if (bin->op == ">") {
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return std::get<double>(left) > std::get<double>(right) ? 1.0 : 0.0;
            }
        } else if (bin->op == "<=") {
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return std::get<double>(left) <= std::get<double>(right) ? 1.0 : 0.0;
            }
        } else if (bin->op == ">=") {
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return std::get<double>(left) >= std::get<double>(right) ? 1.0 : 0.0;
            }
        } else if (bin->op == "&&") {
            bool l = false, r = false;
            if (std::holds_alternative<double>(left)) l = std::get<double>(left) != 0.0;
            if (std::holds_alternative<std::string>(left)) l = !std::get<std::string>(left).empty();
            if (std::holds_alternative<double>(right)) r = std::get<double>(right) != 0.0;
            if (std::holds_alternative<std::string>(right)) r = !std::get<std::string>(right).empty();
            return (l && r) ? 1.0 : 0.0;
        } else if (bin->op == "||") {
            bool l = false, r = false;
            if (std::holds_alternative<double>(left)) l = std::get<double>(left) != 0.0;
            if (std::holds_alternative<std::string>(left)) l = !std::get<std::string>(left).empty();
            if (std::holds_alternative<double>(right)) r = std::get<double>(right) != 0.0;
            if (std::holds_alternative<std::string>(right)) r = !std::get<std::string>(right).empty();
            return (l || r) ? 1.0 : 0.0;
        }
        throw std::runtime_error("Invalid operands for operator: " + bin->op);
    }
    throw std::runtime_error("Unknown expression type");
} 