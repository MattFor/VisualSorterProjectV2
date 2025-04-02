//
// Created by MattFor on 31.03.2025.
//

// #include <GL/gl.h>

#include "../include/Value.h"

// Getters
float Value::getX() const
{
    return m_x;
}

// Setters
void Value::setValue(const float value)
{
    m_value = value;
}

void Value::setValue(const int value)
{
    m_value = static_cast<float>(value);
}

void Value::setX(const float x)
{
    m_x = x;
}


// void Value::draw() const
// {
//     constexpr float barWidth = 10.0f;
//     glBegin(GL_QUADS);
//     glVertex2f(m_x, 0.0f);                   // Bottom left
//     glVertex2f(m_x + barWidth, 0.0f);        // Bottom right
//     glVertex2f(m_x + barWidth, m_value);     // Top right
//     glVertex2f(m_x, m_value);                // Top left
//     glEnd();
// }
