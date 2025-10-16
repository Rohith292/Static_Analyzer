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

//helper function to get the file extension
string getFileExtension(const string& fileName){
    size_t dot_pos=fileName.find_last_of(".");
    if(dot_pos==string::npos){
        return "";
    }
    return fileName.substr(dot_pos+1);
}

void Dispatcher::analyze(const string& fileName){
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
        auto issues=analyzer.analyze(*ast);
        reporter.generateReport(issues);
    }
     else if (extension == "py") {
        std::cout << "Python analysis is not implemented yet." << std::endl;
    } else if (extension == "java") {
        std::cout << "Java analysis is not implemented yet." << std::endl;
    } else {
        std::cerr << "Error: Unsupported file type '" << extension << "'" << std::endl;
    }
}