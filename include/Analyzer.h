// include/Analyzer.h
#pragma once

#include "AstNode.h"
#include "AnalysisIssue.h"
#include <vector>
#include <set> // <<< Make sure <set> is included

class Analyzer {
public:
    std::vector<AnalysisIssue> analyze(const AstNode& root);

private:
    void traverse(const AstNode& node);
    
    void analyzeScope(const AstNode& scopeNode); 

    // HELPER DECLARATIONS
    void collectDirectDeclarations(const AstNode& scopeNode, std::set<std::pair<std::string, int>>& declaredVariables);
    void collectVariableUsages(const AstNode& node, std::set<std::string>& usedVariables);

    std::vector<AnalysisIssue> m_issues;
};