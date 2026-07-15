#include "Evaluator.h"
#include <cmath>
#include <stdexcept>
#include <algorithm>

bool Environment::hasVariable(const std::string& name) const {
    return variables.find(name) != variables.end();
}

double Environment::getVariable(const std::string& name) const {
    auto it = variables.find(name);
    if (it != variables.end()) return it->second;
    throw std::runtime_error("Undefined variable: " + name);
}

void Environment::defineVariable(const std::string& name, double value) {
    if (hasVariable(name)) {
        throw std::runtime_error("Variable '" + name + "' is immutable and cannot be reassigned!");
    }
    variables[name] = value;
}

bool Environment::hasFunction(const std::string& name) const {
    return functions.find(name) != functions.end();
}

void Environment::defineFunction(const std::string& name, std::vector<std::string> params, std::shared_ptr<ASTNode> body) {
    functions[name] = UserFunction{std::move(params), std::move(body)};
}

const UserFunction& Environment::getFunction(const std::string& name) const {
    auto it = functions.find(name);
    if (it != functions.end()) return it->second;
    throw std::runtime_error("Undefined function: " + name);
}

Evaluator::Evaluator(Environment& env) : globalEnv(env) {}

double Evaluator::evaluate(const ASTNode* node) {
    if (!node) return 0.0;

    if (auto num = dynamic_cast<const NumberNode*>(node)) {
        return num->value;
    }
    if (auto var = dynamic_cast<const VariableNode*>(node)) {
        if (localVariables.find(var->name) != localVariables.end()) {
            return localVariables[var->name];
        }
        return globalEnv.getVariable(var->name);
    }
    if (auto binOp = dynamic_cast<const BinaryOpNode*>(node)) {
        return evalBinaryOp(binOp);
    }
    if (auto call = dynamic_cast<const CallNode*>(node)) {
        return evalCall(call);
    }
    if (auto varDecl = dynamic_cast<const VarNode*>(node)) {
        return evalVarDecl(varDecl);
    }
    if (auto funcDef = dynamic_cast<const DefNode*>(node)) {
        return evalFuncDef(funcDef);
    }

    throw std::runtime_error("Unknown AST Node type");
}

double Evaluator::evalBinaryOp(const BinaryOpNode* node) {
    double leftVal = evaluate(node->left.get());
    double rightVal = evaluate(node->right.get());

    switch (node->op) {
        case '+': return leftVal + rightVal;
        case '-': return leftVal - rightVal;
        case '*': return leftVal * rightVal;
        case '/':
            if (rightVal == 0.0) throw std::runtime_error("Division by zero!");
            return leftVal / rightVal;
        default:
            throw std::runtime_error(std::string("Unknown operator: ") + node->op);
    }
}

double Evaluator::evalCall(const CallNode* node) {
    std::vector<double> argValues;
    for (const auto& arg : node->args) {
        argValues.push_back(evaluate(arg.get()));
    }

    if (node->name == "pow") {
        if (argValues.size() != 2) throw std::runtime_error("pow() expects exactly 2 arguments");
        return std::pow(argValues[0], argValues[1]);
    }
    if (node->name == "abs") {
        if (argValues.size() != 1) throw std::runtime_error("abs() expects exactly 1 argument");
        return std::abs(argValues[0]);
    }
    if (node->name == "max") {
        if (argValues.size() != 2) throw std::runtime_error("max() expects exactly 2 arguments");
        return std::max(argValues[0], argValues[1]);
    }
    if (node->name == "min") {
        if (argValues.size() != 2) throw std::runtime_error("min() expects exactly 2 arguments");
        return std::min(argValues[0], argValues[1]);
    }

    if (globalEnv.hasFunction(node->name)) {
        const auto& func = globalEnv.getFunction(node->name);
        if (func.params.size() != argValues.size()) {
            throw std::runtime_error("Function " + node->name + " expects " +
                                     std::to_string(func.params.size()) + " arguments, but got " +
                                     std::to_string(argValues.size()));
        }

        auto oldLocals = this->localVariables;

        for (size_t i = 0; i < func.params.size(); ++i) {
            this->localVariables[func.params[i]] = argValues[i];
        }

        double result = evaluate(func.body.get());
        this->localVariables = oldLocals;

        return result;
    }
    throw std::runtime_error("Unknown function call: " + node->name);
}

double Evaluator::evalVarDecl(const VarNode* node) {
    double val = evaluate(node->value.get());
    globalEnv.defineVariable(node->name, val);
    return val;
}

double Evaluator::evalFuncDef(const DefNode* node) {
    std::shared_ptr<ASTNode> sharedBody = std::move(const_cast<DefNode*>(node)->body);
    globalEnv.defineFunction(node->name, node->params, sharedBody);
    return 0.0;
}
