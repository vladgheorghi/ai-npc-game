#pragma once

#include <iostream>
#include <cmath>

class floatMod {
private:
    float value;
    float limit;

    void wrapToLimit();

public:
    floatMod();

    floatMod(float value, float limit);

    // -----------------------------------------
    // OPERATOR OVERLOADS
    // -----------------------------------------

    // Addition
    floatMod operator+(const floatMod& other) const;

    // Subtraction
    floatMod operator-(const floatMod& other) const;

    // Multiplication
    floatMod operator*(const floatMod& other) const;

    // Division
    floatMod operator/(const floatMod& other) const;

    // Add regular float to a floatMod
    floatMod operator+(float scalar) const;

    // Subtract regular float to a floatMod
    floatMod operator-(float scalar) const;

    // Multiply regular float to a floatMod
    floatMod operator*(float scalar) const;

    // Divide regular float to a floatMod
    floatMod operator/(float scalar) const;

    // Left-hand addition
    friend floatMod operator+(float scalar, const floatMod& fm);

    // Left-hand subtraction
    friend floatMod operator-(float scalar, const floatMod& fm);

    // Left-hand multiplication
    friend floatMod operator*(float scalar, const floatMod& fm);

    // Left-hand division
    friend floatMod operator/(float scalar, const floatMod& fm);

    // Compound addition assignment
    floatMod& operator+=(float scalar);

    // Compound subtraction assignment
    floatMod& operator-=(float scalar);
    
    // Compound multiplication assignment
    floatMod& operator*=(float scalar);

    // Compound division assignment
    floatMod& operator/=(float scalar);

    // Implicit conversion to float
    operator float() const { return value; }
};
