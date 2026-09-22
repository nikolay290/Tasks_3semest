#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

#include <iostream>
#include <string>
#include <cstdlib>
#include "Matrix.h"
#include "RandomGenerator.h"
#include "IStreamGenerator.h"
#include "ZeroGenerator.h"
#include "ConstantGenerator.h"
#include "Task1.h"
#include "Task2.h"

using namespace miit::algebra;
constexpr int PRESET_CONSTANT_VALUE = 7;

// Способ заполнения матрицы
enum class FillMethod {
    Random = 1,
    Manual = 2,
    Zeros = 3,
    Constant = 4
};

size_t getSize(const std::string& message);
FillMethod getChoice();
void demonstrateExercise(Exercise& exercise, const Matrix<int>& original,
    const std::string& taskName);

int main() {
#ifdef _WIN32
    // Включаем ввод/вывод в кодировке UTF-8, чтобы русский текст
    // корректно отображался в консоли Windows.
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    try {

        size_t rows = getSize("Введите количество строк: ");
        size_t cols = getSize("Введите количество столбцов: ");

        Matrix<int> matrix(rows, cols);

        FillMethod choice = getChoice();

        switch (choice) {
        case FillMethod::Random: {
            std::cout << "Введите минимальное значение: ";
            int min;
            std::cin >> min;
            std::cout << "Введите максимальное значение: ";
            int max;
            std::cin >> max;
            if (min > max) {
                std::cerr << "Ошибка: минимальное значение больше максимального\n";
                return 1;
            }
            RandomGenerator generator(min, max);
            matrix.fill(generator);
            break;
        }
        case FillMethod::Manual: {
            std::cout << "Введите элементы матрицы (через пробел):\n";
            IStreamGenerator generator(std::cin);
            matrix.fill(generator);
            break;
        }
        case FillMethod::Zeros: {
            std::cout << "Заполнение нулями\n";
            ZeroGenerator generator;
            matrix.fill(generator);
            break;
        }
        case FillMethod::Constant: {
            std::cout << "Используем константное значение из кода: "
                << PRESET_CONSTANT_VALUE << "\n";
            ConstantGenerator generator(PRESET_CONSTANT_VALUE);
            matrix.fill(generator);
            break;
        }
        default:
            std::cerr << "Ошибка: неверный выбор\n";
            return 1;
        }

        std::cout << "\nСозданная матрица:\n";
        std::cout << matrix.toString() << "\n";

        Task1 task1;
        demonstrateExercise(task1, matrix, "Задание 1");

        Task2 task2;
        demonstrateExercise(task2, matrix, "Задание 2");

        std::cout << "=============================================\n";
        std::cout << "  Дополнительная демонстрация (из задания 4.3)\n";
        std::cout << "=============================================\n";

        Matrix<int> copyMatrix = matrix;

        std::cout << "\nИсходная матрица:\n";
        std::cout << copyMatrix.toString() << "\n";

        copyMatrix.replaceFirstThreeColumnsWithSquares();
        std::cout << "После замены первых трех столбцов на квадраты:\n";
        std::cout << copyMatrix.toString() << "\n";

        Matrix<int> insertedMatrix = matrix.insertFirstRowAfterOddRows();
        std::cout << "После вставки первой строки после каждой нечетной:\n";
        std::cout << insertedMatrix.toString() << "\n";

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\nНажмите Enter для выхода...";
    std::cin.get();
    std::cin.get();

    return 0;
}
size_t getSize(const std::string& message) {
    std::cout << message;
    int value = 0;
    std::cin >> value;
    if (value <= 0) {
        std::cerr << "Ошибка: размер должен быть положительным\n";
        exit(1);
    }
    return static_cast<size_t>(value);
}
FillMethod getChoice() {
    std::cout << "Выберите способ заполнения массива:\n"
        << static_cast<int>(FillMethod::Random) << " - случайными числами\n"
        << static_cast<int>(FillMethod::Manual) << " - вводом с клавиатуры\n"
        << static_cast<int>(FillMethod::Zeros) << " - нулями\n"
        << static_cast<int>(FillMethod::Constant) << " - константным значением (задано в коде)\n"
        << "Ваш выбор: ";
    int choice;
    std::cin >> choice;
    return static_cast<FillMethod>(choice);
}
void demonstrateExercise(Exercise& exercise, const Matrix<int>& original,
    const std::string& taskName) {
    std::cout << "\n=== " << taskName << " ===\n";
    std::cout << "Описание: " << exercise.getDescription() << "\n\n";

    std::cout << "Исходная матрица:\n";
    std::cout << original.toString() << "\n";

    exercise.setMatrix(original);
    exercise.solve();

    std::cout << "Результат:\n";
    std::cout << exercise.getMatrix().toString() << "\n";
}