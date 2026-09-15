#include "Matrix.h"
#include "Generator.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace miit::algebra {

    // ==================== Приватные методы ====================

    template<typename T>
    void Matrix<T>::validateRectangular() const {
        if (data_.empty()) return;
        const std::size_t width = data_.front().size();
        for (const auto& row : data_) {
            if (row.size() != width) {
                throw std::invalid_argument("Matrix rows must have equal length");
            }
        }
    }

    // ==================== Конструкторы ====================

    template<typename T>
    Matrix<T>::Matrix(std::size_t rows, std::size_t columns, const T& value)
        : data_(rows, std::vector<T>(columns, value)) {
    }

    template<typename T>
    Matrix<T>::Matrix(const std::vector<std::vector<T>>& values)
        : data_(values) {
        validateRectangular();
    }

    // ==================== Операторы доступа ====================

    template<typename T>
    std::vector<T>& Matrix<T>::operator[](std::size_t row) {
        if (row >= rows()) {
            throw std::out_of_range("Matrix row index is out of range");
        }
        return data_[row];
    }

    template<typename T>
    const std::vector<T>& Matrix<T>::operator[](std::size_t row) const {
        if (row >= rows()) {
            throw std::out_of_range("Matrix row index is out of range");
        }
        return data_[row];
    }

    // ==================== Операторы сдвига ====================

    template<typename T>
    Matrix<T> Matrix<T>::operator<<(std::size_t positions) const {
        Matrix result(*this);
        for (auto& row : result.data_) {
            if (!row.empty()) {
                const std::size_t shift = positions % row.size();
                std::rotate(row.begin(),
                    row.begin() + static_cast<std::ptrdiff_t>(shift),
                    row.end());
            }
        }
        return result;
    }

    template<typename T>
    Matrix<T> Matrix<T>::operator>>(std::size_t positions) const {
        Matrix result(*this);
        for (auto& row : result.data_) {
            if (!row.empty()) {
                const std::size_t shift = positions % row.size();
                std::rotate(row.rbegin(),
                    row.rbegin() + static_cast<std::ptrdiff_t>(shift),
                    row.rend());
            }
        }
        return result;
    }

    // ==================== Методы получения информации ====================

    template<typename T>
    std::size_t Matrix<T>::rows() const noexcept {
        return data_.size();
    }

    template<typename T>
    std::size_t Matrix<T>::columns() const noexcept {
        return data_.empty() ? 0U : data_.front().size();
    }

    template<typename T>
    bool Matrix<T>::empty() const noexcept {
        return rows() == 0U || columns() == 0U;
    }

    // ==================== Методы модификации ====================

    template<typename T>
    void Matrix<T>::fill(Generator& generator) {
        for (auto& row : data_) {
            for (auto& value : row) {
                value = static_cast<T>(generator.generate());
            }
        }
    }

    template<typename T>
    void Matrix<T>::insertRow(std::size_t index, const std::vector<T>& row) {
        if (index > rows()) {
            throw std::out_of_range("Row insertion index is out of range");
        }
        if (!data_.empty() && row.size() != columns()) {
            throw std::invalid_argument("Inserted row has wrong size");
        }
        data_.insert(data_.begin() + static_cast<std::ptrdiff_t>(index), row);
    }

    template<typename T>
    void Matrix<T>::removeRow(std::size_t index) {
        if (index >= rows()) {
            throw std::out_of_range("Row removal index is out of range");
        }
        data_.erase(data_.begin() + static_cast<std::ptrdiff_t>(index));
    }

    template<typename T>
    void Matrix<T>::insertColumn(std::size_t index, const std::vector<T>& column) {
        if (empty()) {
            throw std::logic_error("Cannot insert a column into an empty matrix");
        }
        if (index > columns()) {
            throw std::out_of_range("Column insertion index is out of range");
        }
        if (column.size() != rows()) {
            throw std::invalid_argument("Inserted column has wrong size");
        }

        for (std::size_t row = 0; row < rows(); ++row) {
            data_[row].insert(
                data_[row].begin() + static_cast<std::ptrdiff_t>(index),
                column[row]
            );
        }
    }

    template<typename T>
    void Matrix<T>::removeColumn(std::size_t index) {
        if (index >= columns()) {
            throw std::out_of_range("Column removal index is out of range");
        }
        for (auto& row : data_) {
            row.erase(row.begin() + static_cast<std::ptrdiff_t>(index));
        }
    }

    // ==================== Методы преобразования ====================

    template<typename T>
    std::string Matrix<T>::toString() const {
        std::ostringstream out;
        for (std::size_t row = 0; row < rows(); ++row) {
            for (std::size_t column = 0; column < columns(); ++column) {
                if (column != 0U) out << ' ';
                out << data_[row][column];
            }
            if (row + 1U != rows()) out << '\n';
        }
        return out.str();
    }

    // ==================== Явная инстанциация ====================

    // Инстанцируем шаблон для типа int
    template class Matrix<int>;

    // Можно добавить и другие типы при необходимости
    // template class Matrix<double>;
    // template class Matrix<float>;
    // template class Matrix<long long>;

} // namespace miit::algebra