// Graph.cpp
#include "Graph.h"
#include <iostream>

using namespace custom_shapes;

void Graph::add_rectangle(const Rectangle& rect) {
    rectangles.push_back(rect);
}

void Graph::print_rectangles() {
    std::cout << "Rectangles in Graph:\n";
    for (size_t i = 0; i < rectangles.size(); ++i) {
        std::cout << "Rectangle " << i << ": ";
        rectangles[i].print();
    }
}
