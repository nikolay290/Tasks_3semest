#pragma once

namespace miit::algebra {
    /**
     * @brief Базовый класс генератора значений
     */
    class Generator {
    public:
        /**
         * @brief Деструктор
         */
        virtual ~Generator();

        /**
         * @brief Генерирует очередное значение (чисто виртуальный)
         * @return сгенерированное значение
         */
        virtual int generate() = 0;
    };
}
