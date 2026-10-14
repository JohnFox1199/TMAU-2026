#pragma once
#include <cmath>

enum class SignalType {
    STEP,      // u = const
    IMPULSE,   // u0 = 1, u_{tau>0} = 0
    HARMONIC   // u = sin(tau)
};

class SignalGenerator {
public:
    SignalGenerator(SignalType type, double amplitude = 1.0)
        : type_(type), amplitude_(amplitude) {}

    double get(double tau) const {
        switch (type_) {
            case SignalType::STEP:
                return amplitude_;
            case SignalType::IMPULSE:
                return (tau < 1e-9) ? amplitude_ : 0.0;
            case SignalType::HARMONIC:
                return amplitude_ * std::sin(tau);
        }
        return 0.0;
    }

private:
    SignalType type_;
    double amplitude_;
};
