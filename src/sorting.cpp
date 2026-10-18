#include <algorithm>
#include<cmath>
#include "sorting.hpp"
#include "TrackedElement.hpp"

void twinArraySort(std::vector<TrackedElement>& data){

    TrackedElement max_element = *std::max_element(data.begin(), data.end());
    int max_value = max_element.value;
    std::vector<TrackedElement> value_store(max_value + 1);
    std::vector<int> count_store(max_value + 1, 0);

    bool has_duplicates = false;

    for( TrackedElement num : data ){
        value_store[num.value] = num;
        count_store[num.value] += 1;

        if( count_store[num.value] > 1){
            has_duplicates = true;
        }
    }

    int idxCounter = 0;

    if(!has_duplicates){
        for(int i = 0; i <= max_value; i++){
            if (count_store[i] == 1){
                data[idxCounter] = value_store[i];
                idxCounter++;
            }
        }
    } else {
        for(int i = 0; i <= max_value; i++){
            while(count_store[i] >= 1){
                data[idxCounter] = value_store[i];
                count_store[i] -= 1;
                idxCounter++;
            }
        }
    }
};

static void combSort(std::vector<TrackedElement>& data, int start, int end) {
    int n = end - start;
    if(n <= 1) return;

    int gap = n;
    bool swapped = true;

    while(gap > 1 || swapped) {
        gap = (gap * 10) / 13;
        if (gap < 1) gap = 1;

        swapped = false;
        for(int i = start; i < end - gap; i++){
            if(data[i] > data[i + gap]) {
                std::swap(data[i], data[i + gap]);
                swapped = true;
            }
        }
    }
}

static void shellSort(std::vector<TrackedElement>& data, int start, int end) {
    int n = end - start;
    if (n <= 1) return;

    for(int gap = n / 2; gap > 0; gap /= 2) {
        for(int i = start + gap; i < end; i++) {
            TrackedElement temp = data[i];
            int j = i;

            while(j >= start + gap && data[j - gap] > temp){
                data[j] = data[j - gap];
                j -= gap;
            }

            data[j] = temp;
        }
    }
}

void clusterSort(std::vector<TrackedElement>& data, bool use_shell){

    size_t n = data.size();
    if (n <= 1) return;

    auto [min_it, max_it] = std::minmax_element(data.begin(), data.end());

    int min_value = min_it->value;
    int max_value = max_it->value;

    if(min_value == max_value) return;
    
    int num_clusters = static_cast<int>(std::sqrt(n));
    double cluster_size = static_cast<double>(max_value - min_value) / num_clusters;

    //how many elements are in each cluster
    std::vector<int> cluster_counts(num_clusters, 0);
    for(const TrackedElement& elem : data){
        //find the cluster index for the element
        int cluster_index = std::min(static_cast<int>((elem.value - min_value) / cluster_size), num_clusters - 1);
        cluster_counts[cluster_index]++;
    }

    //determine start index for each cluster
    std::vector<int> cluster_starts(num_clusters + 1, 0);
    for(int i = 0; i < num_clusters; i++) {
        cluster_starts[i + 1] = cluster_starts[i] + cluster_counts[i];
    }

    std::vector<TrackedElement> buffer(n);
    std::vector<int> write_heads = cluster_starts;
    for(const TrackedElement& elem : data) {
        int cluster_index = std::min(static_cast<int>((elem.value - min_value) / cluster_size), num_clusters - 1);
        buffer[write_heads[cluster_index]++] = elem;
    }
    //move the clustered data from the buffer into the data
    data = std::move(buffer);

    for(int i = 0; i < num_clusters; i++) {
        int start = cluster_starts[i];
        int end = cluster_starts[i + 1];

        if(end - start > 1) {
            if(use_shell){
                shellSort(data, start, end);
            } else {
                combSort(data, start, end);
            }
        }
    }
}

void clusterSortComb(std::vector<TrackedElement>& data) {
    clusterSort(data, false);
}

void clusterSortShell(std::vector<TrackedElement>& data) {
    clusterSort(data, true);
}