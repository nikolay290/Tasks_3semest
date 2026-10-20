#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Date.h"
#include "Person.h"

/**
 * @brief Класс Студент - наследник класса Person
 */
class Student : public Person {
	private:
		/**
		 * @brief Номер зачётной книжки
		 */
		std::string recordBook;

		/**
		 * @brief Номер курса обучения
		 */
		unsigned int course;

		/**
		 * @brief Оценки студента
		 */
		std::vector<double> marks;

	public:
		/**
		 * @brief Конструктор
		 * @param fullName - ФИО студента
		 * @param birthDate - дата рождения
		 * @param recordBook - номер зачётной книжки
		 * @param course - номер курса обучения, не меньше 1
		 * @param id - id человека (0 для автогенерации)
		 * @throws std::invalid_argument если номер зачётной книжки пустой
		 *         или курс меньше 1
		 */
		Student(const std::string& fullName, const Date& birthDate, const std::string& recordBook,
			const unsigned int course, const int id = 0);

		/**
		 * @brief Получить номер зачётной книжки
		 * @return номер зачётной книжки
		 */
		const std::string& getRecordBook() const noexcept;

		/**
		 * @brief Получить номер курса обучения
		 * @return номер курса
		 */
		unsigned int getCourse() const noexcept;

		/**
		 * @brief Добавить оценку студенту
		 * @param mark - оценка, от 2 до 5
		 * @throws std::invalid_argument если оценка вне диапазона 2-5
		 */
		void addMark(const double mark);

		/**
		 * @brief Получить список оценок
		 * @return ссылку на массив оценок
		 */
		const std::vector<double>& getMarks() const noexcept;

		/**
		 * @brief Получить количество оценок
		 * @return число оценок
		 */
		std::size_t getMarkCount() const noexcept;

		/**
		 * @brief Проверить, есть ли у студента оценки
		 * @return true, если у студента есть хотя бы одна оценка
		 */
		bool hasMarks() const noexcept;

		/**
		 * @brief Получить средний балл студента
		 * @return средний балл или 0, если оценок нет
		 */
		double getAverageMark() const noexcept;

		/**
		 * @brief Получить роль человека в ВУЗе
		 * @return строку "Студент"
		 */
		std::string getRole() const override;

		/**
		 * @brief Получить сведения, характерные для конкретной роли
		 * @return строку с номером зачётной книжки, курсом и средним баллом
		 */
		std::string getSpecificInfo() const override;

		/**
		 * @brief Проверить, является ли человек студентом
		 * @return true, так как это студент
		 */
		bool isStudent() const override;

		/**
		 * @brief Стандартный оператор сравнения "равно"
		 * @param other - второй студент
		 * @return true, если id и номера зачётных книжек совпадают
		 */
		bool operator==(const Student& other) const noexcept;

		/**
		 * @brief Стандартный оператор сравнения "не равно"
		 * @param other - второй студент
		 * @return true, если студенты различаются
		 */
		bool operator!=(const Student& other) const noexcept;
};
