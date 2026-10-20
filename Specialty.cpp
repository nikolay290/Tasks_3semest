#include "Specialty.h"

#include <stdexcept>

#include "FormatUtils.h"

Specialty::Specialty(const std::string& code, const std::string& name)
	: code(FormatUtils::trim(code)), name(FormatUtils::trim(name)) {
	if (this->code.empty()) {
		throw std::invalid_argument("Код специальности не может быть пустым");
	}
	if (this->name.empty()) {
		throw std::invalid_argument("Название специальности не может быть пустым");
	}
}

const std::string& Specialty::getCode() const noexcept {return code;}

const std::string& Specialty::getName() const noexcept {return name;}

std::string Specialty::getShortInfo() const {
	return code + " - " + name;
}

std::string Specialty::getInfo() const {
	return "Специальность " + getShortInfo();
}

bool Specialty::operator==(const Specialty& other) const noexcept {
	return code == other.code && name == other.name;
}

bool Specialty::operator!=(const Specialty& other) const noexcept {return !(*this == other);}
