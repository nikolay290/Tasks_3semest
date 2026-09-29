//
// Teacher.cpp
// Реализация типа-наследника Teacher.
//
#include "Teacher.h"
#include "Department.h"
#include "FormatUtils.h"

#include <algorithm>
#include <stdexcept>

Teacher::Teacher(const std::string& fullName, Date birthDate, const std::string& position,
    const std::string& degree, const Department* department)
    : Person(fullName, birthDate)
    , position_(position)
    , degree_(degree)
    , department_(department)
{
    if (FormatUtils::Trim(position_).empty()) {
        throw std::invalid_argument("Teacher: должность не может быть пустой");
    }
    if (department == nullptr) {
        throw std::invalid_argument("Teacher: преподаватель должен работать на кафедре");
    }
    position_ = FormatUtils::Trim(position_);
    degree_ = FormatUtils::Trim(degree_);
}

const std::string& Teacher::GetPosition() const {
    return position_;
}

const std::string& Teacher::GetDegree() const {
    return degree_;
}

const Department* Teacher::GetDepartment() const {
    return department_;
}

bool Teacher::AddGroup(const Group* group) {
    if (group == nullptr) {
        throw std::invalid_argument("Teacher: группа не может быть nullptr");
    }
    if (std::find(groups_.begin(), groups_.end(), group) != groups_.end()) {
        return false; // уже зарегистрирована
    }
    groups_.push_back(group);
    return true;
}

const std::vector<const Group*>& Teacher::GetGroups() const {
    return groups_;
}

bool Teacher::TeachesInGroup(const Group* group) const {
    if (group == nullptr) {
        return false;
    }
    return std::find(groups_.begin(), groups_.end(), group) != groups_.end();
}

bool Teacher::IsTeacher() const {
    return true;
}

std::string Teacher::GetRole() const {
    return "Преподаватель";
}

std::string Teacher::GetInfo() const {
    const std::string departmentName = (department_ != nullptr) ? department_->GetName() : "не определена";

    return "Преподаватель: " + GetFullName()
        + " | должность: " + position_
        + " | степень: " + (degree_.empty() ? "нет" : degree_)
        + " | кафедра: " + departmentName
        + " | ведёт занятия в " + std::to_string(groups_.size()) + " гр.";
}

std::string Teacher::GetShortInfo() const {
    return FormatUtils::ToInitials(GetFullName()) + " (" + position_ + ")";
}

std::string Teacher::GetSpecificInfo() const {
    return position_ + (degree_.empty() ? "" : ", " + degree_);
}
