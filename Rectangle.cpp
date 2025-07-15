// Rectangle.cpp
#include "Rectangle.h"
#include <iostream>

namespace custom_shapes {

    Rectangle::Rectangle(long x1, long y1, long x2, long y2) {
        // Ensure x1 < x2 and y1 < y2
        if (x1 >= x2 || y1 >= y2) {
                throw std::invalid_argument("Invalid rectangle: x1 must be different and smaller from x2 and y1 must be different and smaller from y2.");
        }

        // Assign points in correct order
        points = { {x1, y1}, {x2, y1}, {x2, y2}, {x1, y2} };
    }

    void Rectangle::print() {
        for (auto& p : points) {
            std::cout << "(" << p.first << ", " << p.second << ") ";
        }
        std::cout << std::endl;
    }

    std::vector<std::pair<long, long>> Rectangle::get_points() {
        return points;
    }
}
