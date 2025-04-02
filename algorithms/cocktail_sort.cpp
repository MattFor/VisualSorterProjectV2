//
// Created by MattFor on 31.03.2025.
//

#include "../include/Value.h"

bool cocktail_sort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, const int duration_ms)
{
    // Already sorted
    if (values.size() < 2)
    {
        return true;
    }

    static size_t i = 0;
    static size_t end = 0;
    static size_t start = 0;
    static size_t lastSize = 0;

    static bool forward = true;
    static bool swapped = false;

    // If the size has changed (or this is the first call) reinitialize the state
    if (values.size() != lastSize)
    {
        i = 0;
        start = 0;
        forward = true;
        swapped = false;
        end = values.size() - 1;
        lastSize = values.size();
    }

    // Forward pass: iterate from left to right
    if (forward)
    {
        if (i < end)
        {
            // Read two adjacent elements and count the accesses
            const int left = values[i].getValue();
            arrayAccesses++; // Access for values[i]
            const int right = values[i + 1].getValue();
            arrayAccesses++; // Access for values[i+1]

            comparisons++;
            if (left > right)
            {
                std::swap(values[i], values[i + 1]);
                arrayAccesses += 2;

                // Read the swapped element for tone and count access
                const int newLeft = values[i].getValue();
                // arrayAccesses++;
                playTone(newLeft, duration_ms);

                swapped = true;
            }

            i++;
        }

        if (i >= end)
        {
            forward = false;
            i = end; // Prepare for backward pass
        }
    }
    else // Backward pass: iterate from right to left
    {
        if (i > start)
        {
            // Read the two adjacent elements and count the accesses
            const int left = values[i - 1].getValue();
            arrayAccesses++; // Access for values[i-1]
            const int right = values[i].getValue();
            arrayAccesses++; // Access for values[i]

            comparisons++;
            if (left > right)
            {
                std::swap(values[i - 1], values[i]);
                arrayAccesses += 2;

                // Read the swapped element for tone and count access.
                const int newRight = values[i].getValue();
                arrayAccesses++;
                playTone(newRight, duration_ms);

                swapped = true;
            }

            i--;
        }

        if (i <= start)
        {
            forward = true;
            start++;

            // If no swaps were made in the full pass, the array is sorted
            if (!swapped)
            {
                // Reset the static state for the next sort
                i = 0;
                start = 0;
                forward = true;
                swapped = false;
                end = values.size() - 1;

                return true;
            }

            swapped = false;
        }
    }

    // If the unsorted section has been fully processed, sorting is complete
    if (start >= end)
    {
        return true;
    }

    return false; // Sorting is still in progress
}
