#include "Discipline.h"

#include <stdexcept>

#include "FormatUtils.h"
#include "Group.h"
#include "Specialty.h"
#include "Teacher.h"

Discipline::Discipline(const std::string& name, const Department& department)
	: name(FormatUtils::trim(name)), department(department) {
	if (this->name.empty()) {
		throw std::invalid_argument("Название дисциплины не может быть пустым");
	}
}

const std::string& Discipline::getName() const noexcept {return name;}

const Department& Discipline::getDepartment() const noexcept {return department;}

void Discipline::addTeacher(const Teacher& teacher) {
	for (const Teacher& added : teachers) {
		if (added.getId() == teacher.getId()) {
			return;
		}
	}
	Teacher stored = teacher;
	stored.addDiscipline(*this);
	teachers.push_back(stored);
}

const std::vector<Teacher>& Discipline::getTeachers() const noexcept {return teachers;}

std::size_t Discipline::getTeacherCount() const noexcept {return teachers.size();}

std::string Discipline::getShortInfo() const {
	return name;
}

std::string Discipline::getInfo() const {
	return name + " (читает " + department.getName() + ")"
		+ " | Преподавателей: " + std::to_string(teachers.size());
}

bool Discipline::operator==(const Discipline& other) const noexcept {
	return FormatUtils::equalsIgnoreCase(name, other.name) && department == other.department;
}

bool Discipline::operator!=(const Discipline& other) const noexcept {return !(*this == other);}
