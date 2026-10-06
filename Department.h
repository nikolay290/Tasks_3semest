#pragma once
//
// Department.h
// Класс "Кафедра" ВУЗ��.
//
// Кафедра ведёт занятия по нескольким дисциплинам и имеет штат
// преподавателей. Кафедра владеет своими дисциплинами (композиция).
//
#include <memory>
#include <string>
#include <vector>

class Discipline;
class Teacher;

/**
 * @brief Кафедра ВУЗа с её дисциплинами и преподавателями.
 */
class Department {
public:
    /**
     * @brief Создаёт кафедру.
     * @param name Название кафедры.
     * @throws std::invalid_argument если название пустое.
     */
    explicit Department(const std::string& name);

    /** @brief Возвращает название кафедры. */
    const std::string& GetName() const;

    /**
     * @brief Заводит новую дисциплину кафедры.
     * @param disciplineName Название дисциплины.
     * @return Умный указатель на созданную дисциплину.
     * @throws std::invalid_argument если название пустое или такая
     *         дисциплина уже читается кафедрой.
     */
    std::shared_ptr<Discipline> AddDiscipline(const std::string& disciplineName);

    /** @brief Возвращает дисциплины, читаемые кафедрой. */
    const std::vector<std::shared_ptr<Discipline>>& GetDisciplines() const;

    /** @brief Возвращает число дисциплин кафедры. */
    size_t GetDisciplineCount() const;

    /**
     * @brief Регистрирует преподавателя кафедры (ссылка не владеющая:
     *        преподавателями владеет University).
     * @param teacher Преподаватель для регистрации.
     * @return true, если преподаватель добавлен; false, если уже был в списке.
     * @throws std::invalid_argument если @p teacher равен nullptr.
     */
    bool AddTeacher(Teacher* teacher);

    /** @brief Возвращает преподавателей кафедры. */
    const std::vector<Teacher*>& GetTeachers() const;

    /** @brief Возвращает число преподавателей кафедры. */
    size_t GetTeacherCount() const;

    /**
     * @brief Ищет дисциплину кафедры по названию (регистронезависимо).
     * @param disciplineName Название дисциплины.
     * @return Умный указатель на дисциплину либо nullptr, если не найдена.
     */
    std::shared_ptr<Discipline> FindDisciplineByName(const std::string& disciplineName) const;

    /** @brief Возвращает краткое представление: название кафедры. */
    std::string GetShortInfo() const;

    /**
     * @brief Формирует описание кафедры с перечнем читаемых дисциплин.
     * @return Отформатированная строка с описанием.
     */
    std::string GetInfo() const;

private:
    std::string name_;
    std::vector<std::shared_ptr<Discipline>> disciplines_;
    std::vector<Teacher*> teachers_;
};
