
#pragma once
#include <vector>
#include "TrackedElement.hpp"

void twinArraySort(std::vector<TrackedElement>& data);

void clusterSort(std::vector<TrackedElement>& data, bool use_shell);

//wrapper functions to be passed into the benchmark runner
void clusterSortComb(std::vector<TrackedElement>& data);
void clusterSortShell(std::vector<TrackedElement>& data);

void qrSort(std::vector<TrackedElement>& data);