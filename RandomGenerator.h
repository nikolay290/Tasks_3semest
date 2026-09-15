#pragma once
#include "Generator.h"
#include <random>
namespace miit::algebra {
class RandomGenerator final : public Generator {
private:
    std::mt19937 engine_;
    std::uniform_int_distribution<int> distribution_;
public:
    RandomGenerator(int minimum, int maximum);
    int generate() override;
};
}
