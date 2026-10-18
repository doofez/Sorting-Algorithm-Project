#include "searching.hpp"
#include <algorithm>

int linearSearch(const std::vector<TrackedElement>& data, const TrackedElement& target) {
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i] == target) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

static int binarySearchRange(const std::vector<TrackedElement>& data, const TrackedElement& target, int low, int high) {
    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (data[mid] == target) {
            return mid;
        } else if (data[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

//wrapper to be able to get passed to run_search_benchmark
int binarySearch(const std::vector<TrackedElement>& data, const TrackedElement& target) {
    if (data.empty()) return -1;
    return binarySearchRange(data, target, 0, static_cast<int>(data.size()) - 1);
}

int exponentialSearch(const std::vector<TrackedElement>& data, const TrackedElement& target) {
    size_t n = data.size();
    if (n == 0) return -1;

    if (data[0] == target) {
        return 0;
    }

    size_t bound = 1;
    while (bound < n && data[bound] < target) {
        bound *= 2;
    }

    int low = static_cast<int>(bound / 2);
    int high = static_cast<int>(std::min(bound, n - 1));

    return binarySearchRange(data, target, low, high);
}