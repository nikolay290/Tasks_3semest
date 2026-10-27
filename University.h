#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Date.h"
#include "Department.h"
#include "Discipline.h"
#include "Group.h"
#include "Lesson.h"
#include "Specialty.h"
#include "Teacher.h"

class Person;
class Student;

/**
 * @brief Класс University
 */
class University {
	private:
		/**
		 * @brief Название ВУЗа
		 */
		std::string name;

		/**
		 * @brief Массив специальностей
		 */
		std::vector<Specialty> specialties;

		/**
		 * @brief Массив кафедр
		 */
		std::vector<Department> departments;

		/**
		 * @brief Массив дисциплин
		 */
		std::vector<Discipline> disciplines;

		/**
		 * @brief Массив групп
		 */
		std::vector<Group> groups;

		/**
		 * @brief Массив преподавателей
		 */
		std::vector<Teacher> teachers;

		/**
		 * @brief Массив занятий
		 */
		std::vector<Lesson> lessons;

		/**
		 * @brief Найти индекс кафедры по названию
		 * @param departmentName - название кафедры
		 * @return индекс кафедры или departments.size(), если кафедры нет
		 */
		std::size_t findDepartmentIndex(const std::string& departmentName) const noexcept;

		/**
		 * @brief Зарегистрировать дисциплину у преподавателя ВУЗа
		 * @param discipline - дисциплина
		 * @param teacherId - id преподавателя
		 */
		void registerDisciplineForTeacher(const Discipline& discipline, const int teacherId);

		/**
		 * @brief Зарегистрировать дисциплину у группы ВУЗа
		 * @param discipline - дисциплина
		 * @param groupNumber - номер группы
		 */
		void registerDisciplineForGroup(const Discipline& discipline, const std::string& groupNumber);

	public:
		/**
		 * @brief Конструктор по умолчанию
		 */
		University();

		/**
		 * @brief Конструктор
		 * @param name - название ВУЗа
		 * @throws std::invalid_argument если название пустое
		 */
		explicit University(const std::string& name);

		/**
		 * @brief Деструктор
		 */
		~University() = default;

		/**
		 * @brief Получить название ВУЗа
		 * @return название ВУЗа
		 */
		const std::string& getName() const noexcept;

		/**
		 * @brief Задать название ВУЗа
		 * @param newName - новое название
		 * @throws std::invalid_argument если название пустое
		 */
		void setName(const std::string& newName);

		/**
		 * @brief Добавить специальность
		 * @param specialty - специальность
		 * @throws std::invalid_argument если специальность с таким кодом уже есть
		 */
		void addSpecialty(const Specialty& specialty);

		/**
		 * @brief Добавить кафедру
		 * @param department - кафедра
		 * @throws std::invalid_argument если кафедра с таким названием уже есть
		 */
		void addDepartment(const Department& department);

		/**
		 * @brief Добавить дисциплину
		 * @param discipline - дисциплина
		 * @throws std::invalid_argument если дисциплина с таким названием уже есть
		 */
		void addDiscipline(const Discipline& discipline);

		/**
		 * @brief Добавить группу
		 * @param group - группа, уже заполненная студентами
		 * @throws std::invalid_argument если группа с таким номером уже есть
		 */
		void addGroup(const Group& group);

		/**
		 * @brief Добавить преподавателя
		 * @param teacher - преподаватель
		 * @throws std::invalid_argument если преподаватель с таким id уже есть
		 */
		void addTeacher(const Teacher& teacher);

		/**
		 * @brief Добавить занятие
		 * @param lesson - занятие
		 * @throws std::invalid_argument если занятие ведёт неизвестный
		 *         преподаватель
		 */
		void addLesson(const Lesson& lesson);

		/**
		 * @brief Получить список специальностей
		 * @return ссылку на массив специальностей
		 */
		const std::vector<Specialty>& getSpecialties() const noexcept;

		/**
		 * @brief Получить список кафедр
		 * @return ссылку на массив кафедр
		 */
		const std::vector<Department>& getDepartments() const noexcept;

		/**
		 * @brief Получить список дисциплин
		 * @return ссылку на массив дисциплин
		 */
		const std::vector<Discipline>& getDisciplines() const noexcept;

		/**
		 * @brief Получить список групп
		 * @return ссылку на массив групп
		 */
		const std::vector<Group>& getGroups() const noexcept;

		/**
		 * @brief Получить список преподавателей
		 * @return ссылку на массив преподавателей
		 */
		const std::vector<Teacher>& getTeachers() const noexcept;

		/**
		 * @brief Получить список занятий
		 * @return ссылку на массив занятий
		 */
		const std::vector<Lesson>& getLessons() const noexcept;

		/**
		 * @brief Найти специальность по коду
		 * @param code - код специальности
		 * @return true, если в ВУЗе есть специальность с таким кодом
		 */
		bool findSpecialtyByCode(const std::string& code) const noexcept;

		/**
		 * @brief Найти кафедру по названию
		 * @param departmentName - название кафедры
		 * @return true, если в ВУЗе есть кафедра с таким названием
		 */
		bool findDepartmentByName(const std::string& departmentName) const noexcept;

		/**
		 * @brief Найти группу по номеру
		 * @param groupNumber - номер группы
		 * @return true, если в ВУЗе есть группа с таким номером
		 */
		bool findGroupByNumber(const std::string& groupNumber) const noexcept;

		/**
		 * @brief Найти преподавателя по id
		 * @param id - id преподавателя
		 * @return true, если в ВУЗе есть преподаватель с таким id
		 */
		bool findTeacherById(const int id) const noexcept;

		/**
		 * @brief Найти преподавателя по ФИО
		 * @param fullName - ФИО преподавателя
		 * @return true, если в ВУЗе есть преподаватель с таким ФИО
		 */
		bool findTeacherByFullName(const std::string& fullName) const;

		/**
		 * @brief Найти студента по номеру зачётной книжки
		 * @param recordBook - номер зачётной книжки
		 * @return true, если в ВУЗе есть студент с таким номером
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
		 * @brief Получить преподавателя по ФИО
		 * @param fullName - ФИО преподавателя
		 * @return массив из одного найденного преподавателя или пустой
		 *         массив, если преподавателя нет
		 */
		std::vector<Teacher> getTeacherByFullName(const std::string& fullName) const;

		/**
		 * @brief Найти студентов по части или полному ФИО
		 * @param fullName - ФИО или его часть
		 * @return массив копий найденных студентов
		 */
		std::vector<Student> findStudentsByFullName(const std::string& fullName) const;

		/**
		 * @brief Получить дисциплины, читаемые кафедрой
		 * @param department - кафедра
		 * @return массив копий дисциплин кафедры
		 */
		std::vector<Discipline> getDisciplinesByDepartment(const Department& department) const;

		/**
		 * @brief Получить преподавателей, проводящих занятия в группе
		 * @param group - группа
		 * @return массив копий преподавателей группы
		 */
		std::vector<Teacher> getTeachersByGroup(const Group& group) const;

		/**
		 * @brief Получить группы, обучающиеся на специальности
		 * @param specialty - специальность
		 * @return массив копий групп специальности
		 */
		std::vector<Group> getGroupsBySpecialty(const Specialty& specialty) const;

		/**
		 * @brief Получить всех студентов ВУЗа
		 * @return массив копий студентов ВУЗа
		 */
		std::vector<Student> getAllStudents() const;

		/**
		 * @brief Получить всех преподавателей ВУЗа
		 * @return массив копий преподавателей ВУЗа
		 */
		std::vector<Teacher> getAllTeachers() const;

		/**
		 * @brief Получить статистику ВУЗа одной строкой
		 * @return строка со счётчиками объектов и средним баллом
		 */
		std::string getStatistics() const;
};
