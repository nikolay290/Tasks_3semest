#include "Exercise.h"
#include <stdexcept>
#include <utility>
namespace miit::algebra {
Exercise::Exercise(Matrix<int> matrix, Generator* generator) : matrix_(std::move(matrix)), generator_(generator) {}
Exercise::~Exercise() = default;
void Exercise::fill() {
    if (generator_ == nullptr) throw std::logic_error("Exercise has no generator");
    matrix_.fill(*generator_);
}
const Matrix<int>& Exercise::getMatrix() const noexcept { return matrix_; }
}
