#pragma once
//
// Teacher.h
// Класс "Преподаватель" - тип-наследник Person.
//
// Преподаватель работает на кафедре, ведёт занятия в группах. Лабораторные
// работы он ведёт вместе со вторым преподавателем (см. Lesson).
//
#include <string>
#include <vector>
#include "Person.h"
#include "Date.h"

class Department;
class Group;

/**
 * @brief Преподаватель ВУЗ��: ФИО, дата рождения, должность, учёная степень
 *        и кафедра, на которой он работает.
 */
class Teacher : public Person {
public:
    /**
     * @brief Создаёт преподавателя.
     * @param fullName   ФИО преподавателя.
     * @param birthDate  Дата рождения.
     * @param position   Должность ("профессор", "доцент" и т.п.).
     * @param degree     Учёная степень; может быть пустой строкой.
     * @param department Кафедра, на которой работает преподаватель.
     * @throws std::invalid_argument если ФИО или должность пустые,
     *         либо @p department равен nullptr.
     */
    Teacher(const std::string& fullName, Date birthDate, const std::string& position,
        const std::string& degree, const Department* department);

    /** @brief Возвращает должность. */
    const std::string& GetPosition() const;

    /** @brief Возвращает учёную степень (может быть пустой). */
    const std::string& GetDegree() const;

    /** @brief Возвращает кафедру, на которой работает преподаватель. */
    const Department* GetDepartment() const;

    /**
     * @brief Фиксирует, что преподаватель ведёт занятия в группе.
     *        Заполняется автоматически при добавлении занятия (Lesson).
     * @param group Группа, в которой ведутся занятия.
     * @return true, если группа добавлена; false, если она уже была в списке.
     * @throws std::invalid_argument если @p group равен nullptr.
     */
    bool AddGroup(const Group* group);

    /** @brief Возвращает группы, в которых преподаватель ведёт занятия. */
    const std::vector<const Group*>& GetGroups() const;

    /**
     * @brief Проверяет, ведёт ли преподаватель занятия в указанной группе.
     * @param group Проверяемая группа.
     * @return true, если @p group есть в списке групп преподавателя.
     */
    bool TeachesInGroup(const Group* group) const;

    /** @brief Возвращает true, если преподаватель является преподавателем. */
    bool IsTeacher() const override;

    /** @brief Возвращает "Преподаватель". */
    std::string GetRole() const override;

    /**
     * @brief Формирует описание преподавателя: ФИО, должность, степень,
     *        кафедра и число групп, в которых он ведёт занятия.
     * @return Отформатированная строка с описанием.
     */
    std::string GetInfo() const override;

    /** @brief Возвращает краткое представление: "Фамилия И. О. (должность)". */
    std::string GetShortInfo() const override;

protected:
    /** @brief Часть описания, специфичная для преподавателя: должность и степень. */
    std::string GetSpecificInfo() const override;

private:
    std::string position_;
    std::string degree_;
    const Department* department_;
    std::vector<const Group*> groups_;
};
