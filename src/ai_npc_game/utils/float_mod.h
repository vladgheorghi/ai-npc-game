#pragma once

#include "utils/glm_utils.h"

class FloatMod {
private:
    float value;
    float limit;

    void wrapToLimit();

public:
    FloatMod();

    FloatMod(float value);

    FloatMod(float value, float limit);

    // -----------------------------------------
    // OPERATOR OVERLOADS
    // -----------------------------------------

    // Addition
    FloatMod operator+(const FloatMod& other) const;

    // Subtraction
    FloatMod operator-(const FloatMod& other) const;

    // Multiplication
    FloatMod operator*(const FloatMod& other) const;

    // Division
    FloatMod operator/(const FloatMod& other) const;

    // Add regular float to a FloatMod
    FloatMod operator+(float scalar) const;

    // Subtract regular float to a FloatMod
    FloatMod operator-(float scalar) const;

    // Multiply regular float to a FloatMod
    FloatMod operator*(float scalar) const;

    // Divide regular float to a FloatMod
    FloatMod operator/(float scalar) const;

    // Left-hand addition
    friend FloatMod operator+(float scalar, const FloatMod& fm);

    // Left-hand subtraction
    friend FloatMod operator-(float scalar, const FloatMod& fm);

    // Left-hand multiplication
    friend FloatMod operator*(float scalar, const FloatMod& fm);

    // Left-hand division
    friend FloatMod operator/(float scalar, const FloatMod& fm);

    // Compound addition assignment
    FloatMod& operator+=(float scalar);

    // Compound subtraction assignment
    FloatMod& operator-=(float scalar);
    
    // Compound multiplication assignment
    FloatMod& operator*=(float scalar);

    // Compound division assignment
    FloatMod& operator/=(float scalar);

    // Assignment
    FloatMod& operator=(float scalar);

    // Implicit conversion to float
    operator float() const { return value; }
};

typedef glm::vec<3, FloatMod, glm::packed_highp> Vec3Mod;