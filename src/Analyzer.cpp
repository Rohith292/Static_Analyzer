// src/Analyzer.cpp
#include "Analyzer.h"
#include <set>
#include <vector>

// This helper finds variables DECLARED *directly* in this scope.
// It is NOT recursive.
void Analyzer::collectDirectDeclarations(const AstNode& scopeNode, std::set<std::pair<std::string, int>>& declaredVariables) {
    for (const auto& child : scopeNode.children) {
        if (child->type == NodeType::VARIABLE_DECLARATION) {
            // Insert both the name and the line number
            declaredVariables.insert({child->value, child->line_number});
        }
    }
}

// This helper finds variables USED *anywhere* inside this scope.
// It IS recursive.
void Analyzer::collectVariableUsages(const AstNode& node, std::set<std::string>& usedVariables) {
    // Check for usage in 'if' condition
    if (node.type == NodeType::IF_STATEMENT && !node.children.empty()) {
        usedVariables.insert(node.children[0]->value);
    }
    
    // Recurse into all children to find more usages
    for (const auto& child : node.children) {
        collectVariableUsages(*child, usedVariables);
    }
}

void Analyzer::analyzeScope(const AstNode& scopeNode) {
    std::set<std::pair<std::string, int>> declaredVariables; 
    std::set<std::string> usedVariables;

    //new logic for functions
    std::set<std::pair<std::string,int>>declaredFunctions;
    std::set<std::string> calledFunctions;

    collectDirectDeclarations(scopeNode, declaredVariables);
    collectVariableUsages(scopeNode, usedVariables);
    collectFunctionCalls(scopeNode,calledFunctions);

    for (const auto& child : scopeNode.children) {
        if (child->type == NodeType::FUNCTION_DECLARATION) {
            // We'll ignore "main" as it's the entry point
            if (child->value != "main") {
                declaredFunctions.insert({child->value, child->line_number});
            }
        }
    }

    for (const auto& varPair : declaredVariables) {
        std::string varName = varPair.first;
        int varLine = varPair.second;
        
        if (usedVariables.find(varName) == usedVariables.end()) {
            // Now we can create an issue with the correct line number
            m_issues.push_back({ "Unused variable: " + varName, varLine });
        }
    }
    for (const auto& funcPair : declaredFunctions) {
        std::string funcName = funcPair.first;
        int funcLine = funcPair.second;
        
        // If the declared function is NOT in the called set, report it.
        if (calledFunctions.find(funcName) == calledFunctions.end()) {
            m_issues.push_back({ "Unused function: " + funcName, funcLine });
        }
    }
}

void Analyzer::collectFunctionCalls(const AstNode& node, std::set<std::string>& calledFunctions) {
    if (node.type == NodeType::FUNCTION_CALL) {
        calledFunctions.insert(node.value);
    }
    // Recurse into all children to find more calls
    for (const auto& child : node.children) {
        collectFunctionCalls(*child, calledFunctions);
    }
}

// The traverse function's only job is to find scopes
void Analyzer::traverse(const AstNode& node) {
    // A scope is either the Program (global) or a Function
    if (node.type == NodeType::PROGRAM || node.type == NodeType::FUNCTION_DECLARATION) {
        analyzeScope(node);
    }

    // Continue traversing to find nested scopes
    for (const auto& child : node.children) {
        traverse(*child);
    }
}

std::vector<AnalysisIssue> Analyzer::analyze(const AstNode& root) {
    m_issues.clear();
    traverse(root); // This will start the traversal at the [Program] node
    return m_issues;
}