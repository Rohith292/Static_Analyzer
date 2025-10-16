#include <iostream>
#include <string>
#include "Dispatcher.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./static_analyzer <filename> [--format html]" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::string format = "console"; // Default format

    // Simple argument parsing for the format flag
    if (argc > 3 && std::string(argv[2]) == "--format" && std::string(argv[3]) == "html") {
        format = "html";
    }

    Dispatcher dispatcher;
    dispatcher.analyze(filename, format);

    return 0;
}