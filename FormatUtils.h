#pragma once

#include <string>
#include <vector>

/**
 * @brief Вспомогательные функции форматирования строк
 */
namespace FormatUtils {
	/**
	 * @brief Получить строку из числа с плавающей точкой с двумя знаками после запятой
	 * @param value - форматируемое число
	 * @return строка вида "123.45" без потоков ввода-вывода
	 */
	std::string formatDecimal(const double value);

	/**
	 * @brief Регистронезависимое сравнение двух строк
	 * @param a - первая строка
	 * @param b - вторая строка
	 * @return true, если строки равны без учёта регистра букв
	 */
	bool equalsIgnoreCase(const std::string& a, const std::string& b);

	/**
	 * @brief Регистронезависимая проверка вхождения подстроки
	 * @param haystack - строка, в которой ищем
	 * @param needle - искомая подстрока; пустая строка всегда найдена
	 * @return true, если @p needle входит в @p haystack
	 */
	bool containsIgnoreCase(const std::string& haystack, const std::string& needle);

	/**
	 * @brief Сократить ФИО до вида "Фамилия И. О."
	 * @param fullName - исходное ФИО
	 * @return ФИО с сокращёнными именем и отчеством
	 */
	std::string toInitials(const std::string& fullName);

	/**
	 * @brief Убрать пробелы в начале и в конце строки
	 * @param text - исходная строка
	 * @return строка без крайних пробелов
	 */
	std::string trim(const std::string& text);

	/**
	 * @brief Склеить строки коллекции в одну строку через разделитель
	 * @param items - элементы для склейки
	 * @param separator - разделитель между элементами
	 * @return объединённая строка; для пустой коллекции - пустая строка
	 */
	std::string join(const std::vector<std::string>& items, const std::string& separator);
}
