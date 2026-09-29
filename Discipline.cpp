//
// Discipline.cpp
// Реализация класса Discipline.
//
#include "Discipline.h"
#include "Department.h"
#include "FormatUtils.h"

#include <stdexcept>

Discipline::Discipline(const std::string& name, Department* department)
    : name_(name)
    , department_(department)
{
    if (FormatUtils::Trim(name_).empty()) {
        throw std::invalid_argument("Discipline: название дисциплины не может быть пустым");
    }
    if (department == nullptr) {
        throw std::invalid_argument("Discipline: дисциплина должна читаться кафедрой");
    }
    name_ = FormatUtils::Trim(name_);
}

const std::string& Discipline::GetName() const {
    return name_;
}

const Department* Discipline::GetDepartment() const {
    return department_;
}

bool Discipline::IsReadByOwnDepartment() const {
    if (department_ == nullptr) {
        return false;
    }
    return department_->FindDisciplineByName(name_).get() == this;
}

std::string Discipline::GetShortInfo() const {
    return name_;
}

std::string Discipline::GetInfo() const {
    const std::string departmentName = (department_ != nullptr) ? department_->GetName() : "не определена";
    return "Дисциплина: \"" + name_ + "\", кафедра: " + departmentName;
}
