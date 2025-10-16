#pragma once

#include "AstNode.h"
#include "AnalysisIssue.h"
#include <vector>

class Analyzer{
    public:
        std::vector<AnalysisIssue>analyze(const AstNode& root);
    private:
        void traverse(const AstNode& node);

    std::vector<AnalysisIssue>m_issues;
};