#include "Analyzer.h"
#include <set>

// This new recursive helper function will find all variable declarations
// inside a given node and all its children.
void collectVariableDeclarations(const AstNode& node, std::set<std::string>& declaredVariables) {
    if (node.type == NodeType::VARIABLE_DECLARATION) {
        declaredVariables.insert(node.value);
    }
    
    // Continue the search into all children of the current node
    for (const auto& child : node.children) {
        collectVariableDeclarations(*child, declaredVariables);
    }
}

//finds all variables usages inside a node and its children
void collectVariableUsages(const AstNode& node, std::set<std::string>& usedVariables){
    //the only form of usage right now is the if statement
    if(node.type==NodeType::IF_STATEMENT && !node.children.empty()){
        //conditions value is the name of the variable being used
        usedVariables.insert(node.children[0]->value);
    }
    for(const auto& child :node.children){
        collectVariableUsages(*child,usedVariables);
    }
}

void Analyzer::traverse(const AstNode& node) {
    if (node.type == NodeType::FUNCTION_DECLARATION) {
        std::set<std::string> declaredVariables;
        std::set<std::string> usedVariables;

        // 1. Collect all declarations and usages within the function's scope.
        collectVariableDeclarations(node, declaredVariables);
        collectVariableUsages(node, usedVariables);

        // 2. Find the difference: variables that were declared but never used.
        for (const auto& varName : declaredVariables) {
            // Check if the declared variable exists in the set of used variables.
            if (usedVariables.find(varName) == usedVariables.end()) {
                // If it's not found (.find() returns .end()), it's an unused variable.
                m_issues.push_back({"Unused variable: " + varName});
            }
        }
    }

    // Continue traversing to find other functions in the program.
    for (const auto& child : node.children) {
        traverse(*child);
    }
}

std::vector<AnalysisIssue> Analyzer::analyze(const AstNode& root) {
    m_issues.clear();
    traverse(root);
    return m_issues;
}