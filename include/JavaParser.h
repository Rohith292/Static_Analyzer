#pragma once

#include "Token.h"
#include "AstNode.h"
#include <memory>
#include<vector>

class JavaParser{
    public:
        std::unique_ptr<AstNode>parse(const std::vector<Token>& tokens);

    private:
        size_t current=0;
        std::vector<Token> m_tokens;

        bool isAtEnd();
        Token advance();
        Token peek();
        Token previous();

        //parsing functions for java grammar
        std::unique_ptr<AstNode>parseStatement();
        std::unique_ptr<AstNode>parseMethodDeclaration();
        std::unique_ptr<AstNode>parseVariableDeclaration();
};