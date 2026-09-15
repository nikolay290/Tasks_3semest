#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace miit::algebra {

    // Forward declaration
    class Generator;

    /**
     * @brief Шаблонный класс для работы с двумерными матрицами
     * @tparam T - тип элементов матрицы
     */
    template<typename T>
    class Matrix {
    private:
        std::vector<std::vector<T>> data_; /**< Двумерный вектор для хранения элементов матрицы */

        /**
         * @brief Проверяет, что матрица прямоугольная (все строки одинаковой длины)
         * @throws std::invalid_argument Если строки имеют разную длину
         */
        void validateRectangular() const;

    public:
        /**
         * @brief Конструктор по умолчанию. Создает пустую матрицу
         */
        Matrix() = default;

        /**
         * @brief Конструктор с указанием размеров и начального значения
         * @param rows - количество строк
         * @param columns - количество столбцов
         * @param value - начальное значение для всех элементов (по умолчанию T{})
         */
        Matrix(std::size_t rows, std::size_t columns, const T& value = T{});

        /**
         * @brief Конструктор из двумерного вектора
         * @param values - двумерный вектор с данными
         * @throws std::invalid_argument Если матрица не прямоугольная
         */
        explicit Matrix(const std::vector<std::vector<T>>& values);

        /**
         * @brief Конструктор копирования (по умолчанию)
         * @param other - другой объект Matrix
         */
        Matrix(const Matrix& other) = default;

        /**
         * @brief Конструктор перемещения (по умолчанию)
         * @param other - другой объект Matrix
         */
        Matrix(Matrix&& other) noexcept = default;

        /**
         * @brief Деструктор (по умолчанию)
         */
        ~Matrix() = default;

        /**
         * @brief Оператор присваивания копированием (по умолчанию)
         * @param other - другой объект Matrix
         * @return Ссылка на текущий объект
         */
        Matrix& operator=(const Matrix& other) = default;

        /**
         * @brief Оператор присваивания перемещением (по умолчанию)
         * @param other - другой объект Matrix
         * @return Ссылка на текущий объект
         */
        Matrix& operator=(Matrix&& other) noexcept = default;

        /**
         * @brief Оператор доступа к строке по индексу (неконстантный)
         * @param row - индекс строки
         * @return Ссылка на вектор-строку
         * @throws std::out_of_range Если индекс выходит за пределы
         */
        std::vector<T>& operator[](std::size_t row);

        /**
         * @brief Оператор доступа к строке по индексу (константный)
         * @param row - индекс строки
         * @return Константная ссылка на вектор-строку
         * @throws std::out_of_range Если индекс выходит за пределы
         */
        const std::vector<T>& operator[](std::size_t row) const;

        /**
         * @brief Оператор циклического сдвига строк влево
         * @param positions - количество позиций для сдвига
         * @return Новая матрица со сдвинутыми строками
         */
        Matrix operator<<(std::size_t positions) const;

        /**
         * @brief Оператор циклического сдвига строк вправо
         * @param positions - количество позиций для сдвига
         * @return Новая матрица со сдвинутыми строками
         */
        Matrix operator>>(std::size_t positions) const;

        /**
         * @brief Возвращает количество строк
         * @return Количество строк
         */
        std::size_t rows() const noexcept;

        /**
         * @brief Возвращает количество столбцов
         * @return Количество столбцов
         */
        std::size_t columns() const noexcept;

        /**
         * @brief Проверяет, является ли матрица пустой
         * @return true если матрица пустая, false в противном случае
         */
        bool empty() const noexcept;

        /**
         * @brief Заполняет матрицу значениями из генератора
         * @param generator - генератор значений
         */
        void fill(Generator& generator);

        /**
         * @brief Вставляет строку в указанную позицию
         * @param index - индекс позиции для вставки
         * @param row - вставляемая строка
         * @throws std::out_of_range Если индекс выходит за пределы
         * @throws std::invalid_argument Если размер строки не соответствует матрице
         */
        void insertRow(std::size_t index, const std::vector<T>& row);

        /**
         * @brief Удаляет строку по индексу
         * @param index - индекс удаляемой строки
         * @throws std::out_of_range Если индекс выходит за пределы
         */
        void removeRow(std::size_t index);

        /**
         * @brief Вставляет столбец в указанную позицию
         * @param index - индекс позиции для вставки
         * @param column - вставляемый столбец
         * @throws std::logic_error Если матрица пустая
         * @throws std::out_of_range Если индекс выходит за пределы
         * @throws std::invalid_argument Если размер столбца не соответствует матрице
         */
        void insertColumn(std::size_t index, const std::vector<T>& column);

        /**
         * @brief Удаляет столбец по индексу
         * @param index - индекс удаляемого столбца
         * @throws std::out_of_range Если индекс выходит за пределы
         */
        void removeColumn(std::size_t index);

        /**
         * @brief Преобразует матрицу в строку
         * @return Строковое представление матрицы
         */
        std::string toString() const;
    };

} // namespace miit::algebra