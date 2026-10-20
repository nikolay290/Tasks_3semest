#pragma once

#include <string>

#include "Date.h"

/**
 * @brief Класс Person
 */
class Person {
	protected:
		/**
		 * @brief Статический счетчик ID
		 */
		static int nextId;

		/**
		 * @brief id человека из списка
		 */
		int id;

		/**
		 * @brief ФИО человека
		 */
		std::string fullName;

		/**
		 * @brief Дата рождения
		 */
		Date birthDate;

	public:
		/**
		 * @brief Конструктор
		 * @param fullName - ФИО человека
		 * @param birthDate - дата рождения
		 * @param id - id человека (0 для автогенерации)
		 * @throws std::invalid_argument если ФИО пустое
		 */
		Person(const std::string& fullName, const Date& birthDate, const int id = 0);

		/**
		 * @brief Виртуальный деструктор
		 */
		virtual ~Person() = default;

		/**
		 * @brief Получить id человека
		 * @return id человека
		 */
		int getId() const noexcept;

		/**
		 * @brief Получить ФИО человека
		 * @return ФИО человека
		 */
		const std::string& getFullName() const noexcept;

		/**
		 * @brief Получить дату рождения
		 * @return дату рождения
		 */
		const Date& getBirthDate() const noexcept;

		/**
		 * @brief Задать новое ФИО человека
		 * @param newFullName - новое ФИО
		 * @throws std::invalid_argument если ФИО пустое
		 */
		void setFullName(const std::string& newFullName);

		/**
		 * @brief Получить роль человека в ВУЗе
		 * @return название роли, например "Студент"
		 */
		virtual std::string getRole() const = 0;

		/**
		 * @brief Получить сведения, характерные для конкретной роли
		 * @return строку с данными роли
		 */
		virtual std::string getSpecificInfo() const = 0;

		/**
		 * @brief Получить краткое представление человека
		 * @return строку вида "Иванов И. И. (2000-01-01)"
		 */
		virtual std::string getShortInfo() const;

		/**
		 * @brief Получить полную информацию о человеке
		 * @return строку с id, ФИО, датой рождения и данными роли
		 */
		virtual std::string getInfo() const;

		/**
		 * @brief Получить возраст человека на указанную дату
		 * @param referenceDate - дата, на которую считаем возраст
		 * @return возраст в полных годах
		 */
		int getAge(const Date& referenceDate) const noexcept;

		/**
		 * @brief Проверить, является ли человек студентом
		 * @return true, если человек студент
		 */
		virtual bool isStudent() const;

		/**
		 * @brief Проверить, является ли человек преподавателем
		 * @return true, если человек преподаватель
		 */
		virtual bool isTeacher() const;

		/**
		 * @brief Стандартный оператор сравнения "равно"
		 * @param other - второй человек
		 * @return true, если id людей совпадают
		 */
		bool operator==(const Person& other) const noexcept;

		/**
		 * @brief Стандартный оператор сравнения "не равно"
		 * @param other - второй человек
		 * @return true, если id людей различаются
		 */
		bool operator!=(const Person& other) const noexcept;
};
