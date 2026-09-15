#include "Task2.h"
#include <algorithm>
#include <vector>

namespace miit::algebra {

    void Task2::solve() {
        // Вставить после каждой нечетной строки первую строку
        if (matrix_.empty()) return;

        const auto firstRow = matrix_[0];
        const std::size_t originalRows = matrix_.rows();

        // Проходим по строкам с конца, чтобы индексы не смещались
        for (std::size_t r = originalRows; r-- > 0U; ) {
            // Если строка нечетная (индекс + 1 нечетный)
            if ((r + 1U) % 2U == 1U) {
                matrix_.insertRow(r + 1U, firstRow);
            }
        }
    }

} // namespace miit::algebra