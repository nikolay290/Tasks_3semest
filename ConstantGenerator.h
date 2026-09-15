#pragma once
#include "Generator.h"
namespace miit::algebra {
class ConstantGenerator final : public Generator {
private:
    int value_;
public:
    explicit ConstantGenerator(int value);
    int generate() override;
};
}
