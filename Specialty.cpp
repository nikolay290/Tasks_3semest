//
// Specialty.cpp
// Реализация класса Specialty.
//
#include "Specialty.h"
#include "FormatUtils.h"

#include <algorithm>
#include <stdexcept>

Specialty::Specialty(const std::string& code, const std::string& name)
    : code_(code)
    , name_(name)
{
    if (FormatUtils::Trim(code_).empty()) {
        throw std::invalid_argument("Specialty: код специальности не может быть пустым");
    }
    if (FormatUtils::Trim(name_).empty()) {
        throw std::invalid_argument("Specialty: название специальности не может быть пустым");
    }
    code_ = FormatUtils::Trim(code_);
    name_ = FormatUtils::Trim(name_);
}

const std::string& Specialty::GetCode() const {
    return code_;
}

const std::string& Specialty::GetName() const {
    return name_;
}

bool Specialty::AddGroup(Group* group) {
    if (group == nullptr) {
        throw std::invalid_argument("Specialty: группа не может быть nullptr");
    }
    if (std::find(groups_.begin(), groups_.end(), group) != groups_.end()) {
        return false;
    }
    groups_.push_back(group);
    return true;
}

const std::vector<Group*>& Specialty::GetGroups() const {
    return groups_;
}

size_t Specialty::GetGroupCount() const {
    return groups_.size();
}

std::string Specialty::GetShortInfo() const {
    return code_ + " - " + name_;
}

std::string Specialty::GetInfo() const {
    return "Специальность: " + code_ + " " + name_
        + " | групп: " + std::to_string(groups_.size());
}
