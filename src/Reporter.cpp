#include "Reporter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
#include <map>
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

    // --- 1. Build a map of issues by line number ---
    // We store the "type" (variable or function) to use for styling
    std::map<int, std::string> issueMap;
    for (const auto& issue : issues) {
        if (issue.description.find("function") != std::string::npos) {
            issueMap[issue.line_number] = "function";
        } else {
            issueMap[issue.line_number] = "variable";
        }
    }

    // --- 2. Write HTML Header and NEW CSS ---
    reportFile << "<!DOCTYPE html><html><head><title>Analysis Report</title><style>"
               << "body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif; margin: 2em; background: #f9f9f9; }"
               << "h1, h2 { color: #333; border-bottom: 2px solid #ddd; padding-bottom: 5px; }"
               << "pre { background: #fff; border: 1px solid #ddd; border-radius: 5px; padding: 10px; line-height: 1.4em; }"
               << "code { font-family: 'Consolas', 'Menlo', 'Monaco', 'Courier New', monospace; }"
               << "table { border-collapse: collapse; width: 100%; margin-top: 20px; box-shadow: 0 2px 5px rgba(0,0,0,0.1); }"
               << "th, td { border: 1px solid #ddd; padding: 12px; text-align: left; }"
               << "th { background-color: #f2f2f2; }"
               << ".line-number { display: inline-block; width: 30px; color: #999; text-align: right; padding-right: 10px; user-select: none; }"
               // --- NEW CSS CLASSES ---
               << ".code-line { display: block; }"
               << ".issue-line-variable { background: #fff8f8; color: #D8000C; border-left: 3px solid #D8000C; margin-left: -13px; padding-left: 10px; }"
               << ".issue-line-function { background: #f8fff8; color: #008000; border-left: 3px solid #008000; margin-left: -13px; padding-left: 10px; }"
               << ".row-variable { background-color: #fff8f8; color: #D8000C; }"
               << ".row-function { background-color: #f8fff8; color: #008000; }"
               << "</style></head><body>";
    
    reportFile << "<h1>Static Analysis Report</h1>";

    // --- 3. Write Source Code with NEW Highlight Logic ---
    reportFile << "<h2>Source Code</h2><pre><code>";
    
    std::stringstream codeStream(source_code);
    std::string line;
    int currentLine = 1;
    while (std::getline(codeStream, line)) {
        reportFile << "<span class='line-number'>" << currentLine << "</span>";
        
        // Check if this line has an issue and apply the correct style
        if (issueMap.count(currentLine)) {
            std::string issueType = issueMap[currentLine];
            reportFile << "<span class='code-line issue-line-" << issueType << "'>" << line << "</span>\n";
        } else {
            reportFile << "<span class='code-line'>" << line << "</span>\n";
        }
        currentLine++;
    }
    reportFile << "</code></pre>";

    // --- 4. Write Issues Table with NEW Row Styles ---
    reportFile << "<h2>Issues Found</h2>";
    if (issues.empty()) {
        reportFile << "<p>No issues found. Great job!</p>";
    } else {
        reportFile << "<table><tr><th>Line</th><th>Description</th></tr>";
        for (const auto& issue : issues) {
            std::string rowClass = "";
            if (issue.description.find("function") != std::string::npos) {
                rowClass = "row-function";
            } else {
                rowClass = "row-variable";
            }
            reportFile << "<tr class='" << rowClass << "'><td><b>" << issue.line_number << "</b></td><td>" << issue.description << "</td></tr>";
        }
        reportFile << "</table>";
    }

    reportFile << "</body></html>";
    std::cout << "Successfully generated HTML report: " << filename << std::endl;
}