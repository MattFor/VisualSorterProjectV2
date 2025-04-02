//
// Created by MattFor on 31.03.2025.
//

#include <random>
#include <algorithm>

#include "../include/Value.h"

bool isSorted(const std::vector<Value>& values, int &comparisons, int &arrayAccesses)
{
    for (size_t i = 0; i < values.size() - 1; i++)
    {
        comparisons++;
        arrayAccesses += 2;

        if (values[i].getValue() > values[i+1].getValue())
        {
            return false;
        }
    }

    return true;
}

// Randomly shuffle until we get a sorted array
bool bogo_sort_step(std::vector<Value>& values, int &arrayAccesses, int &comparisons, const int duration_ms)
{
    if (isSorted(values, comparisons, arrayAccesses))
    {
        return true;
    }

    std::mt19937 rng(std::random_device{}());
    std::ranges::shuffle(values, rng);

    if (duration_ms > 0)
    {
        for (auto& v : values)
        {
            playTone(v.getValue(), duration_ms);
        }
    }

    return false;
}
