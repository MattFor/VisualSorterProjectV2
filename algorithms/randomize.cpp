//
// Created by MattFor on 31.03.2025.
//

#include <random>
#include <vector>

#include "../include/Value.h"

// Function to randomise a single step in the value vector
bool randomize_step(std::vector<Value>& values, int& arrayAccesses, int& _, const int duration_ms)
{
    static size_t randomStep = 0; // Static counter to track the current step

    // Check if all steps have been completed
    if (randomStep >= values.size())
    {
        randomStep = 0; // Reset for the next use
        return true;    // Indicate that randomisation is complete
    }

    // Initialize a random number generator and distribution
    static std::random_device rd;                                          // Non-deterministic random number generator
    static std::mt19937 gen(rd());                                      // Mersenne Twister engine seeded with rd()
    std::uniform_int_distribution<size_t> dis(0, values.size() - 1);   // Uniform distribution over valid indices

    // Generate a random index
    const size_t j = dis(gen);

    // Swap the current element with the randomly chosen element
    std::swap(values[randomStep], values[j]);
    arrayAccesses += 2;

    playTone(values[randomStep].getValue(), duration_ms);

    randomStep++;
    return false; // Randomisation is not yet complete
}
