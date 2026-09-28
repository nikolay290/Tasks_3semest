#pragma once

#include "Exercise.h"
#include <string>

namespace miit::algebra {

    /**
     * @brief Задание 2: вставить после каждой нечетной строки первую строку
     */
    class Task2 : public Exercise {
    public:
        /**
         * @brief Конструктор с матрицей
         * @param mat матрица, над которой выполняется задание
         */
        explicit Task2(const Matrix<int>& mat);

        /**
         * @brief Выполняет задание: вставляет копию первой строки после каждой нечетной строки
         */
        void solve() override;

        /**
         * @brief Задание 2: вставить копию первой строки после каждой нечетной строки
         */
        void insertFirstRowAfterOddRows();

        /**
         * @brief Возвращает описание задания
         */
        std::string getDescription() const override;
    };

} // namespace miit::algebra
