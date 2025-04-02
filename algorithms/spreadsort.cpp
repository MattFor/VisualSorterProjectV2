//
// Created by MattFor on 31.03.2025.
//

#include <limits.h>

#include "../include/Value.h"


// Spreadsort step function (simplified bucket-based approach
// This version distributes values into buckets (based on a fixed bucket count)
// sorts each bucket incrementally using insertion sort,
// and then merges buckets back into the original vector
// It uses several phases managed by a static state
bool spreadsort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, const int duration_ms)
{
    enum Phase { INIT, DISTRIBUTE, SORT_BUCKETS, MERGE, RESET, RESET_FINAL };
    static int global_min = 0;
    static int global_max = 0;
    static Phase phase = INIT;
    static constexpr int bucket_count = 10;
    static std::vector<std::vector<Value>> buckets;
    static size_t distribution_index = 0;       // Index for distributing values into buckets
    static int current_bucket = 0;              // Current bucket index for sorting
                                                // For insertion sort within a bucket
                                                // bucket_sort_index is the “outer” index (i) for which we are inserting the element
                                                // innerIndex (j) is the position being examined for a potential swap
    static size_t bucket_sort_index = 1;
    static size_t innerIndex = 1;
    static size_t merge_bucket_index = 0;       // For merging buckets back

    const size_t n = values.size();
    if (n < 2)
    {
        phase = RESET;
        return true;
    }

    // ----- INIT Phase: Compute global min and max -----
    if (phase == INIT)
    {
        const int firstVal = values[0].getValue();
        int previousValue = firstVal;
        bool hasEncounteredUnsorted = false;
        arrayAccesses++;  // access for values[0]
        global_min = firstVal;
        global_max = firstVal;
        for (size_t i = 1; i < n; i++)
        {
            const int v = values[i].getValue();
            arrayAccesses++;  // access for values[i]

            comparisons++;
            if (previousValue > v)
            {
                hasEncounteredUnsorted = true;
            }

            comparisons++;    // compare v with global_min
            if (v < global_min)
            {
                global_min = v;
            }

            comparisons++;    // compare v with global_max
            if (v > global_max)
            {
                global_max = v;
            }

            previousValue = v;
        }

        if (!hasEncounteredUnsorted)
        {
            phase = RESET_FINAL;
            return false;
        }

        buckets.clear();
        buckets.resize(bucket_count);
        distribution_index = 0;
        phase = DISTRIBUTE;
        return false;
    }

    // ----- DISTRIBUTE Phase: Place each value into a bucket -----
    if (phase == DISTRIBUTE)
    {
        if (distribution_index < n)
        {
            const int v = values[distribution_index].getValue();
            arrayAccesses++;  // access for values[distribution_index]

            int bucket_idx = 0;
            if (global_max != global_min)
            {
                bucket_idx = (v - global_min) * bucket_count / (global_max - global_min + 1);
                comparisons++; // for bounds-checking
                if(bucket_idx >= bucket_count)
                    bucket_idx = bucket_count - 1;
            }

            buckets[bucket_idx].push_back(values[distribution_index]);
            distribution_index++;
            return false;
        }
        // Distribution completes: Prepare to sort each bucket.
        current_bucket = 0;
        bucket_sort_index = 1;   // start from the second element in each bucket
        innerIndex = bucket_sort_index;
        phase = SORT_BUCKETS;
        return false;
    }

    // ----- SORT_BUCKETS Phase: Incremental insertion sort in each bucket -----
    if (phase == SORT_BUCKETS)
    {
        // Process buckets one by one.
        while (current_bucket < static_cast<int>(buckets.size()))
        {
            std::vector<Value>& bucket = buckets[current_bucket];

            // If bucket has fewer than 2 elements, it is sorted
            if (bucket.size() < 2)
            {
                innerIndex = 1;
                current_bucket++;
                bucket_sort_index = 1;
                continue;
            }

            // Continue processing the current bucket if:
            // (a) We haven't begun insertion for the last element (bucket_sort_index < size),
            // or (b) The last element is still shifting (innerIndex < bucket_sort_index)
            if (bucket_sort_index < bucket.size() || innerIndex < bucket_sort_index)
            {
                // If in the middle of shifting for the current element, try to shift it further
                if (innerIndex < bucket_sort_index)
                {
                    // Compare the element at innerIndex with its predecessor
                    if (innerIndex > 0)
                    {
                        const int leftVal = bucket[innerIndex - 1].getValue();
                        arrayAccesses++; // read the left element
                        const int curVal = bucket[innerIndex].getValue();
                        arrayAccesses++; // read current element

                        comparisons++;
                        if( leftVal > curVal)
                        {
                            std::swap(bucket[innerIndex - 1], bucket[innerIndex]);
                            arrayAccesses += 2;
                            playTone(bucket[innerIndex].getValue(), duration_ms);
                            innerIndex--; // continue shifting this element left

                            return false;
                        }

                        // No more shifting needed
                        innerIndex = bucket_sort_index;
                    }
                }

                // If not in the middle of shifting, begin insertion for the element at bucket_sort_index
                if (bucket_sort_index < bucket.size() && innerIndex == bucket_sort_index)
                {
                    if (innerIndex > 0)
                    {
                        const int leftVal = bucket[innerIndex - 1].getValue();
                        arrayAccesses++;
                        const int curVal = bucket[innerIndex].getValue();
                        arrayAccesses++;

                        comparisons++;
                        if (leftVal > curVal)
                        {
                            std::swap(bucket[innerIndex - 1], bucket[innerIndex]);
                            arrayAccesses += 2;
                            playTone(bucket[innerIndex].getValue(), duration_ms);
                            innerIndex--; // continue shifting
                            return false;
                        }
                    }

                    // If no swap needed, the element is in place
                    bucket_sort_index++;
                    innerIndex = bucket_sort_index;

                    return false;
                }

                // (If bucket_sort_index == bucket.size() but innerIndex equals bucket_sort_index,
                // it means the last element is fully in place)
            }

            // Finished processing the current bucket moving to next
            innerIndex = 1;
            current_bucket++;
            bucket_sort_index = 1;
        }

        // All buckets processed move to merge phase
        phase = MERGE;
        return false;
    }

    // Merge sorted buckets back into the original array
    if (phase == MERGE)
    {
        if (merge_bucket_index < buckets.size())
        {
            size_t index = 0;
            for (int i = 0; i < static_cast<int>(merge_bucket_index); i++)
            {
                index += buckets[i].size();
            }

            const std::vector<Value>& bucket = buckets[merge_bucket_index];
            for (size_t i = 0; i < bucket.size(); i++)
            {
                values[index + i] = bucket[i];
                arrayAccesses++;
                arrayAccesses++;
                playTone(values[index + i].getValue(), duration_ms);
            }

            merge_bucket_index++;
            return false;
        }

        // Finished merging, move to RESET.
        phase = RESET;

        return false;
    }

    // Reset state for the next run
    if (phase == RESET || phase == RESET_FINAL)
    {
        innerIndex = 1;
        buckets.clear();
        current_bucket = 0;
        bucket_sort_index = 1;
        distribution_index = 0;
        merge_bucket_index = 0;
        if (phase == RESET_FINAL)
        {
            phase = INIT;
            return true;
        } else
        {
            phase = INIT;
            return false;
        }
    }

    return false;
}
