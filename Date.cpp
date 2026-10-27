#include "Date.h"

#include <stdexcept>

namespace {
	/**
	 * @brief Число дней, прошедших от условной эпохи до указанной даты
	 * @param year - год
	 * @param month - месяц, 1-12
	 * @param day - день месяца, уже проверенный на корректность
	 * @return число дней; используется для вычисления разницы дат
	 */
	int daysFromCivil(const int year, const int month, const int day) {
		int adjustedYear = year - (month <= 2 ? 1 : 0);
		const int era = (adjustedYear >= 0 ? adjustedYear : adjustedYear - 399) / 400;
		const unsigned yearOfEra = static_cast<unsigned>(adjustedYear - era * 400);
		const unsigned dayOfYear = (153 * (month + (month > 2 ? -3 : 9)) + 2) / 5 + day - 1;
		const unsigned dayOfEra = yearOfEra * 365 + yearOfEra / 4 - yearOfEra / 100 + dayOfYear;
		return era * 146097 + static_cast<int>(dayOfEra) - 719468;
	}

	/**
	 * @brief Разобрать десятичное число из части строки без потоков ввода-вывода
	 * @param text - строка, из которой читаются ровно @p digits символов
	 * @param offset - позиция первого читаемого символа
	 * @param digits - число читаемых цифр
	 * @return разобранное число; -1, если символ не является цифрой
	 */
	int parseInt(const std::string& text, const std::size_t offset, const std::size_t digits) {
		if (offset + digits > text.length()) {
			return -1;
		}
		int result = 0;
		for (std::size_t index = 0; index < digits; ++index) {
			const char symbol = text[offset + index];
			if (symbol < '0' || symbol > '9') {
				return -1;
			}
			result = result * 10 + (symbol - '0');
		}
		return result;
	}
}

bool Date::isLeapYear(const int year) noexcept {
	return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

int Date::daysInMonth(const int year, const int month) noexcept {
	switch (month) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
		case 2:
			return isLeapYear(year) ? 29 : 28;
		default:
			return 0;
	}
}

Date::Date(const int year, const int month, const int day)
	: year(year), month(month), day(day) {
	if (month < 1 || month > 12) {
		throw std::invalid_argument("Месяц должен быть в диапазоне 1-12");
	}
	if (day < 1 || day > daysInMonth(year, month)) {
		throw std::invalid_argument("Некорректный день месяца для указанного месяца и года");
	}
}

Date Date::parse(const std::string& isoDate) {
	if (isoDate.length() != 10 || isoDate[4] != '-' || isoDate[7] != '-') {
		throw std::invalid_argument("Дата должна быть в формате ГГГГ-ММ-ДД");
	}
	const int parsedYear = parseInt(isoDate, 0, 4);
	const int parsedMonth = parseInt(isoDate, 5, 2);
	const int parsedDay = parseInt(isoDate, 8, 2);
	if (parsedYear < 0 || parsedMonth < 0 || parsedDay < 0) {
		throw std::invalid_argument("Дата должна быть в формате ГГГГ-ММ-ДД");
	}
	return Date(parsedYear, parsedMonth, parsedDay);
}

int Date::getYear() const noexcept {return year;}

int Date::getMonth() const noexcept {return month;}

int Date::getDay() const noexcept {return day;}

std::string Date::toString() const {
	std::string result = std::to_string(year) + "-";
	result += month < 10 ? "0" + std::to_string(month) : std::to_string(month);
	result += "-";
	result += day < 10 ? "0" + std::to_string(day) : std::to_string(day);
	return result;
}

int Date::daysUntil(const Date& other) const noexcept {
	return daysFromCivil(other.year, other.month, other.day) - daysFromCivil(year, month, day);
}

bool Date::operator<(const Date& other) const noexcept {return daysUntil(other) > 0;}

bool Date::operator<=(const Date& other) const noexcept {return daysUntil(other) >= 0;}

bool Date::operator>(const Date& other) const noexcept {return daysUntil(other) < 0;}

bool Date::operator==(const Date& other) const noexcept {
	return year == other.year && month == other.month && day == other.day;
}

bool Date::operator!=(const Date& other) const noexcept {return !(*this == other);}
