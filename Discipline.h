#pragma once

#include <string>

#include "Department.h"

/**
 * @brief Класс Дисциплина
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
		 * @brief Получить краткое представление дисциплины
		 * @return название дисциплины
		 */
		std::string getShortInfo() const;

		/**
		 * @brief Получить полную информацию о дисциплине
		 * @return строка с названием дисциплины и её кафедрой
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
