#pragma once
#include "Generator.h"

namespace miit::algebra {
    /**
     * @brief Генератор, возвращающий одно и то же константное значение
     */
    class ConstantGenerator final : public Generator {
    private:
        int value_; /**< Константное значение, которое возвращает генератор */

    public:
        /**
         * @brief Конструктор через константное значение
         * @param value - значение, которое будет возвращать генератор
         */
        explicit ConstantGenerator(int value);

        /**
         * @brief Возвращает константное значение
         * @return константное значение
         */
        int generate() override;
    };
}
