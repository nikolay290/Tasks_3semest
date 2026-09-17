#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <iostream>

namespace miit::algebra {

    class Generator;

    /**
     * @brief Шаблонный класс двумерной матрицы
     */
    template<typename T>
    class Matrix {
    private:
        std::vector<std::vector<T>> data_;

        /**
         * @brief Проверяет корректность прямоугольной матрицы
         * @return true, если все строки одинаковой длины
         */
        bool isValidRectangular() const;

    public:
        /**
         * @brief Конструктор по умолчанию
         */
        Matrix() = default;

        /**
         * @brief Конструктор через размеры и значение
         * @param rows - количество строк
         * @param columns - количество столбцов
         * @param value - значение по умолчанию для всех элементов
         */
        Matrix(std::size_t rows, std::size_t columns, const T& value = T{});

        /**
         * @brief Конструктор через двумерный вектор
         * @param values - двумерный вектор значений
         */
        explicit Matrix(const std::vector<std::vector<T>>& values);

        /**
         * @brief Конструктор копирования
         * @param other - другой объект Matrix
         */
        Matrix(const Matrix& other);

        /**
         * @brief Конструктор перемещения
         * @param other - другой объект Matrix
         */
        Matrix(Matrix&& other) noexcept;

        /**
         * @brief Деструктор
         */
        ~Matrix() = default;

        /**
         * @brief Оператор присваивания (копирование)
         * @param other - другой объект Matrix
         * @return ссылка на текущий объект
         */
        Matrix& operator=(const Matrix& other);

        /**
         * @brief Оператор присваивания (перемещение)
         * @param other - другой объект Matrix
         * @return ссылка на текущий объект
         */
        Matrix& operator=(Matrix&& other) noexcept;

        /**
         * @brief Оператор доступа к строке по индексу
         * @param row - индекс строки
         * @return ссылка на вектор-строку
         */
        std::vector<T>& operator[](std::size_t row);

        /**
         * @brief Оператор доступа к строке по индексу (константный)
         * @param row - индекс строки
         * @return константная ссылка на вектор-строку
         */
        const std::vector<T>& operator[](std::size_t row) const;

        /**
         * @brief Оператор циклического сдвига строк влево
         * @param positions - количество позиций сдвига
         * @return новая матрица со сдвинутыми строками
         */
        Matrix operator<<(std::size_t positions) const;

        /**
         * @brief Оператор циклического сдвига строк вправо
         * @param positions - количество позиций сдвига
         * @return новая матрица со сдвинутыми строками
         */
        Matrix operator>>(std::size_t positions) const;

        /**
         * @brief Возвращает количество строк
         * @return количество строк
         */
        std::size_t rows() const noexcept;

        /**
         * @brief Возвращает количество столбцов
         * @return количество столбцов
         */
        std::size_t columns() const noexcept;

        /**
         * @brief Проверяет, является ли матрица пустой
         * @return true, если матрица пустая
         */
        bool empty() const noexcept;

        /**
         * @brief Заполняет матрицу значениями из генератора
         * @param generator - генератор значений
         */
        void fill(Generator& generator);

        /**
         * @brief Вставляет строку в указанную позицию
         * @param index - индекс позиции вставки
         * @param row - вставляемая строка
         */
        void insertRow(std::size_t index, const std::vector<T>& row);

        /**
         * @brief Удаляет строку по индексу
         * @param index - индекс удаляемой строки
         */
        void removeRow(std::size_t index);

        /**
         * @brief Вставляет столбец в указанную позицию
         * @param index - индекс позиции вставки
         * @param column - вставляемый столбец
         */
        void insertColumn(std::size_t index, const std::vector<T>& column);

        /**
         * @brief Удаляет столбец по индексу
         * @param index - индекс удаляемого столбца
         */
        void removeColumn(std::size_t index);

        /**
         * @brief Сериализация в строку
         * @return строковое представление матрицы
         */
        std::string toString() const;

        /**
         * @brief Оператор вывода в поток
         * @param os - выходной поток
         * @param matrix - объект Matrix
         * @return ссылка на поток
         */
        template<typename U>
        friend std::ostream& operator<<(std::ostream& os, const Matrix<U>& matrix);

        /**
         * @brief Оператор ввода из потока
         * @param is - входной поток
         * @param matrix - объект Matrix
         * @return ссылка на поток
         */
        template<typename U>
        friend std::istream& operator>>(std::istream& is, Matrix<U>& matrix);
    };

}
