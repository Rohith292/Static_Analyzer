// include/Parser.h
#pragma once

#include <string>   // For std::string
#include <vector>   // For std::vector
#include <memory>   // <<< THIS IS THE FIX. It defines std::unique_ptr.

#include "Token.h"
#include "AstNode.h"

class Parser {
public:
    std::unique_ptr<AstNode> parse(const std::vector<Token>& tokens);

private:
    size_t current = 0;
    std::vector<Token> tokens;
    
    //helper functions
    Token advance();
    bool isAtEnd();
    Token peek();//look at the current token without consuming
    Token previous();

    std::unique_ptr<AstNode>parseStatement();
    std::unique_ptr<AstNode>parseFunctionDeclaration();
    std::unique_ptr<AstNode>parseVariableDeclaration();
    std::unique_ptr<AstNode>parseIfStatement();
};