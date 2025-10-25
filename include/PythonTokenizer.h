#pragma once

#include "Token.h" // We need the definition of Token
#include <string>
#include <vector>

class PythonTokenizer {
public:
    // The main function that takes code and returns a list of tokens
    std::vector<Token> tokenize(const std::string& source_code);
};