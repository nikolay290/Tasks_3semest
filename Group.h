#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Specialty.h"

class Discipline;
class Student;

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

		/**
		 * @brief Дисциплины, которые изучает группа
		 */
		std::vector<Discipline> disciplines;

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
		 * @brief Добавить дисциплину, которую изучает группа
		 * @param discipline - добавляемая дисциплина; повторно не добавляется.
		 *        Хранится облегчённая копия без списка преподавателей
		 */
		void addDiscipline(const Discipline& discipline);

		/**
		 * @brief Получить дисциплины, которые изучает группа
		 * @return ссылку на массив дисциплин
		 */
		const std::vector<Discipline>& getDisciplines() const noexcept;

		/**
		 * @brief Получить число дисциплин группы
		 * @return число дисциплин
		 */
		std::size_t getDisciplineCount() const noexcept;

		/**
		 * @brief Добавить студента в группу
		 * @param student - добавляемый студент; повторно не добавляется
		 * @throws std::invalid_argument если студент с таким номером
		 *         зачётной книжки уже есть
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
		 * @return true, если студент с таким номером есть в группе
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
		 * @return средний балл всех студентов или 0, если студентов нет
		 */
		double getAverageMark() const noexcept;

		/**
		 * @brief Получить краткое представление группы
		 * @return строка вида "Группа 1И (Информатика и вычислительная техника)"
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
		 * @return true, если номера групп совпадают
		 */
		bool operator==(const Group& other) const noexcept;

		/**
		 * @brief Стандартный оператор сравнения "не равно"
		 * @param other - вторая группа
		 * @return true, если группы различаются
		 */
		bool operator!=(const Group& other) const noexcept;
};

// Полное определение Student нужно там, где группа копируется или уничтожается
#include "Student.h"

// Полное определение Discipline нужно там, где группа копируется или уничтожается
#include "Discipline.h"
