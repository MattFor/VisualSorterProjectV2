//
// Created by MattFor on 31.03.2025.
//

#ifndef VALUE_H
#define VALUE_H

// For later files
#include <vector>
#include <GLFW/glfw3.h>

#include "../include/sound.hpp"

class Value
{
    float m_x;      // The x-position where the bar starts being drawn
    float m_value;  // The value to be sorted (height of the bar)

public:
    // Constructors
    Value() : m_x(0.0f), m_value(0.0f) {};
    Value(const int value, const float x)   : m_x(x), m_value(static_cast<float>(value)) {};
    Value(const float value, const float x) : m_x(x), m_value(value) {};

    // Getters
    [[nodiscard]] float     getX()                  const;
    [[nodiscard]] int       getValue()              const { return static_cast<int>(m_value); }
    [[nodiscard]] float     getValueAsFloat()       const { return m_value; }

    // Setters
    void setX       (float x);
    void setValue   (int value);
    void setValue   (float value);

    // Draw function: draws the value as a bar (no need to, we do that elsewhere
    // void draw() const;
};

#endif // VALUE_H
