#include "Discipline.h"

#include <stdexcept>

#include "FormatUtils.h"

Discipline::Discipline(const std::string& name, const Department& department)
	: name(FormatUtils::trim(name)), department(department) {
	if (this->name.empty()) {
		throw std::invalid_argument("Название дисциплины не может быть пустым");
	}
}

const std::string& Discipline::getName() const noexcept {return name;}

const Department& Discipline::getDepartment() const noexcept {return department;}

std::string Discipline::getShortInfo() const {
	return name;
}

std::string Discipline::getInfo() const {
	return name + " (читает " + department.getName() + ")";
}

bool Discipline::operator==(const Discipline& other) const noexcept {
	return FormatUtils::equalsIgnoreCase(name, other.name) && department == other.department;
}

bool Discipline::operator!=(const Discipline& other) const noexcept {return !(*this == other);}
