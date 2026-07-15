#ifndef INTERPRETER_YULIIA_SHAPOVAL_AST_H
#define INTERPRETER_YULIIA_SHAPOVAL_AST_H

#include <string>
#include <vector>
#include <memory>
#include <utility>

class ASTNode {
public:
    virtual ~ASTNode() = default;
};

class NumberNode : public ASTNode {
public:
    double value;
    explicit NumberNode(double val) : value(val) {}
};

class VariableNode : public ASTNode {
public:
    std::string name;
    explicit VariableNode(std::string n) : name(std::move(n)) {}
};

class BinaryOpNode : public ASTNode {
public:
    char op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    BinaryOpNode(char o, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
};

class CallNode : public ASTNode {
public:
    std::string name;
    std::vector<std::unique_ptr<ASTNode>> args;

    CallNode(std::string n, std::vector<std::unique_ptr<ASTNode>> a)
        : name(std::move(n)), args(std::move(a)) {}
};

class VarNode : public ASTNode {
public:
    std::string name;
    std::unique_ptr<ASTNode> value;

    VarNode(std::string n, std::unique_ptr<ASTNode> v)
        : name(std::move(n)), value(std::move(v)) {}
};

class DefNode : public ASTNode {
public:
    std::string name;
    std::vector<std::string> params;
    std::unique_ptr<ASTNode> body;

    DefNode(std::string n, std::vector<std::string> p, std::unique_ptr<ASTNode> b)
        : name(std::move(n)), params(std::move(p)), body(std::move(b)) {}
};

#endif //INTERPRETER_YULIIA_SHAPOVAL_AST_H
