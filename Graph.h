// Graph.h
#ifndef GRAPH_H
#define GRAPH_H

#include "Rectangle.h"
#include <vector>

using namespace custom_shapes;

class Graph {
public:
    std::vector<Rectangle> rectangles;  // Stores all rectangles

    void add_rectangle(const Rectangle& rect);
    void print_rectangles();  // Debug function

};

#endif
