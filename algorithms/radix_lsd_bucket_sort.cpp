//
// Created by MattFor on 31.03.2025.
//

#include <limits>
#include <vector>

#include "../include/Value.h"

bool radix_lsd_bucket_sort_step(std::vector<Value>& values, int &arrayAccesses, int &_, const int duration_ms)
{
    static int exp = 1;       // 1, 10, 100...
    static int maxKey = 0;    // Maximum key after shifting
    static int offset = 0;    // Offset to shift keys if negatives exist
    static bool initialized = false;

    if (!initialized)
    {
        // Compute the minimum and maximum values among all elements
        int minVal = std::numeric_limits<int>::max();
        int maxVal = std::numeric_limits<int>::min();
        for (auto& v : values)
        {
            const int x = v.getValue();
            arrayAccesses++;

            if (x < minVal)
            {
                minVal = x;
            }

            if (x > maxVal)
            {
                maxVal = x;
            }
        }

        // Set offset if negatives exist
        offset = minVal < 0 ? minVal : 0;
        // The maximum key (after shifting) is maxVal - offset
        exp = 1;
        initialized = true;
        maxKey = maxVal - offset;
    }

    // Process each digit (as long as there's a significant digit left)
    if (maxKey / exp > 0)
    {
        // Create bucket counts for digits 0 through 9
        std::vector bucket(10, 0);
        for (auto& v : values)
        {
            const int x = v.getValue();
            arrayAccesses++;
            const int key = x - offset;  // Shift the key
            const int digit = key / exp % 10;
            bucket[digit]++;
        }

        // Convert counts to positions
        for (int i = 1; i < 10; i++)
        {
            bucket[i] += bucket[i - 1];
        }

        // Build an output array
        std::vector<Value> output(values.size());
        for (int i = static_cast<int>(values.size()) - 1; i >= 0; i--)
        {
            const int x = values[i].getValue();
            arrayAccesses++; // Access for values[i].getValue()
            const int key = x - offset;
            const int digit = key / exp % 10;
            output[bucket[digit] - 1] = values[i];
            bucket[digit]--;
        }

        values = output;
        playTone(exp, duration_ms);
        exp *= 10;
        return false;
    }

    // Reset the static state for the next run.
    initialized = false;
    return true;
}
