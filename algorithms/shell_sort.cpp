//
// Created by MattFor on 31.03.2025.
//

#include "../include/Value.h"

bool shell_sort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, const int duration_ms)
{
    static int i = 0;
    static int j = 0;
    static int gap = 0;
    static bool initialized = false;
    static bool inInnerLoop = false;

    if (!initialized)
    {
        gap = static_cast<int>(values.size()) / 2;
        i = gap;
        inInnerLoop = false;
        initialized = true;
    }

    // Sorting done, prepare for next sort
    if (gap <= 0)
    {
        initialized = false;
        return true;
    }

    if (i < values.size())
    {
        if (!inInnerLoop)
        {
            j = i;
            inInnerLoop = true;
        }

        // Continue shifting the element at position j back if necessary
        if (j >= gap)
        {
            const int leftVal = values[j - gap].getValue();
            arrayAccesses++; // Count access for values[j-gap]
            const int currentVal = values[j].getValue();
            arrayAccesses++; // Count access for values[j]

            comparisons++;
            if (leftVal > currentVal)
            {
                std::swap(values[j - gap], values[j]);
                arrayAccesses += 2; // Count two accesses for the swap
                playTone(values[j].getValue(), duration_ms);
                j -= gap;
                return false;  // Continue inner loop in the next step
            }
        }

        // Finished shifting element at i and moving to the next element
        i++;
        inInnerLoop = false;

        return false;
    }

    // Finished one pass, reducing the gap
    gap /= 2;
    i = gap;
    inInnerLoop = false;

    return false;
}
