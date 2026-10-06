#pragma once
//
// University.h
// Класс "University" - ВУЗ как единый агрегат предметной области.
//
// Владеет всеми объектами (кафедрами, дисциплинами, специальностями,
// группами, студентами, преподавателями, занятиями) и реализует четыре
// запроса варианта "ВУЗ":
//
//   1) выдавать информацию о студенте по № зачётной книжки и по ФИО;
//   2) выдавать список предметов, читаемых данной кафедрой;
//   3) выдавать список преподавателей, проводящих занятия в данной группе;
//   4) выдавать список групп, обучающихся на данной специальности.
//
// Класс не использует потоки ввода-вывода: все ответы возвращаются в виде
// std::string / контейнеров STL, а выводом занимается приложение.
//
#include <memory>
#include <string>
#include <vector>
#include "Date.h"
#include "Lesson.h"

class Department;
class Discipline;
class Group;
class Lesson;
class Person;
class Specialty;
class Student;
class Teacher;

class University {
public:
    /**
     * @brief Создаёт пустой ВУЗ с названием по умолчанию "ВУЗ".
     */
    University();

    /** @brief Виртуальный деструктор (корни дерева владения). */
    ~University();

    // ---- Создание объектов предметной области ----

    /**
     * @brief Добавляет специальность.
     * @param code Код специальности.
     * @param name Название специальности.
     * @return Умный указатель на созданную специальность.
     * @throws std::invalid_argument если такая специальность уже есть.
     */
    std::shared_ptr<Specialty> AddSpecialty(const std::string& code, const std::string& name);

    /**
     * @brief Добавляет кафедру.
     * @param name Название кафедры.
     * @return Умный указатель на созданную кафедру.
     * @throws std::invalid_argument если такая кафедра уже есть.
     */
    std::shared_ptr<Department> AddDepartment(const std::string& name);

    /**
     * @brief Создаёт группу на специальности и регистрирует её в ней.
     * @param number    Номер группы.
     * @param specialty Специальность, на которой обучается группа.
     * @return Умный указатель на созданную группу.
     * @throws std::invalid_argument если номер группы уже занят или
     *         @p specialty равен nullptr.
     */
    std::shared_ptr<Group> AddGroup(const std::string& number, Specialty* specialty);

    /**
     * @brief Добавляет преподавателя на кафедру.
     * @param fullName   ФИО.
     * @param birthDate  Дата рождения.
     * @param position   Должность.
     * @param degree     Учёная степень (может быть пустой).
     * @param department Кафедра, на которой работает преподаватель.
     * @return Умный указатель на созданного преподавателя.
     * @throws std::invalid_argument если @p department равен nullptr.
     */
    std::shared_ptr<Teacher> AddTeacher(const std::string& fullName, Date birthDate,
        const std::string& position, const std::string& degree, Department* department);

    /**
     * @brief Добавляет занятие. Занятие автоматически регистрируется
     *        у преподавателей (они запоминают, в каких группах ведут
     *        занятия) - это нужно третьему запросу.
     * @param type          Вид занятия.
     * @param discipline    Дисциплина.
     * @param group         Группа-участник.
     * @param teacher       Основной преподаватель.
     * @param secondTeacher Второй преподаватель (только для лабораторной).
     * @param room          Аудитория.
     * @param date          Дата проведения.
     * @return Умный указатель на созданное занятие.
     * @throws std::invalid_argument при нарушении доменных правил занятия.
     *
     * @note Преподаватели передаются неконстантными указателями намеренно:
     *       University - их владелец и обновляет их список групп, поэтому
     *       константность здесь была бы фиктивной.
     */
    std::shared_ptr AddLesson(const LessonType type, const Discipline* discipline,
        const Group* group, const Teacher* teacher, const Teacher* secondTeacher,
        const std::string& room, const Date date);

    // ---- Общий доступ ----

    /** @brief Возвращает название ВУЗа. */
    const std::string& GetName() const;

    /**
     * @brief Устанавливает название ВУЗ��.
     * @param name Новое название.
     * @throws std::invalid_argument если название пустое.
     */
    void SetName(const std::string& name);

    /** @brief Возвращает все специальности ВУЗа. */
    const std::vector<std::shared_ptr<Specialty>>& GetSpecialties() const;

    /** @brief Возвращает все кафедры ВУЗа. */
    const std::vector<std::shared_ptr<Department>>& GetDepartments() const;

    /** @brief Возвращает все группы ВУЗа. */
    const std::vector<std::shared_ptr<Group>>& GetGroups() const;

    /** @brief Возвращает всех преподавателей ВУЗа. */
    const std::vector<std::shared_ptr<Teacher>>& GetTeachers() const;

    /** @brief Возвращает все занятия ВУЗа. */
    const std::vector<std::shared_ptr<Lesson>>& GetLessons() const;

    // ---- Запрос 1: информация о студенте по № зачётной книжки и по ФИО ----

    /**
     * @brief Ищет студента по номеру зачётной книжки (регистронезависимо).
     * @param recordBook Номер зачётной книжки.
     * @return Умный указатель на студента либо nullptr, если не найден.
     */
    std::shared_ptr<Student> FindStudentByRecordBook(const std::string& recordBook) const;

    /**
     * @brief Ищет студентов по ФИО (по вхождению подстроки, регистронезависимо).
     *        Одному ФИО может соответствовать несколько студентов.
     * @param fullName ФИО или его часть.
     * @return Все подходящие студенты.
     */
    std::vector<std::shared_ptr<Student>> FindStudentsByFullName(const std::string& fullName) const;

    // ---- Запрос 2: список предметов, читаемых данной кафедрой ----

    /**
     * @brief Возвращает предметы, читаемые указанной кафедрой.
     * @param department Кафедра-запрос.
     * @return Дисциплины кафедры; пустой вектор, если @p department неизвестна.
     */
    std::vector<std::shared_ptr<Discipline>> GetDisciplinesByDepartment(const Department* department) const;

    // ---- Запрос 3: список преподавателей, ведущих занятия в группе ----

    /**
     * @brief Возвращает преподавателей, проводящих занятия в указанной группе.
     *        Учитываются в том числе вторые преподаватели лабораторных работ;
     *        повторы исключаются.
     * @param group Группа-запрос.
     * @return Преподаватели группы без повторов.
     */
    std::vector<std::shared_ptr<Teacher>> GetTeachersByGroup(const Group* group) const;

    // ---- Запрос 4: список групп на указанной специальности ----

    /**
     * @brief Возвращает группы, обучающиеся на указанной специальности.
     * @param specialty Специальность-запрос.
     * @return Группы специальности; пустой вектор, если @p specialty неизвестна.
     */
    std::vector<std::shared_ptr<Group>> GetGroupsBySpecialty(const Specialty* specialty) const;

    // ---- Дополнительные запросы ----

    /**
     * @brief Ищет кафедру по названию (регистронезависимо).
     * @param name Название кафедры.
     * @return Умный указатель на кафедру либо nullptr, если не найдена.
     */
    std::shared_ptr<Department> FindDepartmentByName(const std::string& name) const;

    /**
     * @brief Ищет специальность по коду (регистронезависимо).
     * @param code Код специальности.
     * @return Умный указатель на специальность либо nullptr, если не найдена.
     */
    std::shared_ptr<Specialty> FindSpecialtyByCode(const std::string& code) const;

    /**
     * @brief Ищет группу по номеру (регистронезависимо).
     * @param number Номер группы.
     * @return Умный указатель на группу либо nullptr, если не найдена.
     */
    std::shared_ptr<Group> FindGroupByNumber(const std::string& number) const;

    /**
     * @brief Возвращает занятия указанной группы.
     * @param group Группа-запрос.
     * @return Занятия группы в порядке добавления.
     */
    std::vector<std::shared_ptr<Lesson>> GetLessonsByGroup(const Group* group) const;

    /**
     * @brief Возвращает ВСЕХ людей ВУЗА (студентов и преподавателей) как
     *        умные указатели на БАЗОВЫЙ тип Person.
     *
     *        Именно этот метод формирует полиморфную коллекцию базового
     *        типа, которую обходит main.cpp (общее требование задания 1).
     * @return Студенты (сначала), затем преподаватели.
     */
    std::vector<std::shared_ptr<Person>> GetAllPeople() const;

    /**
     * @brief Возвращает всех студентов ВУЗа.
     * @return Студенты всех групп.
     */
    std::vector<std::shared_ptr<Student>> GetAllStudents() const;

    /**
     * @brief Формирует сводную статистику ВУЗа.
     * @return Отформатированная строка со счётчиками.
     */
    std::string GetStatistics() const;

private:
    std::string name_;
    std::vector<std::shared_ptr<Specialty>> specialties_;
    std::vector<std::shared_ptr<Department>> departments_;
    std::vector<std::shared_ptr<Group>> groups_;
    std::vector<std::shared_ptr<Teacher>> teachers_;
    std::vector<std::shared_ptr<Lesson>> lessons_;
};
