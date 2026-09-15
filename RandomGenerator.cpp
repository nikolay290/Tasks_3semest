#include "RandomGenerator.h"
#include <stdexcept>
namespace miit::algebra {
RandomGenerator::RandomGenerator(int minimum, int maximum)
    : engine_(std::random_device{}()), distribution_(minimum, maximum) {
    if (minimum > maximum) throw std::invalid_argument("Minimum must not exceed maximum");
}
int RandomGenerator::generate() { return distribution_(engine_); }
}
