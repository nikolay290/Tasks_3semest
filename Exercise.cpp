#include "Exercise.h"
#include <stdexcept>
#include <utility>

namespace miit::algebra {

    Exercise::Exercise(Matrix<int> matrix, std::unique_ptr<Generator> generator)
        : matrix(std::move(matrix)), generator(std::move(generator)) {}

    void Exercise::setMatrix(const Matrix<int>& mat) {
        matrix = mat;
    }

    void Exercise::setGenerator(std::unique_ptr<Generator> gen) {
        generator = std::move(gen);
    }

    const Matrix<int>& Exercise::getMatrix() const {
        return matrix;
    }

    void Exercise::fill() {
        if (!generator) {
            throw std::logic_error("Exercise has no generator");
        }
        matrix.fill(*generator);
    }

} // namespace miit::algebra