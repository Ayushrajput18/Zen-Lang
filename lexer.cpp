#include "lexer.h"
#include "tokens.h"
#include <sstream>
#include <cctype>

Lexer::Lexer(const std::string& src) : source(src), pos(0), line(1), column(1) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    Token token;
    do {
        token = nextToken();
        tokens.push_back(token);
    } while (token.type != TokenType::EndOfFile);
    return tokens;
}

char Lexer::peek() const {
    if (pos < source.size()) return source[pos];
    return '\0';
}

char Lexer::get() {
    if (pos < source.size()) {
        char c = source[pos++];
        if (c == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        return c;
    }
    return '\0';
}

void Lexer::skipWhitespace() {
    while (isspace(peek())) get();
}

Token Lexer::nextToken() {
    skipWhitespace();
    int startLine = line;
    int startCol = column;
    char c = peek();
    if (c == '\0') {
        return {TokenType::EndOfFile, "", startLine, startCol};
    }
    // Header: #use <...>
    if (c == '#') {
        std::string value;
        while (c != '\0' && c != '\n') {
            value += get();
            c = peek();
        }
        return {TokenType::Header, value, startLine, startCol};
    }
    // Comments
    if (c == '/' && source[pos + 1] == '/') {
        while (peek() != '\n' && peek() != '\0') get();
        return nextToken();
    }
    // Identifiers/Keywords
    if (isalpha(c) || c == '_') {
        std::string value;
        while (isalnum(peek()) || peek() == '_') {
            value += get();
        }
        if (value == "true" || value == "false") {
            return {TokenType::Keyword, value, startLine, startCol};
        }
        if (KEYWORDS.count(value)) {
            return {TokenType::Keyword, value, startLine, startCol};
        } else {
            return {TokenType::Identifier, value, startLine, startCol};
        }
    }
    // Numbers (int or decimal)
    if (isdigit(c)) {
        std::string value;
        bool isDecimal = false;
        while (isdigit(peek()) || peek() == '.') {
            if (peek() == '.') {
                if (isDecimal) break; // Only one dot allowed
                isDecimal = true;
            }
            value += get();
        }
        return {isDecimal ? TokenType::Decimal : TokenType::Number, value, startLine, startCol};
    }
    // Strings
    if (c == '"') {
        get(); // consume opening quote
        std::string value;
        while (peek() != '"' && peek() != '\0') {
            if (peek() == '\\') { // handle escape
                get();
                char next = get();
                value += next;
            } else {
                value += get();
            }
        }
        get(); // consume closing quote
        return {TokenType::String, value, startLine, startCol};
    }
    // Operators and symbols
    // Multi-character operators
    if (c == '=') {
        get();
        if (peek() == '=') { get(); return {TokenType::Operator, "==", startLine, startCol}; }
        return {TokenType::Assign, "=", startLine, startCol};
    }
    if (c == '!') {
        get();
        if (peek() == '=') { get(); return {TokenType::Operator, "!=", startLine, startCol}; }
        return {TokenType::Operator, "!", startLine, startCol};
    }
    if (c == '<') {
        get();
        if (peek() == '=') { get(); return {TokenType::Operator, "<=", startLine, startCol}; }
        return {TokenType::Operator, "<", startLine, startCol};
    }
    if (c == '>') {
        get();
        if (peek() == '=') { get(); return {TokenType::Operator, ">=", startLine, startCol}; }
        return {TokenType::Operator, ">", startLine, startCol};
    }
    if (c == '&') {
        get();
        if (peek() == '&') { get(); return {TokenType::Operator, "&&", startLine, startCol}; }
        return {TokenType::Unknown, "&", startLine, startCol};
    }
    if (c == '|') {
        get();
        if (peek() == '|') { get(); return {TokenType::Operator, "||", startLine, startCol}; }
        return {TokenType::Unknown, "|", startLine, startCol};
    }
    switch (c) {
        case '+': case '-': case '*': case '/': {
            std::string op(1, get());
            return {TokenType::Operator, op, startLine, startCol};
        }
        case '(': get(); return {TokenType::LParen, "(", startLine, startCol};
        case ')': get(); return {TokenType::RParen, ")", startLine, startCol};
        case '[': get(); return {TokenType::LBracket, "[", startLine, startCol};
        case ']': get(); return {TokenType::RBracket, "]", startLine, startCol};
        case '{': get(); return {TokenType::LBrace, "{", startLine, startCol};
        case '}': get(); return {TokenType::RBrace, "}", startLine, startCol};
        case ',': get(); return {TokenType::Comma, ",", startLine, startCol};
    }
    // Unknown character
    get();
    return {TokenType::Unknown, std::string(1, c), startLine, startCol};
}

std::string Token::toString() const {
    std::ostringstream oss;
    oss << "Token(" << static_cast<int>(type) << ", '" << value << "', " << line << ", " << column << ")";
    return oss.str();
} 