//
// Created by MattFor on 31.03.2025.
//

#include <vector>
#include <utility>
#include <cstdlib>
#include <algorithm>
#include "../include/Value.h"

bool timsort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, const int duration_ms)
{
    static bool                                   initialized = false;
    static std::vector<std::pair<size_t, size_t>> runs;                 // each run is [start, end)
    static size_t                                 merge_index = 0;
    const size_t                                  n = values.size();

    if (n < 2)
    {
        return true;
    }

    // Lambda: insertion sort on the segment [start, end)
    auto insertion_sort_segment = [&](const size_t start, const size_t end) {
        for (size_t i = start + 1; i < end; i++)
        {
            // Read key from values[i]
            size_t j = i;
            arrayAccesses++; // for reading values[i]
            Value key = values[i];

            while (j > start)
            {
                // Retrieve the previous value for comparison
                const int prevVal = values[j - 1].getValue();
                arrayAccesses++; // access for values[j-1]
                const int keyVal = key.getValue();
                arrayAccesses++; // access for a key

                comparisons++;
                if (prevVal <= keyVal)
                {
                    break;
                }

                // Shift element right
                arrayAccesses++;
                values[j] = values[j - 1];

                playTone(values[j].getValue(), duration_ms);
                j--;
            }

            arrayAccesses++;
            values[j] = key;
            playTone(key.getValue(), duration_ms);
        }
    };

    // Lambda: merge two sorted segments [l, m) and [m, r)
    auto merge_segments = [&](const size_t l, const size_t m, const size_t r) {
        std::vector<Value> temp;
        size_t i = l, j = m;

        while (i < m && j < r)
        {
            const int leftVal = values[i].getValue();
            arrayAccesses++; // access for values[i]
            const int rightVal = values[j].getValue();
            arrayAccesses++; // access for values[j]

            comparisons++;
            if (leftVal <= rightVal)
            {
                temp.push_back(values[i]);
                i++;
            }
            else
            {
                temp.push_back(values[j]);
                j++;
            }
        }

        while (i < m)
        {
            temp.push_back(values[i]);
            i++;
        }

        while (j < r)
        {
            temp.push_back(values[j]);
            j++;
        }

        // Copy the merged array back to the original vector
        for (size_t k = 0; k < temp.size(); k++)
        {
            values[l + k] = temp[k];
            playTone(temp[k].getValue(), duration_ms);
        }
    };

    // Initialization: Identify runs and sort them using insertion sort
    if (!initialized)
    {
        size_t i = 0;
        while (i < n)
        {
            size_t run_start = i;
            i++;

            // Identify an ascending run
            while (i < n)
            {
                const int currVal = values[i].getValue();
                arrayAccesses++; // access for values[i]
                const int prevVal = values[i - 1].getValue();
                arrayAccesses++; // access for values[i-1]

                comparisons++;
                if (currVal < prevVal)
                {
                    break;
                }

                i++;
            }

            size_t run_end = i;
            // If run is too short, extend it using insertion sort
            if (constexpr size_t MINRUN = 32; run_end - run_start < MINRUN)
            {
                const size_t end_bound = std::min(n, run_start + MINRUN);
                insertion_sort_segment(run_start, end_bound);
                run_end = end_bound;
                i = run_end;
            }

            runs.emplace_back(run_start, run_end);
        }
        initialized = true;
    }

    // If only one run remains, sorting is complete
    if (runs.size() == 1)
    {
        // Reset static state for potential reuse
        merge_index = 0;
        runs.clear();
        initialized = false;

        return true;
    }

    // Merge two adjacent runs per call
    if (merge_index < runs.size() - 1)
    {
        const size_t l = runs[merge_index].first;
        const size_t m = runs[merge_index].second;
        const size_t r = runs[merge_index + 1].second;
        merge_segments(l, m, r);
        // Update the run to cover the merged segment
        runs[merge_index].second = r;
        runs.erase(runs.begin() + static_cast<long long>(merge_index) + 1);
        merge_index++;
    }
    else
    {
        merge_index = 0; // Restart merge pass
    }

    return false;
}
