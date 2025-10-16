#include "Parser.h"

// --- Helper Implementations ---
Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() {
    return current >= tokens.size();
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

// --- Parsing Logic ---

std::unique_ptr<AstNode> Parser::parseVariableDeclaration() {
    Token name = advance(); 
    advance(); // Consume '='
    Token value = advance();
    advance(); // Consume ';'

    auto node = std::make_unique<AstNode>();
    node->type = NodeType::VARIABLE_DECLARATION;
    node->value = name.value;
    return node;
}

std::unique_ptr<AstNode> Parser::parseFunctionDeclaration() {
    Token name = advance();
    advance(); // Consume '('
    advance(); // Consume ')'
    advance(); // Consume '{'

    auto node = std::make_unique<AstNode>();
    node->type = NodeType::FUNCTION_DECLARATION;
    node->value = name.value;

    while (peek().value != "}") {
        auto stmt = parseStatement();
        if (stmt) { // FIX: Only add valid statements to the tree
            node->children.push_back(std::move(stmt));
        }
    }

    advance(); // Consume '}'
    return node;
}

std::unique_ptr<AstNode> Parser::parseStatement() {
    if (peek().type == TokenType::KEYWORD && peek().value == "int") {
        if (tokens.size() > current + 2 && tokens[current + 2].value == "(") {
            advance(); // Consume 'int'
            return parseFunctionDeclaration();
        } else {
            advance(); // Consume 'int'
            return parseVariableDeclaration();
        }
    }
    
    advance(); // Unrecognized token, skip it
    return nullptr;
}

std::unique_ptr<AstNode> Parser::parse(const std::vector<Token>& tokens) {
    this->tokens = tokens;
    this->current = 0;

    auto programNode = std::make_unique<AstNode>();
    programNode->type = NodeType::PROGRAM;

    while (!isAtEnd()) {
        auto statement = parseStatement();
        if (statement) {
            programNode->children.push_back(std::move(statement));
        }
    }
    return programNode;
}