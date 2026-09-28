#pragma once

#include "Generator.h"
#include <random>

namespace miit::algebra {

    /**
     * @brief Генератор случайных целых чисел в заданном диапазоне
     */
    class RandomGenerator : public Generator {
    private:
        std::mt19937 generator;
        std::uniform_int_distribution<int> distribution;

    public:
        /**
         * @brief Конструктор
         * @param min нижняя граница диапазона
         * @param max верхняя граница диапазона
         */
        RandomGenerator(const int min, const int max);

        /**
         * @brief Сгенерировать случайное число из диапазона
         * @return случайное значение
         */
        int generate() override;
    };

} // namespace miit::algebra