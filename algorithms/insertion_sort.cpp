//
// Created by MattFor on 31.03.2025.
//

#include "../include/Value.h"

bool insertion_sort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, const int duration_ms)
{
    // If there are fewer than two elements, nothing to sort
    if (values.size() < 2)
    {
        return true;
    }

    // Static state variables to persist between calls
    static size_t i = 1; // current index in outer loop
    static size_t j = 1; // current index in inner loop
    static size_t lastSize = 0;

    // Reinitialize state if the vector size changes
    if (values.size() != lastSize)
    {
        i = 1;
        j = 1;
        lastSize = values.size();
    }

    // If the outer loop has finished, sorting is complete
    if (i >= values.size())
    {
        // Reset state for the next run
        i = 1;
        j = 1;
        lastSize = 0;

        return true;
    }

    // If we are in the middle of inserting element i
    if (j > 0)
    {
        // Read the two adjacent elements and count the accesses
        const int currentVal = values[j].getValue();
        arrayAccesses++; // Count access for values[j]
        const int prevVal = values[j - 1].getValue();
        arrayAccesses++; // Count access for values[j - 1]

        comparisons++;
        if (currentVal < prevVal)
        {
            std::swap(values[j - 1], values[j]);
            arrayAccesses += 2; // Count the two accesses in the swap
            playTone(values[j].getValue(), duration_ms);
            j--;  // Continue shifting the element left

            return false;
        }
    }

    // No more shifting needed for an element I
    // Move to the next element
    i++;
    j = i;  // Reset inner loop pointer for new element

    return false;
}
