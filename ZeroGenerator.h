#pragma once

#include "Generator.h"

namespace miit::algebra {

    /**
     * @brief Генератор, возвращающий ноль
     */
    class ZeroGenerator : public Generator {
    public:
        /**
         * @brief Возвращает ноль
         * @return ноль
         */
        int generate() override;
    };

} // namespace miit::algebra