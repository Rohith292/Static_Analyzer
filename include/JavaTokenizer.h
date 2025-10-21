#include "Token.h"
#include <string>
#include<vector>

class JavaTokenizer{
    public:
        std::vector<Token> tokenize(const std::string& source_code);
};