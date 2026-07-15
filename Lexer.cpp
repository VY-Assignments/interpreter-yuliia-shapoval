#include "Lexer.h"

Lexer::Lexer(const std::string& source) : src(source) {}

char Lexer::peek() const {
    if (pos >= src.length()) return '\0';
    return src[pos];
}

char Lexer::advance() {
    if (pos >= src.length()) return '\0';
    return src[pos++];
}

void Lexer::skipWhitespace() {
    while (std::isspace(peek())) {
        advance();
    }
}

Token Lexer::readNumber() {
    std::string val;
    while (std::isdigit(peek()) || peek() == '.') {
        val += advance();
    }
    return {TokenType::Number, val};
}

Token Lexer::readIdentifier() {
    std::string val;
    while (std::isalnum(peek()) || peek() == '_') {
        val += advance();
    }
    return {TokenType::Identifier, val};
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (pos < src.length()) {
        skipWhitespace();
        if (pos >= src.length()) break;

        char current = peek();

        if (std::isdigit(current)) {
            tokens.push_back(readNumber());
        } else if (std::isalpha(current) || current == '_') {
            tokens.push_back(readIdentifier());
        } else {
            advance();
            switch (current) {
                case '+': tokens.push_back({TokenType::Plus, "+"}); break;
                case '-': tokens.push_back({TokenType::Minus, "-"}); break;
                case '*': tokens.push_back({TokenType::Multiply, "*"}); break;
                case '/': tokens.push_back({TokenType::Divide, "/"}); break;
                case '=': tokens.push_back({TokenType::Assign, "="}); break;
                case '(': tokens.push_back({TokenType::LParen, "("}); break;
                case ')': tokens.push_back({TokenType::RParen, ")"}); break;
                case '{': tokens.push_back({TokenType::LBrace, "{"}); break;
                case '}': tokens.push_back({TokenType::RBrace, "}"}); break;
                case ',': tokens.push_back({TokenType::Comma, ","}); break;
                default:
                    break;
            }
        }
    }
    tokens.push_back({TokenType::End, ""});
    return tokens;
}