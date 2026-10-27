#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Department.h"

class Group;

/**
 * @brief Класс Специальность
 *
 * Специальность обучается на кафедре и знает группы, которые на ней учатся.
 */
class Specialty {
	private:
		/**
		 * @brief Код специальности
		 */
		std::string code;

		/**
		 * @brief Название специальности
		 */
		std::string name;

		/**
		 * @brief Кафедра, обучающая специальности
		 */
		Department department;

		/**
		 * @brief Группы, обучающиеся на специальности
		 */
		std::vector<Group> groups;

	public:
		/**
		 * @brief Конструктор
		 * @param code - код специальности, например "09.02.07"
		 * @param name - название специальности
		 * @param department - кафедра, обучающая специальности
		 * @throws std::invalid_argument если код или название пустые
		 */
		Specialty(const std::string& code, const std::string& name, const Department& department);

		/**
		 * @brief Получить код специальности
		 * @return код специальности
		 */
		const std::string& getCode() const noexcept;

		/**
		 * @brief Получить название специальности
		 * @return название специальности
		 */
		const std::string& getName() const noexcept;

		/**
		 * @brief Получить кафедру, обучающую специальности
		 * @return ссылку на кафедру
		 */
		const Department& getDepartment() const noexcept;

		/**
		 * @brief Добавить группу, обучающуюся на специальности
		 * @param group - добавляемая группа; повторно не добавляется
		 */
		void addGroup(const Group& group);

		/**
		 * @brief Получить группы, обучающиеся на специальности
		 * @return ссылку на массив групп
		 */
		const std::vector<Group>& getGroups() const noexcept;

		/**
		 * @brief Получить число групп, обучающихся на специальности
		 * @return число групп
		 */
		std::size_t getGroupCount() const noexcept;

		/**
		 * @brief Получить краткое представление специальности
		 * @return строка вида "09.02.07 - Информатика и вычислительная техника"
		 */
		std::string getShortInfo() const;

		/**
		 * @brief Получить полную информацию
		 * @return строка с кодом, названием, кафедрой и числом групп
		 */
		std::string getInfo() const;

		/**
		 * @brief Стандартный оператор сравнения "равно"
		 * @param other - вторая специальность
		 * @return true, если коды и названия специальностей совпадают
		 */
		bool operator==(const Specialty& other) const noexcept;

		/**
		 * @brief Стандартный оператор сравнения "не равно"
		 * @param other - вторая специальность
		 * @return true, если специальности различаются
		 */
		bool operator!=(const Specialty& other) const noexcept;
};
