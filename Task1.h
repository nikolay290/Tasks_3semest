#pragma once
#include "Exercise.h"

namespace miit::algebra {
    /**
     * @brief Первое задание варианта 3: замена элементов первых трех столбцов на их квадраты
     */
    class Task1 final : public Exercise {
    public:
        /**
         * @brief Наследует конструкторы базового класса Exercise
         */
        using Exercise::Exercise;

        /**
         * @brief Выполняет задание над матрицей
         */
        void solve() override;
    };
}
