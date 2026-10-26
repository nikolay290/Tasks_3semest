#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Date.h"
#include "Discipline.h"
#include "Teacher.h"

class Group;

/**
 * @brief Вид занятия
 */
enum class LessonType {
	Lecture,    ///< Лекция
	Practical,  ///< Практическое занятие
	Lab         ///< Лабораторная работа, которую ведут два преподавателя
};

/**
 * @brief Получить название вида занятия
 * @param type - вид занятия
 * @return название вида занятия
 */
std::string toString(const LessonType type);

/**
 * @brief Класс Занятие
 */
class Lesson {
	private:
		/**
		 * @brief Вид занятия
		 */
		LessonType type;

		/**
		 * @brief Дисциплина, по которой проводится занятие
		 */
		Discipline discipline;

		/**
		 * @brief Номер группы, для которой проводится занятие
		 */
		std::string groupNumber;

		/**
		 * @brief Преподаватели занятия: один для лекции или практики,
		 *        два для лабораторной работы
		 */
		std::vector<Teacher> teachers;

		/**
		 * @brief Аудитория проведения занятия
		 */
		std::string room;

		/**
		 * @brief Дата проведения занятия
		 */
		Date date;

	public:
		/**
		 * @brief Конструктор
		 * @param type - вид занятия
		 * @param discipline - дисциплина занятия
		 * @param group - группа, для которой проводится занятие
		 * @param lessonTeachers - преподаватели занятия; для лабораторной
		 *        работы должно быть указано ровно два разных преподавателя,
		 *        для остальных видов занятий - ровно один
		 * @param room - аудитория, может быть пустой строкой
		 * @param date - дата проведения занятия
		 * @throws std::invalid_argument если число преподавателей не
		 *         соответствует виду занятия или для лабораторной работы
		 *         дважды указан один и тот же преподаватель
		 */
		Lesson(const LessonType& type, const Discipline& discipline, const Group& group,
			const std::vector<Teacher>& lessonTeachers, const std::string& room,
			const Date& date);

		/**
		 * @brief Получить вид занятия
		 * @return вид занятия
		 */
		LessonType getType() const noexcept;

		/**
		 * @brief Получить дисциплину занятия
		 * @return ссылку на дисциплину
		 */
		const Discipline& getDiscipline() const noexcept;

		/**
		 * @brief Получить номер группы, для которой проводится занятие
		 * @return номер группы
		 */
		const std::string& getGroupNumber() const noexcept;

		/**
		 * @brief Получить преподавателей занятия
		 * @return ссылку на массив преподавателей
		 */
		const std::vector<Teacher>& getTeachers() const noexcept;

		/**
		 * @brief Получить число преподавателей занятия
		 * @return число преподавателей: 1 для лекции или практики, 2 для лабораторной
		 */
		std::size_t getTeacherCount() const noexcept;

		/**
		 * @brief Получить аудиторию занятия
		 * @return номер аудитории
		 */
		const std::string& getRoom() const noexcept;

		/**
		 * @brief Получить дату проведения занятия
		 * @return дату занятия
		 */
		const Date& getDate() const noexcept;

		/**
		 * @brief Проверить, ведёт ли указанный преподаватель это занятие
		 * @param teacher - проверяемый преподаватель
		 * @return true, если преподаватель ведёт занятие
		 */
		bool isTaughtBy(const Teacher& teacher) const noexcept;

		/**
		 * @brief Получить краткое представление занятия
		 * @return строка с видом занятия, дисциплиной и номером группы
		 */
		std::string getShortInfo() const;

		/**
		 * @brief Получить полную информацию о занятии
		 * @return строка с видом занятия, дисциплиной, группой,
		 *         преподавателями, аудиторией и датой
		 */
		std::string getInfo() const;

		/**
		 * @brief Стандартный оператор сравнения "равно"
		 * @param other - второе занятие
		 * @return true, если вид, дисциплина, группа, преподаватели,
		 *         аудитория и дата совпадают
		 */
		bool operator==(const Lesson& other) const noexcept;

		/**
		 * @brief Стандартный оператор сравнения "не равно"
		 * @param other - второе занятие
		 * @return true, если занятия различаются
		 */
		bool operator!=(const Lesson& other) const noexcept;
};
