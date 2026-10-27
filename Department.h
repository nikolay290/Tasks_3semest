#pragma once

#include <cstddef>
#include <string>
#include <vector>

class Discipline;
class Group;
class Specialty;
class Teacher;

/**
 * @brief Класс Кафедра
 *
 * Кафедра знает, кто учится (группы), кто работает (преподаватели),
 * какие специальности за ней закреплены и какие дисциплины она читает.
 */
class Department {
	private:
		/**
		 * @brief Название кафедры
		 */
		std::string name;

		/**
		 * @brief Группы, обучающиеся по специальностям кафедры
		 */
		std::vector<Group> groups;

		/**
		 * @brief Преподаватели кафедры
		 */
		std::vector<Teacher> teachers;

		/**
		 * @brief Специальности, закреплённые за кафедрой
		 */
		std::vector<Specialty> specialties;

		/**
		 * @brief Дисциплины, читаемые кафедрой
		 */
		std::vector<Discipline> disciplines;

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
		 * @brief Закрепить за кафедрой группу
		 * @param group - добавляемая группа; повторно не добавляется
		 */
		void addGroup(const Group& group);

		/**
		 * @brief Добавить преподавателя на кафедру
		 * @param teacher - добавляемый преподаватель; если преподаватель
		 *        с таким id уже есть, он заменяется новой версией
		 */
		void addTeacher(const Teacher& teacher);

		/**
		 * @brief Закрепить за кафедрой специальность
		 * @param specialty - добавляемая специальность; если специальность
		 *        с таким кодом уже есть, она заменяется новой версией
		 *        (например, с новым составом групп)
		 */
		void addSpecialty(const Specialty& specialty);

		/**
		 * @brief Добавить дисциплину, читаемую кафедрой
		 * @param discipline - добавляемая дисциплина; если такая уже есть,
		 *        она заменяется новой версией (например, с новым составом
		 *        преподавателей)
		 */
		void addDiscipline(const Discipline& discipline);

		/**
		 * @brief Получить группы кафедры
		 * @return ссылку на массив групп
		 */
		const std::vector<Group>& getGroups() const noexcept;

		/**
		 * @brief Получить преподавателей кафедры
		 * @return ссылку на массив преподавателей
		 */
		const std::vector<Teacher>& getTeachers() const noexcept;

		/**
		 * @brief Получить специальности кафедры
		 * @return ссылку на массив специальностей
		 */
		const std::vector<Specialty>& getSpecialties() const noexcept;

		/**
		 * @brief Получить дисциплины кафедры
		 * @return ссылку на массив дисциплин
		 */
		const std::vector<Discipline>& getDisciplines() const noexcept;

		/**
		 * @brief Получить число групп кафедры
		 * @return число групп
		 */
		std::size_t getGroupCount() const noexcept;

		/**
		 * @brief Получить число преподавателей кафедры
		 * @return число преподавателей
		 */
		std::size_t getTeacherCount() const noexcept;

		/**
		 * @brief Получить число специальностей кафедры
		 * @return число специальностей
		 */
		std::size_t getSpecialtyCount() const noexcept;

		/**
		 * @brief Получить число дисциплин кафедры
		 * @return число дисциплин
		 */
		std::size_t getDisciplineCount() const noexcept;

		/**
		 * @brief Получить полную информацию о кафедре
		 * @return строка с названием кафедры и числом её объектов
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
