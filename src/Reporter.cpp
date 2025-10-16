#include "Reporter.h"
#include <iostream>
#include <fstream>
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

void Reporter::generateHTMLReport(const std::vector<AnalysisIssue>& issues, const std::string& fileName){
    ofstream reportFile(fileName);
    if(!reportFile.is_open()){
        cerr<<"Error could not open report file "<<fileName<<endl;
        return;
    }

    //Write the HTML header and some basic CSS for styling
    reportFile << "<!DOCTYPE html><html><head><title>Analysis Report</title><style>"
               << "body { font-family: sans-serif; margin: 2em; }"
               << "h1 { color: #333; }"
               << "table { border-collapse: collapse; width: 100%; }"
               << "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }"
               << "th { background-color: #f2f2f2; }"
               << ".issue { color: #D8000C; background-color: #FFD2D2; }"
               << "</style></head><body>";
    
    reportFile << "<h1>Static Analysis Report</h1>";

    if (issues.empty()) {
        reportFile << "<p>No issues found. Great job!</p>";
    } else {
        reportFile << "<table><tr><th>Description</th></tr>";
        for (const auto& issue : issues) {
            reportFile << "<tr class='issue'><td>" << issue.description << "</td></tr>";
        }
        reportFile << "</table>";
    }

    reportFile << "</body></html>";
    std::cout << "Successfully generated HTML report: " << fileName << std::endl;
}