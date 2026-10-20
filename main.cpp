#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#endif

#include "VuzLib.h"

namespace {

	/**
	 * @brief Переключить консоль на кодировку UTF-8, чтобы кириллица
	 *        выводилась корректно в любой локали Windows.
	 */
	void enableUtf8Console() {
#ifdef _WIN32
		SetConsoleOutputCP(CP_UTF8);
		SetConsoleCP(CP_UTF8);
#endif
	}

	/**
	 * @brief Вывести заголовок раздела отчёта
	 * @param title - заголовок раздела
	 */
	void printDivider(const std::string& title) {
		std::cout << "\n=== " << title << " ===" << std::endl;
	}

	/**
	 * @brief Создать коллекцию объектов базового типа Person и заполнить
	 *        её объектами типов-наследников (Student и Teacher). Именно
	 *        эта коллекция далее обходится в main() как коллекция
	 *        базового класса.
	 * @param university - заполненный ВУЗ
	 * @return не владеющие указатели на людей ВУЗа; ими владеют контейнеры
	 *         внутри @p university, поэтому коллекцию можно читать, пока
	 *         ВУЗ не меняется
	 */
	std::vector<const Person*> createPeopleCollection(const University& university) {
		std::vector<const Person*> people;
		for (const Group& group : university.getGroups()) {
			for (const Student& student : group.getStudents()) {
				people.push_back(&student);
			}
		}
		for (const Teacher& teacher : university.getTeachers()) {
			people.push_back(&teacher);
		}
		return people;
	}

	/**
	 * @brief Обойти коллекцию как коллекцию базового класса и вывести
	 *        информацию о каждом объекте в std::cout
	 * @param people - коллекция людей ВУЗа
	 */
	void printAllPeople(const std::vector<const Person*>& people) {
		for (const Person* person : people) {
			std::cout << "  - " << person->getInfo() << std::endl;
		}
	}

	/**
	 * @brief Вывести информацию о студенте по номеру зачётной книжки
	 * @param university - ВУЗ
	 * @param recordBook - номер зачётной книжки
	 */
	void printStudentInfoByRecordBook(const University& university, const std::string& recordBook) {
		const std::vector<Student> students = university.getStudentByRecordBook(recordBook);
		if (students.empty()) {
			std::cout << "  Студент с номером зачётной книжки " << recordBook << " не найден" << std::endl;
			return;
		}
		std::cout << "  - " << students.at(0).getInfo() << std::endl;
	}

	/**
	 * @brief Вывести информацию о студентах по ФИО или его части
	 * @param university - ВУЗ
	 * @param fullName - ФИО или его часть
	 */
	void printStudentsByFullName(const University& university, const std::string& fullName) {
		const std::vector<Student> students = university.findStudentsByFullName(fullName);
		if (students.empty()) {
			std::cout << "  Студенты по фамилии " << fullName << " не найдены" << std::endl;
			return;
		}
		for (const Student& student : students) {
			std::cout << "  - " << student.getInfo() << std::endl;
		}
	}

	/**
	 * @brief Вывести список дисциплин, читаемых кафедрой
	 * @param university - ВУЗ
	 * @param department - кафедра
	 */
	void printDisciplinesByDepartment(const University& university, const Department& department) {
		std::cout << "  " << department.getInfo() << std::endl;
		const std::vector<Discipline> disciplines = university.getDisciplinesByDepartment(department);
		if (disciplines.empty()) {
			std::cout << "  Кафедра не читает ни одной дисциплины" << std::endl;
			return;
		}
		for (const Discipline& discipline : disciplines) {
			std::cout << "  - " << discipline.getInfo() << std::endl;
		}
	}

	/**
	 * @brief Вывести список преподавателей, проводящих занятия в группе
	 * @param university - ВУЗ
	 * @param group - группа
	 */
	void printTeachersByGroup(const University& university, const Group& group) {
		std::cout << "  " << group.getInfo() << std::endl;
		const std::vector<Teacher> teachers = university.getTeachersByGroup(group);
		if (teachers.empty()) {
			std::cout << "  В группе не проводятся занятия" << std::endl;
			return;
		}
		for (const Teacher& teacher : teachers) {
			std::cout << "  - " << teacher.getInfo() << std::endl;
		}
	}

	/**
	 * @brief Вывести список групп, обучающихся на специальности
	 * @param university - ВУЗ
	 * @param specialty - специальность
	 */
	void printGroupsBySpecialty(const University& university, const Specialty& specialty) {
		std::cout << "  " << specialty.getInfo() << std::endl;
		const std::vector<Group> groups = university.getGroupsBySpecialty(specialty);
		if (groups.empty()) {
			std::cout << "  На специальности нет групп" << std::endl;
			return;
		}
		for (const Group& group : groups) {
			std::cout << "  - " << group.getInfo() << std::endl;
		}
	}
}

/**
 * @brief Точка входа в программу
 * @return 0, если программа выполнена корректно
 */
int main() {
	enableUtf8Console();

	University university("Национальный исследовательский университет");

	const Department appliedMath("Прикладная математика");
	const Department informationSystems("Информационные системы");
	university.addDepartment(appliedMath);
	university.addDepartment(informationSystems);

	const Specialty informatics("09.02.07", "Информатика и вычислительная техника");
	const Specialty economics("38.02.01", "Экономика и бухгалтерский учёт");
	university.addSpecialty(informatics);
	university.addSpecialty(economics);

	const Discipline discreteMath("Дискретная математика", appliedMath);
	const Discipline linearAlgebra("Линейная алгебра", appliedMath);
	const Discipline databases("Базы данных", informationSystems);
	const Discipline webDevelopment("Веб-разработка", informationSystems);
	university.addDiscipline(discreteMath);
	university.addDiscipline(linearAlgebra);
	university.addDiscipline(databases);
	university.addDiscipline(webDevelopment);

	const Teacher ivanov("Иванов Иван Иванович", Date(1975, 3, 12), "профессор",
		"доктор технических наук", appliedMath);
	const Teacher petrov("Петров Пётр Петрович", Date(1982, 7, 25), "доцент",
		"кандидат физико-математических наук", appliedMath);
	const Teacher kuznetsova("Кузнецова Мария Сергеевна", Date(1988, 11, 3), "доцент",
		"кандидат технических наук", informationSystems);
	university.addTeacher(ivanov);
	university.addTeacher(petrov);
	university.addTeacher(kuznetsova);

	Group group1I("1И", informatics);
	Group group2I("2И", informatics);
	Group group1E("1Э", economics);

	Student smirnov("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2);
	smirnov.addMark(5.0);
	smirnov.addMark(4.0);
	smirnov.addMark(5.0);
	group1I.addStudent(smirnov);

	Student volkov("Волков Дмитрий Андреевич", Date(2005, 9, 30), "17-143", 2);
	volkov.addMark(4.0);
	volkov.addMark(4.0);
	group1I.addStudent(volkov);

	Student orlova("Орлова Елена Викторовна", Date(2004, 12, 8), "16-101", 3);
	orlova.addMark(5.0);
	orlova.addMark(5.0);
	group2I.addStudent(orlova);

	Student morozov("Морозов Павел Олегович", Date(2006, 2, 17), "18-201", 1);
	morozov.addMark(3.0);
	morozov.addMark(4.0);
	group1E.addStudent(morozov);

	university.addGroup(group1I);
	university.addGroup(group2I);
	university.addGroup(group1E);

	university.addLesson(Lesson(LessonType::Lecture, discreteMath, group1I, ivanov, nullptr,
		"25", Date(2026, 9, 1)));
	university.addLesson(Lesson(LessonType::Lab, discreteMath, group1I, petrov, &ivanov,
		"12", Date(2026, 9, 3)));
	university.addLesson(Lesson(LessonType::Practical, linearAlgebra, group2I, petrov, nullptr,
		"25", Date(2026, 9, 2)));
	university.addLesson(Lesson(LessonType::Lab, databases, group1I, kuznetsova, &ivanov,
		"34", Date(2026, 9, 4)));
	university.addLesson(Lesson(LessonType::Lecture, webDevelopment, group2I, kuznetsova, nullptr,
		"34", Date(2026, 9, 5)));

	printDivider("Все люди ВУЗа (коллекция базового типа Person)");
	printAllPeople(createPeopleCollection(university));

	printDivider("Запрос 1: студент по номеру зачётной книжки");
	printStudentInfoByRecordBook(university, "17-142");
	printStudentInfoByRecordBook(university, "99-999");

	printDivider("Запрос 1: студенты по ФИО");
	printStudentsByFullName(university, "Орлова");

	printDivider("Запрос 2: дисциплины кафедры");
	printDisciplinesByDepartment(university, appliedMath);
	printDisciplinesByDepartment(university, informationSystems);

	printDivider("Запрос 3: преподаватели группы");
	printTeachersByGroup(university, group1I);
	printTeachersByGroup(university, group1E);

	printDivider("Запрос 4: группы специальности");
	printGroupsBySpecialty(university, informatics);
	printGroupsBySpecialty(university, economics);

	printDivider("Расписание занятий");
	for (const Lesson& lesson : university.getLessons()) {
		std::cout << "  - " << lesson.getInfo() << std::endl;
	}

	printDivider("Статистика");
	std::cout << "  " << university.getStatistics() << std::endl;

	return 0;
}
