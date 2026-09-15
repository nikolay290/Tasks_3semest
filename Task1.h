#pragma once
#include "Exercise.h"

namespace miit::algebra {
    class Task1 final : public Exercise {
    public:
        using Exercise::Exercise;
        void solve() override;
    };
}