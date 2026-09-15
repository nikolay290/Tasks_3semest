#include "Task1.h"
#include <algorithm>
#include <limits>
#include <stdexcept>

namespace miit::algebra {

    namespace {
        int checkedSquare(int value) {
            const long long result = static_cast<long long>(value) * static_cast<long long>(value);
            if (result > std::numeric_limits<int>::max())
                throw std::overflow_error("Squared value does not fit into int");
            return static_cast<int>(result);
        }
    }

    void Task1::solve() {
        // Заменить все элементы первых трех столбцов на их квадраты
        for (std::size_t r = 0; r < matrix_.rows(); ++r) {
            for (std::size_t c = 0; c < std::min<std::size_t>(3U, matrix_.columns()); ++c) {
                matrix_[r][c] = checkedSquare(matrix_[r][c]);
            }
        }
    }

} // namespace miit::algebra