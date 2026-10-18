#pragma once

#include<string>
#include <vector>
#include "TrackedElement.hpp"

struct BenchmarkResult {
    std::string algorithm_name;
    std::string dataset_type;
    size_t dataset_size;

    double execution_time_ms;
    size_t memory_usage_kb;
    unsigned long long num_comparisons;
};

struct SearchBenchmarkResult {
    std::string algorithm_name;

    double best_case_time_ms;
    double average_search_time_ms;
    double worst_case_time_ms;

    unsigned long long best_case_comparisons;
    double average_comparisons;
    unsigned long long worst_case_comparisons;

    size_t memory_usage_kb;
};

size_t get_current_memory_kb();

BenchmarkResult run_sorting_benchmark(
    const std::string& algorithm_name, 
    const std::string& dataset_type, 
    std::vector<TrackedElement>& data, 
    void (*sort_function)(std::vector<TrackedElement>&)
);

SearchBenchmarkResult run_search_benchmark(
    const std::string& algorithm_name, 
    const std::vector<TrackedElement>& data, 
    const std::vector<TrackedElement>& queries, 
    int (*search_function)(const std::vector<TrackedElement>&, const TrackedElement&)
);