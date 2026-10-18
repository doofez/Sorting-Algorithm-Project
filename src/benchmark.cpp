#include "benchmark.hpp"
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>

// Define the static counter declared in TrackedElement.hpp
unsigned long long TrackedElement::comparison_count = 0;

size_t get_current_memory_kb() {
    std::ifstream status_file("/proc/self/status");
    std::string line;

    while (std::getline(status_file, line)) {
        // VmRSS gives current physical memory usage in KB on Linux
        if (line.rfind("VmRSS:", 0) == 0) {
            std::istringstream iss(line.substr(6));
            size_t memory_kb = 0;
            iss >> memory_kb;
            return memory_kb;
        }
    }
    return 0;
}

BenchmarkResult run_sorting_benchmark(
    const std::string& algorithm_name,
    const std::string& dataset_type,
    std::vector<TrackedElement>& data,
    void (*sort_function)(std::vector<TrackedElement>&)
) {
    TrackedElement::comparison_count = 0;
    size_t mem_before = get_current_memory_kb();

    auto start_time = std::chrono::high_resolution_clock::now();
    sort_function(data);
    auto end_time = std::chrono::high_resolution_clock::now();

    size_t mem_after = get_current_memory_kb();
    size_t mem_used = (mem_after > mem_before) ? (mem_after - mem_before) : 0;

    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;

    BenchmarkResult result;
    result.algorithm_name = algorithm_name;
    result.dataset_type = dataset_type;
    result.dataset_size = data.size();
    result.execution_time_ms = elapsed.count();
    result.memory_usage_kb = mem_used;
    result.num_comparisons = TrackedElement::comparison_count;

    return result;
}

SearchBenchmarkResult run_search_benchmark(
    const std::string& algorithm_name,
    const std::vector<TrackedElement>& data,
    const std::vector<TrackedElement>& queries,
    int (*search_function)(const std::vector<TrackedElement>&, const TrackedElement&)
) {
    double total_time_ms = 0.0;
    double worst_time_ms = 0.0;
    size_t mem_before = get_current_memory_kb();

    for (const auto& query : queries) {
        auto start_time = std::chrono::high_resolution_clock::now();
        volatile int idx = search_function(data, query);
        (void)idx;
        auto end_time = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
        double current_ms = elapsed.count();
        total_time_ms += current_ms;
        if (current_ms > worst_time_ms) {
            worst_time_ms = current_ms;
        }
    }

    size_t mem_after = get_current_memory_kb();
    size_t mem_used = (mem_after > mem_before) ? (mem_after - mem_before) : 0;

    SearchBenchmarkResult result;
    result.algorithm_name = algorithm_name;
    result.average_search_time_ms = queries.empty() ? 0.0 : (total_time_ms / queries.size());
    result.worst_case_time_ms = worst_time_ms;
    result.memory_usage_kb = mem_used;

    return result;
}