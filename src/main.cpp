#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <utility>

#include "TrackedElement.hpp"
#include "sorting.hpp"
#include "searching.hpp"
#include "data_generator.hpp"
#include "benchmark.hpp"
#include "utils.hpp"

int main() {
    std::vector<size_t> sizes;
    for (size_t s = 50000; s <= 1000000; s += 50000) {
        sizes.push_back(s);
    }

    sizes.push_back(100000000);

    std::vector<std::pair<std::string, std::vector<TrackedElement> (*)(size_t)>> generators = {
        {"Random", generate_random},
        {"Nearly Sorted", generate_nearly_sorted},
        {"Reverse Sorted", generate_reverse_sorted},
        {"Duplicates", generate_duplicate_values}
    };

    std::vector<std::pair<std::string, void (*)(std::vector<TrackedElement>&)>> sort_algorithms = {
        {"Twin Array Sort", twinArraySort},
        {"Cluster Sort (Comb)", clusterSortComb},
        {"Cluster Sort (Shell)", clusterSortShell},
        {"QR Sort", qrSort}
    };

    std::vector<std::pair<std::string, int (*)(const std::vector<TrackedElement>&, const TrackedElement&)>> search_algorithms = {
        {"Binary Search", binarySearch},
        {"Exponential Search", exponentialSearch},
        {"Linear Search", linearSearch}
    };

    std::vector<BenchmarkResult> sorting_results;
    std::vector<SearchBenchmarkResult> searching_results;

    // ==================== 1. SORTING EXPERIMENT ====================
    std::cout << "=== SORTING BENCHMARK ===\n";
    std::cout << std::left
              << std::setw(22) << "Algorithm"
              << std::setw(16) << "Dataset"
              << std::setw(12) << "Size"
              << std::setw(14) << "Time (ms)"
              << std::setw(14) << "Mem (KB)"
              << std::setw(16) << "Comparisons"
              << "Status\n";
    std::cout << std::string(100, '-') << "\n";

    std::vector<TrackedElement> final_sorted_data;

    for (size_t size : sizes) {
        for (const auto& [dataset_name, gen_func] : generators) {
            std::vector<TrackedElement> base_data = gen_func(size);

            for (const auto& [algo_name, algo_func] : sort_algorithms) {
                std::vector<TrackedElement> working_copy = base_data;

                BenchmarkResult res = run_sorting_benchmark(
                    algo_name, dataset_name, working_copy, algo_func
                );

                bool passed = verify_sorted(working_copy);
                sorting_results.push_back(res);

                std::cout << std::left
                          << std::setw(22) << res.algorithm_name
                          << std::setw(16) << res.dataset_type
                          << std::setw(12) << res.dataset_size
                          << std::setw(14) << std::fixed << std::setprecision(3) << res.execution_time_ms
                          << std::setw(14) << res.memory_usage_kb
                          << std::setw(16) << res.num_comparisons
                          << (passed ? "PASS" : "FAIL!") << "\n";

                // Save the 1,000,000-element sorted Random array for the search experiment
                if (size == 1000000 && dataset_name == "Random" && algo_name == "QR Sort") {
                    final_sorted_data = std::move(working_copy);
                }
            }
            std::cout << std::string(100, '-') << "\n";
        }
    }

    export_sorting_to_csv("sorting_results.csv", sorting_results);

    // ==================== 2. SEARCH EXPERIMENT ====================
    const size_t NUM_QUERIES = 1000;
    std::vector<TrackedElement> queries = generate_search_queries(final_sorted_data, NUM_QUERIES);

    std::cout << "\n=== SEARCH BENCHMARK (N = " << final_sorted_data.size()
              << ", Queries = " << NUM_QUERIES << ") ===\n";
    std::cout << std::left
              << std::setw(20) << "Algorithm"
              << std::setw(15) << "Best (ms)"
              << std::setw(15) << "Avg (ms)"
              << std::setw(15) << "Worst (ms)"
              << std::setw(14) << "Best Comps"
              << std::setw(14) << "Avg Comps"
              << std::setw(14) << "Worst Comps"
              << "Mem (KB)\n";
    std::cout << std::string(115, '-') << "\n";

    for (const auto& [search_name, search_func] : search_algorithms) {
        SearchBenchmarkResult s_res = run_search_benchmark(
            search_name, final_sorted_data, queries, search_func
        );
        searching_results.push_back(s_res);

        std::cout << std::left
                  << std::setw(20) << s_res.algorithm_name
                  << std::setw(15) << std::fixed << std::setprecision(6) << s_res.best_case_time_ms
                  << std::setw(15) << std::fixed << std::setprecision(6) << s_res.average_search_time_ms
                  << std::setw(15) << std::fixed << std::setprecision(6) << s_res.worst_case_time_ms
                  << std::setw(14) << s_res.best_case_comparisons
                  << std::setw(14) << std::fixed << std::setprecision(1) << s_res.average_comparisons
                  << std::setw(14) << s_res.worst_case_comparisons
                  << s_res.memory_usage_kb << "\n";
    }

    export_searching_to_csv("searching_results.csv", searching_results);
    std::cout << "\nBenchmarks complete! Results exported to sorting_results.csv and searching_results.csv\n";

    return 0;
}