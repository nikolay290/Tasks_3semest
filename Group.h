#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Specialty.h"
#include "Student.h"

/**
 * @brief Класс Группа
 */
class Group {
	private:
		/**
		 * @brief Номер группы
		 */
		std::string number;

		/**
		 * @brief Специальность группы
		 */
		Specialty specialty;

		/**
		 * @brief Студенты группы
		 */
		std::vector<Student> students;

	public:
		/**
		 * @brief Конструктор
		 * @param number - номер группы, например "1И"
		 * @param specialty - специальность, на которой обучается группа
		 * @throws std::invalid_argument если номер группы пустой
		 */
		Group(const std::string& number, const Specialty& specialty);

		/**
		 * @brief Получить номер группы
		 * @return номер группы
		 */
		const std::string& getNumber() const noexcept;

		/**
		 * @brief Получить специальность группы
		 * @return ссылку на специальность
		 */
		const Specialty& getSpecialty() const noexcept;

		/**
		 * @brief Добавить студента в группу
		 * @param student - добавляемый студент
		 * @throws std::invalid_argument если студент с таким номером
		 *         зачётной книжки уже есть в группе
		 */
		void addStudent(const Student& student);

		/**
		 * @brief Получить список студентов группы
		 * @return ссылку на массив студентов
		 */
		const std::vector<Student>& getStudents() const noexcept;

		/**
		 * @brief Получить число студентов в группе
		 * @return число студентов
		 */
		std::size_t getStudentCount() const noexcept;

		/**
		 * @brief Найти студента по номеру зачётной книжки
		 * @param recordBook - номер зачётной книжки
		 * @return true, если в группе есть студент с таким номером
		 *         зачётной книжки
		 */
		bool findStudentByRecordBook(const std::string& recordBook) const;

		/**
		 * @brief Получить студента по номеру зачётной книжки
		 * @param recordBook - номер зачётной книжки
		 * @return массив из одного найденного студента или пустой массив,
		 *         если студента нет
		 */
		std::vector<Student> getStudentByRecordBook(const std::string& recordBook) const;

		/**
		 * @brief Получить средний балл группы
		 * @return средний балл по всем оценкам всех студентов или 0,
		 *         если оценок нет
		 */
		double getAverageMark() const noexcept;

		/**
		 * @brief Получить краткое представление группы
		 * @return номер группы
		 */
		std::string getShortInfo() const;

		/**
		 * @brief Получить полную информацию о группе
		 * @return строку с номером группы, специальностью и числом студентов
		 */
		std::string getInfo() const;

		/**
		 * @brief Стандартный оператор сравнения "равно"
		 * @param other - вторая группа
		 * @return true, если номера и специальности групп совпадают
		 */
		bool operator==(const Group& other) const noexcept;

		/**
		 * @brief Стандартный оператор сравнения "не равно"
		 * @param other - вторая группа
		 * @return true, если группы различаются
		 */
		bool operator!=(const Group& other) const noexcept;
};
