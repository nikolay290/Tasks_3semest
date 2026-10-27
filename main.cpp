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
	void enableUtf8Console();

	/**
	 * @brief Вывести заголовок раздела отчёта
	 * @param title - заголовок раздела
	 */
	void printDivider(const std::string& title);

	/**
	 * @brief Вывести всех людей ВУЗа. Обход построен как в образце: два
	 *        цикла по контейнерам, элемент привязан к const Person&, а
	 *        getInfo() вызывается через базовый тип. Так как getInfo()
	 *        виртуальный, для студента и преподавателя выполняется их
	 *        собственная версия метода.
	 * @param students - студенты ВУЗа
	 * @param teachers - преподаватели ВУЗа
	 */
	void printAllPeople(const std::vector<Student>& students,
		const std::vector<Teacher>& teachers);

	/**
	 * @brief Вывести информацию о студенте по номеру зачётной книжки
	 * @param university - ВУЗ
	 * @param recordBook - номер зачётной книжки
	 */
	void printStudentInfoByRecordBook(const University& university,
		const std::string& recordBook);

	/**
	 * @brief Вывести информацию о студентах по ФИО или его части
	 * @param university - ВУЗ
	 * @param fullName - ФИО или его часть
	 */
	void printStudentsByFullName(const University& university, const std::string& fullName);

	/**
	 * @brief Вывести студентов и группы, в которых они обучаются.
	 *        Показывает, что студент знает свою группу.
	 * @param university - ВУЗ
	 */
	void printStudentGroups(const University& university);

	/**
	 * @brief Вывести специальности, их кафедры и обучающиеся группы.
	 *        Показывает, что специальность знает кафедру и свои группы.
	 * @param university - ВУЗ
	 */
	void printSpecialties(const University& university);

	/**
	 * @brief Вывести состав кафедр: специальности, группы, преподавателей
	 *        и дисциплины. Показывает, что кафедра знает все свои объекты.
	 * @param university - ВУЗ
	 */
	void printDepartments(const University& university);

	/**
	 * @brief Вывести дисциплины и преподавателей, которые их читают.
	 *        Показывает, что дисциплина знает свой преподавательский состав.
	 * @param university - ВУЗ
	 */
	void printDisciplineTeachers(const University& university);

	/**
	 * @brief Вывести преподавателей и дисциплины, которые они читают.
	 *        Показывает, что преподаватель знает свои дисциплины.
	 * @param university - ВУЗ
	 */
	void printTeacherDisciplines(const University& university);

	/**
	 * @brief Вывести список дисциплин, читаемых кафедрой
	 * @param university - ВУЗ
	 * @param department - кафедра
	 */
	void printDisciplinesByDepartment(const University& university,
		const Department& department);

	/**
	 * @brief Вывести список преподавателей, проводящих занятия в группе
	 * @param university - ВУЗ
	 * @param group - группа
	 */
	void printTeachersByGroup(const University& university, const Group& group);

	/**
	 * @brief Вывести список групп, обучающихся на специальности
	 * @param university - ВУЗ
	 * @param specialty - специальность
	 */
	void printGroupsBySpecialty(const University& university, const Specialty& specialty);
}

/**
 * @brief Точка входа в программу
 * @return 0, если программа выполнена корректно
 */
int main() {
	enableUtf8Console();

	University university("Российский университет транспорта");

	const Department appliedMath("Прикладная математика");
	const Department informationSystems("Информационные системы");
	const Department economicsManagement("Экономика и управление");
	university.addDepartment(appliedMath);
	university.addDepartment(informationSystems);
	university.addDepartment(economicsManagement);

	const Specialty informatics("09.02.07", "Информатика и вычислительная техника",
		informationSystems);
	const Specialty economics("38.02.01", "Экономика и бухгалтерский учёт",
		economicsManagement);
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

	Student smirnov("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2, group1I);
	smirnov.addMark(5.0);
	smirnov.addMark(4.0);
	smirnov.addMark(5.0);
	group1I.addStudent(smirnov);

	Student volkov("Волков Дмитрий Андреевич", Date(2005, 9, 30), "17-143", 2, group1I);
	volkov.addMark(4.0);
	volkov.addMark(4.0);
	group1I.addStudent(volkov);

	Student orlova("Орлова Елена Викторовна", Date(2004, 12, 8), "16-101", 3, group2I);
	orlova.addMark(5.0);
	orlova.addMark(5.0);
	group2I.addStudent(orlova);

	Student morozov("Морозов Павел Олегович", Date(2006, 2, 17), "18-201", 1, group1E);
	morozov.addMark(3.0);
	morozov.addMark(4.0);
	group1E.addStudent(morozov);

	university.addGroup(group1I);
	university.addGroup(group2I);
	university.addGroup(group1E);

	university.addLesson(Lesson(LessonType::Lecture, discreteMath, group1I, {ivanov},
		"25", Date(2026, 9, 1)));
	university.addLesson(Lesson(LessonType::Lab, discreteMath, group1I, {petrov, ivanov},
		"12", Date(2026, 9, 3)));
	university.addLesson(Lesson(LessonType::Practical, linearAlgebra, group2I, {petrov},
		"25", Date(2026, 9, 2)));
	university.addLesson(Lesson(LessonType::Lab, databases, group1I, {kuznetsova, ivanov},
		"34", Date(2026, 9, 4)));
	university.addLesson(Lesson(LessonType::Lecture, webDevelopment, group2I, {kuznetsova},
		"34", Date(2026, 9, 5)));

	printDivider("Студенты и их группы (студент знает группу, в которой обучается)");
	printStudentGroups(university);

	printDivider("Все люди ВУЗа (обход коллекции базового типа Person)");
	printAllPeople(university.getAllStudents(), university.getAllTeachers());

	printDivider("Состав кафедр (кафедра знает свои специальности, группы, преподавателей и дисциплины)");
	printDepartments(university);

	printDivider("Специальности и их группы (специальность знает кафедру и обучающиеся группы)");
	printSpecialties(university);

	printDivider("Дисциплины и их преподаватели (дисциплина знает, кто её читает)");
	printDisciplineTeachers(university);

	printDivider("Преподаватели и их дисциплины (преподаватель знает, что читает)");
	printTeacherDisciplines(university);

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

namespace {

	void enableUtf8Console() {
#ifdef _WIN32
		SetConsoleOutputCP(CP_UTF8);
		SetConsoleCP(CP_UTF8);
#endif
	}

	void printDivider(const std::string& title) {
		std::cout << "\n=== " << title << " ===" << std::endl;
	}

	void printAllPeople(const std::vector<Student>& students,
		const std::vector<Teacher>& teachers) {
		for (const Person& student : students) {
			std::cout << "  - " << student.getInfo() << std::endl;
		}
		for (const Person& teacher : teachers) {
			std::cout << "  - " << teacher.getInfo() << std::endl;
		}
	}

	void printStudentInfoByRecordBook(const University& university,
		const std::string& recordBook) {
		const std::vector<Student> students = university.getStudentByRecordBook(recordBook);
		if (students.empty()) {
			std::cout << "  Студент с номером зачётной книжки " << recordBook << " не найден" << std::endl;
			return;
		}
		std::cout << "  - " << students.at(0).getInfo() << std::endl;
	}

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

	void printStudentGroups(const University& university) {
		for (const Student& student : university.getAllStudents()) {
			std::cout << "  - " << student.getFullName()
				<< " | Группа: " << student.getGroup().getNumber()
				<< " | Специальность: " << student.getGroup().getSpecialty().getShortInfo()
				<< std::endl;
		}
	}

	void printSpecialties(const University& university) {
		for (const Specialty& specialty : university.getSpecialties()) {
			std::cout << "  - " << specialty.getInfo() << std::endl;
			std::cout << "    Группы:";
			if (specialty.getGroups().empty()) {
				std::cout << " нет";
			}
			for (const Group& group : specialty.getGroups()) {
				std::cout << " " << group.getNumber() << ";";
			}
			std::cout << std::endl;
		}
	}

	void printDepartments(const University& university) {
		for (const Department& department : university.getDepartments()) {
			std::cout << "  " << department.getInfo() << std::endl;

			std::cout << "    Специальности:";
			if (department.getSpecialties().empty()) {
				std::cout << " нет";
			}
			for (const Specialty& specialty : department.getSpecialties()) {
				std::cout << " " << specialty.getShortInfo()
					<< " (групп: " << specialty.getGroupCount() << ");";
			}
			std::cout << std::endl;

			std::cout << "    Группы:";
			if (department.getGroups().empty()) {
				std::cout << " нет";
			}
			for (const Group& group : department.getGroups()) {
				std::cout << " " << group.getNumber() << ";";
			}
			std::cout << std::endl;

			std::cout << "    Преподаватели:";
			if (department.getTeachers().empty()) {
				std::cout << " нет";
			}
			for (const Teacher& teacher : department.getTeachers()) {
				std::cout << " " << teacher.getFullName() << ";";
			}
			std::cout << std::endl;

			std::cout << "    Дисциплины:";
			if (department.getDisciplines().empty()) {
				std::cout << " нет";
			}
			for (const Discipline& discipline : department.getDisciplines()) {
				std::cout << " " << discipline.getName() << ";";
			}
			std::cout << std::endl;
		}
	}

	void printDisciplineTeachers(const University& university) {
		for (const Discipline& discipline : university.getDisciplines()) {
			std::cout << "  - " << discipline.getInfo() << std::endl;
			std::cout << "    Преподаватели:";
			if (discipline.getTeachers().empty()) {
				std::cout << " нет";
			}
			for (const Teacher& teacher : discipline.getTeachers()) {
				std::cout << " " << teacher.getFullName() << ";";
			}
			std::cout << std::endl;
		}
	}

	void printTeacherDisciplines(const University& university) {
		for (const Teacher& teacher : university.getAllTeachers()) {
			std::cout << "  - " << teacher.getFullName() << " | Дисциплины:";
			if (teacher.getDisciplines().empty()) {
				std::cout << " нет";
			}
			for (const Discipline& discipline : teacher.getDisciplines()) {
				std::cout << " " << discipline.getName() << ";";
			}
			std::cout << std::endl;
		}
	}

	void printDisciplinesByDepartment(const University& university,
		const Department& department) {
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
