#pragma once

#include "AnalysisIssue.h"
#include<vector>

class Reporter{
    public:
        void generateReport(const std::vector<AnalysisIssue>& issues);
};