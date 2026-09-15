#include <gtest/gtest.h>
#include "ConstantGenerator.h"
#include "IStreamGenerator.h"
#include "RandomGenerator.h"
#include "Task1.h"
#include "Task2.h"
#include "ZeroGenerator.h"
#include <memory>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>
using namespace miit::algebra;

Matrix<int> makeMatrix(std::initializer_list<std::initializer_list<int>> values) {
    std::vector<std::vector<int>> rows;
    for (const auto& row : values) rows.emplace_back(row);
    return Matrix<int>(rows);
}

void expectMatrix(const Matrix<int>& actual, const Matrix<int>& expected) {
    ASSERT_EQ(actual.rows(), expected.rows());
    ASSERT_EQ(actual.columns(), expected.columns());
    for (std::size_t row = 0; row < actual.rows(); ++row)
        for (std::size_t column = 0; column < actual.columns(); ++column)
            EXPECT_EQ(actual[row][column], expected[row][column]);
}

TEST(MatrixConstruction, DefaultConstructor) {
    Matrix<int> empty;
    EXPECT_TRUE(empty.empty());
    EXPECT_EQ(empty.rows(), 0U);
    EXPECT_EQ(empty.columns(), 0U);
}

TEST(MatrixConstruction, SizedConstructor) {
    Matrix<int> sized(2, 3, 7);
    EXPECT_FALSE(sized.empty());
    EXPECT_EQ(sized.rows(), 2U);
    EXPECT_EQ(sized.columns(), 3U);
    EXPECT_EQ(sized[1][2], 7);
}

TEST(MatrixConstruction, ValuesConstructorRejectsRaggedRows) {
    EXPECT_THROW((Matrix<int>(std::vector<std::vector<int>>{{1, 2}, {3}})), std::invalid_argument);
}

TEST(MatrixValueSemantics, CopyConstructor) {
    Matrix<int> source = makeMatrix({{1, 2}, {3, 4}});
    Matrix<int> copied(source);
    expectMatrix(copied, source);
}

TEST(MatrixValueSemantics, MoveConstructor) {
    Matrix<int> source = makeMatrix({{1, 2}, {3, 4}});
    Matrix<int> moved(std::move(source));
    expectMatrix(moved, makeMatrix({{1, 2}, {3, 4}}));
}

TEST(MatrixValueSemantics, CopyAssignment) {
    Matrix<int> source = makeMatrix({{1, 2}, {3, 4}});
    Matrix<int> copyAssigned;
    copyAssigned = source;
    expectMatrix(copyAssigned, source);
}

TEST(MatrixValueSemantics, MoveAssignment) {
    Matrix<int> source = makeMatrix({{1, 2}, {3, 4}});
    Matrix<int> moveAssigned;
    moveAssigned = std::move(source);
    expectMatrix(moveAssigned, makeMatrix({{1, 2}, {3, 4}}));
}

TEST(MatrixAccess, MutableIndexOperator) {
    Matrix<int> matrix(1, 1);
    matrix[0][0] = 9;
    EXPECT_EQ(matrix[0][0], 9);
}

TEST(MatrixAccess, ConstIndexOperator) {
    Matrix<int> matrix = makeMatrix({{9}});
    const Matrix<int>& constant = matrix;
    EXPECT_EQ(constant[0][0], 9);
}

TEST(MatrixAccess, MutableIndexRejectsInvalidRow) {
    Matrix<int> matrix(1, 1);
    EXPECT_THROW((void)matrix[1], std::out_of_range);
}

TEST(MatrixAccess, ConstIndexRejectsInvalidRow) {
    const Matrix<int> matrix(1, 1);
    const Matrix<int>& constant = matrix;
    EXPECT_THROW((void)constant[1], std::out_of_range);
}

TEST(MatrixOperations, LeftShiftOperator) {
    const Matrix<int> source = makeMatrix({{1, 2, 3}, {4, 5, 6}});
    expectMatrix(source << 1U, makeMatrix({{2, 3, 1}, {5, 6, 4}}));
    expectMatrix(source << 4U, source << 1U);
}

TEST(MatrixOperations, RightShiftOperator) {
    const Matrix<int> source = makeMatrix({{1, 2, 3}, {4, 5, 6}});
    expectMatrix(source >> 1U, makeMatrix({{3, 1, 2}, {6, 4, 5}}));
}

TEST(MatrixOperations, ToString) {
    const Matrix<int> source = makeMatrix({{1, 2, 3}, {4, 5, 6}});
    EXPECT_EQ(source.toString(), "1 2 3\n4 5 6");
}

TEST(MatrixMutation, InsertRow) {
    Matrix<int> matrix = makeMatrix({{1, 2}, {5, 6}});
    matrix.insertRow(1U, std::vector<int>{3, 4});
    expectMatrix(matrix, makeMatrix({{1, 2}, {3, 4}, {5, 6}}));
    EXPECT_THROW(matrix.insertRow(matrix.rows() + 1U, std::vector<int>{7, 8}), std::out_of_range);
    EXPECT_THROW(matrix.insertRow(1U, std::vector<int>{7}), std::invalid_argument);
}

TEST(MatrixMutation, RemoveRow) {
    Matrix<int> matrix = makeMatrix({{1, 2}, {3, 4}, {5, 6}});
    matrix.removeRow(1U);
    expectMatrix(matrix, makeMatrix({{1, 2}, {5, 6}}));
    EXPECT_THROW(matrix.removeRow(2U), std::out_of_range);
}

TEST(MatrixMutation, InsertColumn) {
    Matrix<int> matrix = makeMatrix({{1, 3}, {4, 6}});
    matrix.insertColumn(1U, std::vector<int>{2, 5});
    expectMatrix(matrix, makeMatrix({{1, 2, 3}, {4, 5, 6}}));
    EXPECT_THROW(matrix.insertColumn(4U, std::vector<int>{2, 5}), std::out_of_range);
    EXPECT_THROW(matrix.insertColumn(1U, std::vector<int>{2}), std::invalid_argument);
    Matrix<int> empty;
    EXPECT_THROW(empty.insertColumn(0U, std::vector<int>{}), std::logic_error);
}

TEST(MatrixMutation, RemoveColumn) {
    Matrix<int> matrix = makeMatrix({{1, 2, 3}, {4, 5, 6}});
    matrix.removeColumn(1U);
    expectMatrix(matrix, makeMatrix({{1, 3}, {4, 6}}));
    EXPECT_THROW(matrix.removeColumn(2U), std::out_of_range);
}

TEST(ConstantGeneratorTest, Generate) {
    ConstantGenerator constant(42);
    EXPECT_EQ(constant.generate(), 42);
    EXPECT_EQ(constant.generate(), 42);
}

TEST(ZeroGeneratorTest, Generate) {
    ZeroGenerator zero;
    EXPECT_EQ(zero.generate(), 0);
}

TEST(RandomGeneratorTest, GenerateInRange) {
    RandomGenerator random(-3, 4);
    for (int index = 0; index < 100; ++index) {
        const int value = random.generate();
        EXPECT_GE(value, -3);
        EXPECT_LE(value, 4);
    }
}

TEST(RandomGeneratorTest, RejectsInvalidRange) {
    EXPECT_THROW((RandomGenerator(5, 4)), std::invalid_argument);
}

TEST(IStreamGeneratorTest, ReadsValues) {
    std::istringstream input("8 -2");
    IStreamGenerator keyboard(input);
    EXPECT_EQ(keyboard.generate(), 8);
    EXPECT_EQ(keyboard.generate(), -2);
}

TEST(IStreamGeneratorTest, RejectsExhaustedInput) {
    std::istringstream input;
    IStreamGenerator keyboard(input);
    EXPECT_THROW(keyboard.generate(), std::runtime_error);
}

TEST(GeneratorTest, WorksPolymorphically) {
    std::unique_ptr<Generator> polymorphic = std::make_unique<ConstantGenerator>(11);
    EXPECT_EQ(polymorphic->generate(), 11);
}

TEST(MatrixOperations, FillWithGenerator) {
    Matrix<int> matrix(1, 2);
    ConstantGenerator constant(6);
    matrix.fill(constant);
    expectMatrix(matrix, makeMatrix({{6, 6}}));
}

TEST(ExerciseApi, FillAndGetMatrix) {
    ConstantGenerator constant(6);
    Task1 exercise(Matrix<int>(1, 2), &constant);
    exercise.fill();
    expectMatrix(exercise.getMatrix(), makeMatrix({{6, 6}}));
}

TEST(ExerciseApi, FillRejectsMissingGenerator) {
    Matrix<int> matrix(1, 2);
    Task1 withoutGenerator(matrix);
    EXPECT_THROW(withoutGenerator.fill(), std::logic_error);
}

TEST(VariantOperations, FirstTransformation) {
    Task1 task(makeMatrix({{2, 3, 4, 5}, {-2, 0, 1, 6}}));
    task.solve();
    expectMatrix(task.getMatrix(), makeMatrix({{4, 9, 16, 5}, {4, 0, 1, 6}}));
}

TEST(VariantOperations, SecondTransformation) {
    Task2 task(makeMatrix({{1, 2}, {3, 4}, {5, 6}}));
    task.solve();
    expectMatrix(task.getMatrix(), makeMatrix({{1, 2}, {1, 2}, {3, 4}, {5, 6}, {1, 2}}));
}

TEST(VariantOperations, EmptyAndSingleElementMatrices) {
    Task1 emptyFirst(Matrix<int>{}); emptyFirst.solve();
    Task2 emptySecond(Matrix<int>{}); emptySecond.solve();
    EXPECT_TRUE(emptyFirst.getMatrix().empty());
    EXPECT_TRUE(emptySecond.getMatrix().empty());
    Task1 singleFirst(makeMatrix({{1}})); singleFirst.solve();
    Task2 singleSecond(makeMatrix({{1}})); singleSecond.solve();
    EXPECT_TRUE(singleFirst.getMatrix().empty() || (singleFirst.getMatrix().rows() > 0U && singleFirst.getMatrix().columns() > 0U));
    EXPECT_TRUE(singleSecond.getMatrix().empty() || (singleSecond.getMatrix().rows() > 0U && singleSecond.getMatrix().columns() > 0U));
}

TEST(VariantOperations, RejectsOverflow) {
    Task1 task(makeMatrix({{50000}}));
    EXPECT_THROW(task.solve(), std::overflow_error);
}

