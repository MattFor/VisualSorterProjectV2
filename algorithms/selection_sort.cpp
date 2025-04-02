//
// Created by MattFor on 31.03.2025.
//

#include "../include/Value.h"

bool selection_sort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, const int duration_ms)
{
    if (values.size() < 2)
    {
        return true;
    }

    // lastSize is used to detect when a new vector (or new sort) is used
    static size_t i = 0;
    static size_t j = 1;
    static size_t minIndex = 0;
    static size_t lastSize = 0;

    // Reinitialize state if the vector size has changed
    if (values.size() != lastSize)
    {
        i = 0;
        j = 1;
        minIndex = 0;
        lastSize = values.size();
    }

    // When we reach the last element, the sort is done
    if (i >= values.size() - 1)
    {
        // Reset state for future sorts
        i = 0;
        j = 1;
        minIndex = 0;

        return true;
    }

    // Traverse the unsorted portion, searching for the minimum element
    if (j < values.size())
    {
        // Retrieve values for comparison and count accesses
        const int currentVal = values[j].getValue();
        arrayAccesses++; // Access for values[j]
        const int minVal = values[minIndex].getValue();
        arrayAccesses++; // Access for values[minIndex]

        comparisons++;
        if (currentVal < minVal)
        {
            minIndex = j;
        }

        j++;
        return false;
    }

    // End of inner loop: swap the smallest element with element at index i
    if (minIndex != i)
    {
        std::swap(values[i], values[minIndex]);
        arrayAccesses += 2;
        playTone(values[i].getValue(), duration_ms);
    }

    i++;
    j = i + 1;
    minIndex = i;

    return false;
}
