//
// Created by MattFor on 31.03.2025.
//

#ifndef SORTING_ALGORITHMS_H
#define SORTING_ALGORITHMS_H

#include "../include/Value.h"

bool randomize_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, int duration_ms);

bool timsort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, int duration_ms);
bool quicksort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, int duration_ms);
bool heap_sort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, int duration_ms);
bool bogo_sort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, int duration_ms);
bool spreadsort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, int duration_ms);
bool shell_sort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, int duration_ms);
bool gnome_sort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, int duration_ms);
bool bubble_sort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, int duration_ms);
bool cocktail_sort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, int duration_ms);
bool insertion_sort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, int duration_ms);
bool selection_sort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, int duration_ms);
bool radix_lsd_bucket_sort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, int duration_ms);


#endif //SORTING_ALGORITHMS_H
