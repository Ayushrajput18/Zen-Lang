#pragma once
#include <string>
#include <vector>

enum class TokenType {
    Identifier,
    Number,
    Decimal,
    String,
    Keyword,
    Operator,
    Header,
    LParen,
    RParen,
    LBracket,
    RBracket,
    LBrace,
    RBrace,
    Comma,
    Assign,
    EndOfFile,
    Unknown
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
    std::string toString() const;
};

class Lexer {
public:
    Lexer(const std::string& src);
    std::vector<Token> tokenize();
private:
    std::string source;
    size_t pos;
    int line;
    int column;
    char peek() const;
    char get();
    void skipWhitespace();
    Token nextToken();
}; 