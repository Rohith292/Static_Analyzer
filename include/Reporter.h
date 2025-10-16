#pragma once

#include "AnalysisIssue.h"
#include<vector>

class Reporter{
    public:
        void generateConoleReport(const std::vector<AnalysisIssue>& issues);
        void generateHTMLReport(const std::vector<AnalysisIssue>& issues, const std::string& fileName);
};