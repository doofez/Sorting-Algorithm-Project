#include <algorithm>
#include "sorting.hpp"
#include "TrackedElement.hpp"

void twinArraySort(std::vector<TrackedElement>& data){

    TrackedElement max_element = *std::max_element(data.begin(), data.end());
    int max_value = max_element.value;
    std::vector<TrackedElement> value_store(max_value + 1);
    std::vector<int> count_store(max_value + 1);

    bool has_duplicates = false;

    for( TrackedElement num : data ){
        value_store[num.value] = num;
        count_store[num.value] += 1;

        if( count_store[num.value] > 1){
            has_duplicates = true;
        }
    }

    if(!has_duplicates){
        for(int i = 0; i <= max_value; i++){
            if(count_store[0] >= 1){
                
            }
        }
    }
};
