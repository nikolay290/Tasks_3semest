#pragma once
//
// Group.h
// Класс "Группа" - объединение студентов, обучающихся на одной специальности.
//
// Группа владеет своими студентами (композиция) и знает свою специальность.
// Студенты группы хранятся как std::shared_ptr<Student>, чтобы на них могли
// ссылаться и University, и Specialty без дублирования владения.
//
#include <memory>
#include <string>
#include <vector>
#include "Date.h"

class Student;
class Specialty;

/**
 * @brief Группа студентов с её номером, специальностью и списком студентов.
 */
class Group {
public:
    /**
     * @brief Создаёт группу и сразу регистрирует её в списке групп
     *        специальности @p specialty. Благодаря этому сохраняется
     *        инвариант "каждая группа обучается ровно на одной
     *        специальности и присутствует в её списке" независимо от
     *        того, создана группа напрямую или через University.
     * @param number    Номер группы (например, "1И").
     * @param specialty Специальность, на которой обучается группа.
     * @throws std::invalid_argument если номер пуст или @p specialty равен nullptr.
     */
    Group(const std::string& number, Specialty* specialty);

    /** @brief Возвращает номер группы. */
    const std::string& GetNumber() const;

    /** @brief Возвращает специальность группы. */
    const Specialty* GetSpecialty() const;

    /**
     * @brief Добавляет студента в группу.
     * @param fullName   ФИО студента.
     * @param birthDate  Дата рождения.
     * @param recordBook Номер зачётной книжки.
     * @param course     Номер курса.
     * @return Умный указатель на созданного студента.
     * @throws std::invalid_argument если данные некорректны или зачётная
     *         книжка уже есть в группе.
     */
    std::shared_ptr<Student> AddStudent(const std::string& fullName, Date birthDate,
        const std::string& recordBook, unsigned course);

    /** @brief Возвращает студентов группы. */
    const std::vector<std::shared_ptr<Student>>& GetStudents() const;

    /** @brief Возвращает количество студентов в группе. */
    size_t GetStudentCount() const;

    /**
     * @brief Ищет студента в группе по номеру зачётной книжки
     *        (регистронезависимо).
     * @param recordBook Номер зачётной книжки.
     * @return Умный указатель на студента либо nullptr, если не найден.
     */
    std::shared_ptr<Student> FindStudentByRecordBook(const std::string& recordBook) const;

    /**
     * @brief Ищет первого студента, чьё ФИО содержит @p fullName
     *        (регистронезависимо).
     * @param fullName ФИО или его часть.
     * @return Умный указатель на студента либо nullptr, если не найден.
     */
    std::shared_ptr<Student> FindStudentByFullName(const std::string& fullName) const;

    /**
     * @brief Возвращает средний балл группы.
     * @return Средний балл; 0.0, если в группе нет студентов.
     */
    double GetAverageMark() const;

    /** @brief Возвращает краткое представление: "номер (код специальности)". */
    std::string GetShortInfo() const;

    /**
     * @brief Формирует описание группы: номер, специальность, число студентов.
     * @return Отформатированная строка с описанием.
     */
    std::string GetInfo() const;

private:
    std::string number_;
    Specialty* specialty_;
    std::vector<std::shared_ptr<Student>> students_;
};
