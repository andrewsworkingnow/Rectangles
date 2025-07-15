// Algorithms.cpp
#include "Algorithms.h"
#include <algorithm>
#include <iostream>

std::vector<std::vector<int>> Algorithms::classify_rectangles(const Graph& graph) {
    std::vector<std::vector<int>> groups;

    // Placeholder classification logic (to be implemented)
    for (size_t i = 0; i < graph.rectangles.size(); ++i) {
        groups.push_back({ static_cast<int>(i) }); // Each rectangle in its own group for now
    }

    return groups;
}

std::vector<std::vector<int>> Algorithms::classify_by_index(const Graph& graph) {
    std::vector<int> even_indices, odd_indices, divisible_by_5;

    for (size_t i = 0; i < graph.rectangles.size(); ++i) {
        if (i % 5 == 0) divisible_by_5.push_back(i);
        else if (i % 2 == 0) even_indices.push_back(i);
        else if (i % 2 != 0) odd_indices.push_back(i);
    }

    return { even_indices, odd_indices, divisible_by_5 };
}

void Algorithms::print_groups(const std::vector<std::vector<int>>& groups) {
    std::cout << "Classified Groups:\n";
    for (size_t i = 0; i < groups.size(); ++i) {
        std::cout << "Group " << i << ": ";
        for (int index : groups[i]) {
            std::cout << index << " ";
        }
        std::cout << "\n";
    }
}

std::vector<std::vector<int>> Algorithms::non_overlapping_rectangles(const Graph& graph) {
    std::vector<std::vector<int>> groups;
    std::vector<Rectangle> rectangles = graph.rectangles;

    // Sort rectangles by x1 coordinate (leftmost edge)
    std::sort(rectangles.begin(), rectangles.end(), [](const Rectangle& a, const Rectangle& b) {
        return a.points[0].first < b.points[0].first;  // Sort by x1
        });

    for (int i = 0; i < rectangles.size(); i++) {
        bool placed = false;

        // Try to place the rectangle in an existing group
        for (auto& group : groups) {
            bool overlaps = false;

            for (int index : group) {
                if (!(rectangles[i].points[2].first < rectangles[index].points[0].first ||   // R1 is completely left of R2
                    rectangles[i].points[0].first > rectangles[index].points[2].first ||   // R1 is completely right of R2
                    rectangles[i].points[2].second < rectangles[index].points[0].second || // R1 is completely below R2
                    rectangles[i].points[0].second > rectangles[index].points[2].second))  // R1 is completely above R2
                {
                    overlaps = true;
                    break;
                }
            }

            if (!overlaps) {
                group.push_back(i); // Add to existing group
                placed = true;
                break;
            }
        }

        // If no existing group was found, create a new one
        if (!placed) {
            groups.push_back({ i });
        }
    }

    return groups;
}

bool Algorithms::doesOverlap(const Rectangle& r1, const Rectangle& r2) {
    long x1_r1 = r1.points[0].first, y1_r1 = r1.points[0].second;
    long x2_r1 = r1.points[2].first, y2_r1 = r1.points[2].second;
    long x1_r2 = r2.points[0].first, y1_r2 = r2.points[0].second;
    long x2_r2 = r2.points[2].first, y2_r2 = r2.points[2].second;

    return !(x2_r1 <= x1_r2 || x2_r2 <= x1_r1 || y2_r1 <= y1_r2 || y2_r2 <= y1_r1);
}

std::vector<std::vector<int>> Algorithms:: minGroups(const Graph& graphs) {
    std::vector<Rectangle> rectangles = graphs.rectangles;
    int n = rectangles.size();
    std::vector<std::vector<int>> graph(n);

    // Step 1: Build the intersection graph
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (doesOverlap(rectangles[i], rectangles[j])) {
                graph[i].push_back(j);
                graph[j].push_back(i);
            }
        }
    }

    // Step 2: Greedy Graph Coloring
    std::vector<int> color(n, -1); // -1 means uncolored
    int maxColor = 0;

    for (int i = 0; i < n; i++) {
        std::vector<bool> available(n, true);

        // Mark unavailable colors used by neighbors
        for (int neighbor : graph[i]) {
            if (color[neighbor] != -1) {
                available[color[neighbor]] = false;
            }
        }

        // Assign the lowest available color
        for (int c = 0; c < n; c++) {
            if (available[c]) {
                color[i] = c;
                maxColor = std::max(maxColor, c + 1);
                break;
            }
        }
    }
    // Step 3: Group rectangles by their assigned colors
    std::vector<std::vector<int>> groups(maxColor);
    for (int i = 0; i < n; i++) {
        groups[color[i]].push_back(i);
    }

    return groups;
}

// Build an adjacency list where edges connect non-overlapping rectangles
std::unordered_map<int, std::unordered_set<int>> Algorithms::build_adjacency_list(const std::vector<custom_shapes::Rectangle>& rectangles) {
    std::unordered_map<int, std::unordered_set<int>> adjacency_list;
    int n = rectangles.size();

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (!doesOverlap(rectangles[i], rectangles[j])) {
                adjacency_list[i].insert(j);
                adjacency_list[j].insert(i);
            }
        }
    }
    return adjacency_list;
}

// Find a maximal independent set (largest possible group of pairwise non-overlapping rectangles)
std::vector<int> Algorithms::find_maximal_independent_set(std::unordered_map<int, std::unordered_set<int>>& adjacency_list) {
    std::vector<int> independent_set;
    std::unordered_set<int> considered;

    while (!adjacency_list.empty()) {
        int best_node = -1;
        size_t max_connections = 0;

        for (const auto& [node, connections] : adjacency_list) {
            if (connections.size() > max_connections) {
                best_node = node;
                max_connections = connections.size();
            }
        }

        if (best_node == -1) break;

        independent_set.push_back(best_node);
        considered.insert(best_node);

        std::vector<int> to_remove;
        for (int neighbor : adjacency_list[best_node]) {
            considered.insert(neighbor);
            to_remove.push_back(neighbor);
        }

        for (int node : to_remove) {
            adjacency_list.erase(node);
        }

        adjacency_list.erase(best_node);
    }

    return independent_set;
}

// Main function to classify rectangles into the minimal number of non-overlapping groups
std::vector<std::vector<int>> Algorithms::non_overlapping_rectangles2(const Graph& graph) {
    std::vector<std::vector<int>> groups;
    std::unordered_map<int, std::unordered_set<int>> adjacency_list = build_adjacency_list(graph.rectangles);

    while (!adjacency_list.empty()) {
        std::vector<int> group = find_maximal_independent_set(adjacency_list);
        groups.push_back(group);

        for (int index : group) {
            adjacency_list.erase(index);
        }
    }

    return groups;
}
