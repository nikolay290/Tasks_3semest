#pragma once
#include "Generator.h"
#include <istream>
namespace miit::algebra {
class IStreamGenerator final : public Generator {
private:
    std::istream& input_;
public:
    explicit IStreamGenerator(std::istream& input);
    int generate() override;
};
}
