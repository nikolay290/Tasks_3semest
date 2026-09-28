#include "CppUnitTest.h"

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#include "ConstantGenerator.h"
#include "IStreamGenerator.h"
#include "Matrix.h"
#include "RandomGenerator.h"
#include "Task1.h"
#include "Task2.h"
#include "ZeroGenerator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace miit::algebra;

namespace MatrixTests {

    namespace {
        /**
         * @brief Преобразование std::string в std::wstring
         *        (для сравнения строк через Assert::AreEqual)
         * @param value строка в кодировке UTF-8
         * @return строка в кодировке UTF-16
         */
        std::wstring ToWide(const std::string& value) {
            std::wstring result;
            result.reserve(value.size());
            for (const char ch : value) {
                result.push_back(static_cast<wchar_t>(static_cast<unsigned char>(ch)));
            }
            return result;
        }
    }

    // ============================================================
    // Тесты для конструкторов Matrix
    // ============================================================

    TEST_CLASS(MatrixConstructorTest) {
    public:
        TEST_METHOD(DefaultConstructor) {
            Matrix<int> m;
            Assert::AreEqual(static_cast<size_t>(0), m.getRows());
            Assert::AreEqual(static_cast<size_t>(0), m.getCols());
        }

        TEST_METHOD(ParameterizedConstructor) {
            Matrix<int> m(3, 4);
            Assert::AreEqual(static_cast<size_t>(3), m.getRows());
            Assert::AreEqual(static_cast<size_t>(4), m.getCols());
        }

        TEST_METHOD(CopyConstructor) {
            Matrix<int> m1(2, 3);
            m1[0][0] = 1;
            m1[0][1] = 2;
            m1[0][2] = 3;
            m1[1][0] = 4;
            m1[1][1] = 5;
            m1[1][2] = 6;

            Matrix<int> m2(m1);
            Assert::AreEqual(static_cast<size_t>(2), m2.getRows());
            Assert::AreEqual(static_cast<size_t>(3), m2.getCols());
            Assert::AreEqual(1, m2[0][0]);
            Assert::AreEqual(2, m2[0][1]);
            Assert::AreEqual(3, m2[0][2]);
            Assert::AreEqual(4, m2[1][0]);
            Assert::AreEqual(5, m2[1][1]);
            Assert::AreEqual(6, m2[1][2]);
        }

        TEST_METHOD(MoveConstructor) {
            Matrix<int> m1(2, 3);
            m1[0][0] = 1;
            m1[0][1] = 2;
            m1[0][2] = 3;

            Matrix<int> m2(std::move(m1));
            Assert::AreEqual(static_cast<size_t>(2), m2.getRows());
            Assert::AreEqual(static_cast<size_t>(3), m2.getCols());
            Assert::AreEqual(1, m2[0][0]);
            Assert::AreEqual(2, m2[0][1]);
            Assert::AreEqual(3, m2[0][2]);
            Assert::AreEqual(static_cast<size_t>(0), m1.getRows());
            Assert::AreEqual(static_cast<size_t>(0), m1.getCols());
        }
    };

    // ============================================================
    // Тесты для операторов присваивания Matrix
    // ============================================================

    TEST_CLASS(MatrixAssignmentTest) {
    public:
        TEST_METHOD(CopyAssignment) {
            Matrix<int> m1(2, 2);
            m1[0][0] = 1;
            m1[0][1] = 2;
            m1[1][0] = 3;
            m1[1][1] = 4;

            Matrix<int> m2;
            m2 = m1;
            Assert::AreEqual(static_cast<size_t>(2), m2.getRows());
            Assert::AreEqual(static_cast<size_t>(2), m2.getCols());
            Assert::AreEqual(1, m2[0][0]);
            Assert::AreEqual(2, m2[0][1]);
            Assert::AreEqual(3, m2[1][0]);
            Assert::AreEqual(4, m2[1][1]);
        }

        TEST_METHOD(MoveAssignment) {
            Matrix<int> m1(2, 2);
            m1[0][0] = 1;
            m1[0][1] = 2;
            m1[1][0] = 3;
            m1[1][1] = 4;

            Matrix<int> m2;
            m2 = std::move(m1);
            Assert::AreEqual(static_cast<size_t>(2), m2.getRows());
            Assert::AreEqual(static_cast<size_t>(2), m2.getCols());
            Assert::AreEqual(1, m2[0][0]);
            Assert::AreEqual(2, m2[0][1]);
            Assert::AreEqual(3, m2[1][0]);
            Assert::AreEqual(4, m2[1][1]);
            Assert::AreEqual(static_cast<size_t>(0), m1.getRows());
            Assert::AreEqual(static_cast<size_t>(0), m1.getCols());
        }

        TEST_METHOD(SelfAssignment) {
            Matrix<int> m(2, 2);
            m[0][0] = 1;
            m[0][1] = 2;
            m[1][0] = 3;
            m[1][1] = 4;

            m = m;
            Assert::AreEqual(1, m[0][0]);
            Assert::AreEqual(2, m[0][1]);
            Assert::AreEqual(3, m[1][0]);
            Assert::AreEqual(4, m[1][1]);
        }
    };

    // ============================================================
    // Тесты для оператора доступа по индексу []
    // ============================================================

    TEST_CLASS(MatrixIndexTest) {
    public:
        TEST_METHOD(OperatorBrackets) {
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

            Assert::AreEqual(1, m[0][0]);
            Assert::AreEqual(2, m[0][1]);
            Assert::AreEqual(3, m[0][2]);
            Assert::AreEqual(4, m[1][0]);
            Assert::AreEqual(5, m[1][1]);
            Assert::AreEqual(6, m[1][2]);
            Assert::AreEqual(7, m[2][0]);
            Assert::AreEqual(8, m[2][1]);
            Assert::AreEqual(9, m[2][2]);
        }

        TEST_METHOD(ConstOperatorBrackets) {
            Matrix<int> m(2, 2);
            m[0][0] = 1;
            m[0][1] = 2;
            m[1][0] = 3;
            m[1][1] = 4;

            const Matrix<int>& constRef = m;
            Assert::AreEqual(1, constRef[0][0]);
            Assert::AreEqual(2, constRef[0][1]);
            Assert::AreEqual(3, constRef[1][0]);
            Assert::AreEqual(4, constRef[1][1]);
        }

        TEST_METHOD(OutOfRange) {
            Matrix<int> m(2, 2);
            // operator[] проверяет границы по строкам; индекс столбца после
            // этого адресует обычный std::vector, поэтому здесь проверяется
            // именно выход за границу по строкам.
            Assert::ExpectException<std::out_of_range>([&] { (void)m[2]; });
            Assert::ExpectException<std::out_of_range>([&] { (void)m[5]; });
        }
    };

    // ============================================================
    // Тесты для операторов циклического сдвига << и >>
    // ============================================================

    TEST_CLASS(MatrixShiftTest) {
    public:
        TEST_METHOD(LeftShift) {
            Matrix<int> m(2, 3);
            m[0] = { 1, 2, 3 };
            m[1] = { 4, 5, 6 };

            Matrix<int> shifted = m << 1U;
            Assert::AreEqual(static_cast<size_t>(2), shifted.getRows());
            Assert::AreEqual(static_cast<size_t>(3), shifted.getCols());
            Assert::AreEqual(2, shifted[0][0]);
            Assert::AreEqual(3, shifted[0][1]);
            Assert::AreEqual(1, shifted[0][2]);
            Assert::AreEqual(5, shifted[1][0]);
            Assert::AreEqual(6, shifted[1][1]);
            Assert::AreEqual(4, shifted[1][2]);
        }

        TEST_METHOD(RightShift) {
            Matrix<int> m(2, 3);
            m[0] = { 1, 2, 3 };
            m[1] = { 4, 5, 6 };

            Matrix<int> shifted = m >> 1U;
            Assert::AreEqual(3, shifted[0][0]);
            Assert::AreEqual(1, shifted[0][1]);
            Assert::AreEqual(2, shifted[0][2]);
            Assert::AreEqual(6, shifted[1][0]);
            Assert::AreEqual(4, shifted[1][1]);
            Assert::AreEqual(5, shifted[1][2]);
        }
    };

    // ============================================================
    // Тесты для методов getRows и getCols
    // ============================================================

    TEST_CLASS(MatrixGetMethodsTest) {
    public:
        TEST_METHOD(GetRowsAndCols) {
            Matrix<int> m(5, 7);
            Assert::AreEqual(static_cast<size_t>(5), m.getRows());
            Assert::AreEqual(static_cast<size_t>(7), m.getCols());

            m.resize(3, 4);
            Assert::AreEqual(static_cast<size_t>(3), m.getRows());
            Assert::AreEqual(static_cast<size_t>(4), m.getCols());
        }
    };

    // ============================================================
    // Тесты для метода resize
    // ============================================================

    TEST_CLASS(MatrixResizeTest) {
    public:
        TEST_METHOD(ResizeLarger) {
            Matrix<int> m(2, 2);
            m[0][0] = 1;
            m[0][1] = 2;
            m[1][0] = 3;
            m[1][1] = 4;

            m.resize(3, 3);
            Assert::AreEqual(static_cast<size_t>(3), m.getRows());
            Assert::AreEqual(static_cast<size_t>(3), m.getCols());
            Assert::AreEqual(1, m[0][0]);
            Assert::AreEqual(2, m[0][1]);
            Assert::AreEqual(3, m[1][0]);
            Assert::AreEqual(4, m[1][1]);
        }

        TEST_METHOD(ResizeSmaller) {
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
            Assert::AreEqual(static_cast<size_t>(2), m.getRows());
            Assert::AreEqual(static_cast<size_t>(2), m.getCols());
            Assert::AreEqual(1, m[0][0]);
            Assert::AreEqual(2, m[0][1]);
            Assert::AreEqual(4, m[1][0]);
            Assert::AreEqual(5, m[1][1]);
        }
    };

    // ============================================================
    // Тесты для метода fill(Generator&)
    // ============================================================

    TEST_CLASS(MatrixFillGeneratorTest) {
    public:
        TEST_METHOD(FillWithConstantGenerator) {
            Matrix<int> m(2, 3);
            ConstantGenerator gen(7);
            m.fill(gen);

            for (size_t i = 0; i < m.getRows(); ++i) {
                for (size_t j = 0; j < m.getCols(); ++j) {
                    Assert::AreEqual(7, m[i][j]);
                }
            }
        }

        TEST_METHOD(FillWithRandomGenerator) {
            Matrix<int> m(4, 4);
            RandomGenerator gen(1, 10);
            m.fill(gen);

            for (size_t i = 0; i < m.getRows(); ++i) {
                for (size_t j = 0; j < m.getCols(); ++j) {
                    Assert::IsTrue(m[i][j] >= 1);
                    Assert::IsTrue(m[i][j] <= 10);
                }
            }
        }

        TEST_METHOD(FillWithIStreamGenerator) {
            std::stringstream ss;
            ss << "1 2 3 4 5 6";

            Matrix<int> m(2, 3);
            IStreamGenerator gen(ss);
            m.fill(gen);

            Assert::AreEqual(1, m[0][0]);
            Assert::AreEqual(2, m[0][1]);
            Assert::AreEqual(3, m[0][2]);
            Assert::AreEqual(4, m[1][0]);
            Assert::AreEqual(5, m[1][1]);
            Assert::AreEqual(6, m[1][2]);
        }

        TEST_METHOD(FillWithZeroGenerator) {
            Matrix<int> m(2, 2);
            ZeroGenerator gen;
            m.fill(gen);

            Assert::AreEqual(0, m[0][0]);
            Assert::AreEqual(0, m[1][1]);
        }
    };

    // ============================================================
    // Тесты для метода toString
    // ============================================================

    TEST_CLASS(MatrixToStringTest) {
    public:
        TEST_METHOD(ToStringWithData) {
            Matrix<int> m(2, 3);
            m[0][0] = 1;
            m[0][1] = 2;
            m[0][2] = 3;
            m[1][0] = 4;
            m[1][1] = 5;
            m[1][2] = 6;

            std::string expected = "1 2 3 \n4 5 6 \n";
            Assert::AreEqual(ToWide(expected), ToWide(m.toString()));
        }

        TEST_METHOD(ToStringEmpty) {
            Matrix<int> m;
            Assert::AreEqual(ToWide(""), ToWide(m.toString()));
        }

        TEST_METHOD(ToStringWithNegative) {
            Matrix<int> m(2, 2);
            m[0][0] = -1;
            m[0][1] = -2;
            m[1][0] = -3;
            m[1][1] = -4;

            std::string expected = "-1 -2 \n-3 -4 \n";
            Assert::AreEqual(ToWide(expected), ToWide(m.toString()));
        }
    };

    // ============================================================
    // Тесты задания 1: замена первых трёх столбцов на квадраты
    // (выполняется через класс Task1, как и в приложении)
    // ============================================================

    TEST_CLASS(Task1ReplaceColumnsTest) {
    public:
        TEST_METHOD(BasicCase) {
            Matrix<int> m(2, 4);
            m[0] = { 2, 3, 4, 5 };
            m[1] = { 1, 2, 3, 4 };

            Task1 task(m);
            task.solve();
            const Matrix<int>& result = task.getMatrix();

            Assert::AreEqual(4, result[0][0]);
            Assert::AreEqual(9, result[0][1]);
            Assert::AreEqual(16, result[0][2]);
            Assert::AreEqual(5, result[0][3]);
            Assert::AreEqual(1, result[1][0]);
            Assert::AreEqual(4, result[1][1]);
            Assert::AreEqual(9, result[1][2]);
            Assert::AreEqual(4, result[1][3]);
        }

        TEST_METHOD(LessThanThreeColumns) {
            Matrix<int> m(2, 2);
            m[0] = { 2, 3 };
            m[1] = { 4, 5 };

            Task1 task(m);
            task.solve();
            const Matrix<int>& result = task.getMatrix();

            Assert::AreEqual(4, result[0][0]);
            Assert::AreEqual(9, result[0][1]);
            Assert::AreEqual(16, result[1][0]);
            Assert::AreEqual(25, result[1][1]);
        }

        TEST_METHOD(ExactlyThreeColumns) {
            Matrix<int> m(2, 3);
            m[0] = { 2, 3, 4 };
            m[1] = { 5, 6, 7 };

            Task1 task(m);
            task.solve();
            const Matrix<int>& result = task.getMatrix();

            Assert::AreEqual(4, result[0][0]);
            Assert::AreEqual(9, result[0][1]);
            Assert::AreEqual(16, result[0][2]);
            Assert::AreEqual(25, result[1][0]);
            Assert::AreEqual(36, result[1][1]);
            Assert::AreEqual(49, result[1][2]);
        }

        TEST_METHOD(SingleRow) {
            Matrix<int> m(1, 3);
            m[0] = { 3, 4, 5 };

            Task1 task(m);
            task.solve();
            const Matrix<int>& result = task.getMatrix();

            Assert::AreEqual(9, result[0][0]);
            Assert::AreEqual(16, result[0][1]);
            Assert::AreEqual(25, result[0][2]);
        }

        TEST_METHOD(NegativeNumbers) {
            Matrix<int> m(2, 3);
            m[0] = { -2, -3, -4 };
            m[1] = { 1, -2, 3 };

            Task1 task(m);
            task.solve();
            const Matrix<int>& result = task.getMatrix();

            Assert::AreEqual(4, result[0][0]);
            Assert::AreEqual(9, result[0][1]);
            Assert::AreEqual(16, result[0][2]);
            Assert::AreEqual(1, result[1][0]);
            Assert::AreEqual(4, result[1][1]);
            Assert::AreEqual(9, result[1][2]);
        }

        TEST_METHOD(EmptyMatrix) {
            // Пустая матрица: задание 1 проверяет непустоту и бросает исключение
            Matrix<int> m;
            Task1 task(m);
            Assert::ExpectException<std::runtime_error>([&] { task.solve(); });
        }
    };

    // ============================================================
    // Тесты для класса Task1
    // ============================================================

    TEST_CLASS(Task1Test) {
    public:
        TEST_METHOD(SolveBasic) {
            Matrix<int> m(2, 4);
            m[0] = { 2, 3, 4, 5 };
            m[1] = { -2, 0, 1, 6 };

            Task1 task(m);
            task.solve();

            Matrix<int> result = task.getMatrix();

            Assert::AreEqual(4, result[0][0]);
            Assert::AreEqual(9, result[0][1]);
            Assert::AreEqual(16, result[0][2]);
            Assert::AreEqual(5, result[0][3]);
            Assert::AreEqual(4, result[1][0]);
            Assert::AreEqual(0, result[1][1]);
            Assert::AreEqual(1, result[1][2]);
            Assert::AreEqual(6, result[1][3]);
        }

        TEST_METHOD(GetDescription) {
            Task1 task(Matrix<int>{});
            Assert::AreEqual(
                ToWide("Заменить все элементы первых трех столбцов на их квадраты"),
                ToWide(task.getDescription()));
        }
    };

    // ============================================================
    // Тесты для класса Task2
    // ============================================================

    TEST_CLASS(Task2Test) {
    public:
        TEST_METHOD(SolveBasic) {
            Matrix<int> m(3, 2);
            m[0] = { 1, 2 };
            m[1] = { 3, 4 };
            m[2] = { 5, 6 };

            Task2 task(m);
            task.solve();

            Matrix<int> result = task.getMatrix();

            Assert::AreEqual(static_cast<size_t>(5), result.getRows());
            Assert::AreEqual(static_cast<size_t>(2), result.getCols());
            Assert::AreEqual(1, result[0][0]);
            Assert::AreEqual(2, result[0][1]);
            Assert::AreEqual(1, result[1][0]);
            Assert::AreEqual(2, result[1][1]);
            Assert::AreEqual(3, result[2][0]);
            Assert::AreEqual(4, result[2][1]);
            Assert::AreEqual(5, result[3][0]);
            Assert::AreEqual(6, result[3][1]);
            Assert::AreEqual(1, result[4][0]);
            Assert::AreEqual(2, result[4][1]);
        }

        TEST_METHOD(GetDescription) {
            Task2 task(Matrix<int>{});
            Assert::AreEqual(
                ToWide("Вставить первую строку после каждой нечетной строки"),
                ToWide(task.getDescription()));
        }
    };

    // ============================================================
    // Тесты для класса RandomGenerator
    // ============================================================

    TEST_CLASS(RandomGeneratorTest) {
    public:
        TEST_METHOD(GenerateInRange) {
            RandomGenerator gen(1, 10);
            for (int i = 0; i < 100; ++i) {
                int value = gen.generate();
                Assert::IsTrue(value >= 1);
                Assert::IsTrue(value <= 10);
            }
        }

        TEST_METHOD(GenerateSingleValue) {
            RandomGenerator gen(5, 5);
            for (int i = 0; i < 10; ++i) {
                Assert::AreEqual(5, gen.generate());
            }
        }

        TEST_METHOD(GenerateNegativeRange) {
            RandomGenerator gen(-10, -5);
            for (int i = 0; i < 100; ++i) {
                int value = gen.generate();
                Assert::IsTrue(value >= -10);
                Assert::IsTrue(value <= -5);
            }
        }
    };

    // ============================================================
    // Тесты для класса IStreamGenerator
    // ============================================================

    TEST_CLASS(IStreamGeneratorTest) {
    public:
        TEST_METHOD(GenerateFromStringStream) {
            std::stringstream ss;
            ss << "42";

            IStreamGenerator gen(ss);
            Assert::AreEqual(42, gen.generate());
        }

        TEST_METHOD(GenerateMultipleValues) {
            std::stringstream ss;
            ss << "10 20 30";

            IStreamGenerator gen(ss);
            Assert::AreEqual(10, gen.generate());
            Assert::AreEqual(20, gen.generate());
            Assert::AreEqual(30, gen.generate());
        }

        TEST_METHOD(GenerateNegativeValue) {
            std::stringstream ss;
            ss << "-42";

            IStreamGenerator gen(ss);
            Assert::AreEqual(-42, gen.generate());
        }
    };

    // ============================================================
    // Тесты для класса ConstantGenerator
    // ============================================================

    TEST_CLASS(ConstantGeneratorTest) {
    public:
        TEST_METHOD(GenerateReturnsConstructorValue) {
            ConstantGenerator gen(7);
            Assert::AreEqual(7, gen.generate());
            Assert::AreEqual(7, gen.generate());
            Assert::AreEqual(7, gen.generate());
        }

        TEST_METHOD(DefaultValueIsZero) {
            ConstantGenerator gen;
            Assert::AreEqual(0, gen.generate());
        }
    };

    // ============================================================
    // Тесты для класса ZeroGenerator
    // ============================================================

    TEST_CLASS(ZeroGeneratorTest) {
    public:
        TEST_METHOD(Generate) {
            ZeroGenerator zero;
            Assert::AreEqual(0, zero.generate());
            Assert::AreEqual(0, zero.generate());
        }
    };

    // ============================================================
    // Тесты для класса Generator (полиморфизм)
    // ============================================================

    TEST_CLASS(GeneratorTest) {
    public:
        TEST_METHOD(WorksPolymorphically) {
            std::unique_ptr<Generator> polymorphic = std::make_unique<ConstantGenerator>(11);
            Assert::AreEqual(11, polymorphic->generate());
        }
    };

    // ============================================================
    // Тесты для класса Exercise (абстрактный - тестируем через Task1)
    // ============================================================

    TEST_CLASS(ExerciseTest) {
    public:
        TEST_METHOD(SetAndGetMatrix) {
            Matrix<int> m(2, 2);
            m[0][0] = 1;
            m[0][1] = 2;
            m[1][0] = 3;
            m[1][1] = 4;

            Task1 task(Matrix<int>{});
            task.setMatrix(m);
            const Matrix<int>& result = task.getMatrix();

            Assert::AreEqual(1, result[0][0]);
            Assert::AreEqual(2, result[0][1]);
            Assert::AreEqual(3, result[1][0]);
            Assert::AreEqual(4, result[1][1]);
        }
    };

    // ============================================================
    // Интеграционные тесты
    // ============================================================

    TEST_CLASS(IntegrationTest) {
    public:
        TEST_METHOD(FullWorkflow) {
            // Создаем матрицу
            Matrix<int> m(2, 4);
            m[0] = { 2, 3, 4, 5 };
            m[1] = { -2, 0, 1, 6 };

            // Применяем задание 1
            Task1 task1(m);
            task1.solve();
            Matrix<int> result1 = task1.getMatrix();

            // Проверяем результат
            Assert::AreEqual(4, result1[0][0]);
            Assert::AreEqual(0, result1[1][1]);
            Assert::AreEqual(5, result1[0][3]);

            // Применяем задание 2 к результату
            Task2 task2(result1);
            task2.solve();
            Matrix<int> result2 = task2.getMatrix();

            // Обе операции применились без ошибок
            Assert::AreEqual(static_cast<size_t>(3), result2.getRows());
            Assert::AreEqual(static_cast<size_t>(4), result2.getCols());
        }

        TEST_METHOD(MultipleOperations) {
            // Создаем матрицу
            Matrix<int> m(3, 4);
            m[0] = { 2, -3, 4, 1 };
            m[1] = { 5, -2, 3, 6 };
            m[2] = { -1, 0, 2, -4 };

            // 1. Заменяем первые 3 столбца на квадраты (задание 1)
            Task1 squareTask(m);
            squareTask.solve();
            const Matrix<int>& m1 = squareTask.getMatrix();

            // Проверяем
            Assert::AreEqual(4, m1[0][0]);
            Assert::AreEqual(9, m1[0][1]);
            Assert::AreEqual(16, m1[0][2]);
            Assert::AreEqual(1, m1[0][3]);

            // 2. Вставляем первую строку после нечетных (операция в Task2)
            Task2 insertTask(m);
            insertTask.solve();
            Assert::AreEqual(static_cast<size_t>(5), insertTask.getMatrix().getRows());

            // 3. Применяем задания
            Task1 task1(m);
            task1.solve();

            Task2 task2(task1.getMatrix());
            task2.solve();

            // Проверяем, что операции выполнились без ошибок
        }
    };

    // ============================================================
    // Тесты на производительность (стресс-тесты)
    // ============================================================

    TEST_CLASS(StressTest) {
    public:
        TEST_METHOD(LargeMatrixReplaceFirstThreeColumns) {
            const size_t size = 100;
            Matrix<int> m(size, size);

            RandomGenerator gen(1, 1000);
            m.fill(gen);

            // Просто проверяем, что задание 1 выполняется без ошибок
            Task1 task(m);
            try {
                task.solve();
            }
            catch (...) {
                Assert::Fail(L"Task1::solve() бросил исключение");
            }
        }

        TEST_METHOD(LargeMatrixInsertFirstRow) {
            const size_t size = 100;
            Matrix<int> m(size, size);

            RandomGenerator gen(1, 1000);
            m.fill(gen);

            Task2 task(m);
            try {
                task.solve();
            }
            catch (...) {
                Assert::Fail(L"task.solve() бросил исключение");
            }
        }
    };

    // ============================================================
    // Тесты на корректность данных (валидация)
    // ============================================================

    TEST_CLASS(ValidationTest) {
    public:
        TEST_METHOD(MatrixValuesNotChangedAfterGetMethods) {
            Matrix<int> m(2, 2);
            m[0][0] = 1;
            m[0][1] = 2;
            m[1][0] = 3;
            m[1][1] = 4;

            m.getRows();
            m.getCols();
            m.toString();

            // Значения не должны измениться
            Assert::AreEqual(1, m[0][0]);
            Assert::AreEqual(2, m[0][1]);
            Assert::AreEqual(3, m[1][0]);
            Assert::AreEqual(4, m[1][1]);
        }

        TEST_METHOD(CopyDoesNotAffectOriginal) {
            Matrix<int> m1(2, 2);
            m1[0][0] = 1;
            m1[0][1] = 2;
            m1[1][0] = 3;
            m1[1][1] = 4;

            Matrix<int> m2 = m1;
            m2[0][0] = 99;

            // Оригинал не должен измениться
            Assert::AreEqual(1, m1[0][0]);
            Assert::AreEqual(99, m2[0][0]);
        }
    };
