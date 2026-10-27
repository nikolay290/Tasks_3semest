#include "Discipline.h"
#include "Teacher.h"

#include "Group.h"

#include <stdexcept>

#include "FormatUtils.h"

namespace {
	/**
	 * @brief Построить облегчённую копию дисциплины: название и кафедра без
	 *        списка преподавателей. Так группа знает изучаемые дисциплины,
	 *        но копии преподавателей не вкладываются друг в друга бесконечно.
	 * @param discipline - исходная дисциплина
	 * @return дисциплина с тем же названием и кафедрой без преподавателей
	 */
	Discipline makeLightDiscipline(const Discipline& discipline) {
		const Department department(discipline.getDepartment().getName());
		return Discipline(discipline.getName(), department);
	}
}

Group::Group(const std::string& number, const Specialty& specialty)
	: number(FormatUtils::trim(number)), specialty(specialty) {
	if (this->number.empty()) {
		throw std::invalid_argument("Номер группы не может быть пустым");
	}
}

const std::string& Group::getNumber() const noexcept {return number;}

const Specialty& Group::getSpecialty() const noexcept {return specialty;}

void Group::addDiscipline(const Discipline& discipline) {
	for (const Discipline& added : disciplines) {
		if (added == discipline) {
			return;
		}
	}
	disciplines.push_back(makeLightDiscipline(discipline));
}

const std::vector<Discipline>& Group::getDisciplines() const noexcept {return disciplines;}

std::size_t Group::getDisciplineCount() const noexcept {return disciplines.size();}

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
	std::vector<Student> result;
	const std::string trimmedRecordBook = FormatUtils::trim(recordBook);
	for (const Student& student : students) {
		if (FormatUtils::equalsIgnoreCase(student.getRecordBook(), trimmedRecordBook)) {
			result.push_back(student);
		}
	}
	return result;
}

double Group::getAverageMark() const noexcept {
	if (students.empty()) {
		return 0.0;
	}
	double sum = 0.0;
	std::size_t count = 0;
	for (const Student& student : students) {
		for (const double mark : student.getMarks()) {
			sum += mark;
			++count;
		}
	}
	if (count == 0) {
		return 0.0;
	}
	return sum / static_cast<double>(count);
}

std::string Group::getShortInfo() const {
	return "Группа " + number + " (" + specialty.getShortInfo() + ")";
}

std::string Group::getInfo() const {
	return getShortInfo() + " | Студентов: " + std::to_string(students.size())
		+ " | Дисциплин: " + std::to_string(disciplines.size());
}

bool Group::operator==(const Group& other) const noexcept {
	return number == other.number && specialty == other.specialty;
}

bool Group::operator!=(const Group& other) const noexcept {return !(*this == other);}
