#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include "Matrix.h"
#include "Task1.h"
#include "Task2.h"
#include "RandomGenerator.h"
#include "IStreamGenerator.h"
#include "ConstantGenerator.h"
#include "ZeroGenerator.h"

using namespace miit::algebra;

// ============================================================
// Тесты для конструкторов Matrix
// ============================================================

TEST(MatrixConstructorTest, DefaultConstructor) {
    Matrix<int> m;
    EXPECT_EQ(m.getRows(), 0);
    EXPECT_EQ(m.getCols(), 0);
}

TEST(MatrixConstructorTest, ParameterizedConstructor) {
    Matrix<int> m(3, 4);
    EXPECT_EQ(m.getRows(), 3);
    EXPECT_EQ(m.getCols(), 4);
}

TEST(MatrixConstructorTest, CopyConstructor) {
    Matrix<int> m1(2, 3);
    m1[0][0] = 1;
    m1[0][1] = 2;
    m1[0][2] = 3;
    m1[1][0] = 4;
    m1[1][1] = 5;
    m1[1][2] = 6;

    Matrix<int> m2(m1);
    EXPECT_EQ(m2.getRows(), 2);
    EXPECT_EQ(m2.getCols(), 3);
    EXPECT_EQ(m2[0][0], 1);
    EXPECT_EQ(m2[0][1], 2);
    EXPECT_EQ(m2[0][2], 3);
    EXPECT_EQ(m2[1][0], 4);
    EXPECT_EQ(m2[1][1], 5);
    EXPECT_EQ(m2[1][2], 6);
}

TEST(MatrixConstructorTest, MoveConstructor) {
    Matrix<int> m1(2, 3);
    m1[0][0] = 1;
    m1[0][1] = 2;
    m1[0][2] = 3;

    Matrix<int> m2(std::move(m1));
    EXPECT_EQ(m2.getRows(), 2);
    EXPECT_EQ(m2.getCols(), 3);
    EXPECT_EQ(m2[0][0], 1);
    EXPECT_EQ(m2[0][1], 2);
    EXPECT_EQ(m2[0][2], 3);
    EXPECT_EQ(m1.getRows(), 0);
    EXPECT_EQ(m1.getCols(), 0);
}

// ============================================================
// Тесты для операторов присваивания Matrix
// ============================================================

TEST(MatrixAssignmentTest, CopyAssignment) {
    Matrix<int> m1(2, 2);
    m1[0][0] = 1;
    m1[0][1] = 2;
    m1[1][0] = 3;
    m1[1][1] = 4;

    Matrix<int> m2;
    m2 = m1;
    EXPECT_EQ(m2.getRows(), 2);
    EXPECT_EQ(m2.getCols(), 2);
    EXPECT_EQ(m2[0][0], 1);
    EXPECT_EQ(m2[0][1], 2);
    EXPECT_EQ(m2[1][0], 3);
    EXPECT_EQ(m2[1][1], 4);
}

TEST(MatrixAssignmentTest, MoveAssignment) {
    Matrix<int> m1(2, 2);
    m1[0][0] = 1;
    m1[0][1] = 2;
    m1[1][0] = 3;
    m1[1][1] = 4;

    Matrix<int> m2;
    m2 = std::move(m1);
    EXPECT_EQ(m2.getRows(), 2);
    EXPECT_EQ(m2.getCols(), 2);
    EXPECT_EQ(m2[0][0], 1);
    EXPECT_EQ(m2[0][1], 2);
    EXPECT_EQ(m2[1][0], 3);
    EXPECT_EQ(m2[1][1], 4);
    EXPECT_EQ(m1.getRows(), 0);
    EXPECT_EQ(m1.getCols(), 0);
}

TEST(MatrixAssignmentTest, SelfAssignment) {
    Matrix<int> m(2, 2);
    m[0][0] = 1;
    m[0][1] = 2;
    m[1][0] = 3;
    m[1][1] = 4;

    m = m;
    EXPECT_EQ(m[0][0], 1);
    EXPECT_EQ(m[0][1], 2);
    EXPECT_EQ(m[1][0], 3);
    EXPECT_EQ(m[1][1], 4);
}

// ============================================================
// Тесты для оператора доступа по индексу []
// ============================================================

TEST(MatrixIndexTest, OperatorBrackets) {
    Matrix<int> m(3, 3);
    m[0][0] = 1;
    m[0][1] = 2;
    m[0][2] = 3;
    m[1][0] = 4;
    m[1][1] = 5;
    m[1][2] = 6;
    m[2][0] = 7;
    m[2][1] = 8;
    m[2][2] = 9;

    EXPECT_EQ(m[0][0], 1);
    EXPECT_EQ(m[0][1], 2);
    EXPECT_EQ(m[0][2], 3);
    EXPECT_EQ(m[1][0], 4);
    EXPECT_EQ(m[1][1], 5);
    EXPECT_EQ(m[1][2], 6);
    EXPECT_EQ(m[2][0], 7);
    EXPECT_EQ(m[2][1], 8);
    EXPECT_EQ(m[2][2], 9);
}

TEST(MatrixIndexTest, ConstOperatorBrackets) {
    Matrix<int> m(2, 2);
    m[0][0] = 1;
    m[0][1] = 2;
    m[1][0] = 3;
    m[1][1] = 4;

    const Matrix<int>& constRef = m;
    EXPECT_EQ(constRef[0][0], 1);
    EXPECT_EQ(constRef[0][1], 2);
    EXPECT_EQ(constRef[1][0], 3);
    EXPECT_EQ(constRef[1][1], 4);
}

TEST(MatrixIndexTest, OutOfRange) {
    Matrix<int> m(2, 2);
    // operator[] проверяет границы по строкам; индекс столбца после
    // этого адресует обычный std::vector, поэтому здесь проверяется
    // именно выход за границу по строкам.
    EXPECT_THROW(m[2], std::out_of_range);
    EXPECT_THROW(m[5], std::out_of_range);
}

// ============================================================
// Тесты для операторов циклического сдвига << и >>
// ============================================================

TEST(MatrixShiftTest, LeftShift) {
    Matrix<int> m(2, 3);
    m[0] = { 1, 2, 3 };
    m[1] = { 4, 5, 6 };

    Matrix<int> shifted = m << 1U;
    EXPECT_EQ(shifted.getRows(), 2);
    EXPECT_EQ(shifted.getCols(), 3);
    EXPECT_EQ(shifted[0][0], 2);
    EXPECT_EQ(shifted[0][1], 3);
    EXPECT_EQ(shifted[0][2], 1);
    EXPECT_EQ(shifted[1][0], 5);
    EXPECT_EQ(shifted[1][1], 6);
    EXPECT_EQ(shifted[1][2], 4);
}

TEST(MatrixShiftTest, RightShift) {
    Matrix<int> m(2, 3);
    m[0] = { 1, 2, 3 };
    m[1] = { 4, 5, 6 };

    Matrix<int> shifted = m >> 1U;
    EXPECT_EQ(shifted[0][0], 3);
    EXPECT_EQ(shifted[0][1], 1);
    EXPECT_EQ(shifted[0][2], 2);
    EXPECT_EQ(shifted[1][0], 6);
    EXPECT_EQ(shifted[1][1], 4);
    EXPECT_EQ(shifted[1][2], 5);
}

// ============================================================
// Тесты для методов getRows и getCols
// ============================================================

TEST(MatrixGetMethodsTest, GetRowsAndCols) {
    Matrix<int> m(5, 7);
    EXPECT_EQ(m.getRows(), 5);
    EXPECT_EQ(m.getCols(), 7);

    m.resize(3, 4);
    EXPECT_EQ(m.getRows(), 3);
    EXPECT_EQ(m.getCols(), 4);
}

// ============================================================
// Тесты для метода resize
// ============================================================

TEST(MatrixResizeTest, ResizeLarger) {
    Matrix<int> m(2, 2);
    m[0][0] = 1;
    m[0][1] = 2;
    m[1][0] = 3;
    m[1][1] = 4;

    m.resize(3, 3);
    EXPECT_EQ(m.getRows(), 3);
    EXPECT_EQ(m.getCols(), 3);
    EXPECT_EQ(m[0][0], 1);
    EXPECT_EQ(m[0][1], 2);
    EXPECT_EQ(m[1][0], 3);
    EXPECT_EQ(m[1][1], 4);
}

TEST(MatrixResizeTest, ResizeSmaller) {
    Matrix<int> m(3, 3);
    m[0][0] = 1;
    m[0][1] = 2;
    m[0][2] = 3;
    m[1][0] = 4;
    m[1][1] = 5;
    m[1][2] = 6;
    m[2][0] = 7;
    m[2][1] = 8;
    m[2][2] = 9;

    m.resize(2, 2);
    EXPECT_EQ(m.getRows(), 2);
    EXPECT_EQ(m.getCols(), 2);
    EXPECT_EQ(m[0][0], 1);
    EXPECT_EQ(m[0][1], 2);
    EXPECT_EQ(m[1][0], 4);
    EXPECT_EQ(m[1][1], 5);
}

// ============================================================
// Тесты для метода fill
// ============================================================

TEST(MatrixFillTest, FillWithValue) {
    Matrix<int> m(2, 3);
    m.fill(42);

    for (size_t i = 0; i < m.getRows(); ++i) {
        for (size_t j = 0; j < m.getCols(); ++j) {
            EXPECT_EQ(m[i][j], 42);
        }
    }
}

TEST(MatrixFillTest, FillWithZero) {
    Matrix<int> m(3, 3);
    m[0][0] = 1;
    m[0][1] = 2;
    m[0][2] = 3;
    m[1][0] = 4;
    m[1][1] = 5;
    m[1][2] = 6;
    m[2][0] = 7;
    m[2][1] = 8;
    m[2][2] = 9;

    m.fill(0);
    for (size_t i = 0; i < m.getRows(); ++i) {
        for (size_t j = 0; j < m.getCols(); ++j) {
            EXPECT_EQ(m[i][j], 0);
        }
    }
}

// ============================================================
// Тесты для метода fill(Generator&)
// ============================================================

TEST(MatrixFillGeneratorTest, FillWithConstantGenerator) {
    Matrix<int> m(2, 3);
    ConstantGenerator gen(7);
    m.fill(gen);

    for (size_t i = 0; i < m.getRows(); ++i) {
        for (size_t j = 0; j < m.getCols(); ++j) {
            EXPECT_EQ(m[i][j], 7);
        }
    }
}

TEST(MatrixFillGeneratorTest, FillWithRandomGenerator) {
    Matrix<int> m(4, 4);
    RandomGenerator gen(1, 10);
    m.fill(gen);

    for (size_t i = 0; i < m.getRows(); ++i) {
        for (size_t j = 0; j < m.getCols(); ++j) {
            EXPECT_GE(m[i][j], 1);
            EXPECT_LE(m[i][j], 10);
        }
    }
}

TEST(MatrixFillGeneratorTest, FillWithIStreamGenerator) {
    std::stringstream ss;
    ss << "1 2 3 4 5 6";

    Matrix<int> m(2, 3);
    IStreamGenerator gen(ss);
    m.fill(gen);

    EXPECT_EQ(m[0][0], 1);
    EXPECT_EQ(m[0][1], 2);
    EXPECT_EQ(m[0][2], 3);
    EXPECT_EQ(m[1][0], 4);
    EXPECT_EQ(m[1][1], 5);
    EXPECT_EQ(m[1][2], 6);
}

TEST(MatrixFillGeneratorTest, FillWithZeroGenerator) {
    Matrix<int> m(2, 2);
    ZeroGenerator gen;
    m.fill(gen);

    EXPECT_EQ(m[0][0], 0);
    EXPECT_EQ(m[1][1], 0);
}

// ============================================================
// Тесты для метода toString
// ============================================================

TEST(MatrixToStringTest, ToStringWithData) {
    Matrix<int> m(2, 3);
    m[0][0] = 1;
    m[0][1] = 2;
    m[0][2] = 3;
    m[1][0] = 4;
    m[1][1] = 5;
    m[1][2] = 6;

    std::string expected = "1 2 3 \n4 5 6 \n";
    EXPECT_EQ(m.toString(), expected);
}

TEST(MatrixToStringTest, ToStringEmpty) {
    Matrix<int> m;
    EXPECT_EQ(m.toString(), "");
}

TEST(MatrixToStringTest, ToStringWithNegative) {
    Matrix<int> m(2, 2);
    m[0][0] = -1;
    m[0][1] = -2;
    m[1][0] = -3;
    m[1][1] = -4;

    std::string expected = "-1 -2 \n-3 -4 \n";
    EXPECT_EQ(m.toString(), expected);
}

// ============================================================
// Тесты для метода replaceFirstThreeColumnsWithSquares (задание 1)
// ============================================================

TEST(MatrixReplaceThreeColumnsTest, BasicCase) {
    Matrix<int> m(2, 4);
    m[0] = { 2, 3, 4, 5 };
    m[1] = { 1, 2, 3, 4 };

    m.replaceFirstThreeColumnsWithSquares();

    EXPECT_EQ(m[0][0], 4);
    EXPECT_EQ(m[0][1], 9);
    EXPECT_EQ(m[0][2], 16);
    EXPECT_EQ(m[0][3], 5);
    EXPECT_EQ(m[1][0], 1);
    EXPECT_EQ(m[1][1], 4);
    EXPECT_EQ(m[1][2], 9);
    EXPECT_EQ(m[1][3], 4);
}

TEST(MatrixReplaceThreeColumnsTest, LessThanThreeColumns) {
    Matrix<int> m(2, 2);
    m[0] = { 2, 3 };
    m[1] = { 4, 5 };

    m.replaceFirstThreeColumnsWithSquares();

    EXPECT_EQ(m[0][0], 4);
    EXPECT_EQ(m[0][1], 9);
    EXPECT_EQ(m[1][0], 16);
    EXPECT_EQ(m[1][1], 25);
}

TEST(MatrixReplaceThreeColumnsTest, ExactlyThreeColumns) {
    Matrix<int> m(2, 3);
    m[0] = { 2, 3, 4 };
    m[1] = { 5, 6, 7 };

    m.replaceFirstThreeColumnsWithSquares();

    EXPECT_EQ(m[0][0], 4);
    EXPECT_EQ(m[0][1], 9);
    EXPECT_EQ(m[0][2], 16);
    EXPECT_EQ(m[1][0], 25);
    EXPECT_EQ(m[1][1], 36);
    EXPECT_EQ(m[1][2], 49);
}

TEST(MatrixReplaceThreeColumnsTest, SingleRow) {
    Matrix<int> m(1, 3);
    m[0] = { 3, 4, 5 };

    m.replaceFirstThreeColumnsWithSquares();

    EXPECT_EQ(m[0][0], 9);
    EXPECT_EQ(m[0][1], 16);
    EXPECT_EQ(m[0][2], 25);
}

TEST(MatrixReplaceThreeColumnsTest, NegativeNumbers) {
    Matrix<int> m(2, 3);
    m[0] = { -2, -3, -4 };
    m[1] = { 1, -2, 3 };

    m.replaceFirstThreeColumnsWithSquares();

    EXPECT_EQ(m[0][0], 4);
    EXPECT_EQ(m[0][1], 9);
    EXPECT_EQ(m[0][2], 16);
    EXPECT_EQ(m[1][0], 1);
    EXPECT_EQ(m[1][1], 4);
    EXPECT_EQ(m[1][2], 9);
}

TEST(MatrixReplaceThreeColumnsTest, EmptyMatrix) {
    Matrix<int> m;
    m.replaceFirstThreeColumnsWithSquares();
    EXPECT_EQ(m.getRows(), 0);
    EXPECT_EQ(m.getCols(), 0);
}

// ============================================================
// Тесты для метода insertFirstRowAfterOddRows (задание 2)
// ============================================================

TEST(MatrixInsertFirstRowTest, BasicCase) {
    Matrix<int> m(3, 2);
    m[0] = { 1, 2 };
    m[1] = { 3, 4 };
    m[2] = { 5, 6 };

    Matrix<int> result = m.insertFirstRowAfterOddRows();

    // Строки с индексами 0, 2 - нечетные (вставляем после них)
    // Исходные строки: 0, 1, 2
    // После вставки: 0, (первая), 1, 2, (первая)
    EXPECT_EQ(result.getRows(), 5);
    EXPECT_EQ(result.getCols(), 2);

    EXPECT_EQ(result[0][0], 1);
    EXPECT_EQ(result[0][1], 2);
    EXPECT_EQ(result[1][0], 1);
    EXPECT_EQ(result[1][1], 2);
    EXPECT_EQ(result[2][0], 3);
    EXPECT_EQ(result[2][1], 4);
    EXPECT_EQ(result[3][0], 5);
    EXPECT_EQ(result[3][1], 6);
    EXPECT_EQ(result[4][0], 1);
    EXPECT_EQ(result[4][1], 2);
}

TEST(MatrixInsertFirstRowTest, EvenNumberOfRows) {
    Matrix<int> m(4, 2);
    m[0] = { 1, 2 };
    m[1] = { 3, 4 };
    m[2] = { 5, 6 };
    m[3] = { 7, 8 };

    Matrix<int> result = m.insertFirstRowAfterOddRows();

    // Нечетные строки: 0, 2
    // Вставка после 0 и 2
    // Результат: 0, (первая), 1, 2, (первая), 3
    EXPECT_EQ(result.getRows(), 6);
    EXPECT_EQ(result.getCols(), 2);

    EXPECT_EQ(result[0][0], 1);
    EXPECT_EQ(result[0][1], 2);
    EXPECT_EQ(result[1][0], 1);
    EXPECT_EQ(result[1][1], 2);
    EXPECT_EQ(result[2][0], 3);
    EXPECT_EQ(result[2][1], 4);
    EXPECT_EQ(result[3][0], 5);
    EXPECT_EQ(result[3][1], 6);
    EXPECT_EQ(result[4][0], 1);
    EXPECT_EQ(result[4][1], 2);
    EXPECT_EQ(result[5][0], 7);
    EXPECT_EQ(result[5][1], 8);
}

TEST(MatrixInsertFirstRowTest, SingleRow) {
    Matrix<int> m(1, 3);
    m[0] = { 1, 2, 3 };

    Matrix<int> result = m.insertFirstRowAfterOddRows();

    // Строка 0 - нечетная, вставляем после неё
    EXPECT_EQ(result.getRows(), 2);
    EXPECT_EQ(result.getCols(), 3);
    EXPECT_EQ(result[0][0], 1);
    EXPECT_EQ(result[0][1], 2);
    EXPECT_EQ(result[0][2], 3);
    EXPECT_EQ(result[1][0], 1);
    EXPECT_EQ(result[1][1], 2);
    EXPECT_EQ(result[1][2], 3);
}

TEST(MatrixInsertFirstRowTest, TwoRows) {
    Matrix<int> m(2, 2);
    m[0] = { 1, 2 };
    m[1] = { 3, 4 };

    Matrix<int> result = m.insertFirstRowAfterOddRows();

    // Строка 0 - нечетная, вставляем после неё
    // Строка 1 - четная, ничего не вставляем
    EXPECT_EQ(result.getRows(), 3);
    EXPECT_EQ(result.getCols(), 2);
    EXPECT_EQ(result[0][0], 1);
    EXPECT_EQ(result[0][1], 2);
    EXPECT_EQ(result[1][0], 1);
    EXPECT_EQ(result[1][1], 2);
    EXPECT_EQ(result[2][0], 3);
    EXPECT_EQ(result[2][1], 4);
}

TEST(MatrixInsertFirstRowTest, EmptyMatrix) {
    Matrix<int> m;
    Matrix<int> result = m.insertFirstRowAfterOddRows();
    EXPECT_EQ(result.getRows(), 0);
    EXPECT_EQ(result.getCols(), 0);
}

TEST(MatrixInsertFirstRowTest, SingleColumn) {
    Matrix<int> m(3, 1);
    m[0] = { 1 };
    m[1] = { 2 };
    m[2] = { 3 };

    Matrix<int> result = m.insertFirstRowAfterOddRows();

    EXPECT_EQ(result.getRows(), 5);
    EXPECT_EQ(result.getCols(), 1);
    EXPECT_EQ(result[0][0], 1);
    EXPECT_EQ(result[1][0], 1);
    EXPECT_EQ(result[2][0], 2);
    EXPECT_EQ(result[3][0], 3);
    EXPECT_EQ(result[4][0], 1);
}

// ============================================================
// Тесты для класса Task1
// ============================================================

TEST(Task1Test, SolveBasic) {
    Matrix<int> m(2, 4);
    m[0] = { 2, 3, 4, 5 };
    m[1] = { -2, 0, 1, 6 };

    Task1 task;
    task.setMatrix(m);
    task.solve();

    Matrix<int> result = task.getMatrix();

    EXPECT_EQ(result[0][0], 4);
    EXPECT_EQ(result[0][1], 9);
    EXPECT_EQ(result[0][2], 16);
    EXPECT_EQ(result[0][3], 5);
    EXPECT_EQ(result[1][0], 4);
    EXPECT_EQ(result[1][1], 0);
    EXPECT_EQ(result[1][2], 1);
    EXPECT_EQ(result[1][3], 6);
}

TEST(Task1Test, GetDescription) {
    Task1 task;
    EXPECT_EQ(task.getDescription(),
        "Заменить все элементы первых трех столбцов на их квадраты");
}

// ============================================================
// Тесты для класса Task2
// ============================================================

TEST(Task2Test, SolveBasic) {
    Matrix<int> m(3, 2);
    m[0] = { 1, 2 };
    m[1] = { 3, 4 };
    m[2] = { 5, 6 };

    Task2 task;
    task.setMatrix(m);
    task.solve();

    Matrix<int> result = task.getMatrix();

    EXPECT_EQ(result.getRows(), 5);
    EXPECT_EQ(result.getCols(), 2);
    EXPECT_EQ(result[0][0], 1);
    EXPECT_EQ(result[0][1], 2);
    EXPECT_EQ(result[1][0], 1);
    EXPECT_EQ(result[1][1], 2);
    EXPECT_EQ(result[2][0], 3);
    EXPECT_EQ(result[2][1], 4);
    EXPECT_EQ(result[3][0], 5);
    EXPECT_EQ(result[3][1], 6);
    EXPECT_EQ(result[4][0], 1);
    EXPECT_EQ(result[4][1], 2);
}

TEST(Task2Test, GetDescription) {
    Task2 task;
    EXPECT_EQ(task.getDescription(), "Вставить первую строку после каждой нечетной строки");
}

// ============================================================
// Тесты для класса RandomGenerator
// ============================================================

TEST(RandomGeneratorTest, GenerateInRange) {
    RandomGenerator gen(1, 10);
    for (int i = 0; i < 100; ++i) {
        int value = gen.generate();
        EXPECT_GE(value, 1);
        EXPECT_LE(value, 10);
    }
}

TEST(RandomGeneratorTest, GenerateSingleValue) {
    RandomGenerator gen(5, 5);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(gen.generate(), 5);
    }
}

TEST(RandomGeneratorTest, GenerateNegativeRange) {
    RandomGenerator gen(-10, -5);
    for (int i = 0; i < 100; ++i) {
        int value = gen.generate();
        EXPECT_GE(value, -10);
        EXPECT_LE(value, -5);
    }
}

// ============================================================
// Тесты для класса IStreamGenerator
// ============================================================

TEST(IStreamGeneratorTest, GenerateFromStringStream) {
    std::stringstream ss;
    ss << "42";

    IStreamGenerator gen(ss);
    EXPECT_EQ(gen.generate(), 42);
}

TEST(IStreamGeneratorTest, GenerateMultipleValues) {
    std::stringstream ss;
    ss << "10 20 30";

    IStreamGenerator gen(ss);
    EXPECT_EQ(gen.generate(), 10);
    EXPECT_EQ(gen.generate(), 20);
    EXPECT_EQ(gen.generate(), 30);
}

TEST(IStreamGeneratorTest, GenerateNegativeValue) {
    std::stringstream ss;
    ss << "-42";

    IStreamGenerator gen(ss);
    EXPECT_EQ(gen.generate(), -42);
}

// ============================================================
// Тесты для класса ConstantGenerator
// ============================================================

TEST(ConstantGeneratorTest, GenerateReturnsConstructorValue) {
    ConstantGenerator gen(7);
    EXPECT_EQ(gen.generate(), 7);
    EXPECT_EQ(gen.generate(), 7);
    EXPECT_EQ(gen.generate(), 7);
}

TEST(ConstantGeneratorTest, DefaultValueIsZero) {
    ConstantGenerator gen;
    EXPECT_EQ(gen.generate(), 0);
}

TEST(ConstantGeneratorTest, SetValueChangesGeneratedValue) {
    ConstantGenerator gen(1);
    gen.setValue(99);
    EXPECT_EQ(gen.generate(), 99);
    EXPECT_EQ(gen.getValue(), 99);
}

// ============================================================
// Тесты для класса ZeroGenerator
// ============================================================

TEST(ZeroGeneratorTest, Generate) {
    ZeroGenerator zero;
    EXPECT_EQ(zero.generate(), 0);
    EXPECT_EQ(zero.generate(), 0);
}

// ============================================================
// Тесты для класса Generator (полиморфизм)
// ============================================================

TEST(GeneratorTest, WorksPolymorphically) {
    std::unique_ptr<Generator> polymorphic = std::make_unique<ConstantGenerator>(11);
    EXPECT_EQ(polymorphic->generate(), 11);
}

// ============================================================
// Тесты для класса Exercise (абстрактный - тестируем через Task1)
// ============================================================

TEST(ExerciseTest, SetAndGetMatrix) {
    Matrix<int> m(2, 2);
    m[0][0] = 1;
    m[0][1] = 2;
    m[1][0] = 3;
    m[1][1] = 4;

    Task1 task;
    task.setMatrix(m);
    const Matrix<int>& result = task.getMatrix();

    EXPECT_EQ(result[0][0], 1);
    EXPECT_EQ(result[0][1], 2);
    EXPECT_EQ(result[1][0], 3);
    EXPECT_EQ(result[1][1], 4);
}

// ============================================================
// Тесты для агрегации генератора в Exercise
// ============================================================

TEST(ExerciseApi, FillAndGetMatrix) {
    Task1 exercise;
    exercise.setMatrix(Matrix<int>(1, 2));
    exercise.setGenerator(std::make_unique<ConstantGenerator>(6));
    exercise.fill();

    const Matrix<int>& result = exercise.getMatrix();
    EXPECT_EQ(result[0][0], 6);
    EXPECT_EQ(result[0][1], 6);
}

TEST(ExerciseApi, FillRejectsMissingGenerator) {
    Task1 withoutGenerator;
    withoutGenerator.setMatrix(Matrix<int>(1, 2));
    EXPECT_THROW(withoutGenerator.fill(), std::logic_error);
}

// ============================================================
// Интеграционные тесты
// ============================================================

TEST(IntegrationTest, FullWorkflow) {
    // Создаем матрицу
    Matrix<int> m(2, 4);
    m[0] = { 2, 3, 4, 5 };
    m[1] = { -2, 0, 1, 6 };

    // Применяем задание 1
    Task1 task1;
    task1.setMatrix(m);
    task1.solve();
    Matrix<int> result1 = task1.getMatrix();

    // Проверяем результат
    EXPECT_EQ(result1[0][0], 4);
    EXPECT_EQ(result1[1][1], 0);
    EXPECT_EQ(result1[0][3], 5);

    // Применяем задание 2 к результату
    Task2 task2;
    task2.setMatrix(result1);
    task2.solve();
    Matrix<int> result2 = task2.getMatrix();

    // Обе операции применились без ошибок
    EXPECT_EQ(result2.getRows(), 3);
    EXPECT_EQ(result2.getCols(), 4);
}

TEST(IntegrationTest, MultipleOperations) {
    // Создаем матрицу
    Matrix<int> m(3, 4);
    m[0] = { 2, -3, 4, 1 };
    m[1] = { 5, -2, 3, 6 };
    m[2] = { -1, 0, 2, -4 };

    // 1. Заменяем первые 3 столбца на квадраты
    Matrix<int> m1 = m;
    m1.replaceFirstThreeColumnsWithSquares();

    // Проверяем
    EXPECT_EQ(m1[0][0], 4);
    EXPECT_EQ(m1[0][1], 9);
    EXPECT_EQ(m1[0][2], 16);
    EXPECT_EQ(m1[0][3], 1);

    // 2. Вставляем первую строку после нечетных
    Matrix<int> m2 = m.insertFirstRowAfterOddRows();
    EXPECT_EQ(m2.getRows(), 5);

    // 3. Применяем задания
    Task1 task1;
    task1.setMatrix(m);
    task1.solve();

    Task2 task2;
    task2.setMatrix(task1.getMatrix());
    task2.solve();

    // Проверяем, что операции выполнились без ошибок
    SUCCEED();
}

// ============================================================
// Тесты на производительность (стресс-тесты)
// ============================================================

TEST(StressTest, LargeMatrixReplaceFirstThreeColumns) {
    const size_t size = 100;
    Matrix<int> m(size, size);

    RandomGenerator gen(1, 1000);
    m.fill(gen);

    // Просто проверяем, что операция выполняется без ошибок
    EXPECT_NO_THROW(m.replaceFirstThreeColumnsWithSquares());
}

TEST(StressTest, LargeMatrixInsertFirstRow) {
    const size_t size = 100;
    Matrix<int> m(size, size);

    RandomGenerator gen(1, 1000);
    m.fill(gen);

    EXPECT_NO_THROW(m.insertFirstRowAfterOddRows());
}

// ============================================================
// Тесты на корректность данных (валидация)
// ============================================================

TEST(ValidationTest, MatrixValuesNotChangedAfterGetMethods) {
    Matrix<int> m(2, 2);
    m[0][0] = 1;
    m[0][1] = 2;
    m[1][0] = 3;
    m[1][1] = 4;

    m.getRows();
    m.getCols();
    m.toString();

    // Значения не должны измениться
    EXPECT_EQ(m[0][0], 1);
    EXPECT_EQ(m[0][1], 2);
    EXPECT_EQ(m[1][0], 3);
    EXPECT_EQ(m[1][1], 4);
}

TEST(ValidationTest, CopyDoesNotAffectOriginal) {
    Matrix<int> m1(2, 2);
    m1[0][0] = 1;
    m1[0][1] = 2;
    m1[1][0] = 3;
    m1[1][1] = 4;

    Matrix<int> m2 = m1;
    m2[0][0] = 99;

    // Оригинал не должен измениться
    EXPECT_EQ(m1[0][0], 1);
    EXPECT_EQ(m2[0][0], 99);
}

// ============================================================
// Точка входа для тестов
// ============================================================

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}