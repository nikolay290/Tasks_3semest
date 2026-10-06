#pragma once
//
// Person.h
// Абстрактный базовый класс "Человек" - общий предок студентов и
// преподавателей ВУЗа.
//
// Именно этот класс используется как БАЗОВЫЙ ТИП коллекции в main.cpp:
// в std::vector<std::shared_ptr<Person>> помещаются объекты типов-наследников
// (Student, Teacher), и коллекция обходится как коллекция базового класса.
//
// Важно: библиотека нигде не использует потоки ввода-вывода (<iostream>,
// <fstream>, <sstream> и т.п.) - весь вывод собирается в обычные
// std::string и печатается вызывающим кодом (см. UniversityApp/main.cpp).
//
#include <string>
#include "Date.h"

/**
 * @brief Абстрактный базовый класс любого человека, обучающегося или
 *        работающего в ВУЗе. Хранит ФИО и дату рождения, объявляет
 *        точки расширения (GetRole / GetInfo), которые переопределяют
 *        классы-наследники, добавляя собственные данные.
 */
class Person {
public:
    /**
     * @brief Создаёт человека с его общими атрибутами.
     * @param fullName   Фамилия, имя и отчество.
     * @param birthDate  Дата рождения.
     * @throws std::invalid_argument если ФИО пустое.
     */
    Person(const std::string fullName, const Date birthDate);

    /** @brief Виртуальный деструктор, необходим для полиморфных базовых классов. */
    virtual ~Person() = default;

    /** @brief Возвращает ФИО. */
    const std::string& GetFullName() const;

    /** @brief Возвращает дату рождения. */
    const Date& GetBirthDate() const;

    /**
     * @brief Устанавливает новое ФИО.
     * @param fullName Новое ФИО.
     * @throws std::invalid_argument если ФИО пустое.
     */
    void SetFullName(const std::string& fullName);

    /**
     * @brief Возвращает возраст в полных годах на дату @p asOf.
     * @param asOf Дата, на которую считается возраст.
     * @return Полных прожитых лет; 0, если дата рождения ещё не наступила.
     */
    int GetAge(const Date& asOf) const;

    /**
     * @brief Возвращает роль человека в ВУЗе ("Студент", "Преподаватель").
     *        Переопределяется в каждом классе-наследнике.
     * @return Название роли.
     */
    virtual std::string GetRole() const = 0;

    /**
     * @brief Формирует однострочное человекочитаемое описание человека:
     *        общие поля плюс поля, специфичные для конкретной роли
     *        (через GetSpecificInfo()).
     * @return Отформатированная строка с описанием (без завершающего перевода строки).
     */
    virtual std::string GetInfo() const;

    /**
     * @brief Возвращает краткое представление человека: "Фамилия И. О.".
     *        Используется в отчётах, где не нужно перечислять все поля.
     * @return Краткое описание.
     */
    virtual std::string GetShortInfo() const;

    /**
     * @brief Возвращает true, если человек является студентом.
     *        Позволяет различать типы элементов коллекции базового типа
     *        без dynamic_cast (см. IsStudent() в наследниках).
     * @return true для Student, false иначе.
     */
    virtual bool IsStudent() const;

    /**
     * @brief Возвращает true, если человек является преподавателем.
     * @return true для Teacher, false иначе.
     */
    virtual bool IsTeacher() const;

    /** @brief Сравнение на равенство по ФИО и дате рождения. */
    bool operator==(const Person& other) const;
    /** @brief Сравнение на неравенство, отрицание operator==. */
    bool operator!=(const Person& other) const;

protected:
    /**
     * @brief Возвращает часть описания, специфичную для роли (например,
     *        номер зачётной книжки для Student). Реализуется в каждом
     *        классе-наследнике.
     * @return Короткая строка с деталями, специфичными для роли.
     */
    virtual std::string GetSpecificInfo() const = 0;

    std::string fullName_;
    Date birthDate_;
};
