#pragma once
#include "Model.h"

class NonlinearModel26 final : public Model {
public:
    NonlinearModel26(double a1, double a2, double b)
        : a1_(a1), a2_(a2), b_(b) {
        reset();
    }

    void reset() override {
        y_ = 0.0;
        yPrev_ = 0.0;
    }

    double step(double u) override {
        const double y_next = a1_ * y_ * y_ * y_
                            - a2_ * yPrev_
                            + b_ * u * u;
        yPrev_ = y_;
        y_ = y_next;
        return y_;
    }

    const char* name() const override {
        return "Model_2_6";
    }

private:
    double a1_;
    double a2_;
    double b_;
    double y_ = 0.0;
    double yPrev_ = 0.0;
};
