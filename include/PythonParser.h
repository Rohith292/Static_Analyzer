#pragma once

#include "Token.h"
#include "AstNode.h"
#include <vector>
#include <memory>

class PythonParser {
public:
    std::unique_ptr<AstNode> parse(const std::vector<Token>& tokens);

private:
    size_t current = 0;
    std::vector<Token> m_tokens;

    // Helper functions
    bool isAtEnd();
    Token advance();
    Token peek();
    bool check(TokenType type);

    // Parsing functions
    std::unique_ptr<AstNode> parseStatement();
    std::unique_ptr<AstNode> parseFunctionDeclaration();
    std::unique_ptr<AstNode> parseIfStatement();
    std::unique_ptr<AstNode> parseVariableAssignment();
};