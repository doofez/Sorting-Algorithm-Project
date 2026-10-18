#pragma once

#include <string>
#include <vector>
#include "benchmark.hpp"
#include "TrackedElement.hpp"

void export_sorting_to_csv(const std::string& filename, const std::vector<BenchmarkResult>& results);

void export_searching_to_csv(const std::string& filename, const std::vector<SearchBenchmarkResult>& results);

bool verify_sorted(const std::vector<TrackedElement>& data);