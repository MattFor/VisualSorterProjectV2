//
// Created by MattFor on 31.03.2025.
//

#include <vector>
#include <utility>
#include <algorithm>

#include "../include/Value.h"

bool quicksort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, const int duration_ms)
{
    static int i = 0, j = 0;
    static float pivot = 0.0f;
    static int low = 0, high = 0;
    static bool initialized = false;
    static bool partitioning = false;
    static std::vector<std::pair<int, int>> stack;

    if (!initialized)
    {
        stack.clear();
        stack.emplace_back(0, static_cast<int>(values.size()) - 1);
        initialized = true;
    }

    // Process until all partitions are sorted
    if (!stack.empty() || partitioning)
    {
        if (!partitioning)
        {
            // Pop the next subarray
            auto [l, r] = stack.back();
            stack.pop_back();
            low = l;
            high = r;

            if (low >= high)
            {
                return false; // This segment is already sorted
            }

            // Check if the current segment is already sorted
            bool sortedSegment = true;
            for (int k = low; k < high; k++)
            {
                const int leftVal = values[k].getValue();
                arrayAccesses++; // access for values[k]
                const int rightVal = values[k + 1].getValue();
                arrayAccesses++; // access for values[k+1]

                comparisons++;
                if (leftVal > rightVal)
                {
                    sortedSegment = false;
                    break;
                }
            }
            if (sortedSegment)
            {
                return false;
            }

            // Use median-of-three pivot selection
            const int mid = low + (high - low) / 2;
            const auto a = static_cast<float>(values[low].getValue());
            arrayAccesses++; // access for values[low]
            const auto b = static_cast<float>(values[mid].getValue());
            arrayAccesses++; // access for values[mid]
            const auto c = static_cast<float>(values[high].getValue());
            arrayAccesses++; // access for values[high]

            float med;
            // Determine the median.
            comparisons++; // for (a <= b)
            comparisons++; // for (b <= c)
            if ((a <= b && b <= c) || (c <= b && b <= a))
            {
                med = b;
            }
            else
            {
                comparisons++; // for (b <= a)
                comparisons++; // for (a <= c)
                if ((b <= a && a <= c) || (c <= a && a <= b))
                {
                    med = a;
                }
                else
                {
                    med = c;
                }
            }

            // Identify the pivot index
            comparisons++; // for (med == a)
            comparisons++; // for (med == b) in the ternary operation
            const int pivotIndex = med == a ? low : med == b ? mid : high;

            // Move pivot to end
            std::swap(values[pivotIndex], values[high]);
            arrayAccesses += 2; // Count swap accesses
            // Retrieve pivot value
            pivot = static_cast<float>(values[high].getValue());
            arrayAccesses++; // access for pivot

            i = low;
            j = low;
            partitioning = true;
        }

        // Partitioning: Process one element per call
        if (j < high)
        {
            const int currentVal = values[j].getValue();
            arrayAccesses++; // access for values[j]

            comparisons++;   // for currentVal < pivot comparison
            if (static_cast<float>(currentVal) < pivot)
            {
                std::swap(values[i], values[j]);
                arrayAccesses += 2; // Count swap accesses.
                playTone(values[i].getValue(), duration_ms);
                i++;
            }

            j++;
            return false;
        }

        // Finalize partition: place pivot in its correct position
        std::swap(values[i], values[high]);
        arrayAccesses += 2; // Count swap accesses
        playTone(values[i].getValue(), duration_ms);
        partitioning = false;

        // Push left subarray
        comparisons++; // for low < i - 1
        if (low < i - 1)
        {
            stack.emplace_back(low, i - 1);
        }

        // Push right subarray
        comparisons++; // for i + 1 < high
        if (i + 1 < high)
        {
            stack.emplace_back(i + 1, high);
        }

        return false;
    }

    // Reset static state for future runs
    initialized = false;
    return true;
}
