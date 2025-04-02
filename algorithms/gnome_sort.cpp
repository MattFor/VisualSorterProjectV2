//
// Created by MattFor on 31.03.2025.
//

#include "../include/Value.h"

bool gnome_sort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, const int duration_ms)
{
    static size_t index = 0;

    if (values.size() < 2)
    {
        return true;
    }

    // Start from index 1 if at the beginning
    if (index == 0)
    {
        index = 1;
    }

    if (index < values.size())
    {
        // Retrieve the two adjacent elements and count each access
        const int left = values[index - 1].getValue();
        arrayAccesses++; // Access for values[index - 1]
        const int right = values[index].getValue();
        arrayAccesses++; // Access for values[index]

        comparisons++;
        if (left <= right)
        {
            index++;
        }
        else
        {
            std::swap(values[index - 1], values[index]);
            arrayAccesses += 2;

            playTone(values[index].getValue(), duration_ms);

            if (index > 0)
            {
                index--;
            }
        }
    }

    // If the index has reached or exceeded the vector size, reset for the next pass
    if (index >= values.size())
    {
        index = 0;
        return true;
    }

    return false;
}
