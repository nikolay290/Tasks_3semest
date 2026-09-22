#pragma once

#include "Generator.h"
#include <iostream>

namespace miit::algebra {

    /**
     * @brief Генератор, считывающий значения из потока ввода (например, с клавиатуры)
     */
    class IStreamGenerator : public Generator {
    private:
        std::istream& in;

    public:
        /**
         * @brief Конструктор
         * @param in поток ввода, из которого считываются значения (по умолчанию std::cin)
         */
        IStreamGenerator(std::istream& in = std::cin);

        /**
         * @brief Считывает и возвращает очередное значение из потока
         * @return считанное значение
         */
        int generate() override;
    };

} // namespace miit::algebra