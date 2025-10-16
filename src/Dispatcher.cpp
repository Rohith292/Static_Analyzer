#include "Dispatcher.h"
#include "Tokenizer.h"
#include "Parser.h"
#include "Analyzer.h"
#include "Reporter.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;

void printAst(const AstNode& node, int indent = 0) {
    for (int i = 0; i < indent; ++i) {
        std::cout << "  ";
    }
    
    switch (node.type) {
        case NodeType::PROGRAM:   
            std::cout << "[Program]" << std::endl; 
            break;
        case NodeType::FUNCTION_DECLARATION:
            std::cout << "[Function Declaration] -> name: " << node.value << std::endl;
            break;
        case NodeType::VARIABLE_DECLARATION:
            std::cout << "[Variable Declaration] -> name: " << node.value << std::endl; 
            break;
        case NodeType::IF_STATEMENT:
            std::cout << "[If Statement]" << std::endl;
            if (!node.children.empty()) {
                for (int i = 0; i < indent + 1; ++i) std::cout << "  ";
                std::cout << "Condition: " << node.children[0]->value << std::endl;
                
                for (size_t i = 1; i < node.children.size(); ++i) {
                    printAst(*node.children[i], indent + 1);
                }
            }
            break;
        case NodeType::STATEMENT: 
            std::cout << "[Statement] -> " << node.value << std::endl; 
            break;
    }

    if (node.type != NodeType::IF_STATEMENT) {
        for (const auto& child : node.children) {
            printAst(*child, indent + 1);
        }
    }
}
//helper function to get the file extension
string getFileExtension(const string& fileName){
    size_t dot_pos=fileName.find_last_of(".");
    if(dot_pos==string::npos){
        return "";
    }
    return fileName.substr(dot_pos+1);
}

void Dispatcher::analyze(const string& fileName,const string& format){
    string extension=getFileExtension(fileName);

    ifstream file(fileName);
    if(!file.is_open()){
        cerr<<"Error:could not open file"<<fileName<<endl;
        return;
    }

    stringstream buffer;
    buffer<<file.rdbuf();
    string source_code =buffer.str();

    if(extension=="cpp"){
        cout<<"Analyzing c++ file.."<<endl;

        //c++ pipeline
        Tokenizer tokenizer;
        Parser parser;
        Analyzer analyzer;
        Reporter reporter;

        auto tokens =tokenizer.tokenize(source_code);
        auto ast=parser.parse(tokens);
        //lines to print AST
        std::cout<<"----Abstract Syntax Tree---"<<std::endl;
        if(ast){
            printAst(*ast);
        }
        std::cout<<"---------------------------"<<std::endl;
        auto issues=analyzer.analyze(*ast);
        if(format=="html"){
            reporter.generateHTMLReport(issues,"report.html");

        }else{
            reporter.generateConoleReport(issues);
        }
    }
     else if (extension == "py") {
        std::cout << "Python analysis is not implemented yet." << std::endl;
    } else if (extension == "java") {
        std::cout << "Java analysis is not implemented yet." << std::endl;
    } else {
        std::cerr << "Error: Unsupported file type '" << extension << "'" << std::endl;
    }
}