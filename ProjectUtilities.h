// ProjectUtilities.h
#ifndef PROJECT_UTILITIES_H
#define PROJECT_UTILITIES_H

#include <string>
#include "Algorithms.h"
#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <sys/resource.h>
#endif
#include "Graph.h"



class ProjectUtilities {
public:

    static std::string get_dataset_path();
    static Graph load_from_file();
    static Graph load_from_file(std::string& file_path);
    static void save_to_file(const std::string& file_path, const Graph& graph);
    static void save_groups_to_files(const Graph& graph, const std::vector<std::vector<int>>& groups);
    static bool prompt_user();
    static void process_all_datasets();
    static void process_x_dataset();
    static void process_all_files_in_directory(const std::string& directory);
    static void process_input_method();
    static double get_memory_usage();
};

#endif
