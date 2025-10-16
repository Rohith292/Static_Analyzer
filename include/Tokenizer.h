//declares a tokenizer class with a single public method called tokenize
#pragma once

#include<string>
#include<vector>
#include "Token.h"
using namespace std;

class Tokenizer{
    public:
        //the main function of this class:takes code as a string, returns a list of tokens
        vector<Token> tokenize(const string& source_code);

};