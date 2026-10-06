#pragma once
//
// Student.h
// Класс "Студент" - тип-наследник Person.
//
// Студент состоит в группе и имеет номер зачётной книжки (по нему
// выполняется первый запрос варианта "ВУЗ"). Ссылка на группу - не владеющий
// указатель: группыми владеет University, чтобы не было циклического владения.
//
#include <string>
#include <vector>
#include "Person.h"
#include "Date.h"

class Group;

/**
 * @brief Студент ВУЗа: ФИО, дата рождения, номер зачётной книжки, курс
 *        и группа обучения.
 */
class Student : public Person {
public:
    /**
     * @brief Создаёт студента.
     * @param fullName   ФИО студента.
     * @param birthDate  Дата рождения.
     * @param recordBook Номер зачётной книжки.
     * @param course     Номер курса обучения (1-based).
     * @param group      Группа, в которой обучается студент.
     * @throws std::invalid_argument если ФИО или номер зачётной книжки
     *         пустые, course == 0 или @p group равен nullptr.
     */
    Student(const std::string& fullName, Date birthDate, const std::string& recordBook,
        unsigned course, const Group* group);

    /** @brief Возвращает номер зачётной книжки. */
    const std::string& GetRecordBook() const;

    /** @brief Возвращает номер курса обучения. */
    unsigned GetCourse() const;

    /** @brief Возвращает группу, в которой обучается студент. */
    const Group* GetGroup() const;

    /**
     * @brief Добавляет оценку студенту.
     * @param mark Оценка по пятибалльной шкале.
     * @throws std::invalid_argument если оценка вне диапазона 1..5.
     */
     void AddMark(const double mark);

    /**
     * @brief Возвращает средний балл студента.
     * @return Средний балл; 0.0, если оценок ещё не было.
     */
    double GetAverageMark() const;

    /** @brief Возвращает true, если студент является студентом. */
    bool IsStudent() const override;

    /** @brief Возвращает "Студент". */
    std::string GetRole() const override;

    /**
     * @brief Формирует описание студента: ФИО, зачётная книжка, курс,
     *        группа, специальность и средний балл.
     * @return Отформатированная строка с описанием.
     */
    std::string GetInfo() const override;

    /** @brief Возвращает краткое представление: "Фамилия И. О. (гр. 1И)". */
    std::string GetShortInfo() const override;

protected:
    /** @brief Часть описания, специфичная для студента: зачётная книжка и курс. */
    std::string GetSpecificInfo() const override;

private:
    std::string recordBook_;
    unsigned course_ = 0;
    const Group* group_;
    std::vector<double> marks_;
};
