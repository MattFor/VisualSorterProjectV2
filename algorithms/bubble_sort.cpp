//
// Created by MattFor on 31.03.2025.
//

#include "../include/Value.h"

bool bubble_sort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, const int duration_ms)
{
    // If there are fewer than two elements, it's already sorted
    if (values.size() < 2)
    {
        return true;
    }

    // Static variables to maintain state between calls
    static size_t lastSize = 0;
    static size_t i = 0;
    static size_t n = 0;
    static bool swapped = false;

    // If the vector size has changed (or this is the first call) reinitialize state
    if (values.size() != lastSize)
    {
        lastSize = values.size();
        n = values.size();
        i = 0;
        swapped = false;
    }

    // If only one element remains in the unsorted portion, sorting is complete
    if (n <= 1)
    {
        // Reset state for future sorts
        i = 0;
        n = 0;
        lastSize = 0;
        swapped = false;

        return true;
    }

    // Check if we've reached the end of the current pass
    if (i >= n - 1)
    {
        if (!swapped)
        {
            // No swaps in this pass mean the array is sorted
            // Reset state for future runs
            i = 0;
            n = 0;
            lastSize = 0;
            swapped = false;

            return true;
        }

        // Prepare for the next pass
        n--;        // The last element is now in its final position
        i = 0;      // Reset index to start new pass
        swapped = false;

        return false;
    }

    // Count array accesses for the two elements being compared
    const int left = values[i].getValue();
    arrayAccesses++; // Access for values[i]
    const int right = values[i + 1].getValue();
    arrayAccesses++; // Access for values[i+1]

    // Count the comparison between the two elements
    comparisons++;
    if (left > right)
    {
        std::swap(values[i], values[i + 1]);
        arrayAccesses += 2;

        // Count access when retrieving the updated value for the tone
        const int newLeft = values[i].getValue();
        // arrayAccesses++;
        playTone(newLeft, duration_ms);
        swapped = true;
    }

    i++;
    return false; // Sorting is still in progress
}
