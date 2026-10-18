#pragma once

#include <vector>
#include<cstddef>
#include "TrackedElement.hpp"

std::vector<TrackedElement> generate_random(size_t size);

std::vector<TrackedElement> generate_nearly_sorted(size_t size);

std::vector<TrackedElement> generate_reverse_sorted(size_t size);

std::vector<TrackedElement> generate_duplicate_values(size_t size);

std::vector<TrackedElement> generate_search_queries(const std::vector<TrackedElement>& data, size_t num_queries);