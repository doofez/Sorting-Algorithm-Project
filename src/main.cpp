#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <utility>

#include "TrackedElement.hpp"
#include "sorting.hpp"
#include "data_generator.hpp"
#include "benchmark.hpp"
#include "utils.hpp"

int main() {
    // Test sizes (start smaller to verify correctness, then scale up to 100,000 or 1,000,000)
    std::vector<size_t> sizes = {10000, 100000};

    // Sorting algorithms to test
    std::vector<std::pair<std::string, void (*)(std::vector<TrackedElement>&)>> algorithms = {
        {"Twin Array Sort", twinArraySort},
        {"Cluster Sort (Comb)", clusterSortComb},
        {"Cluster Sort (Shell)", clusterSortShell},
        {"QR Sort", qrSort}
    };

    std::vector<BenchmarkResult> all_results;

    std::cout << std::left
              << std::setw(22) << "Algorithm"
              << std::setw(18) << "Dataset"
              << std::setw(12) << "Size"
              << std::setw(14) << "Time (ms)"
              << std::setw(16) << "Comparisons"
              << "Status\n";
    std::cout << std::string(90, '-') << "\n";

    for (size_t size : sizes) {
        std::vector<std::pair<std::string, std::vector<TrackedElement>>> datasets = {
            {"Random", generate_random(size)},
            {"Nearly Sorted", generate_nearly_sorted(size)},
            {"Reverse Sorted", generate_reverse_sorted(size)},
            {"Duplicates", generate_duplicate_values(size)}
        };

        for (const auto& [dataset_name, base_data] : datasets) {
            for (const auto& [algo_name, algo_func] : algorithms) {
                // Clone the unsorted dataset so each algorithm sorts the exact same input
                std::vector<TrackedElement> working_copy = base_data;

                BenchmarkResult res = run_sorting_benchmark(
                    algo_name, dataset_name, working_copy, algo_func
                );

                bool passed = verify_sorted(working_copy);
                all_results.push_back(res);

                std::cout << std::left
                          << std::setw(22) << res.algorithm_name
                          << std::setw(18) << res.dataset_type
                          << std::setw(12) << res.dataset_size
                          << std::setw(14) << std::fixed << std::setprecision(3) << res.execution_time_ms
                          << std::setw(16) << res.num_comparisons
                          << (passed ? "PASS" : "FAIL!") << "\n";
            }
            std::cout << std::string(90, '-') << "\n";
        }
    }

    export_sorting_to_csv("sorting_results.csv", all_results);
    std::cout << "\nBenchmark complete! Results exported to sorting_results.csv\n";

    return 0;
}