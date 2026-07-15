#ifndef INTERPRETER_YULIIA_SHAPOVAL_LEXER_H
#define INTERPRETER_YULIIA_SHAPOVAL_LEXER_H
#include <string>
#include <vector>

enum class TokenType {
    Number,
    Identifier,
    Plus,
    Minus,
    Multiply,
    Divide,
    Assign,
    LParen,
    RParen,
    LBrace,
    RBrace,
    Comma,
    End
};

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
public:
    explicit Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string src;
    size_t pos = 0;

    char peek() const;
    char advance();
    void skipWhitespace();
    Token readNumber();
    Token readIdentifier();
};


#endif //INTERPRETER_YULIIA_SHAPOVAL_LEXER_H
