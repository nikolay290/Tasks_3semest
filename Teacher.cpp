#include "Teacher.h"

#include <stdexcept>

#include "FormatUtils.h"

Teacher::Teacher(const std::string& fullName, const Date& birthDate, const std::string& position,
	const std::string& degree, const Department& department, const int id)
	: Person(fullName, birthDate, id), position(FormatUtils::trim(position)),
	degree(FormatUtils::trim(degree)), department(department) {
	if (this->position.empty()) {
		throw std::invalid_argument("Должность преподавателя не может быть пустой");
	}
}

const std::string& Teacher::getPosition() const noexcept {return position;}

const std::string& Teacher::getDegree() const noexcept {return degree;}

const Department& Teacher::getDepartment() const noexcept {return department;}

std::string Teacher::getRole() const {return "Преподаватель";}

std::string Teacher::getSpecificInfo() const {
	std::string result = "Должность: " + position + " | Кафедра: " + department.getName();
	if (!degree.empty()) {
		result += " | Учёная степень: " + degree;
	}
	return result;
}

bool Teacher::isTeacher() const {return true;}

bool Teacher::operator==(const Teacher& other) const noexcept {
	return id == other.id &&
		FormatUtils::equalsIgnoreCase(position, other.position) &&
		FormatUtils::equalsIgnoreCase(degree, other.degree) &&
		department == other.department;
}

bool Teacher::operator!=(const Teacher& other) const noexcept {return !(*this == other);}
