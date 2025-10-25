#include "PythonTokenizer.h"
#include <regex>
#include <vector>
#include <string>
#include <sstream> // For splitting the file into lines

// Helper function to get the indentation level (count leading spaces)
int getIndentation(const std::string& line) {
    int count = 0;
    for (char ch : line) {
        if (ch == ' ') {
            count++;
        } else {
            break;
        }
    }
    return count;
}

std::vector<Token> PythonTokenizer::tokenize(const std::string& source_code) {
    std::vector<Token> tokens;
    std::vector<int> indentStack; // A stack to keep track of indent levels
    indentStack.push_back(0); // Start with zero indentation
    int lineNumber = 0;

    std::stringstream stream(source_code);
    std::string line;

    // Regex for the rest of the line (same as our previous attempt)
    const std::regex tokenRegex(
        "\\b(def|if|else|return|print)\\b|" // Keywords
        "([a-zA-Z_][a-zA-Z0-9_]*)|"       // Identifiers
        "(\\d+)|"                         // Numbers
        "(\".*?\")|"                      // Strings
        "(\\(|\\)|:|=|==|\\+|-|\\*|/)|"  // Separators/Operators
        "(#.*)"                           // Comments
    );

    while (std::getline(stream, line)) {
        lineNumber++;
        
        // Skip empty lines and full-line comments
        if (line.find_first_not_of(' ') == std::string::npos || line.find_first_not_of(' ') == line.find('#')) {
            continue;
        }

        // 1. Handle Indentation
        int currentIndent = getIndentation(line);
        if (currentIndent > indentStack.back()) {
            // New indent level
            indentStack.push_back(currentIndent);
            tokens.push_back({TokenType::INDENT, "INDENT", lineNumber});
        } else {
            // Check for dedents
            while (currentIndent < indentStack.back()) {
                indentStack.pop_back();
                tokens.push_back({TokenType::DEDENT, "DEDENT", lineNumber});
            }
        }

        // 2. Tokenize the rest of the line
        auto words_begin = std::sregex_iterator(line.begin(), line.end(), tokenRegex);
        auto words_end = std::sregex_iterator();

        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            const std::smatch& match = *i;
            
            if (match[1].matched) {
                tokens.push_back({TokenType::KEYWORD, match[1].str(), lineNumber});
            } else if (match[2].matched) {
                tokens.push_back({TokenType::IDENTIFIER, match[2].str(), lineNumber});
            } else if (match[3].matched || match[4].matched) {
                tokens.push_back({TokenType::LITERAL, match.str(0), lineNumber});
            } else if (match[5].matched) {
                tokens.push_back({TokenType::SEPARATOR, match[5].str(), lineNumber});
            } else if (match[6].matched) {
                tokens.push_back({TokenType::COMMENT, match[6].str(), lineNumber});
            }
        }
    }

    // Add final dedents to close any open blocks at the end of the file
    while (indentStack.back() > 0) {
        indentStack.pop_back();
        tokens.push_back({TokenType::DEDENT, "DEDENT", lineNumber});
    }

    return tokens;
}