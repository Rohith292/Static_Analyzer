//include/Token.h
#pragma once //prevents header from being added multiple times

#include<string>
using namespace std;

//Enum for all the diff types of tokens we can have
enum class TokenType{
    KEYWORD,  //if,else,return,int
    IDENTIFIER, //main,x,my_variable
    OPERATOR,   //-,+,*,/
    SEPARATOR,  //,{..}
    LITERAL,    //10,"hello"
    COMMENT,
    UNKNOWN,    //for any character that doesnt fit
    INDENT,
    DEDENT    
};

//a struct to hold information about a single token
struct Token{
    TokenType type;
    string value;
    int line_number;
};