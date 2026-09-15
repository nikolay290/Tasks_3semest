#pragma once
#include "Generator.h"
namespace miit::algebra {
class ZeroGenerator final : public Generator {
public:
    int generate() override;
};
}
