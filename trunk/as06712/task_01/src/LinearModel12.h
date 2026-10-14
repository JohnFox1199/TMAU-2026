#pragma once
#include "Model.h"
#include <deque>

class LinearModel12 final : public Model {
public:
    LinearModel12(double a, double b, int k)
        : a_(a), b_(b), k_(k) {
        reset();
    }

    void reset() override {
        y_ = 0.0;
        uHistory_.assign(k_ + 1, 0.0);
    }

    double step(double u) override {
        uHistory_.push_back(u);
        if (static_cast<int>(uHistory_.size()) > k_ + 1) {
            uHistory_.pop_front();
        }
        const double u_delayed = uHistory_.front();
        y_ = a_ * y_ + b_ * u_delayed;
        return y_;
    }

    const char* name() const override {
        return "Model_1_2";
    }

private:
    double a_;
    double b_;
    int k_;
    double y_ = 0.0;
    std::deque<double> uHistory_;
};
