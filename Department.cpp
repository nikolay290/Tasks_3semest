//
// Department.cpp
// Реализация класса Department.
//
#include "Department.h"
#include "Discipline.h"
#include "Teacher.h"
#include "FormatUtils.h"

#include <algorithm>
#include <stdexcept>

Department::Department(const std::string& name)
    : name_(name)
{
    if (FormatUtils::Trim(name_).empty()) {
        throw std::invalid_argument("Department: название кафедры не может быть пустым");
    }
    name_ = FormatUtils::Trim(name_);
}

const std::string& Department::GetName() const {
    return name_;
}

std::shared_ptr<Discipline> Department::AddDiscipline(const std::string& disciplineName) {
    const std::string trimmed = FormatUtils::Trim(disciplineName);
    if (trimmed.empty()) {
        throw std::invalid_argument("Department: название дисциплины не может быть пустым");
    }
    if (FindDisciplineByName(trimmed) != nullptr) {
        throw std::invalid_argument("Department: дисциплина \"" + trimmed + "\" уже читается кафедрой");
    }

    auto discipline = std::make_shared<Discipline>(trimmed, this);
    disciplines_.push_back(discipline);
    return discipline;
}

const std::vector<std::shared_ptr<Discipline>>& Department::GetDisciplines() const {
    return disciplines_;
}

size_t Department::GetDisciplineCount() const {
    return disciplines_.size();
}

bool Department::AddTeacher(Teacher* teacher) {
    if (teacher == nullptr) {
        throw std::invalid_argument("Department: преподаватель не может быть nullptr");
    }
    if (std::find(teachers_.begin(), teachers_.end(), teacher) != teachers_.end()) {
        return false;
    }
    teachers_.push_back(teacher);
    return true;
}

const std::vector<Teacher*>& Department::GetTeachers() const {
    return teachers_;
}

size_t Department::GetTeacherCount() const {
    return teachers_.size();
}

std::shared_ptr<Discipline> Department::FindDisciplineByName(const std::string& disciplineName) const {
    const std::string target = FormatUtils::Trim(disciplineName);
    for (const std::shared_ptr<Discipline>& discipline : disciplines_) {
        if (FormatUtils::EqualsIgnoreCase(discipline->GetName(), target)) {
            return discipline;
        }
    }
    return nullptr;
}

std::string Department::GetShortInfo() const {
    return name_;
}

std::string Department::GetInfo() const {
    std::vector<std::string> names;
    names.reserve(disciplines_.size());
    for (const std::shared_ptr<Discipline>& discipline : disciplines_) {
        names.push_back(discipline->GetName());
    }

    return "Кафедра: " + name_
        + " | дисциплин: " + std::to_string(disciplines_.size())
        + " (" + FormatUtils::Join(names, ", ") + ")"
        + " | преподавателей: " + std::to_string(teachers_.size());
}
