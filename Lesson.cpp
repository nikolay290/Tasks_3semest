#include "Lesson.h"

#include <stdexcept>

#include "FormatUtils.h"
#include "Group.h"

std::string toString(const LessonType type) {
	switch (type) {
		case LessonType::Lecture:
			return "Лекция";
		case LessonType::Practical:
			return "Практическое занятие";
		case LessonType::Lab:
			return "Лабораторная работа";
		default:
			return "Неизвестный вид занятия";
	}
}

Lesson::Lesson(const LessonType& type, const Discipline& discipline, const Group& group,
	const Teacher& teacher, const Teacher* secondTeacher, const std::string& room,
	const Date& date)
	: type(type), discipline(discipline), groupNumber(group.getNumber()), room(room), date(date) {
	if (type == LessonType::Lab) {
		if (secondTeacher == nullptr) {
			throw std::invalid_argument("Лабораторную работу должен вести второй преподаватель");
		}
		if (teacher.getId() == secondTeacher->getId()) {
			throw std::invalid_argument("Лабораторную работу должны вести два разных преподавателя");
		}
		teachers.push_back(teacher);
		teachers.push_back(*secondTeacher);
		return;
	}
	if (secondTeacher != nullptr) {
		throw std::invalid_argument("Второй преподаватель нужен только для лабораторной работы");
	}
	teachers.push_back(teacher);
}

LessonType Lesson::getType() const noexcept {return type;}

const Discipline& Lesson::getDiscipline() const noexcept {return discipline;}

const std::string& Lesson::getGroupNumber() const noexcept {return groupNumber;}

const std::vector<Teacher>& Lesson::getTeachers() const noexcept {return teachers;}

std::size_t Lesson::getTeacherCount() const noexcept {return teachers.size();}

const std::string& Lesson::getRoom() const noexcept {return room;}

const Date& Lesson::getDate() const noexcept {return date;}

bool Lesson::isTaughtBy(const Teacher& teacher) const noexcept {
	for (const Teacher& lessonTeacher : teachers) {
		if (lessonTeacher.getId() == teacher.getId()) {
			return true;
		}
	}
	return false;
}

std::string Lesson::getShortInfo() const {
	return toString(type) + " | " + discipline.getShortInfo() + " | группа " + groupNumber;
}

std::string Lesson::getInfo() const {
	std::vector<std::string> teacherNames;
	for (const Teacher& lessonTeacher : teachers) {
		teacherNames.push_back(lessonTeacher.getFullName());
	}
	std::string result = getShortInfo();
	result += " | " + FormatUtils::join(teacherNames, ", ");
	if (!room.empty()) {
		result += " | ауд. " + room;
	}
	result += " | " + date.toString();
	return result;
}

bool Lesson::operator==(const Lesson& other) const noexcept {
	if (type != other.type ||
		discipline != other.discipline ||
		groupNumber != other.groupNumber ||
		room != other.room ||
		date != other.date ||
		teachers.size() != other.teachers.size()) {
		return false;
	}
	for (const Teacher& lessonTeacher : teachers) {
		if (!other.isTaughtBy(lessonTeacher)) {
			return false;
		}
	}
	return true;
}

bool Lesson::operator!=(const Lesson& other) const noexcept {return !(*this == other);}
