#include "ConstantGenerator.h"

namespace miit::algebra {

    ConstantGenerator::ConstantGenerator(const int value) : constantValue(value) {}

    int ConstantGenerator::generate() {
        return constantValue;
    }

    void ConstantGenerator::setValue(const int value) {
        constantValue = value;
    }

    int ConstantGenerator::getValue() const {
        return constantValue;
    }

} // namespace miit::algebra