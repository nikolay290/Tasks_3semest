#pragma once

#include "Generator.h"

namespace miit::algebra {

    /**
     * @brief Генератор, возвращающий константное значение
     */
    class ConstantGenerator : public Generator {
    private:
        int constantValue;

    public:
        /**
         * @brief Конструктор
         * @param value константное значение (по умолчанию 0)
         */
        ConstantGenerator(const int value = 0);

        /**
         * @brief Возвращает константное значение
         * @return константное значение
         */
        int generate() override;
    };

} // namespace miit::algebra
