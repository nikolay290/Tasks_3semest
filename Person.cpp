//
// Person.cpp
// Реализация абстрактного базового класса Person.
//
#include "Person.h"
#include "FormatUtils.h"

#include <stdexcept>

Person::Person(std::string fullName, Date birthDate)
    : fullName_(std::move(fullName))
    , birthDate_(birthDate)
{
    if (FormatUtils::Trim(fullName_).empty()) {
        throw std::invalid_argument("Person: ФИО не может быть пустым");
    }
    fullName_ = FormatUtils::Trim(fullName_);
}

const std::string& Person::GetFullName() const {
    return fullName_;
}

const Date& Person::GetBirthDate() const {
    return birthDate_;
}

void Person::SetFullName(const std::string& fullName) {
    if (FormatUtils::Trim(fullName).empty()) {
        throw std::invalid_argument("Person: ФИО не может быть пустым");
    }
    fullName_ = FormatUtils::Trim(fullName);
}

int Person::GetAge(const Date& asOf) const {
    if (asOf < birthDate_) {
        return 0; // дата рождения ещё не наступила
    }

    int age = asOf.GetYear() - birthDate_.GetYear();
    // Возраст не подрос, если день рождения в текущем году ещё не наступил.
    const bool birthdayNotYet = (asOf.GetMonth() < birthDate_.GetMonth())
        || (asOf.GetMonth() == birthDate_.GetMonth() && asOf.GetDay() < birthDate_.GetDay());
    if (birthdayNotYet) {
        --age;
    }
    return age;
}

std::string Person::GetInfo() const {
    return GetRole() + ": " + GetShortInfo() + " (" + GetSpecificInfo() + ")";
}

std::string Person::GetShortInfo() const {
    return FormatUtils::ToInitials(fullName_);
}

bool Person::IsStudent() const {
    return false;
}

bool Person::IsTeacher() const {
    return false;
}

bool Person::operator==(const Person& other) const {
    return fullName_ == other.fullName_ && birthDate_ == other.birthDate_;
}

bool Person::operator!=(const Person& other) const {
    return !(*this == other);
}
