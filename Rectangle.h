// Rectangle.h
#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <vector>
#include <utility>  // For std::pair

namespace custom_shapes {

    class Rectangle {
    public:
        std::vector<std::pair<long, long>> points; // Stores 4 points

        Rectangle(long x1, long y1, long x2, long y2);
        void print(); // Debugging function to print rectangle points
        std::vector<std::pair<long, long>> get_points();
    };
}
#endif
