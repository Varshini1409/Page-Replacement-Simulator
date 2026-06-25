#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>

struct Result {
    int hits;
    int faults;
};

Result FIFO(std::vector<int>& pages, int capacity, bool showSteps = true);

Result LRU(std::vector<int>& pages, int capacity, bool showSteps = true);

Result Optimal(std::vector<int>& pages, int capacity, bool showSteps = true);

void compareAll(std::vector<int>& pages, int frames);

void beladyDemo();

#endif