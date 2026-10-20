#include "University.h"

#include <stdexcept>

#include "FormatUtils.h"
#include "Person.h"
#include "Student.h"
#include "Specialty.h"

University::University()
	: name("ВУЗ") {}

University::University(const std::string& name)
	: name(FormatUtils::trim(name)) {
	if (this->name.empty()) {
		throw std::invalid_argument("Название ВУЗа не может быть пустым");
	}
}

const std::string& University::getName() const noexcept {return name;}

void University::setName(const std::string& newName) {
	const std::string trimmedName = FormatUtils::trim(newName);
	if (trimmedName.empty()) {
		throw std::invalid_argument("Название ВУЗа не может быть пустым");
	}
	this->name = trimmedName;
}

void University::addSpecialty(const Specialty& specialty) {
	if (findSpecialtyByCode(specialty.getCode())) {
		throw std::invalid_argument("Специальность с кодом " + specialty.getCode() + " уже есть в ВУЗе");
	}
	specialties.push_back(specialty);
}

void University::addDepartment(const Department& department) {
	if (findDepartmentByName(department.getName())) {
		throw std::invalid_argument("Кафедра " + department.getName() + " уже есть в ВУЗе");
	}
	departments.push_back(department);
}

void University::addDiscipline(const Discipline& discipline) {
	for (const Discipline& added : disciplines) {
		if (added == discipline) {
			throw std::invalid_argument("Дисциплина " + discipline.getName() + " уже есть в ВУЗе");
		}
	}
	disciplines.push_back(discipline);
}

void University::addGroup(const Group& group) {
	if (findGroupByNumber(group.getNumber())) {
		throw std::invalid_argument("Группа " + group.getNumber() + " уже есть в ВУЗе");
	}
	groups.push_back(group);
}

void University::addTeacher(const Teacher& teacher) {
	if (findTeacherById(teacher.getId())) {
		throw std::invalid_argument("Преподаватель с id " + std::to_string(teacher.getId()) +
			" уже есть в ВУЗе");
	}
	teachers.push_back(teacher);
}

void University::addLesson(const Lesson& lesson) {
	if (!findGroupByNumber(lesson.getGroupNumber())) {
		throw std::invalid_argument("Группа " + lesson.getGroupNumber() + " не найдена в ВУЗе");
	}
	for (const Teacher& lessonTeacher : lesson.getTeachers()) {
		if (!findTeacherById(lessonTeacher.getId())) {
			throw std::invalid_argument("Преподаватель " + lessonTeacher.getFullName() +
				" не найден в ВУЗе");
		}
	}
	lessons.push_back(lesson);
}

const std::vector<Specialty>& University::getSpecialties() const noexcept {return specialties;}

const std::vector<Department>& University::getDepartments() const noexcept {return departments;}

const std::vector<Discipline>& University::getDisciplines() const noexcept {return disciplines;}

const std::vector<Group>& University::getGroups() const noexcept {return groups;}

const std::vector<Teacher>& University::getTeachers() const noexcept {return teachers;}

const std::vector<Lesson>& University::getLessons() const noexcept {return lessons;}

bool University::findSpecialtyByCode(const std::string& code) const noexcept {
	for (const Specialty& specialty : specialties) {
		if (FormatUtils::equalsIgnoreCase(specialty.getCode(), code)) {
			return true;
		}
	}
	return false;
}

bool University::findDepartmentByName(const std::string& departmentName) const noexcept {
	for (const Department& department : departments) {
		if (FormatUtils::equalsIgnoreCase(department.getName(), departmentName)) {
			return true;
		}
	}
	return false;
}

bool University::findGroupByNumber(const std::string& groupNumber) const noexcept {
	for (const Group& group : groups) {
		if (group.getNumber() == groupNumber) {
			return true;
		}
	}
	return false;
}

bool University::findTeacherById(const int id) const noexcept {
	for (const Teacher& teacher : teachers) {
		if (teacher.getId() == id) {
			return true;
		}
	}
	return false;
}

bool University::findTeacherByFullName(const std::string& fullName) const {
	return !getTeacherByFullName(fullName).empty();
}

bool University::findStudentByRecordBook(const std::string& recordBook) const {
	return !getStudentByRecordBook(recordBook).empty();
}

std::vector<Student> University::getStudentByRecordBook(const std::string& recordBook) const {
	for (const Group& group : groups) {
		const std::vector<Student> found = group.getStudentByRecordBook(recordBook);
		if (!found.empty()) {
			return found;
		}
	}
	return {};
}

std::vector<Teacher> University::getTeacherByFullName(const std::string& fullName) const {
	std::vector<Teacher> result;
	for (const Teacher& teacher : teachers) {
		if (FormatUtils::equalsIgnoreCase(teacher.getFullName(), fullName)) {
			result.push_back(teacher);
			break;
		}
	}
	return result;
}

std::vector<Student> University::findStudentsByFullName(const std::string& fullName) const {
	std::vector<Student> result;
	for (const Group& group : groups) {
		for (const Student& student : group.getStudents()) {
			if (FormatUtils::containsIgnoreCase(student.getFullName(), fullName)) {
				result.push_back(student);
			}
		}
	}
	return result;
}

std::vector<Discipline> University::getDisciplinesByDepartment(const Department& department) const {
	std::vector<Discipline> result;
	for (const Discipline& discipline : disciplines) {
		if (discipline.getDepartment() == department) {
			result.push_back(discipline);
		}
	}
	return result;
}

std::vector<Teacher> University::getTeachersByGroup(const Group& group) const {
	std::vector<Teacher> result;
	for (const Lesson& lesson : lessons) {
		if (lesson.getGroupNumber() != group.getNumber()) {
			continue;
		}
		for (const Teacher& lessonTeacher : lesson.getTeachers()) {
			bool alreadyAdded = false;
			for (const Teacher& added : result) {
				if (added.getId() == lessonTeacher.getId()) {
					alreadyAdded = true;
					break;
				}
			}
			if (alreadyAdded) {
				continue;
			}
			for (const Teacher& teacher : teachers) {
				if (teacher.getId() == lessonTeacher.getId()) {
					result.push_back(teacher);
					break;
				}
			}
		}
	}
	return result;
}

std::vector<Group> University::getGroupsBySpecialty(const Specialty& specialty) const {
	std::vector<Group> result;
	for (const Group& group : groups) {
		if (group.getSpecialty() == specialty) {
			result.push_back(group);
		}
	}
	return result;
}

std::vector<Student> University::getAllStudents() const {
	std::vector<Student> result;
	for (const Group& group : groups) {
		for (const Student& student : group.getStudents()) {
			result.push_back(student);
		}
	}
	return result;
}

std::vector<Teacher> University::getAllTeachers() const {
	return teachers;
}

std::string University::getStatistics() const {
	std::size_t studentCount = 0;
	std::size_t markCount = 0;
	double totalMarkSum = 0.0;
	for (const Group& group : groups) {
		studentCount += group.getStudentCount();
		for (const Student& student : group.getStudents()) {
			markCount += student.getMarkCount();
			totalMarkSum += student.getAverageMark() * static_cast<double>(student.getMarkCount());
		}
	}
	const double averageMark = markCount == 0 ? 0.0 : totalMarkSum / static_cast<double>(markCount);
	return "ВУЗ: " + name +
		" | Специальностей: " + std::to_string(specialties.size()) +
		" | Кафедр: " + std::to_string(departments.size()) +
		" | Дисциплин: " + std::to_string(disciplines.size()) +
		" | Групп: " + std::to_string(groups.size()) +
		" | Студентов: " + std::to_string(studentCount) +
		" | Преподавателей: " + std::to_string(teachers.size()) +
		" | Занятий: " + std::to_string(lessons.size()) +
		" | Оценок: " + std::to_string(markCount) +
		" | Средний балл: " + FormatUtils::formatDecimal(averageMark);
}
