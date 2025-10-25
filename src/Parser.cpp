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

//new function to parse the function calls
std::unique_ptr<AstNode> Parser::parseFunctionCall() {
    // We already consumed the identifier, which is at tokens[current - 1]
    Token name = previous();

    auto node = std::make_unique<AstNode>();
    node->type = NodeType::FUNCTION_CALL;
    node->value = name.value;
    node->line_number = name.line_number;

    // Consume the rest of the function call (the "()" and ";")
    while(peek().value != ";") {
        advance();
    }
    advance(); // Consume the ";"
    return node;
}


// Replace the existing parseStatement function in src/Parser.cpp with this one

std::unique_ptr<AstNode> Parser::parseFunctionDeclaration() {
    // Assumes 'int' (or a type) has been consumed
    Token name = advance(); // Consume function name
    
    // Consume everything up to the opening brace '{'
    while(peek().value != "{") {
        advance();
    }
    advance(); // Consume '{'

    auto node = std::make_unique<AstNode>();
    node->type = NodeType::FUNCTION_DECLARATION;
    node->value = name.value;
    node->line_number = name.line_number; // <<< THIS FIXES THE "LINE 0" BUG

    // Parse statements inside the function body
    while(peek().value != "}") {
        auto stmt = parseStatement();
        if (stmt) {
            node->children.push_back(std::move(stmt));
        }
    }
    advance(); // Consume '}'
    return node;
}

std::unique_ptr<AstNode> Parser::parseStatement() {
    // Check if the current statement is a keyword 'int'.
    if (peek().type == TokenType::KEYWORD && peek().value == "int") {
        // Look ahead to distinguish between a function and a variable.
        if (tokens[current + 2].value == "(") {
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
    else if (peek().type == TokenType::IDENTIFIER && tokens[current + 1].value == "(") {
        advance(); // Consume the identifier
        return parseFunctionCall();
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