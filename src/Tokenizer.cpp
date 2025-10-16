#include "Tokenizer.h"
#include<regex>
#include<iostream>
using namespace std;

vector<Token> Tokenizer::tokenize(const string& source_code){
    vector<Token>tokens;
    int line_number=1;

    //this regex is the heart of the tokenizer.Each set of paranthesis will be used to determine the token type.
    const regex token_regex(
        // Group 1: Keywords (int, return, if, else)
        "\\b(int|return|if|else)\\b|"
        // Group 2: Identifiers (variable names, function names)
        "([a-zA-Z_][a-zA-Z0-9_]*)|"
        // Group 3: Numbers (Literals)
        "(\\d+)|"
        // Group 4: Operators and Separators
        "(\\{|\\}|\\(|\\)|;|==|!=|<=|>=|=|<|>|\\+|-|\\*|/)|"
        // Group 5: Newlines (to track line numbers)
        "(\\n)|"
        // Group 6: Whitespace (to be ignored, but captured to advance position)
        "(\\s+)|"
        // Group 7: Comments (single line)
        "(//.*)"
    );

    auto words_begin=sregex_iterator(source_code.begin(),source_code.end(),token_regex);
    auto words_end=sregex_iterator();

    for(sregex_iterator i =words_begin;i!=words_end;++i){
        const smatch& match=*i;

        //determine the token type by seeing which capture group was successful
        if(match[1].matched){
            //keyword
            tokens.push_back({TokenType::KEYWORD,match[1].str(),line_number});

        }else if(match[2].matched){
            //identifier
            tokens.push_back({TokenType::IDENTIFIER,match[2].str(),line_number});
        }else if(match[3].matched){
            //literal
            tokens.push_back({TokenType::LITERAL,match[3].str(),line_number});
        }else if(match[4].matched){
            //seperators/operator
            tokens.push_back({TokenType::SEPARATOR,match[4].str(),line_number});
        }else if(match[5].matched){
            //newline
            line_number++;
        }else if(match[7].matched){
            //comment
            tokens.push_back({TokenType::COMMENT,match[7].str(),line_number});
        }
    }
    return tokens;
};