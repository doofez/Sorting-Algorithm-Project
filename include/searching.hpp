#pragma once

#include <vector>
#include "TrackedElement.hpp"

int linearSearch(const std::vector<TrackedElement>& data, const TrackedElement& target);

int binarySearch(const std::vector<TrackedElement>& data, const TrackedElement& target);

int exponentialSearch(const std::vector<TrackedElement>& data, const TrackedElement& target);