# Multi-Language Static Code Analyzer in C++

A modular static code analysis tool built in C++ that detects issues in source code. This project was developed as a demonstration of compiler front-end principles, including lexical analysis, parsing into an Abstract Syntax Tree (AST), and rule-based analysis.

## 🚀 Key Features

- **Modular Architecture:** A clean pipeline consisting of a Tokenizer, Parser, Analyzer, Reporter, and Dispatcher.
- **Multi-Language Support:** Designed to be extensible, with current support for C++ and stubs for Python and Java.
- **Rule-Based Analysis:** The analyzer walks the AST to find potential issues.
  - Currently implemented: Detects unused variable declarations in C++.
- **AST Generation:** Parses source code into a hierarchical Abstract Syntax Tree for structural analysis.

## 🛠️ How to Build and Run

This project is built using g++.

1.  **Clone the repository:**
    ```bash
    git clone <your-repo-url>
    cd Static-Code-Analyzer
    ```

2.  **Create a build directory and compile:**
    ```bash
    mkdir build
    cd build
    g++ -std=c++17 -I../include ../src/main.cpp ../src/Tokenizer.cpp ../src/Parser.cpp ../src/Analyzer.cpp ../src/Reporter.cpp ../src/Dispatcher.cpp -o static_analyzer
    ```

3.  **Run the analyzer on a sample file:**
    ```bash
    ./static_analyzer ../samples/test.cpp
    ```

## ✨ Example

Given the following code in `samples/test.cpp`:
```cpp
int main() {
    int x = 10;
    int y = 20; // An unused variable
    return 0;
}
```

The tool will produce the following output:
```
Analyzing C++ file...

--- Analysis Report ---
[Issue] Unused variable: x
[Issue] Unused variable: y
-----------------------
```