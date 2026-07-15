#include "Parser.h"
#include <stdexcept>

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

std::unique_ptr<ASTNode> Parser::parse() {
    return statement();
}

const Token& Parser::peek() const {
    return tokens[current];
}

const Token& Parser::previous() const {
    return tokens[current - 1];
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::End;
}

bool Parser::match(TokenType type) {
    if (isAtEnd()) return false;
    if (peek().type == type) {
        current++;
        return true;
    }
    return false;
}

const Token& Parser::consume(TokenType type, const std::string& errorMessage) {
    if (peek().type == type) {
        current++;
        return previous();
    }
    throw std::runtime_error(errorMessage);
}

std::unique_ptr<ASTNode> Parser::statement() {
    if (peek().type == TokenType::Identifier) {
        if (peek().value == "var") {
            return varDeclaration();
        }
        if (peek().value == "def") {
            return funcDeclaration();
        }
    }
    return expression();
}

std::unique_ptr<VarNode> Parser::varDeclaration() {
    consume(TokenType::Identifier, "Expected 'var'"); 
    const Token& nameToken = consume(TokenType::Identifier, "Expected variable name");
    consume(TokenType::Assign, "Expected '=' after variable name");
    auto expr = expression();
    return std::make_unique<VarNode>(nameToken.value, std::move(expr));
}

std::unique_ptr<DefNode> Parser::funcDeclaration() {
    consume(TokenType::Identifier, "Expected 'def'");
    const Token& nameToken = consume(TokenType::Identifier, "Expected function name");
    consume(TokenType::LParen, "Expected '(' after function name");
    
    std::vector<std::string> params;
    if (peek().type != TokenType::RParen) {
        do {
            const Token& paramToken = consume(TokenType::Identifier, "Expected parameter name");
            params.push_back(paramToken.value);
        } while (match(TokenType::Comma));
    }
    consume(TokenType::RParen, "Expected ')' after parameters");
    consume(TokenType::LBrace, "Expected '{' to start function body");
    
    auto body = expression();
    
    consume(TokenType::RBrace, "Expected '}' after function body");
    return std::make_unique<DefNode>(nameToken.value, params, std::move(body));
}

std::unique_ptr<ASTNode> Parser::expression() {
    auto expr = term();
    while (match(TokenType::Plus) || match(TokenType::Minus)) {
        char op = previous().value[0];
        auto right = term();
        expr = std::make_unique<BinaryOpNode>(op, std::move(expr), std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::term() {
    auto expr = primary();
    while (match(TokenType::Multiply) || match(TokenType::Divide)) {
        char op = previous().value[0];
        auto right = primary();
        expr = std::make_unique<BinaryOpNode>(op, std::move(expr), std::move(right));
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::primary() {
    if (match(TokenType::Number)) {
        return std::make_unique<NumberNode>(std::stod(previous().value));
    }

    if (match(TokenType::Minus)) {
        auto operand = primary();
        return std::make_unique<BinaryOpNode>('-', std::make_unique<NumberNode>(0.0), std::move(operand));
    }

    if (match(TokenType::Identifier)) {
        std::string name = previous().value;

        if (match(TokenType::LParen)) {
            std::vector<std::unique_ptr<ASTNode>> args;
            if (peek().type != TokenType::RParen) {
                do {
                    args.push_back(expression());
                } while (match(TokenType::Comma));
            }
            consume(TokenType::RParen, "Expected ')' after arguments");
            return std::make_unique<CallNode>(name, std::move(args));
        }

        return std::make_unique<VariableNode>(name);
    }
    
    if (match(TokenType::LParen)) {
        auto expr = expression();
        consume(TokenType::RParen, "Expected ')' after expression");
        return expr;
    }
    
    throw std::runtime_error("Unexpected token: " + peek().value);
}