#include <iostream>
#include "Dispatcher.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./static_analyzer <filename>" << std::endl;
        return 1;
    }

    Dispatcher dispatcher;
    dispatcher.analyze(argv[1]);

    return 0;
}