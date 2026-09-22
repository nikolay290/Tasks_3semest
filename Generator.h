#pragma once

namespace miit::algebra {

    /**
     * @brief Абстрактный генератор целочисленных значений
     *
     * Базовый класс для всех способов заполнения матрицы: случайными
     * числами (RandomGenerator), значениями с клавиатуры/из потока
     * (IStreamGenerator), нулями (ZeroGenerator) и константой
     * (ConstantGenerator). Конкретный алгоритм генерации реализуется
     * в классе-наследнике.
     */
    class Generator {
    public:
        /**
         * @brief Виртуальный деструктор
         */
        virtual ~Generator() = 0;

        /**
         * @brief Сгенерировать очередное значение
         * @return сгенерированное значение
         */
        virtual int generate() = 0;
    };

} // namespace miit::algebra