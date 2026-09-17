#include "ConstantGenerator.h"
#include "IStreamGenerator.h"
#include "RandomGenerator.h"
#include "Task1.h"
#include "Task2.h"
#include "ZeroGenerator.h"
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>

using namespace miit::algebra;

/**
 * @brief Считывает значение из стандартного ввода с повторной попыткой при ошибке
 * @param prompt - текст к вводу
 * @return считанное значение типа T
 */
template<typename T>
T readValue(const char* prompt) {
    for (;;) {
        std::cout << prompt;
        T value{};
        if (std::cin >> value) return value;
        if (std::cin.eof()) throw std::runtime_error("Input ended");
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid value. Try again.\n";
    }
}

/**
 * @brief Точка входа в демонстрационную программу
 * @return код возврата (0 - успех, 1 - ошибка)
 */
int main() {
    try {
        std::size_t rows = 0U;
        std::size_t columns = 0U;
        while (rows == 0U) rows = readValue<std::size_t>("Rows (> 0): ");
        while (columns == 0U) columns = readValue<std::size_t>("Columns (> 0): ");

        std::cout << "Fill: 1 - keyboard, 2 - random, 3 - zeros, 4 - constant\n";
        const int fillChoice = readValue<int>("Choice: ");
        std::unique_ptr<Generator> generator;
        switch (fillChoice) {
        case 1:
            std::cout << "Enter " << rows * columns << " integers:\n";
            generator = std::make_unique<IStreamGenerator>(std::cin);
            break;
        case 2: {
            const int minimum = readValue<int>("Minimum: ");
            const int maximum = readValue<int>("Maximum: ");
            generator = std::make_unique<RandomGenerator>(minimum, maximum);
            break;
        }
        case 3:
            generator = std::make_unique<ZeroGenerator>();
            break;
        case 4:
            generator = std::make_unique<ConstantGenerator>(readValue<int>("Constant: "));
            break;
        default:
            std::cerr << "Unknown fill method\n";
            return 1;
        }

        Matrix<int> original(rows, columns);
        original.fill(*generator);
        std::cout << "Initial matrix:\n" << original.toString() << '\n';
        for (;;) {
            std::cout << "\n0 - exit, 1 - first transformation, 2 - second transformation, 3 - both\n";
            const int action = readValue<int>("Action: ");
            if (action == 0) break;
            if (action == 1 || action == 3) {
                Task1 task(original);
                task.solve();
                std::cout << "Task 1 result:\n" << task.getMatrix().toString() << '\n';
            }
            if (action == 2 || action == 3) {
                Task2 task(original);
                task.solve();
                std::cout << "Task 2 result:\n" << task.getMatrix().toString() << '\n';
            }
            if (action < 1 || action > 3) std::cout << "Unknown action\n";
        }
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
