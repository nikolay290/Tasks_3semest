#include "Department.h"

#include <stdexcept>

#include "Discipline.h"
#include "FormatUtils.h"
#include "Group.h"
#include "Specialty.h"
#include "Teacher.h"

Department::Department(const std::string& name)
	: name(FormatUtils::trim(name)) {
	if (this->name.empty()) {
		throw std::invalid_argument("Название кафедры не может быть пустым");
	}
}

const std::string& Department::getName() const noexcept {return name;}

void Department::addGroup(const Group& group) {
	for (const Group& added : groups) {
		if (added == group) {
			return;
		}
	}
	groups.push_back(group);
}

void Department::addTeacher(const Teacher& teacher) {
	for (Teacher& added : teachers) {
		if (added.getId() == teacher.getId()) {
			added = teacher;
			return;
		}
	}
	teachers.push_back(teacher);
}

void Department::addSpecialty(const Specialty& specialty) {
	for (Specialty& added : specialties) {
		if (FormatUtils::equalsIgnoreCase(added.getCode(), specialty.getCode())) {
			added = specialty;
			return;
		}
	}
	specialties.push_back(specialty);
}

void Department::addDiscipline(const Discipline& discipline) {
	for (Discipline& added : disciplines) {
		if (added == discipline) {
			added = discipline;
			return;
		}
	}
	disciplines.push_back(discipline);
}

const std::vector<Group>& Department::getGroups() const noexcept {return groups;}

const std::vector<Teacher>& Department::getTeachers() const noexcept {return teachers;}

const std::vector<Specialty>& Department::getSpecialties() const noexcept {return specialties;}

const std::vector<Discipline>& Department::getDisciplines() const noexcept {return disciplines;}

std::size_t Department::getGroupCount() const noexcept {return groups.size();}

std::size_t Department::getTeacherCount() const noexcept {return teachers.size();}

std::size_t Department::getSpecialtyCount() const noexcept {return specialties.size();}

std::size_t Department::getDisciplineCount() const noexcept {return disciplines.size();}

std::string Department::getInfo() const {
	return "Кафедра " + name
		+ " | Специальностей: " + std::to_string(specialties.size())
		+ " | Групп: " + std::to_string(groups.size())
		+ " | Преподавателей: " + std::to_string(teachers.size())
		+ " | Дисциплин: " + std::to_string(disciplines.size());
}

bool Department::operator==(const Department& other) const noexcept {
	return FormatUtils::equalsIgnoreCase(name, other.name);
}

bool Department::operator!=(const Department& other) const noexcept {return !(*this == other);}
