#include "floatMod.h"

floatMod::floatMod(float value, float limit) : value(value), limit(limit) {
    wrapToLimit();
}

void floatMod::wrapToLimit() {
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
floatMod floatMod::operator+(const floatMod& other) const {
    return floatMod(this->value + other.value, this->limit);
}

// Subtraction
floatMod floatMod::operator-(const floatMod& other) const {
    return floatMod(this->value - other.value, this->limit);
}

// Multiplication
floatMod floatMod::operator*(const floatMod& other) const {
    return floatMod(this->value * other.value, this->limit);
}

// Division
floatMod floatMod::operator/(const floatMod& other) const {
    return floatMod(this->value / other.value, this->limit);
}

// Add regular float to floatMod
floatMod floatMod::operator+(float scalar) const {
    return floatMod(this->value + scalar, this->limit);
}

// Subtract regular float to floatMod
floatMod floatMod::operator-(float scalar) const {
    return floatMod(this->value - scalar, this->limit);
}

// Multiply regular float to floatMod
floatMod floatMod::operator*(float scalar) const {
    return floatMod(this->value * scalar, this->limit);
}

// Divide regular float to floatMod
floatMod floatMod::operator/(float scalar) const {
    return floatMod(this->value / scalar, this->limit);
}

// Left-hand addition
floatMod operator+(float scalar, const floatMod& fm) {
    return floatMod(scalar + fm.value, fm.limit);
}

// Left-hand subtraction
floatMod operator-(float scalar, const floatMod& fm) {
    return floatMod(scalar - fm.value, fm.limit);
}

// Left-hand multiplication
floatMod operator*(float scalar, const floatMod& fm) {
    return floatMod(scalar * fm.value, fm.limit);
}

// Left-hand division
floatMod operator/(float scalar, const floatMod& fm) {
    return floatMod(scalar / fm.value, fm.limit);
}

// Compound addition assignment
floatMod& floatMod::operator+=(float scalar) {
    this->value += scalar;
    wrapToLimit(); // Re-wrap after modifying
    return *this;
}

// Compound subtraction assignment
floatMod& floatMod::operator-=(float scalar) {
    this->value -= scalar;
    wrapToLimit(); // Re-wrap after modifying
    return *this;
}

// Compound multiplication assignment
floatMod& floatMod::operator*=(float scalar) {
    this->value *= scalar;
    wrapToLimit(); // Re-wrap after modifying
    return *this;
}


// Compound division assignment
floatMod& floatMod::operator/=(float scalar) {
    this->value /= scalar;
    wrapToLimit(); // Re-wrap after modifying
    return *this;
}
