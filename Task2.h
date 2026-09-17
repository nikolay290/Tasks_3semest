#pragma once
#include "Exercise.h"

namespace miit::algebra {
    /**
     * @brief Второе задание варианта 3: вставка после каждой нечетной строки первой строки
     */
    class Task2 final : public Exercise {
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
