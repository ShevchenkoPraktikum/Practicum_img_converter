
#pragma once

#include <string>
#include <optional>
#include <cmath>
#include "rational.h"
#include "pow.h"

using Error = std::string;

template<class Number>
class Calculator {
public:
    Calculator() : number_(0), mem_(std::nullopt) {}

    void Set(Number num) {
        number_ = num;
    }

    Number GetNumber() const {
        return number_;
    }

    std::optional<Error> Add(Number rhs) {
        number_ += rhs;
        return std::nullopt;
    }

    std::optional<Error> Sub(Number rhs) {
        number_ -= rhs;
        return std::nullopt;
    }

    std::optional<Error> Mul(Number rhs) {
        number_ *= rhs;
        return std::nullopt;
    }

    std::optional<Error> Div(Number rhs) {
        if constexpr (std::is_integral_v<Number> || std::is_same_v<Number, Rational>) {
            if (rhs == Number(0)) {
                return "Division by zero";
            }
        }
        number_ /= rhs;
        return std::nullopt;
    }

    std::optional<Error> Pow(Number rhs) {
        if (number_ == Number(0) && rhs == Number(0)) {
            return "Zero power to zero";
        }

        if constexpr (std::is_floating_point_v<Number>) {
            number_ = std::pow(number_, rhs);
            return std::nullopt;
        } else if constexpr (std::is_integral_v<Number>) {
            if (rhs < 0) {
                return "Integer negative power";
            }
            number_ = IntegerPow(number_, rhs);
            return std::nullopt;
        } else if constexpr (std::is_same_v<Number, Rational>) {
            if constexpr (std::is_same_v<decltype(rhs), Rational>) {
                if (rhs.GetDenominator() != 1) {
                    return "Fractional power is not supported";
                }
            }
            try {
                number_ = ::Pow(number_, rhs);
                return std::nullopt;
            } catch (const std::exception&) {
                return "Power operation failed";
            }
        } else {
            return "Unsupported number type";
        }
    }

    bool GetHasMem() const {
        return mem_.has_value();
    }

    void Save() {
        mem_ = number_;
    }

    std::optional<Number> Load() {
        if (mem_.has_value()) {
            number_ = mem_.value();
            return number_;
        }
        return std::nullopt;
    }

    void ClearMem() {
        mem_.reset();
    }

private:
    Number number_;
    std::optional<Number> mem_;
};



