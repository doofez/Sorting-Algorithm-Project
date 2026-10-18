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
    std::vector<size_t> sizes;
    for (size_t s = 50000; s <= 1000000; s += 50000) {
        sizes.push_back(s);
    }

    sizes.push_back(100000000);

    // Dataset generator functions from data_generator.hpp
    std::vector<std::pair<std::string, std::vector<TrackedElement> (*)(size_t)>> generators = {
        {"Random", generate_random},
        {"Nearly Sorted", generate_nearly_sorted},
        {"Reverse Sorted", generate_reverse_sorted},
        {"Duplicates", generate_duplicate_values}
    };

    // Sorting algorithms from sorting.hpp
    std::vector<std::pair<std::string, void (*)(std::vector<TrackedElement>&)>> algorithms = {
        {"Twin Array Sort", twinArraySort},
        {"Cluster Sort (Comb)", clusterSortComb},
        {"Cluster Sort (Shell)", clusterSortShell},
        {"QR Sort", qrSort}
    };

    std::vector<BenchmarkResult> all_results;
    all_results.reserve(sizes.size() * generators.size() * algorithms.size());

    std::cout << std::left
              << std::setw(22) << "Algorithm"
              << std::setw(16) << "Dataset"
              << std::setw(12) << "Size"
              << std::setw(14) << "Time (ms)"
              << std::setw(14) << "Mem (KB)"
              << std::setw(16) << "Comparisons"
              << "Status\n";
    std::cout << std::string(100, '-') << "\n";

    for (size_t size : sizes) {
        for (const auto& [dataset_name, gen_func] : generators) {
            // Generate only one base dataset at a time to keep RAM usage clean at N = 1,000,000
            std::vector<TrackedElement> base_data = gen_func(size);

            for (const auto& [algo_name, algo_func] : algorithms) {
                std::vector<TrackedElement> working_copy = base_data;

                BenchmarkResult res = run_sorting_benchmark(
                    algo_name, dataset_name, working_copy, algo_func
                );

                bool passed = verify_sorted(working_copy);
                all_results.push_back(res);

                std::cout << std::left
                          << std::setw(22) << res.algorithm_name
                          << std::setw(16) << res.dataset_type
                          << std::setw(12) << res.dataset_size
                          << std::setw(14) << std::fixed << std::setprecision(3) << res.execution_time_ms
                          << std::setw(14) << res.memory_usage_kb
                          << std::setw(16) << res.num_comparisons
                          << (passed ? "PASS" : "FAIL!") << "\n";
            }
            std::cout << std::string(100, '-') << "\n";
        }
    }

    export_sorting_to_csv("sorting_results.csv", all_results);
    std::cout << "\nBenchmark complete! " << all_results.size()
              << " runs exported to sorting_results.csv\n";

    return 0;
}