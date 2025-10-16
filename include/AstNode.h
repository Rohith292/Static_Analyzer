// include/AstNode.h
#pragma once

#include <string>
#include <vector>
#include <memory>

// NOTE: "using namespace std;" has been REMOVED.

// Defines the different types of nodes we can have in our tree
enum class NodeType {
    PROGRAM,
    FUNCTION_DECLARATION,
    VARIABLE_DECLARATION,
    STATEMENT
};

// Represents a single node in our abstract syntax tree
struct AstNode {
    NodeType type;
    std::string value; // Use std::string

    // Each node can have multiple children forming a tree structure
    std::vector<std::unique_ptr<AstNode>> children; // Use std::vector and std::unique_ptr
};