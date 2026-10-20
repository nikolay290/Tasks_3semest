#pragma once

#include <string>

/**
 * @brief Класс Специальность
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

	public:
		/**
		 * @brief Конструктор
		 * @param code - код специальности, например "09.02.07"
		 * @param name - название специальности
		 * @throws std::invalid_argument если код или название пустые
		 */
		Specialty(const std::string& code, const std::string& name);

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
		 * @brief Получить краткое представление специальности
		 * @return строка вида "09.02.07 - Информатика и вычислительная техника"
		 */
		std::string getShortInfo() const;

		/**
		 * @brief Получить полную информацию о специальности
		 * @return строка с кодом и названием специальности
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
