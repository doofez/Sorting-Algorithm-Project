#include "benchmark.hpp"
#include <chrono>
#include <cstdlib>
#include <new>

// Global memory tracking variables
static size_t current_heap_memory = 0;
static size_t peak_heap_memory = 0;

// Override global 'new' to intercept all heap allocations (e.g., std::vector)
void* operator new(size_t size) {
    // Allocate 16 extra bytes to store the size while maintaining 16-byte alignment
    void* base_ptr = std::malloc(size + 16);
    if (!base_ptr) throw std::bad_alloc();

    // Store the requested size in the first 8 bytes
    *static_cast<size_t*>(base_ptr) = size;

    current_heap_memory += size;
    if (current_heap_memory > peak_heap_memory) {
        peak_heap_memory = current_heap_memory;
    }

    // Return the pointer shifted past our 16-byte tracking header
    return static_cast<char*>(base_ptr) + 16;
}

// Override global 'delete' to track memory being freed
void operator delete(void* ptr) noexcept {
    if (!ptr) return;
    
    // Shift back 16 bytes to find the original pointer and the allocation size
    void* base_ptr = static_cast<char*>(ptr) - 16;
    size_t size = *static_cast<size_t*>(base_ptr);

    current_heap_memory -= size;
    std::free(base_ptr);
}

// Forward sized delete to the unsized delete above
void operator delete(void* ptr, size_t /*size*/) noexcept {
    ::operator delete(ptr);
}

// Intercept array allocations just in case
void* operator new[](size_t size) { return ::operator new(size); }
void operator delete[](void* ptr) noexcept { ::operator delete(ptr); }
void operator delete[](void* ptr, size_t) noexcept { ::operator delete(ptr); }


// Define the static comparison counter declared in TrackedElement.hpp
unsigned long long TrackedElement::comparison_count = 0;

BenchmarkResult run_sorting_benchmark(
    const std::string& algorithm_name,
    const std::string& dataset_type,
    std::vector<TrackedElement>& data,
    void (*sort_function)(std::vector<TrackedElement>&)
) {
    TrackedElement::comparison_count = 0;
    
    // Reset the peak tracker right before the algorithm runs
    peak_heap_memory = current_heap_memory; 
    size_t mem_before = current_heap_memory;

    auto start_time = std::chrono::high_resolution_clock::now();
    sort_function(data);
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate peak overhead used exclusively BY THIS ALGORITHM
    size_t peak_overhead_bytes = peak_heap_memory - mem_before;
    size_t mem_used_kb = peak_overhead_bytes / 1024;

    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;

    BenchmarkResult result;
    result.algorithm_name = algorithm_name;
    result.dataset_type = dataset_type;
    result.dataset_size = data.size();
    result.execution_time_ms = elapsed.count();
    result.memory_usage_kb = mem_used_kb;
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
    
    peak_heap_memory = current_heap_memory;
    size_t mem_before = current_heap_memory;

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

    size_t peak_overhead_bytes = peak_heap_memory - mem_before;
    size_t mem_used_kb = peak_overhead_bytes / 1024;

    SearchBenchmarkResult result;
    result.algorithm_name = algorithm_name;
    result.average_search_time_ms = queries.empty() ? 0.0 : (total_time_ms / queries.size());
    result.worst_case_time_ms = worst_time_ms;
    result.memory_usage_kb = mem_used_kb;

    return result;
}