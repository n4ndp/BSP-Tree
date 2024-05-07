#ifndef DATATYPE_HPP
#define DATATYPE_HPP

#include <cmath>
#include <stdexcept>
#include <type_traits>
#include <iostream>

template <typename T>
class Safe {
    static_assert(std::is_floating_point<T>::value, "Template type must be floating point");

private:
    T value;
    static constexpr T EPSILON = static_cast<T>(1e-4);

public:
    Safe() : value(static_cast<T>(0)) {}
    Safe(T value) : value(value) {}

    bool operator==(const Safe& other) const {
        return std::abs(value - other.value) < EPSILON;
    }
    bool operator==(const T& scalar) const {
        return std::abs(value - scalar) < EPSILON;
    }

    bool operator!=(const Safe& other) const {
        return !(*this == other);
    }
    bool operator!=(const T& scalar) const {
        return !(*this == scalar);
    }

    bool operator<(const Safe& other) const {
        return value < other.value - EPSILON;
    }
    bool operator<(const T& scalar) const {
        return value < scalar - EPSILON;
    }

    bool operator<=(const Safe& other) const {
        return value <= other.value + EPSILON;
    }
    bool operator<=(const T& scalar) const {
        return value <= scalar + EPSILON;
    }

    bool operator>(const Safe& other) const {
        return value > other.value + EPSILON;
    }
    bool operator>(const T& scalar) const {
        return value > scalar + EPSILON;
    }

    bool operator>=(const Safe& other) const {
        return value >= other.value - EPSILON;
    }
    bool operator>=(const T& scalar) const {
        return value >= scalar - EPSILON;
    }

    Safe operator+(const Safe& other) const {
        return Safe(value + other.value);
    }
    Safe operator-(const Safe& other) const {
        return Safe(value - other.value);
    }
    Safe operator*(const Safe& other) const {
        return Safe(value * other.value);
    }
    Safe operator*(const double& other) const {
        return Safe(value * other);
    }
    Safe operator*(const float& other) const {
        return Safe(value * other);
    }
    Safe operator/(const Safe& other) const {
        if (std::abs(other.value) < EPSILON) {
            throw std::runtime_error("Division by zero");
        }
        return Safe(value / other.value);
    }
    Safe operator-() const {
        return Safe(-value);
    }
    Safe operator+=(const Safe& other) {
        value += other.value;
        return *this;
    }
    Safe operator-=(const Safe& other) {
        value -= other.value;
        return *this;
    }

    T getValue() const {
        return value;
    }
    void setValue(T value) {
        this->value = value;
    }
    
    static Safe abs(const Safe& other) {
        return Safe(std::abs(other.value));
    }
    static Safe sqrt(const Safe& other) {
        if (other.value < 0) {
            throw std::runtime_error("Attempted to calculate square root of a negative number");
        }
        return Safe(std::sqrt(other.value));
    }
    static Safe pow(const Safe& base, const int& exponent) {
        return Safe(std::pow(base.value, exponent));
    }
    static Safe min(const Safe& a, const Safe& b) {
        return a < b ? a : b;
    }
    static Safe max(const Safe& a, const Safe& b) {
        return a > b ? a : b;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Safe& other) {
        os << other.value;
        return os;
    }
};

template <typename T>
Safe<T> abs(const Safe<T>& x) {
    return Safe<T>::abs(x);
}
template <typename T>
Safe<T> sqrt(const Safe<T>& x) {
    return Safe<T>::sqrt(x);
}
template <typename T>
Safe<T> pow(const Safe<T>& base, const int& exponent) {
    return Safe<T>::pow(base, exponent);
}
template <typename T>
Safe<T> min(const Safe<T>& a, const Safe<T>& b) {
    return Safe<T>::min(a, b);
}
template <typename T>
Safe<T> max(const Safe<T>& a, const Safe<T>& b) {
    return Safe<T>::max(a, b);
}

using NType = Safe<float>;

#endif // DATATYPE_HPP
