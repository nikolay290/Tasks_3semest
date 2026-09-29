#pragma once
//
// Discipline.h
// Класс "Дисциплина" - предмет, который читает кафедра.
//
// Дисциплина принадлежит кафедре, ведущей занятия по ней. Это основа
// второго запроса варианта "ВУЗ" (список предметов кафедры).
//
#include <string>

class Department;

/**
 * @brief Дисциплина ВУЗа с её названием и кафедрой-владельцем.
 */
class Discipline {
public:
    /**
     * @brief Создаёт дисциплину.
     * @param name       Название дисциплины.
     * @param department Кафедра, читающая эту дисциплину.
     * @throws std::invalid_argument если название пустое или @p department
     *         равен nullptr.
     */
    Discipline(const std::string& name, Department* department);

    /** @brief Возвращает название дисциплины. */
    const std::string& GetName() const;

    /** @brief Возвращает кафедру, читающую дисциплину. */
    const Department* GetDepartment() const;

    /**
     * @brief Проверяет, что дисциплина зарегистрирована у своей кафедры.
     * @return true, если кафедра-владелец возвращает этот же объект.
     */
    bool IsReadByOwnDepartment() const;

    /** @brief Возвращает краткое представление: название дисциплины. */
    std::string GetShortInfo() const;

    /**
     * @brief Формирует описание дисциплины с названием кафедры.
     * @return Отформатированная строка с описанием.
     */
    std::string GetInfo() const;

private:
    std::string name_;
    Department* department_;
};
