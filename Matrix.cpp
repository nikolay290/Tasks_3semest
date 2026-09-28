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
    void Matrix<T>::fill(Generator& generator) {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] = static_cast<T>(generator.generate());
            }
        }
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

    // Явная инстанциация шаблона для типа int
    template class Matrix<int>;

} // namespace miit::algebra
