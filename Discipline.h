#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Department.h"

class Teacher;

/**
 * @brief Класс Дисциплина
 *
 * Дисциплина читается кафедрой и знает преподавателей, которые её ведут.
 */
class Discipline {
	private:
		/**
		 * @brief Название дисциплины
		 */
		std::string name;

		/**
		 * @brief Кафедра, читающая дисциплину
		 */
		Department department;

		/**
		 * @brief Преподаватели, читающие дисциплину
		 */
		std::vector<Teacher> teachers;

	public:
		/**
		 * @brief Конструктор
		 * @param name - название дисциплины
		 * @param department - кафедра, читающая дисциплину
		 * @throws std::invalid_argument если название пустое
		 */
		Discipline(const std::string& name, const Department& department);

		/**
		 * @brief Получить название дисциплины
		 * @return название дисциплины
		 */
		const std::string& getName() const noexcept;

		/**
		 * @brief Получить кафедру, читающую дисциплину
		 * @return ссылку на кафедру
		 */
		const Department& getDepartment() const noexcept;

		/**
		 * @brief Добавить преподавателя, читающего дисциплину
		 * @param teacher - добавляемый преподаватель; повторно не добавляется
		 */
		void addTeacher(const Teacher& teacher);

		/**
		 * @brief Получить преподавателей, читающих дисциплину
		 * @return ссылку на массив преподавателей
		 */
		const std::vector<Teacher>& getTeachers() const noexcept;

		/**
		 * @brief Получить число преподавателей, читающих дисциплину
		 * @return число преподавателей
		 */
		std::size_t getTeacherCount() const noexcept;

		/**
		 * @brief Получить краткое представление дисциплины
		 * @return название дисциплины
		 */
		std::string getShortInfo() const;

		/**
		 * @brief Получить полную информацию о дисциплине
		 * @return строка с названием дисциплины, её кафедрой и числом преподавателей
		 */
		std::string getInfo() const;

		/**
		 * @brief Стандартный оператор сравнения "равно"
		 * @param other - вторая дисциплина
		 * @return true, если названия и кафедры дисциплин совпадают
		 */
		bool operator==(const Discipline& other) const noexcept;

		/**
		 * @brief Стандартный оператор сравнения "не равно"
		 * @param other - вторая дисциплина
		 * @return true, если дисциплины различаются
		 */
		bool operator!=(const Discipline& other) const noexcept;
};

// Полное определение Teacher нужно там, где дисциплина копируется или
// уничтожается: её поле std::vector<Teacher> требует завершённого типа.
#include "Teacher.h"
