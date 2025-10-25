#include "Reporter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
using namespace std;


void Reporter::generateConoleReport(const std::vector<AnalysisIssue>& issues) {
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


void Reporter::generateHTMLReport(const std::vector<AnalysisIssue>& issues, const std::string& filename, const std::string& source_code) {
    std::ofstream reportFile(filename);
    if (!reportFile.is_open()) {
        std::cerr << "Error: Could not open report file " << filename << std::endl;
        return;
    }

    // --- 1. Collect all issue line numbers ---
    std::set<int> issueLines;
    for (const auto& issue : issues) {
        issueLines.insert(issue.line_number);
    }

    // --- 2. Write HTML Header and CSS ---
    reportFile << "<!DOCTYPE html><html><head><title>Analysis Report</title><style>"
               << "body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif; margin: 2em; background: #f9f9f9; }"
               << "h1, h2 { color: #333; border-bottom: 2px solid #ddd; padding-bottom: 5px; }"
               << "pre { background: #fff; border: 1px solid #ddd; border-radius: 5px; padding: 10px; line-height: 1.4em; }"
               << "code { font-family: 'Consolas', 'Menlo', 'Monaco', 'Courier New', monospace; }"
               << "table { border-collapse: collapse; width: 100%; margin-top: 20px; }"
               << "th, td { border: 1px solid #ddd; padding: 12px; text-align: left; }"
               << "th { background-color: #f2f2f2; }"
               << ".line-number { display: inline-block; width: 30px; color: #999; text-align: right; padding-right: 10px; }"
               << ".issue-line { background-color: #fff8f8; color: #D8000C; display: block; margin-left: -10px; padding-left: 10px; border-left: 3px solid #D8000C; }"
               << "</style></head><body>";
    
    reportFile << "<h1>Static Analysis Report</h1>";

    // --- 3. Write Source Code with Highlights ---
    reportFile << "<h2>Source Code</h2><pre><code>";
    
    std::stringstream codeStream(source_code);
    std::string line;
    int currentLine = 1;
    while (std::getline(codeStream, line)) {
        reportFile << "<span class='line-number'>" << currentLine << "</span>";
        // Check if this line has an issue
        if (issueLines.count(currentLine)) {
            reportFile << "<span class='issue-line'>" << line << "</span>\n";
        } else {
            reportFile << line << "\n";
        }
        currentLine++;
    }
    reportFile << "</code></pre>";

    // --- 4. Write Issues Table ---
    reportFile << "<h2>Issues Found</h2>";
    if (issues.empty()) {
        reportFile << "<p>No issues found. Great job!</p>";
    } else {
        reportFile << "<table><tr><th>Line</th><th>Description</th></tr>";
        for (const auto& issue : issues) {
            reportFile << "<tr><td><b>" << issue.line_number << "</b></td><td>" << issue.description << "</td></tr>";
        }
        reportFile << "</table>";
    }

    reportFile << "</body></html>";
    std::cout << "Successfully generated HTML report: " << filename << std::endl;
}