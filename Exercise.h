#pragma once

#include "Matrix.h"
#include "Generator.h"
#include <memory>
#include <string>

namespace miit::algebra {

    /**
     * @brief Абстрактный базовый класс задания, агрегирующий матрицу и генератор
     *
     * Хранит матрицу, над которой выполняется задание, и генератор для её
     * заполнения. Конкретные задания реализуются в классах-наследниках,
     * которые переопределяют чисто виртуальный метод solve().
     */
    class Exercise {
    protected:
        /**
         * @brief Матрица, над которой выполняется задание
         */
        Matrix<int> matrix;

        /**
         * @brief Генератор для заполнения матрицы
         */
        std::unique_ptr<Generator> generator;

    public:
        /**
         * @brief Конструктор по умолчанию
         */
        Exercise() = default;

        /**
         * @brief Конструктор через матрицу и генератор
         * @param matrix матрица, над которой выполняется задание
         * @param generator генератор для заполнения матрицы
         */
        explicit Exercise(Matrix<int> matrix, std::unique_ptr<Generator> generator = nullptr);

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
         * @brief Устанавливает генератор
         * @param gen генератор для заполнения матрицы
         */
        void setGenerator(std::unique_ptr<Generator> gen);

        /**
         * @brief Заполняет матрицу значениями из генератора
         */
        void fill();

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