#include "Person.h"

#include <stdexcept>

#include "FormatUtils.h"

int Person::nextId = 1;

Person::Person(const std::string& fullName, const Date& birthDate, const int id)
	: id(id > 0 ? id : nextId++), fullName(FormatUtils::trim(fullName)), birthDate(birthDate) {
	if (this->fullName.empty()) {
		throw std::invalid_argument("ФИО человека не может быть пустым");
	}
	if (this->id >= nextId) {
		nextId = this->id + 1;
	}
}

int Person::getId() const noexcept {return id;}

const std::string& Person::getFullName() const noexcept {return fullName;}

const Date& Person::getBirthDate() const noexcept {return birthDate;}

void Person::setFullName(const std::string& newFullName) {
	const std::string trimmedName = FormatUtils::trim(newFullName);
	if (trimmedName.empty()) {
		throw std::invalid_argument("ФИО человека не может быть пустым");
	}
	this->fullName = trimmedName;
}

std::string Person::getShortInfo() const {
	return FormatUtils::toInitials(fullName) + " (" + birthDate.toString() + ")";
}

std::string Person::getInfo() const {
	return "ID: " + std::to_string(id) +
		" | ФИО: " + fullName +
		" | Дата рождения: " + birthDate.toString() +
		" | Роль: " + getRole() +
		" | " + getSpecificInfo();
}

int Person::getAge(const Date& referenceDate) const noexcept {
	int years = referenceDate.getYear() - birthDate.getYear();
	const bool birthdayIsYetToCome =
		referenceDate.getMonth() < birthDate.getMonth() ||
		(referenceDate.getMonth() == birthDate.getMonth() && referenceDate.getDay() < birthDate.getDay());
	if (birthdayIsYetToCome) {
		--years;
	}
	return years < 0 ? 0 : years;
}

bool Person::isStudent() const {return false;}

bool Person::isTeacher() const {return false;}

bool Person::operator==(const Person& other) const noexcept {return id == other.id;}

bool Person::operator!=(const Person& other) const noexcept {return !(*this == other);}
