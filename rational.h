#pragma once

#include <numeric>
#include <iostream>
#include <stdexcept>
#include <compare>

class Rational {
public:

    Rational() : numerator_(0), denominator_(1) {}

    Rational(int numerator)
        : numerator_(numerator), denominator_(1) {
        Reduction();
    }

    Rational(int numerator, int denominator) 
        : numerator_(numerator), denominator_(denominator) {
        if (denominator == 0) {
            throw std::invalid_argument("Denominator cannot be zero");
        }
        Reduction();
    }

    Rational(const Rational& other)
        : numerator_(other.numerator_), denominator_(other.denominator_) {}
    
    int GetNumerator() const {
        return numerator_;
    }

    int GetDenominator() const {
        return denominator_;
    }

    Rational Inv() const {
        if(numerator_ == 0) {
            throw std::invalid_argument("Cannot invert zero");
        }
        return Rational(denominator_, numerator_);
    }

    Rational operator+() const {
        return *this;
    }

    Rational operator-() const {
        if (numerator_ == 0) {
            return *this;
        }
        return Rational(-numerator_, denominator_);
    }

    Rational operator+(const Rational& other) const {
        int new_numerator = numerator_ * other.denominator_ + other.numerator_ * denominator_;
        int new_denominator = denominator_ * other.denominator_;
        return Rational(new_numerator, new_denominator);
    }

    Rational operator-(const Rational& other) const {
        int new_numerator = numerator_ * other.denominator_ - other.numerator_ * denominator_;
        int new_denominator = denominator_ * other.denominator_;
        return Rational(new_numerator, new_denominator);
    }

    Rational operator*(const Rational& other) const {
        return Rational(numerator_ * other.numerator_, denominator_ * other.denominator_);
    }

    Rational operator/(const Rational& other) const {
        if(other.numerator_ == 0) {
            throw std::invalid_argument("Division by zero");
        }
        return (*this) * other.Inv();
    }

    Rational& operator+=(const Rational& other) {
        *this = (*this) + other;
        return *this;
    }

    Rational& operator-=(const Rational& other) {
        *this = (*this) - other;
        return *this;
    }

    Rational& operator*=(const Rational& other) {
        *this = (*this) * other;
        return *this;
    }

    Rational& operator/=(const Rational& other) {
        *this = (*this) / other;
        return *this;
    }


Rational& operator=(const Rational& other) {
    if (this != &other) {
        numerator_ = other.numerator_;
        denominator_ = other.denominator_;
        Reduction();
    }
    return *this;
}

    bool operator==(const Rational& other) const {
        return static_cast<long long>(numerator_) * other.denominator_
        == static_cast<long long>(other.numerator_) * denominator_;
    }

    auto operator<=>(const Rational& other) const {
        long long left = static_cast<long long>(numerator_) * other.denominator_;
        long long right = static_cast<long long>(other.numerator_) * denominator_;

        if (left < right) return std::strong_ordering::less;
        if (left > right) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }
private:
    int numerator_;
    int denominator_;

    void Reduction() {
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        
        if (numerator_ == 0) {
            denominator_ = 1;
            return;
        }

        const int divisor = std::gcd(numerator_, denominator_);
        numerator_ /= divisor;
        denominator_ /= divisor;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Rational& rational) {
    if (rational.GetDenominator() == 1) {
        os << rational.GetNumerator();
    } else {
        os << rational.GetNumerator() << " / " << rational.GetDenominator();
    }
    return os;
}

inline std::istream& operator>>(std::istream& is, Rational& rational) {
    int numerator = 0, denominator = 1;
    char slash;

    is >> numerator;
    if (!is) return is;

    while (is.peek() == ' ') is.get();


    if (is.peek() == '/') {
        is >> slash;
        while (is.peek() == ' ') is.get();
        is >> denominator;
        if (!is) return is;

        if (denominator == 0) {
            is.setstate(std::ios::failbit);
            return is;
        }
    }

    rational = Rational(numerator, denominator);
    return is;
}
