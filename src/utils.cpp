#include "utils.hpp"
#include <fstream>
#include <iostream>

void export_sorting_to_csv(const std::string& filename, const std::vector<BenchmarkResult>& results) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << "\n";
        return;
    }

    file << "Algorithm,Dataset Type,Dataset Size,Execution Time (ms),Memory Usage (KB),Comparisons\n";
    for (const auto& r : results) {
        file << r.algorithm_name << ","
             << r.dataset_type << ","
             << r.dataset_size << ","
             << r.execution_time_ms << ","
             << r.memory_usage_kb << ","
             << r.num_comparisons << "\n";
    }
}

void export_searching_to_csv(const std::string& filename, const std::vector<SearchBenchmarkResult>& results) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << "\n";
        return;
    }

    file << "Algorithm,Average Search Time (ms),Worst Case Time (ms),Memory Usage (KB)\n";
    for (const auto& r : results) {
        file << r.algorithm_name << ","
             << r.average_search_time_ms << ","
             << r.worst_case_time_ms << ","
             << r.memory_usage_kb << "\n";
    }
}

bool verify_sorted(const std::vector<TrackedElement>& data) {
    for (size_t i = 1; i < data.size(); i++) {
        if (data[i].value < data[i - 1].value) {
            return false;
        }
    }
    return true;
}