#pragma once
//
// Lesson.h
// Класс "Занятие" - связка "дисциплина + группа + преподаватель".
//
// Предметная особенность ВУЗа: занятия ведут преподаватели с разных
// кафедр, а ЛАБОРАТОРНЫЕ РАБОТЫ проводят ДВА преподавателя (основной и
// второй). Это правило проверяется в конструкторе.
//
#include <string>
#include <vector>
#include "Date.h"

class Discipline;
class Group;
class Teacher;

/**
 * @brief Вид занятия.
 */
enum class LessonType {
    Lecture,    ///< Лекция
    Practical,  ///< Практическое занятие
    Lab         ///< Лабораторная работа (ведут два преподавателя)
};

/**
 * @brief Возвращает человекочитаемое название вида занятия.
 * @param type Вид занятия.
 * @return "Лекция" / "Практика" / "Лабораторная работа".
 */
const char* ToString(LessonType type);

/**
 * @brief Одно учебное занятие: дисциплина, группа, преподаватель(и),
 *        аудитория и дата проведения.
 */
class Lesson {
public:
    /**
     * @brief Создаёт занятие.
     * @param type         Вид занятия.
     * @param discipline   Дисциплина, по которой проводится занятие.
     * @param group        Группа-участник.
     * @param teacher      Основной преподаватель.
     * @param secondTeacher Второй преподаватель; обязателен для LessonType::Lab
     *                      и должен быть nullptr для остальных видов.
     * @param room         Аудитория (может быть пустой).
     * @param date         Дата проведения.
     * @throws std::invalid_argument при нарушении правил:
     *         - nullptr вместо discipline, group или teacher;
     *         - для лабораторной работы не задан второй преподаватель;
     *         - для не-лабораторной задан второй преподаватель;
     *         - второй преподаватель совпадает с основным.
     */
    Lesson(LessonType type, const Discipline* discipline, const Group* group,
        const Teacher* teacher, const Teacher* secondTeacher, const std::string& room, Date date);

    /** @brief Возвращает вид занятия. */
    LessonType GetType() const;

    /** @brief Возвращает дисциплину занятия. */
    const Discipline* GetDiscipline() const;

    /** @brief Возвращает группу-участника. */
    const Group* GetGroup() const;

    /** @brief Возвращает основного преподавателя. */
    const Teacher* GetTeacher() const;

    /** @brief Возвращает второго преподавателя (nullptr, если его нет). */
    const Teacher* GetSecondTeacher() const;

    /** @brief Возвращает основного и (если есть) второго преподавателя. */
    std::vector<const Teacher*> GetTeachers() const;

    /** @brief Возвращает число преподавателей занятия (1 или 2). */
    size_t GetTeacherCount() const;

    /** @brief Возвращает аудиторию. */
    const std::string& GetRoom() const;

    /** @brief Возвращает дату проведения. */
    const Date& GetDate() const;

    /**
     * @brief Проверяет, ведёт ли занятие указанный преподаватель (основной
     *        или второй).
     * @param teacher Проверяемый преподаватель.
     * @return true, если @p teacher ведёт это занятие.
     */
    bool IsTaughtBy(const Teacher* teacher) const;

    /** @brief Возвращает краткое представление: "вид: дисциплина". */
    std::string GetShortInfo() const;

    /**
     * @brief Формирует описание занятия: вид, дисциплина, группа,
     *        преподаватели, аудитория и дата.
     * @return Отформатированная строка с описанием.
     */
    std::string GetInfo() const;

private:
    /**
     * @brief Проверяет корректность связки занятия.
     * @throws std::invalid_argument при нарушении доменных правил.
     */
    static void Validate(LessonType type, const Discipline* discipline, const Group* group,
        const Teacher* teacher, const Teacher* secondTeacher);

    LessonType type_;
    const Discipline* discipline_;
    const Group* group_;
    const Teacher* teacher_;
    const Teacher* secondTeacher_;
    std::string room_;
    Date date_;
};
