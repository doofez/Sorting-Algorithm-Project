#include "data_generator.hpp"
#include <random>
#include <algorithm>

std::vector<TrackedElement> generate_random(size_t size) {
    std::vector<TrackedElement> data(size);
    std::mt19937 rng(42); // Fixed seed for reproducible benchmarks
    std::uniform_int_distribution<int> dist(0, static_cast<int>(size * 2));

    for (size_t i = 0; i < size; i++) {
        data[i] = TrackedElement(dist(rng));
    }
    return data;
}

std::vector<TrackedElement> generate_nearly_sorted(size_t size) {
    std::vector<TrackedElement> data(size);
    for (size_t i = 0; i < size; i++) {
        data[i] = TrackedElement(static_cast<int>(i));
    }

    // Swap ~5% of elements randomly
    std::mt19937 rng(42);
    std::uniform_int_distribution<size_t> dist(0, size > 0 ? size - 1 : 0);
    size_t swaps = size / 20;

    for (size_t i = 0; i < swaps; i++) {
        std::swap(data[dist(rng)], data[dist(rng)]);
    }
    return data;
}

std::vector<TrackedElement> generate_reverse_sorted(size_t size) {
    std::vector<TrackedElement> data(size);
    for (size_t i = 0; i < size; i++) {
        data[i] = TrackedElement(static_cast<int>(size - i));
    }
    return data;
}

std::vector<TrackedElement> generate_duplicate_values(size_t size) {
    std::vector<TrackedElement> data(size);
    std::mt19937 rng(42);
    // 1% unique values relative to size (e.g., [0, 10000] at N = 1,000,000)
    int max_dup_val = std::max(100, static_cast<int>(size / 100));
    std::uniform_int_distribution<int> dist(0, max_dup_val);

    for (size_t i = 0; i < size; i++) {
        data[i] = TrackedElement(dist(rng));
    }
    return data;
}

std::vector<TrackedElement> generate_search_queries(const std::vector<TrackedElement>& data, size_t num_queries) {
    std::vector<TrackedElement> queries(num_queries);
    if (data.empty()) return queries;

    std::mt19937 rng(1337);
    std::uniform_int_distribution<size_t> index_dist(0, data.size() - 1);
    std::uniform_int_distribution<int> miss_dist(0, static_cast<int>(data.size() * 3));

    for (size_t i = 0; i < num_queries; i++) {
        // 80% existing elements, 20% random values (potential misses)
        if (i % 5 != 0) {
            queries[i] = data[index_dist(rng)];
        } else {
            queries[i] = TrackedElement(miss_dist(rng));
        }
    }
    return queries;
}