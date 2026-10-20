#pragma once

#include <string>

/**
 * @brief Класс Кафедра
 */
class Department {
	private:
		/**
		 * @brief Название кафедры
		 */
		std::string name;

	public:
		/**
		 * @brief Конструктор
		 * @param name - название кафедры
		 * @throws std::invalid_argument если название пустое
		 */
		explicit Department(const std::string& name);

		/**
		 * @brief Получить название кафедры
		 * @return название кафедры
		 */
		const std::string& getName() const noexcept;

		/**
		 * @brief Получить полную информацию о кафедре
		 * @return строка с названием кафедры
		 */
		std::string getInfo() const;

		/**
		 * @brief Стандартный оператор сравнения "равно"
		 * @param other - вторая кафедра
		 * @return true, если названия кафедр совпадают
		 */
		bool operator==(const Department& other) const noexcept;

		/**
		 * @brief Стандартный оператор сравнения "не равно"
		 * @param other - вторая кафедра
		 * @return true, если кафедры различаются
		 */
		bool operator!=(const Department& other) const noexcept;
};
