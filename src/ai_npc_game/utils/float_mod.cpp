#include "float_mod.h"

FloatMod::FloatMod() : value(0.0f), limit(180.0f) { }

FloatMod::FloatMod(float value, float limit) : value(value), limit(limit) {
    wrapToLimit();
}

FloatMod::FloatMod(float value) : FloatMod(value, 180.0f) { }

void FloatMod::wrapToLimit() {
    // wrap strictly between [-limit, limit], total range is 2x.
    float range = 2.0f * limit;

    value = std::fmod(value + limit, range);
    if (value < 0.0f) value += range; // Fix negative fmod behavior
    value -= limit;
}

// -----------------------------------------
// OPERATOR OVERLOADS
// -----------------------------------------

// Addition
FloatMod FloatMod::operator+(const FloatMod& other) const {
    return FloatMod(this->value + other.value, this->limit);
}

// Subtraction
FloatMod FloatMod::operator-(const FloatMod& other) const {
    return FloatMod(this->value - other.value, this->limit);
}

// Multiplication
FloatMod FloatMod::operator*(const FloatMod& other) const {
    return FloatMod(this->value * other.value, this->limit);
}

// Division
FloatMod FloatMod::operator/(const FloatMod& other) const {
    return FloatMod(this->value / other.value, this->limit);
}

// Add regular float to FloatMod
FloatMod FloatMod::operator+(float scalar) const {
    return FloatMod(this->value + scalar, this->limit);
}

// Subtract regular float to FloatMod
FloatMod FloatMod::operator-(float scalar) const {
    return FloatMod(this->value - scalar, this->limit);
}

// Multiply regular float to FloatMod
FloatMod FloatMod::operator*(float scalar) const {
    return FloatMod(this->value * scalar, this->limit);
}

// Divide regular float to FloatMod
FloatMod FloatMod::operator/(float scalar) const {
    return FloatMod(this->value / scalar, this->limit);
}

// Left-hand addition
FloatMod operator+(float scalar, const FloatMod& fm) {
    return FloatMod(scalar + fm.value, fm.limit);
}

// Left-hand subtraction
FloatMod operator-(float scalar, const FloatMod& fm) {
    return FloatMod(scalar - fm.value, fm.limit);
}

// Left-hand multiplication
FloatMod operator*(float scalar, const FloatMod& fm) {
    return FloatMod(scalar * fm.value, fm.limit);
}

// Left-hand division
FloatMod operator/(float scalar, const FloatMod& fm) {
    return FloatMod(scalar / fm.value, fm.limit);
}

// Compound addition assignment
FloatMod& FloatMod::operator+=(float scalar) {
    this->value += scalar;
    wrapToLimit(); // Re-wrap after modifying
    return *this;
}

// Compound subtraction assignment
FloatMod& FloatMod::operator-=(float scalar) {
    this->value -= scalar;
    wrapToLimit(); // Re-wrap after modifying
    return *this;
}

// Compound multiplication assignment
FloatMod& FloatMod::operator*=(float scalar) {
    this->value *= scalar;
    wrapToLimit(); // Re-wrap after modifying
    return *this;
}

// Compound division assignment
FloatMod& FloatMod::operator/=(float scalar) {
    this->value /= scalar;
    wrapToLimit(); // Re-wrap after modifying
    return *this;
}

// Assignment
FloatMod& FloatMod::operator=(float scalar) {
    this->value = scalar;
    wrapToLimit(); // Re-wrap after modifying
    return *this;
}