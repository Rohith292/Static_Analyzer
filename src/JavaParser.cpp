#include "JavaParser.h"

//helper implementations
bool JavaParser::isAtEnd(){
    return current>=m_tokens.size();
}

Token JavaParser::peek(){
    return m_tokens[current];
}
Token JavaParser::advance(){
    if(!isAtEnd()) current++;
    return previous();
}
Token JavaParser::previous(){
    return m_tokens[current-1];
}

//parsing logic
std::unique_ptr<AstNode> JavaParser::parseVariableDeclaration(){
    //assumes int has been consumed
    Token name=advance();//consume identifier
    advance();//consumes '='
    Token value=advance(); //consume value
    advance();//consume ';'

    auto node=std::make_unique<AstNode>();
    node->type=NodeType::VARIABLE_DECLARATION;
    node->value=name.value;
    node->line_number=name.line_number;
    return node;
}

std::unique_ptr<AstNode> JavaParser::parseMethodDeclaration() {
    // Simplified parser for "public static void main(String[] args)"
    advance(); // consume 'static'
    advance(); // consume 'void'
    Token name = advance(); // consume 'main'
    
    // Consume everything up to the opening brace '{'
    while(peek().value != "{") {
        advance();
    }
    advance(); // Consume '{'

    auto node = std::make_unique<AstNode>();
    node->type = NodeType::FUNCTION_DECLARATION; // We can reuse this node type
    node->value = name.value;

    // Parse statements inside the method body
    while(peek().value != "}") {
        auto stmt = parseStatement();
        if (stmt) {
            node->children.push_back(std::move(stmt));
        }
    }
    advance(); // Consume '}'
    return node;
}

std::unique_ptr<AstNode> JavaParser::parseFunctionCall() {
    // We already consumed the identifier (e.g., "myCall")
    Token name = m_tokens[current - 1]; // Get the token we just passed

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

std::unique_ptr<AstNode> JavaParser::parseStatement(){
    //look for a method declaration
    if(peek().value=="public"){
        advance();//consume 'public'
        return parseMethodDeclaration();
    }
    //look for a variable declaration
    if(peek().value=="int"){
        advance();//consume 'int'
        return parseVariableDeclaration();
    }
    if (peek().type == TokenType::IDENTIFIER && m_tokens[current + 1].value == "(") {
        advance(); // Consume the identifier
        return parseFunctionCall();
    }
    advance();
    return nullptr;
}
//main parser function
std::unique_ptr<AstNode> JavaParser::parse(const std::vector<Token>& tokens){
    this->m_tokens=tokens;
    this->current=0;

    auto programNode=std::make_unique<AstNode>();
    programNode->type=NodeType::PROGRAM;
    while(!isAtEnd()){
        auto statement=parseStatement();
        if(statement){
            programNode->children.push_back(std::move(statement));
        }
    }
    return programNode;
}