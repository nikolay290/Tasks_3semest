#pragma once
#include "Generator.h"
#include "Matrix.h"
namespace miit::algebra {
class Exercise {
protected:
    Matrix<int> matrix_;
    Generator* generator_;
public:
    explicit Exercise(Matrix<int> matrix, Generator* generator = nullptr);
    virtual ~Exercise();
    void fill();
    virtual void solve() = 0;
    const Matrix<int>& getMatrix() const noexcept;
};
}
