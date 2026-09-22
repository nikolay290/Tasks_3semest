#pragma once

#include "Exercise.h"
#include <string>

namespace miit::algebra {

    /**
     * @brief Задание 2: вставить после каждой нечетной строки первую строку
     */
    class Task2 : public Exercise {
    public:
        Task2() = default;

        /**
         * @brief Выполняет задание: вставляет копию первой строки после каждой нечетной строки
         */
        void solve() override;

        /**
         * @brief Возвращает описание задания
         */
        std::string getDescription() const override;
    };

} // namespace miit::algebra