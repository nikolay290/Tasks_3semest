#pragma once

#include "Exercise.h"
#include <string>

namespace miit::algebra {

    /**
     * @brief Задание 1: заменить все элементы первых трех столбцов на их квадраты
     */
    class Task1 : public Exercise {
    public:
        /**
         * @brief Конструктор с матрицей
         * @param mat матрица, над которой выполняется задание
         */
        explicit Task1(const Matrix<int>& mat);

        /**
         * @brief Выполняет задание: возводит элементы первых трех столбцов в квадрат
         */
        void solve() override;

        /**
         * @brief Задание 1: заменить все элементы первых трех столбцов на их квадраты
         */
        void replaceFirstThreeColumnsWithSquares();

        /**
         * @brief Возвращает описание задания
         */
        std::string getDescription() const override;

    private:
        /**
         * @brief Проверяет, что матрица не пустая
         */
        void checkMatrixNotEmpty() const;
    };

} // namespace miit::algebra
