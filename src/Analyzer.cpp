#include "Analyzer.h"
#include <set>
void Analyzer::traverse(const AstNode& node){
    //when we find a function we check for its variables
    if(node.type==NodeType::FUNCTION_DECLARATION){
        std::set<std::string>declaredVariables;

        //find all the variables declared within this function
        for(const auto& child : node.children){
            if(child->type==NodeType::VARIABLE_DECLARATION){
                declaredVariables.insert(child->value);
            }
        }
        //simply return the unused variables
        for(const auto& varname:declaredVariables){
            m_issues.push_back({"unused variable :"+varname});
        }
    }
    //continue traversing down the tree for other functions
    for(const auto& child:node.children){
        traverse(*child);
    }
}

std::vector<AnalysisIssue> Analyzer::analyze(const AstNode& root){
    m_issues.clear();
    traverse(root);
    return m_issues;
}