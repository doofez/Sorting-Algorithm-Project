//custom struct for holding variables used to be able to track the # of comparisons in algorithms

#pragma once

struct TrackedElement {
    int value;

    //counter
    static unsigned long long comparison_count;

    //constructor
    TrackedElement(int val = 0) : value(val) {}

    //overloading comparison operators so that they perform normally with the addition of incrementing comparison_count

    bool operator<(const TrackedElement& other) const {
        comparison_count++;
        return this->value < other.value;
    }

    bool operator<=(const TrackedElement& other) const {
        comparison_count++;
        return this->value <= other.value;
    }

    bool operator>(const TrackedElement& other) const {
        comparison_count++;
        return this->value > other.value;
    }

    bool operator>=(const TrackedElement& other) const {
        comparison_count++;
        return this->value >= other.value;
    }

    bool operator==(const TrackedElement& other) const {
        comparison_count++;
        return this->value == other.value;
    }

    bool operator!=(const TrackedElement& other) const {
        comparison_count++;
        return this->value != other.value;
    }
};