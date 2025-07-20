#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace Utils {
    const float PI = 3.14159265f;

    inline sf::Vector2f normalize(const sf::Vector2f& v) {
        float length = std::sqrt(v.x * v.x + v.y * v.y);
        if (length != 0)
            return sf::Vector2f(v.x / length, v.y / length);
        return v;
    }

    inline float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    inline float toDegrees(float radians) {
        return radians * 180.0f / PI;
    }

    inline float toRadians(float degrees) {
        return degrees * PI / 180.0f;
    }
}