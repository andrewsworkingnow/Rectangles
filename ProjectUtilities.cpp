// ProjectUtilities.cpp
#include "ProjectUtilities.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <vector>
#include <limits>  

const std::string base_path = "internship_project_datasets/";
const std::vector<std::string> categories = { "small", "medium", "large", "huge" };
const std::vector<std::vector<std::string>> datasets = {
    {"data_set_1", "data_set_2", "data_set_3", "data_set_4", "data_set_5"},
    {"data_set_6", "data_set_7", "data_set_8", "data_set_9", "data_set_10"},
    {"data_set_11", "data_set_12", "data_set_13", "data_set_14", "data_set_15"},
    {"data_set_16"}
};
int category_index = -1;
int dataset_index = 0;
int random_index = 0;


namespace fs = std::filesystem;

std::string ProjectUtilities::get_dataset_path() {
    // Ask user for category
    std::cout << "Select a dataset category:\n";
    for (size_t i = 0; i < categories.size(); ++i) {
        std::cout << i + 1 << ". " << categories[i] << std::endl;
    }
    while (true) {
        std::cout << "Enter your choice (1-" << categories.size() << "): ";
        std::cin >> category_index;
        if (category_index >= 1 && category_index <= static_cast<int>(categories.size())) break;
        std::cout << "Invalid choice, please try again.\n";
    }
    category_index--; // Adjust for 0-based index

    // Ask user for dataset within the chosen category
    std::cout << "Select a dataset from " << categories[category_index] << ":\n";
    for (size_t i = 0; i < datasets[category_index].size(); ++i) {
        std::cout << i + 1 << ". " << datasets[category_index][i] << std::endl;
    }
    while (true) {
        std::cout << "Enter your choice (1-" << datasets[category_index].size() << "): ";
        std::cin >> dataset_index;
        if (dataset_index >= 1 && dataset_index <= static_cast<int>(datasets[category_index].size())) break;
        std::cout << "Invalid choice, please try again.\n";
    }
    dataset_index--; // Adjust for 0-based index

    // Construct full path
    return base_path + categories[category_index] + "/" + datasets[category_index][dataset_index] + ".txt";
}

Graph ProjectUtilities::load_from_file() {
    std::string file_path = get_dataset_path();
    return load_from_file(file_path);
}

Graph ProjectUtilities::load_from_file(std::string& file_path) {
    Graph graph;
    std::ifstream file(file_path);

    if (!file) {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return graph;
    }

    std::string line;

    long long x1, y1, x2, y2;        
    while (std::getline(file, line)) {
        //long x1, y1, x2, y2;
        std::istringstream iss(line);

        if (!(iss >> x1 >> y1 >> x2 >> y2)) {
            std::cerr << "Warning: Invalid input format. Skipping this line: " << line << std::endl;
            continue;
        }

        // Check if values are within valid range
        if (x1 > LONG_MAX || x1 < LONG_MIN ||
            y1 > LONG_MAX || y1 < LONG_MIN ||
            x2 > LONG_MAX || x2 < LONG_MIN ||
            y2 > LONG_MAX || y2 < LONG_MIN) {
            std::cerr << "Warning: Values exceed long range. Skipping this rectangle." << std::endl;
            continue;
        }

        try {
            graph.add_rectangle(custom_shapes::Rectangle(x1, y1, x2, y2));
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Warning: " << e.what() << " Skipping this rectangle." << std::endl;
        }
    }
    file.close();
    return graph;
}

void ProjectUtilities::save_to_file(const std::string& file_path, const Graph& graph) {
    std::ofstream file(file_path);
    if (!file) {
        std::cerr << "Error opening file for writing: " << file_path << std::endl;
        return;
    }

    for (const auto& rect : graph.rectangles) {
        for (const auto& p : rect.points) {
            file << p.first << " " << p.second << " ";
        }
        file << "\n";
    }

    file.close();
}


void ProjectUtilities::save_groups_to_files(const Graph& graph, const std::vector<std::vector<int>>& groups) {
    std::string output_folder = "output/";
    if (category_index != -1)
        output_folder += categories[category_index] + "/" + datasets[category_index][dataset_index];
    else
        output_folder += "random/test_" + std::to_string(random_index++);

    // Create the category-specific output folder if it doesn't exist
    std::filesystem::create_directories(output_folder);

    for (size_t i = 0; i < groups.size(); ++i) {

        std::string filename = output_folder + "/group_" + std::to_string(i + 1) + ".txt";
        std::ofstream file(filename);

        if (!file) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

       // file << "Group " << i + 1 << "\n";
        for (int index : groups[i]) {
            const custom_shapes::Rectangle& rect = graph.rectangles[index];
            file << rect.points[0].first << " " << rect.points[0].second << " "
                << rect.points[2].first << " " << rect.points[2].second << "\n";
        }
        file << "\n";
        file.close();
        std::cout << "Saved group_" << i + 1 << " to " << filename << std::endl;

    }
    //std::cout << "Saved all groups to " << output_folder << std::endl;

}

bool ProjectUtilities::prompt_user() {
    char choice;
    while (true) {
        std::cout << "Do you want to perform another test? (y/n): ";
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            return true;
        }
        if (choice == 'n' || choice == 'N') {
            std::cout << "Thanks for using Rectangle tool, Have a good day!" << std::endl;
            return false;
        }
        std::cout << "Wrong input, please enter 'y' or 'n'." << std::endl;
    }
}

size_t get_current_memory_usage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return pmc.PrivateUsage; // Memory in bytes
}

void ProjectUtilities::process_all_datasets() {
    std::string log_filename = "output/all_datasets_results.csv";
    bool file_exists = std::filesystem::exists(log_filename);

    std::ofstream log_file(log_filename, std::ios::app); // Append mode
    if (!log_file) {
        std::cerr << "Error opening log file: " << log_filename << std::endl;
        return;
    }

    // Write headers if the file is newly created
    if (!file_exists) {
        log_file << "Category,Dataset,Number of inputs,Number of output groups,Runtime (s),Memory Usage (MB)\n";
    }
    std::vector<double> runtimes;
    std::vector<double> memory_usages;
    for (size_t cat_index = 0; cat_index < categories.size() - 1; ++cat_index) {
        for (size_t data_index = 0; data_index < datasets[cat_index].size(); ++data_index) {
            category_index = cat_index;
            dataset_index = data_index;
            auto start_time = std::chrono::high_resolution_clock::now();
            size_t memory_before = get_current_memory_usage();
            std::cout << "Processing " << categories[cat_index]
                << " -> " << datasets[cat_index][data_index] << std::endl;
            std::string path = base_path + categories[cat_index] + "/" + datasets[cat_index][data_index] + ".txt";
            Graph graph;

            try {
                 graph = load_from_file(path);

                if (graph.rectangles.size() == 0) {
                    throw std::runtime_error("No valid rectangles found in the file. The file may be empty or all rectangles were invalid.");
                    continue;
                }

                // Proceed with processing
                auto groups = Algorithms::non_overlapping_rectangles(graph);
                //Algorithms::print_groups(groups);

                save_groups_to_files(graph, groups);
                size_t memory_after = get_current_memory_usage();
                auto end_time = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed_time = end_time - start_time;

                double runtime = elapsed_time.count();
                double memory_usage = static_cast<double>(memory_after - memory_before) / (1024.0 * 1024.0); // Convert to MB

                runtimes.push_back(runtime);
                memory_usages.push_back(memory_usage);

                std::cout << "Dataset: " << datasets[category_index][dataset_index]
                    << " | Runtime: " << runtime << "s"
                    << " | Memory Usage: " << memory_usage << " MB\n";
                log_file << categories[category_index] << ","
                    << datasets[category_index][dataset_index] << ","
                    << graph.rectangles.size() << ","
                    << groups.size() << ","
                    << runtime << ","
                    << memory_usage << "\n";

            }
            catch (const std::runtime_error& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
            //graph.print_rectangles();


        }
    }
    std::cout << "All datasets processed successfully!" << std::endl;
}

void ProjectUtilities::process_all_files_in_directory(const std::string& directory = "input") {
    std::vector<std::string> dataset_files;
    category_index = -1;
    // Scan directory for files
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            dataset_files.push_back(entry.path().string());
        }
    }

    // Check if any files were found
    if (dataset_files.empty()) {
        std::cout << "No dataset files found in directory: " << directory << std::endl;
        return;
    }

    // Process each file
    for (auto& file : dataset_files) {
        std::cout << "Processing file: " << file << std::endl;

        Graph graph;
       // graph.print_rectangles();
        try {
            graph = load_from_file(file);

            if (graph.rectangles.empty()) {
                throw std::runtime_error("No valid rectangles found in the file. The file may be empty or all rectangles were invalid.");
                continue;
            }

            // Proceed with processing
            auto groups = Algorithms::non_overlapping_rectangles(graph);
            //Algorithms::print_groups(groups);

            ProjectUtilities::save_groups_to_files(graph, groups);

        }
        catch (const std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

    }
}

void ProjectUtilities::process_x_dataset() {
    bool operate = true;
    while (operate) {
        auto start_time = std::chrono::steady_clock::now();
        double initial_memory = ProjectUtilities::get_memory_usage();
        Graph graph;
        // graph.print_rectangles();
        try {
            graph = ProjectUtilities::load_from_file();
            graph.print_rectangles();

            if (graph.rectangles.empty()) {
                throw std::runtime_error("No valid rectangles found in the file. The file may be empty or all rectangles were invalid.");
                continue;
            }

            // Proceed with processing

            auto groups = Algorithms::non_overlapping_rectangles(graph);
            Algorithms::print_groups(groups);

            ProjectUtilities::save_groups_to_files(graph, groups);

            auto end_time = std::chrono::steady_clock::now();
            double final_memory = ProjectUtilities::get_memory_usage();
            std::chrono::duration<double> runtime = end_time - start_time;

            std::cout << "Runtime: " << runtime.count() << " seconds" << std::endl;
            std::cout << "Memory Usage: " << (final_memory - initial_memory) << " MB" << std::endl;

            operate = ProjectUtilities::prompt_user();

            ProjectUtilities::save_groups_to_files(graph, groups);

        }
        catch (const std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }


    }
}

double ProjectUtilities::get_memory_usage() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS memCounter;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter))) {
        return static_cast<double>(memCounter.WorkingSetSize) / (1024 * 1024); // Convert to MB
    }
#else
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        return static_cast<double>(usage.ru_maxrss) / 1024; // Convert to MB
    }
#endif
    return 0.0;
}

void ProjectUtilities::process_input_method() {
    int choice;
    std::cout << "Select input method:\n";
    std::cout << "1. Process a single dataset\n";
    std::cout << "2. Process all datasets (small, medium, large)\n";
    std::cout << "3. Process all files in directory\n";
    std::cout << "Enter choice: ";
    std::cin >> choice;

    switch (choice) {
    case 1:
        ProjectUtilities::process_x_dataset();
        break;
    case 2:
        ProjectUtilities::process_all_datasets();
        break;
    case 3:
        ProjectUtilities::process_all_files_in_directory("input");
        break;
    default:
        std::cerr << "Invalid choice! Please select a valid option.\n";
    }
}
