#include "Department.h"

#include <stdexcept>

#include "FormatUtils.h"

Department::Department(const std::string& name)
	: name(FormatUtils::trim(name)) {
	if (this->name.empty()) {
		throw std::invalid_argument("Название кафедры не может быть пустым");
	}
}

const std::string& Department::getName() const noexcept {return name;}

std::string Department::getInfo() const {
	return "Кафедра " + name;
}

bool Department::operator==(const Department& other) const noexcept {
	return FormatUtils::equalsIgnoreCase(name, other.name);
}

bool Department::operator!=(const Department& other) const noexcept {return !(*this == other);}
