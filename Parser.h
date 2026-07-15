
#ifndef INTERPRETER_YULIIA_SHAPOVAL_PARSER_H
#define INTERPRETER_YULIIA_SHAPOVAL_PARSER_H

#pragma once
#include <vector>
#include <memory>
#include "Lexer.h"
#include "AST.h"

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    std::unique_ptr<ASTNode> parse();

private:
    std::vector<Token> tokens;
    size_t current = 0;

    const Token& peek() const;
    const Token& previous() const;
    bool isAtEnd() const;
    bool match(TokenType type);
    const Token& consume(TokenType type, const std::string& errorMessage);

    std::unique_ptr<ASTNode> statement();

    std::unique_ptr<VarNode> varDeclaration();

    std::unique_ptr<DefNode> funcDeclaration();
    std::unique_ptr<ASTNode> expression();
    std::unique_ptr<ASTNode> term();
    std::unique_ptr<ASTNode> primary();
};
#endif //INTERPRETER_YULIIA_SHAPOVAL_PARSER_H
