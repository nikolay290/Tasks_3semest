#pragma once

#include "Matrix.h"
#include <string>

namespace miit::algebra {

    /**
     * @brief Абстрактный базовый класс задания, агрегирующий матрицу
     *
     * Хранит матрицу, над которой выполняется задание. Конкретные задания
     * реализуются в классах-наследниках, которые переопределяют чисто
     * виртуальный метод solve().
     */
    class Exercise {
    protected:
        /**
         * @brief Матрица, над которой выполняется задание
         */
        Matrix<int> matrix;

    public:
        /**
         * @brief Конструктор через матрицу
         * @param matrix матрица, над которой выполняется задание
         */
        explicit Exercise(Matrix<int> matrix);

        /**
         * @brief Деструктор
         */
        virtual ~Exercise() = default;

        /**
         * @brief Устанавливает матрицу
         */
        void setMatrix(const Matrix<int>& mat);

        /**
         * @brief Получает текущую матрицу
         * @return ссылка на матрицу
         */
        const Matrix<int>& getMatrix() const;

        /**
         * @brief Решение задачи (чисто виртуальный метод)
         */
        virtual void solve() = 0;

        /**
         * @brief Возвращает описание задачи
         */
        virtual std::string getDescription() const = 0;
    };

} // namespace miit::algebra
