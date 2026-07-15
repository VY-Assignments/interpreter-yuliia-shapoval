#ifndef INTERPRETER_YULIIA_SHAPOVAL_EVALUATOR_H
#define INTERPRETER_YULIIA_SHAPOVAL_EVALUATOR_H

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include "AST.h"

struct UserFunction {
    std::vector<std::string> params;
    std::shared_ptr<ASTNode> body;
};

class Environment {
public:
    bool hasVariable(const std::string& name) const;
    double getVariable(const std::string& name) const;
    void defineVariable(const std::string& name, double value);

    bool hasFunction(const std::string& name) const;
    void defineFunction(const std::string& name, std::vector<std::string> params, std::shared_ptr<ASTNode> body);
    const UserFunction& getFunction(const std::string& name) const;

private:
    std::unordered_map<std::string, double> variables;
    std::unordered_map<std::string, UserFunction> functions;
};

class Evaluator {
public:
    explicit Evaluator(Environment& env);
    double evaluate(const ASTNode* node);

private:
    Environment& globalEnv;
    std::unordered_map<std::string, double> localVariables;

    double evalBinaryOp(const BinaryOpNode* node);
    double evalCall(const CallNode* node);
    double evalVarDecl(const VarNode* node);
    double evalFuncDef(const DefNode* node);
};


#endif //INTERPRETER_YULIIA_SHAPOVAL_EVALUATOR_H
