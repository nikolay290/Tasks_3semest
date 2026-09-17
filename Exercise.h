#pragma once
#include "Generator.h"
#include "Matrix.h"

namespace miit::algebra {
    /**
     * @brief Базовый класс задания, агрегирующий матрицу и генератор
     */
    class Exercise {
    protected:
        Matrix<int> matrix_;      /**< Матрица, над которой выполняется задание */
        Generator* generator_;    /**< Генератор для заполнения матрицы (может быть nullptr) */

    public:
        /**
         * @brief Конструктор через матрицу и генератор
         * @param matrix - матрица, над которой выполняется задание
         * @param generator - генератор для заполнения (по умолчанию nullptr)
         */
        explicit Exercise(Matrix<int> matrix, Generator* generator = nullptr);

        /**
         * @brief Деструктор
         */
        virtual ~Exercise();

        /**
         * @brief Заполняет матрицу значениями из генератора
         */
        void fill();

        /**
         * @brief Выполняет задание над матрицей (чисто виртуальный)
         */
        virtual void solve() = 0;

        /**
         * @brief Возвращает матрицу задания
         * @return константная ссылка на матрицу
         */
        const Matrix<int>& getMatrix() const noexcept;
    };
}
