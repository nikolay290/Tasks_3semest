/**
 * @file Task2.cpp
 * @brief Реализация класса Task2
 */

#include "Task2.h"

namespace miit::algebra {

    void Task2::solve() {
        const size_t rows = matrix.getRows();

        if (rows == 0) {
            return;
        }

        // Количество нечетных строк (нумерация с 1): ceil(rows / 2)
        const size_t copies = (rows + 1) / 2;

        // Собираем новую матрицу: каждая исходная строка,
        // а после каждой нечетной — копия первой строки
        Matrix<int> result(rows + copies, matrix.getCols());

        size_t out = 0;
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < matrix.getCols(); ++j) {
                result[out][j] = matrix[i][j];
            }
            ++out;

            // После строки с нечетным номером (индексы 0, 2, 4, ...)
            if ((i + 1) % 2 == 1) {
                for (size_t j = 0; j < matrix.getCols(); ++j) {
                    result[out][j] = matrix[0][j];
                }
                ++out;
            }
        }

        matrix = result;
    }

    std::string Task2::getDescription() const {
        return "Вставить первую строку после каждой нечетной строки";
    }

} // namespace miit::algebra