#pragma once
#include "Generator.h"

namespace miit::algebra {
    /**
     * @brief Генератор, возвращающий ноль
     */
    class ZeroGenerator final : public Generator {
    public:
        /**
         * @brief Возвращает ноль
         * @return ноль
         */
        int generate() override;
    };
}
