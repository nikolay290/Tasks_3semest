#pragma once
#include "Generator.h"
#include <random>

namespace miit::algebra {
    /**
     * @brief Генератор случайных целых чисел в заданном диапазоне
     */
    class RandomGenerator final : public Generator {
    private:
        std::mt19937 engine_;                            /**< Генератор псевдослучайных чисел Mersenne Twister */
        std::uniform_int_distribution<int> distribution_; /**< Равномерное распределение целых чисел */

    public:
        /**
         * @brief Конструктор через границы диапазона
         * @param minimum - нижняя граница диапазона
         * @param maximum - верхняя граница диапазона
         */
        RandomGenerator(int minimum, int maximum);

        /**
         * @brief Генерирует случайное число в заданном диапазоне
         * @return случайное число
         */
        int generate() override;
    };
}
