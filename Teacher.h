#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Date.h"
#include "Department.h"
#include "Person.h"

class Discipline;

/**
 * @brief Класс Преподаватель - наследник класса Person
 */
class Teacher : public Person {
	private:
		/**
		 * @brief Должность преподавателя
		 */
		std::string position;

		/**
		 * @brief Учёная степень
		 */
		std::string degree;

		/**
		 * @brief Кафедра, на которой работает преподаватель
		 */
		Department department;

		/**
		 * @brief Дисциплины, которые читает преподаватель
		 */
		std::vector<Discipline> disciplines;

	public:
		/**
		 * @brief Конструктор
		 * @param fullName - ФИО преподавателя
		 * @param birthDate - дата рождения
		 * @param position - должность преподавателя
		 * @param degree - учёная степень, может быть пустой строкой
		 * @param department - кафедра, на которой работает преподаватель
		 * @param id - id человека (0 для автогенерации)
		 * @throws std::invalid_argument если ФИО или должность пустые
		 */
		Teacher(const std::string& fullName, const Date& birthDate, const std::string& position,
			const std::string& degree, const Department& department, const int id = 0);

		/**
		 * @brief Получить должность преподавателя
		 * @return должность
		 */
		const std::string& getPosition() const noexcept;

		/**
		 * @brief Получить учёную степень преподавателя
		 * @return учёную степень
		 */
		const std::string& getDegree() const noexcept;

		/**
		 * @brief Получить кафедру преподавателя
		 * @return ссылку на кафедру
		 */
		const Department& getDepartment() const noexcept;

		/**
		 * @brief Добавить дисциплину, которую читает преподаватель
		 * @param discipline - добавляемая дисциплина; повторно не добавляется.
		 *        Хранится облегчённая копия без списка преподавателей, чтобы
		 *        не получить бесконечную вложенность Teacher и Discipline
		 */
		void addDiscipline(const Discipline& discipline);

		/**
		 * @brief Получить дисциплины, которые читает преподаватель
		 * @return ссылку на массив дисциплин
		 */
		const std::vector<Discipline>& getDisciplines() const noexcept;

		/**
		 * @brief Получить число дисциплин преподавателя
		 * @return число дисциплин
		 */
		std::size_t getDisciplineCount() const noexcept;

		/**
		 * @brief Получить роль человека в ВУЗе
		 * @return строку "Преподаватель"
		 */
		std::string getRole() const override;

		/**
		 * @brief Получить сведения, характерные для конкретной роли
		 * @return строку с должностью, степенью, кафедрой и числом дисциплин
		 */
		std::string getSpecificInfo() const override;

		/**
		 * @brief Проверить, является ли человек преподавателем
		 * @return true, так как это преподаватель
		 */
		bool isTeacher() const override;

		/**
		 * @brief Стандартный оператор сравнения "равно"
		 * @param other - второй преподаватель
		 * @return true, если id и данные преподавателей совпадают
		 */
		bool operator==(const Teacher& other) const noexcept;

		/**
		 * @brief Стандартный оператор сравнения "не равно"
		 * @param other - второй преподаватель
		 * @return true, если преподаватели различаются
		 */
		bool operator!=(const Teacher& other) const noexcept;
};

// Полное определение Discipline нужно там, где преподаватель копируется или
// уничтожается: его поле std::vector<Discipline> требует завершённого типа.
#include "Discipline.h"
