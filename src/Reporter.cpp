#include "Reporter.h"
#include <iostream>

void Reporter::generateReport(const std::vector<AnalysisIssue>& issues) {
    std::cout << "\n--- Analysis Report ---" << std::endl;
    if (issues.empty()) {
        std::cout << "No issues found." << std::endl;
    } else {
        for (const auto& issue : issues) {
            // We'll add line numbers to AnalysisIssue later for more detail
            std::cout << "[Issue] " << issue.description << std::endl;
        }
    }
    std::cout << "-----------------------" << std::endl;
}