#include "JavaTokenizer.h"
#include <regex>

std::vector<Token> JavaTokenizer::tokenize(const std::string& source_code) {
    std::vector<Token> tokens;
    int line_number = 1;

    // This regex is almost identical to the C++ one, just with Java keywords.
    const std::regex token_regex(
        // Group 1: Keywords (public, class, static, void, int, if, etc.)
        "\\b(public|class|static|void|int|if|else|return|String)\\b|"
        // Group 2: Identifiers
        "([a-zA-Z_][a-zA-Z0-9_]*)|"
        // Group 3: Numbers (Literals)
        "(\\d+)|"
        // Group 4: String Literals
        "(\".*?\")|"
        // Group 5: Operators and Separators
        "(\\{|\\}|\\(|\\)|\\[|\\]|;|==|=|<|>|\\+|-|\\*|/)|"
        // Group 6: Newlines
        "(\\n)|"
        // Group 7: Whitespace (to be ignored)
        "(\\s+)|"
        // Group 8: Comments (single line)
        "(//.*)"
    );

    auto words_begin = std::sregex_iterator(source_code.begin(), source_code.end(), token_regex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        const std::smatch& match = *i;
        
        if (match[1].matched) {
            tokens.push_back({TokenType::KEYWORD, match[1].str(), line_number});
        } else if (match[2].matched) {
            tokens.push_back({TokenType::IDENTIFIER, match[2].str(), line_number});
        } else if (match[3].matched || match[4].matched) { // Numbers or Strings
            tokens.push_back({TokenType::LITERAL, match.str(0), line_number});
        } else if (match[5].matched) {
            tokens.push_back({TokenType::SEPARATOR, match[5].str(), line_number});
        } else if (match[6].matched) {
            line_number++;
        } else if (match[8].matched) {
            tokens.push_back({TokenType::COMMENT, match[8].str(), line_number});
        }
    }
    return tokens;
}