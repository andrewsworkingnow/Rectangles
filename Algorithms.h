// Algorithms.h
#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Graph.h"
#include <unordered_map>
#include <unordered_set>

class Algorithms {
public:
    static std::vector<std::vector<int>> classify_rectangles(const Graph& graph);
    static std::vector<std::vector<int>> classify_by_index(const Graph& graph);
    static std::vector<std::vector<int>> non_overlapping_rectangles(const Graph& graph);
    static std::vector<std::vector<int>> non_overlapping_rectangles2(const Graph& graph);
    static std::unordered_map<int, std::unordered_set<int>> build_adjacency_list(const std::vector<custom_shapes::Rectangle>& rectangles);
    static std::vector<int> find_maximal_independent_set(std::unordered_map<int, std::unordered_set<int>>& adjacency_list);
    static std::vector<std::vector<int>> minGroups(const Graph& graph);
    static bool doesOverlap(const Rectangle& r1, const Rectangle& r2);
    static void print_groups(const std::vector<std::vector<int>>& groups);
};

#endif
