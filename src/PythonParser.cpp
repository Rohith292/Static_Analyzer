#include "PythonParser.h"
#include <iostream>

// --- Helper Implementations ---
bool PythonParser::isAtEnd() { return current >= m_tokens.size(); }
Token PythonParser::peek() { return m_tokens[current]; }
Token PythonParser::advance() {
    if (!isAtEnd()) current++;
    return m_tokens[current - 1];
}
bool PythonParser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

// --- Parsing Logic ---

std::unique_ptr<AstNode> PythonParser::parseVariableAssignment() {
    Token name = m_tokens[current - 1]; // We already consumed the identifier
    advance(); // Consume '='
    Token value = advance(); // Consume the value

    auto node = std::make_unique<AstNode>();
    node->type = NodeType::VARIABLE_DECLARATION;
    node->value = name.value;
    node->line_number=name.line_number;
    return node;
}

std::unique_ptr<AstNode> PythonParser::parseIfStatement() {
    // Assumes 'if' has been consumed
    Token condition = advance(); // Consume condition
    advance(); // Consume ':'
    advance(); // Consume INDENT

    auto node = std::make_unique<AstNode>();
    node->type = NodeType::IF_STATEMENT;
    // Store the condition in the 'value' field for this simple parser
    node->children.push_back(std::make_unique<AstNode>(AstNode{NodeType::STATEMENT, condition.value}));

    // Parse statements inside the block until we find a DEDENT
    while (!check(TokenType::DEDENT) && !isAtEnd()) {
        auto stmt = parseStatement();
        if (stmt) {
            node->children.push_back(std::move(stmt));
        }
    }
    advance(); // Consume DEDENT
    return node;
}

std::unique_ptr<AstNode> PythonParser::parseFunctionDeclaration() {
    // Assumes 'def' has been consumed
    Token name = advance(); // Consume function name
    advance(); // Consume '('
    advance(); // Consume ')'
    advance(); // Consume ':'
    advance(); // Consume INDENT

    auto node = std::make_unique<AstNode>();
    node->type = NodeType::FUNCTION_DECLARATION;
    node->value = name.value;

    // Parse statements inside the function until we find a DEDENT
    while (!check(TokenType::DEDENT) && !isAtEnd()) {
        auto stmt = parseStatement();
        if (stmt) {
            node->children.push_back(std::move(stmt));
        }
    }
    advance(); // Consume DEDENT
    return node;
}

std::unique_ptr<AstNode> PythonParser::parseStatement() {
    if (check(TokenType::COMMENT)) {
        advance(); // Skip comments
        return nullptr;
    }

    if (check(TokenType::KEYWORD)) {
        if (peek().value == "def") {
            advance();
            return parseFunctionDeclaration();
        }
        if (peek().value == "if") {
            advance();
            return parseIfStatement();
        }
    }

    // <<< ADD THIS BLOCK TO RECOGNIZE 'print' >>>
        if (peek().value == "print") {
            advance(); // Consume 'print'
            auto node = std::make_unique<AstNode>();
            node->type = NodeType::STATEMENT;
            node->value = "print";
            
            // Consume the rest of the print statement
            while(peek().type != TokenType::INDENT && peek().type != TokenType::DEDENT && !isAtEnd()) {
                advance();
            }
            return node;
        }

    // Check for variable assignment: IDENTIFIER followed by '='
    if (check(TokenType::IDENTIFIER) && m_tokens[current + 1].value == "=") {
        advance();
        return parseVariableAssignment();
    }
    
    advance(); // Unrecognized token, skip it
    return nullptr;
}

std::unique_ptr<AstNode> PythonParser::parse(const std::vector<Token>& tokens) {
    this->m_tokens = tokens;
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