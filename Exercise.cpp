#include "Exercise.h"
#include <utility>

namespace miit::algebra {

    Exercise::Exercise(Matrix<int> matrix)
        : matrix(std::move(matrix)) {}

    void Exercise::setMatrix(const Matrix<int>& mat) {
        matrix = mat;
    }

    const Matrix<int>& Exercise::getMatrix() const {
        return matrix;
    }

} // namespace miit::algebra
