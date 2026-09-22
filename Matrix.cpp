#include "Matrix.h"
#include "Generator.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace miit::algebra {

    template <typename T>
    Matrix<T>::Matrix() : rows(0), cols(0) {}

    template <typename T>
    Matrix<T>::Matrix(const size_t rows, const size_t cols) : rows(rows), cols(cols) {
        data.resize(rows, std::vector<T>(cols, T()));
    }

    template <typename T>
    Matrix<T>::Matrix(Matrix&& other) noexcept
        : rows(other.rows), cols(other.cols), data(std::move(other.data)) {
        other.rows = 0;
        other.cols = 0;
    }

    template <typename T>
    Matrix<T>& Matrix<T>::operator=(Matrix&& other) noexcept {
        if (this != &other) {
            rows = other.rows;
            cols = other.cols;
            data = std::move(other.data);
            other.rows = 0;
            other.cols = 0;
        }
        return *this;
    }

    template <typename T>
    size_t Matrix<T>::getRows() const {
        return rows;
    }

    template <typename T>
    size_t Matrix<T>::getCols() const {
        return cols;
    }

    template <typename T>
    bool Matrix<T>::isEmpty() const {
        return rows == 0 || cols == 0;
    }

    template <typename T>
    std::vector<T>& Matrix<T>::operator[](const size_t index) {
        if (index >= rows) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    template <typename T>
    const std::vector<T>& Matrix<T>::operator[](const size_t index) const {
        if (index >= rows) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    template <typename T>
    Matrix<T> Matrix<T>::operator<<(const size_t positions) const {
        Matrix<T> result(*this);
        for (auto& row : result.data) {
            if (!row.empty()) {
                const size_t shift = positions % row.size();
                std::rotate(row.begin(),
                    row.begin() + static_cast<std::ptrdiff_t>(shift),
                    row.end());
            }
        }
        return result;
    }

    template <typename T>
    Matrix<T> Matrix<T>::operator>>(const size_t positions) const {
        Matrix<T> result(*this);
        for (auto& row : result.data) {
            if (!row.empty()) {
                const size_t shift = positions % row.size();
                std::rotate(row.rbegin(),
                    row.rbegin() + static_cast<std::ptrdiff_t>(shift),
                    row.rend());
            }
        }
        return result;
    }

    template <typename T>
    void Matrix<T>::resize(const size_t newRows, const size_t newCols) {
        rows = newRows;
        cols = newCols;
        data.resize(rows);
        for (auto& row : data) {
            row.resize(cols);
        }
    }

    template <typename T>
    void Matrix<T>::clear() {
        resize(0, 0);
    }

    template <typename T>
    void Matrix<T>::fill(const T& value) {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] = value;
            }
        }
    }

    template <typename T>
    void Matrix<T>::fill(Generator& generator) {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] = static_cast<T>(generator.generate());
            }
        }
    }

    template <typename T>
    void Matrix<T>::replaceFirstThreeColumnsWithSquares() {
        const size_t limit = std::min(cols, static_cast<size_t>(3));
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < limit; ++j) {
                data[i][j] = data[i][j] * data[i][j];
            }
        }
    }

    template <typename T>
    Matrix<T> Matrix<T>::insertFirstRowAfterOddRows() const {
        if (rows == 0) {
            return Matrix<T>();
        }

        // Количество нечетных строк (нумерация с 1): ceil(rows / 2)
        Matrix<T> result(rows + (rows + 1) / 2, cols);

        size_t out = 0;
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result.data[out][j] = data[i][j];
            }
            ++out;

            // После строки с нечетным номером (индексы 0, 2, 4, ...)
            // вставляем копию первой строки
            if ((i + 1) % 2 == 1) {
                for (size_t j = 0; j < cols; ++j) {
                    result.data[out][j] = data[0][j];
                }
                ++out;
            }
        }

        return result;
    }

    template <typename T>
    std::string Matrix<T>::toString() const {
        std::ostringstream oss;
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                oss << data[i][j] << " ";
            }
            oss << "\n";
        }
        return oss.str();
    }

    template <typename T>
    std::unique_ptr<Matrix<T>> Matrix<T>::clone() const {
        return std::make_unique<Matrix<T>>(*this);
    }

    // Явная инстанциация шаблона для типа int
    template class Matrix<int>;

} // namespace miit::algebra