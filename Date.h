#pragma once

#include <string>

/**
 * @brief Неизменяемая календарная дата
 */
class Date {
	private:
		/**
		 * @brief Год
		 */
		int year;

		/**
		 * @brief Месяц, 1-12
		 */
		int month;

		/**
		 * @brief День месяца
		 */
		int day;

		/**
		 * @brief Проверить, високосный ли год
		 * @param year - проверяемый год
		 * @return true, если год високосный
		 */
		static bool isLeapYear(const int year) noexcept;

		/**
		 * @brief Получить число дней в месяце
		 * @param year - год, нужен только для февраля
		 * @param month - месяц, 1-12
		 * @return число дней в указанном месяце
		 */
		static int daysInMonth(const int year, const int month) noexcept;

	public:
		/**
		 * @brief Конструктор
		 * @param year - год
		 * @param month - месяц, 1-12
		 * @param day - день месяца, допустимый для этого месяца и года
		 * @throws std::invalid_argument если месяц или день недопустимы
		 */
		Date(const int year, const int month, const int day);

		/**
		 * @brief Разобрать дату из строки строгого формата "ГГГГ-ММ-ДД"
		 * @param isoDate - строка с датой, например "2004-03-15"
		 * @return разобранная дата
		 * @throws std::invalid_argument если строка не соответствует формату
		 *         или не является корректной календарной датой
		 */
		static Date parse(const std::string& isoDate);

		/**
		 * @brief Получить год
		 * @return год
		 */
		int getYear() const noexcept;

		/**
		 * @brief Получить номер месяца
		 * @return номер месяца, 1-12
		 */
		int getMonth() const noexcept;

		/**
		 * @brief Получить день месяца
		 * @return день месяца
		 */
		int getDay() const noexcept;

		/**
		 * @brief Получить дату строкой формата "ГГГГ-ММ-ДД"
		 * @return дата в виде строки
		 */
		std::string toString() const;

		/**
		 * @brief Сколько дней от этой даты до указанной
		 * @param other - дата, до которой считаем
		 * @return число дней; отрицательное, если @p other раньше этой даты
		 */
		int daysUntil(const Date& other) const noexcept;

		/**
		 * @brief Стандартный оператор сравнения "меньше"
		 * @param other - вторая дата
		 * @return true, если эта дата раньше @p other
		 */
		bool operator<(const Date& other) const noexcept;

		/**
		 * @brief Стандартный оператор сравнения "меньше или равно"
		 * @param other - вторая дата
		 * @return true, если эта дата не позже @p other
		 */
		bool operator<=(const Date& other) const noexcept;

		/**
		 * @brief Стандартный оператор сравнения "больше"
		 * @param other - вторая дата
		 * @return true, если эта дата позже @p other
		 */
		bool operator>(const Date& other) const noexcept;

		/**
		 * @brief Стандартный оператор сравнения "равно"
		 * @param other - вторая дата
		 * @return true, если даты совпадают по всем компонентам
		 */
		bool operator==(const Date& other) const noexcept;

		/**
		 * @brief Стандартный оператор сравнения "не равно"
		 * @param other - вторая дата
		 * @return true, если даты различаются
		 */
		bool operator!=(const Date& other) const noexcept;
};
