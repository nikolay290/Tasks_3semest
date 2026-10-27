#include "Discipline.h"
#include "Teacher.h"

#include "Group.h"

#include <stdexcept>

#include "FormatUtils.h"

Group::Group(const std::string& number, const Specialty& specialty)
	: number(FormatUtils::trim(number)), specialty(specialty) {
	if (this->number.empty()) {
		throw std::invalid_argument("Номер группы не может быть пустым");
	}
}

const std::string& Group::getNumber() const noexcept {return number;}

const Specialty& Group::getSpecialty() const noexcept {return specialty;}

void Group::addStudent(const Student& student) {
	if (findStudentByRecordBook(student.getRecordBook())) {
		throw std::invalid_argument("Студент с номером зачётной книжки " +
			student.getRecordBook() + " уже есть в группе " + number);
	}
	Student stored = student;
	stored.setGroup(*this);
	students.push_back(stored);
}

const std::vector<Student>& Group::getStudents() const noexcept {return students;}

std::size_t Group::getStudentCount() const noexcept {return students.size();}

bool Group::findStudentByRecordBook(const std::string& recordBook) const {
	return !getStudentByRecordBook(recordBook).empty();
}

std::vector<Student> Group::getStudentByRecordBook(const std::string& recordBook) const {
	for (const Student& student : students) {
		if (FormatUtils::equalsIgnoreCase(student.getRecordBook(), recordBook)) {
			return {student};
		}
	}
	return {};
}

double Group::getAverageMark() const noexcept {
	double sum = 0.0;
	std::size_t markCount = 0;
	for (const Student& student : students) {
		sum += student.getAverageMark() * static_cast<double>(student.getMarkCount());
		markCount += student.getMarkCount();
	}
	if (markCount == 0) {
		return 0.0;
	}
	return sum / static_cast<double>(markCount);
}

std::string Group::getShortInfo() const {
	return number;
}

std::string Group::getInfo() const {
	return "Группа " + number + " | " + specialty.getShortInfo() +
		" | Студентов: " + std::to_string(students.size());
}

bool Group::operator==(const Group& other) const noexcept {
	return number == other.number && specialty == other.specialty;
}

bool Group::operator!=(const Group& other) const noexcept {return !(*this == other);}
