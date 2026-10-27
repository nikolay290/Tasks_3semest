#include "Specialty.h"

#include <stdexcept>

#include "Discipline.h"
#include "FormatUtils.h"
#include "Teacher.h"
#include "Group.h"

Specialty::Specialty(const std::string& code, const std::string& name, const Department& department)
	: code(FormatUtils::trim(code)), name(FormatUtils::trim(name)), department(department) {
	if (this->code.empty()) {
		throw std::invalid_argument("Код специальности не может быть пустым");
	}
	if (this->name.empty()) {
		throw std::invalid_argument("Название специальности не может быть пустым");
	}
}

const std::string& Specialty::getCode() const noexcept {return code;}

const std::string& Specialty::getName() const noexcept {return name;}

const Department& Specialty::getDepartment() const noexcept {return department;}

void Specialty::addGroup(const Group& group) {
	for (const Group& added : groups) {
		if (added.getNumber() == group.getNumber()) {
			return;
		}
	}
	groups.push_back(group);
}

const std::vector<Group>& Specialty::getGroups() const noexcept {return groups;}

std::size_t Specialty::getGroupCount() const noexcept {return groups.size();}

std::string Specialty::getShortInfo() const {
	return code + " - " + name;
}

std::string Specialty::getInfo() const {
	return "Специальность " + getShortInfo()
		+ " | Кафедра: " + department.getName()
		+ " | Групп: " + std::to_string(groups.size());
}

bool Specialty::operator==(const Specialty& other) const noexcept {
	return code == other.code && name == other.name;
}

bool Specialty::operator!=(const Specialty& other) const noexcept {return !(*this == other);}
