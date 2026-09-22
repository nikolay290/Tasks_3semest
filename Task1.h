#pragma once

#include "Exercise.h"
#include <string>

namespace miit::algebra {

    /**
     * @brief Задание 1: заменить все элементы первых трех столбцов на их квадраты
     */
    class Task1 : public Exercise {
    public:
        Task1() = default;

        /**
         * @brief Выполняет задание: возводит элементы первых трех столбцов в квадрат
         */
        void solve() override;

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