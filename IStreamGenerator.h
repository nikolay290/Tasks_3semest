#pragma once
#include "Generator.h"
#include <istream>

namespace miit::algebra {
    /**
     * @brief Генератор, читающий значения из входного потока
     */
    class IStreamGenerator final : public Generator {
    private:
        std::istream& input_; /**< Ссылка на входной поток */

    public:
        /**
         * @brief Конструктор через входной поток
         * @param input - входной поток для чтения значений
         */
        explicit IStreamGenerator(std::istream& input);

        /**
         * @brief Читает очередное значение из потока
         * @return считанное значение
         */
        int generate() override;
    };
}
