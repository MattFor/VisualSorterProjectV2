//
// Created by MattFor on 31.03.2025.
//

#include "../include/Value.h"

bool heap_sort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, const int duration_ms)
{
    // Static state for incremental sorting
    static bool buildingHeap = true;
    static int  heapSize     = static_cast<int>(values.size());
    static int  i            = heapSize / 2 - 1; // Last non-leaf index

    // Sift down the element at index j until the subtree is a valid heap
    auto heapify_full = [&](const int n, int j) {
        while (true)
        {
            int largest = j;
            const int left = 2 * j + 1;
            const int right = 2 * j + 2;

            // Check left child
            if (left < n)
            {
                const int leftVal = values[left].getValue();
                arrayAccesses++; // Count access for left child
                const int largestVal = values[largest].getValue();
                arrayAccesses++; // Count access for current largest

                comparisons++;
                if (leftVal > largestVal)
                {
                    largest = left;
                }
            }

            // Check right child
            if (right < n)
            {
                const int rightVal = values[right].getValue();
                arrayAccesses++; // Count access for right child
                const int largestVal = values[largest].getValue();
                arrayAccesses++; // Count access for current largest

                comparisons++;
                if (rightVal > largestVal)
                {
                    largest = right;
                }
            }

            if (largest == j)
            {
                break; // Heap property satisfied
            }

            std::swap(values[j], values[largest]);
            arrayAccesses += 2; // Increment for the two element accesses in the swap

            playTone(values[j].getValue(), duration_ms);

            j = largest; // Continue sifting down the element
        }
    };

    // Building phase: Build the heap
    if (buildingHeap)
    {
        if (i >= 0)
        {
            heapify_full(heapSize, i);  // Fully heapify the subtree at index i
            i--;

            return false;
        }

        // Finished building the heap
        buildingHeap = false;
        return false;
    }

    // Extraction phase: Remove the max element (at index 0) and reheapify
    if (heapSize > 1)
    {
        std::swap(values[0], values[heapSize - 1]);
        arrayAccesses += 2; // Increment for swap accesses
        playTone(values[0].getValue(), duration_ms);
        heapSize--;

        // Fully reheapify the new root
        heapify_full(heapSize, 0);
        return false;
    }

    // Sorting complete. Reset static state for future runs
    buildingHeap = true;
    heapSize     = static_cast<int>(values.size());
    i            = heapSize / 2 - 1;

    return true;
}
