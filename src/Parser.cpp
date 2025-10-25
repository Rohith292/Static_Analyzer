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
    node->line_number=name.line_number;
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

// Replace the existing parseStatement function in src/Parser.cpp with this one

std::unique_ptr<AstNode> Parser::parseStatement() {
    // Check if the current statement is a keyword 'int'.
    if (peek().type == TokenType::KEYWORD && peek().value == "int") {
        // Look ahead to distinguish between a function and a variable.
        if (tokens.size() > current + 2 && tokens[current + 2].value == "(") {
            advance(); // Consume 'int'
            return parseFunctionDeclaration();
        } else {
            advance(); // Consume 'int'
            return parseVariableDeclaration();
        }
    } 
    // <<< THIS IS THE MISSING LOGIC >>>
    else if (peek().type == TokenType::KEYWORD && peek().value == "if") {
        advance(); // Consume 'if'
        return parseIfStatement();
    }
    
    // If we don't recognize the statement, we advance to avoid getting stuck.
    advance();
    return nullptr;
}
//new function to parse the if-statements
std::unique_ptr<AstNode> Parser::parseIfStatement(){
    advance();

    //we'll treat the condition as a simple statement
    //a real parser would have the parseExpression() function here
    auto condition=std::make_unique<AstNode>();
    condition->type=NodeType::STATEMENT;
    condition->value=peek().value;
    advance();//consume the condition token

    advance();//')'
    advance();//'{'

    auto ifNode=std::make_unique<AstNode>();
    ifNode->type=NodeType::IF_STATEMENT;
    ifNode->children.push_back(std::move(condition));

    //parse the statements inside the if-else body
    while(peek().value!="}"){
        auto stmt=parseStatement();
        if(stmt){
            ifNode->children.push_back(std::move(stmt));
        }
    }
    advance();
    return ifNode;

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