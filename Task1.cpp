/**
 * @file Task1.cpp
 * @brief Реализация класса Task1
 */

#include "Task1.h"
#include <algorithm>
#include <stdexcept>

namespace miit::algebra {

    // ============================================================
    // Публичные методы
    // ============================================================

    void Task1::solve() {
        checkMatrixNotEmpty();

        const size_t cols = matrix.getCols();
        const size_t limit = std::min(cols, static_cast<size_t>(3));

        // Заменяем элементы первых трех столбцов на их квадраты
        for (size_t i = 0; i < matrix.getRows(); ++i) {
            for (size_t j = 0; j < limit; ++j) {
                matrix[i][j] = matrix[i][j] * matrix[i][j];
            }
        }
    }

    std::string Task1::getDescription() const {
        return "Заменить все элементы первых трех столбцов на их квадраты";
    }

    // ============================================================
    // Приватные методы
    // ============================================================

    void Task1::checkMatrixNotEmpty() const {
        if (matrix.getRows() == 0 || matrix.getCols() == 0) {
            throw std::runtime_error("Matrix is empty. Cannot perform operation.");
        }
    }

} // namespace miit::algebra