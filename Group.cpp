//
// Group.cpp
// Реализация класса Group.
//
#include "Group.h"
#include "Student.h"
#include "Specialty.h"
#include "FormatUtils.h"

#include <stdexcept>

Group::Group(const std::string& number, Specialty* specialty)
    : number_(number)
    , specialty_(specialty)
{
    if (FormatUtils::Trim(number_).empty()) {
        throw std::invalid_argument("Group: номер группы не может быть пустым");
    }
    if (specialty == nullptr) {
        throw std::invalid_argument("Group: группа должна относиться к специальности");
    }
    number_ = FormatUtils::Trim(number_);

    // Регистрируем себя у специальности сразу при создании. Так сохраняется
    // инвариант "каждая группа входит в список своей специальности"
    // независимо от того, кто создал группу: сам конструктор Group
    // или агрегатор University.
    specialty_->AddGroup(this);
}

const std::string& Group::GetNumber() const {
    return number_;
}

const Specialty* Group::GetSpecialty() const {
    return specialty_;
}

std::shared_ptr<Student> Group::AddStudent(const std::string& fullName, Date birthDate,
    const std::string& recordBook, unsigned course)
{
    if (FormatUtils::Trim(fullName).empty()) {
        throw std::invalid_argument("Group: ФИО студента не может быть пустым");
    }
    if (FindStudentByRecordBook(recordBook) != nullptr) {
        throw std::invalid_argument("Group: в группе уже есть студент с зачётной книжкой " + recordBook);
    }

    auto student = std::make_shared<Student>(fullName, birthDate, recordBook, course, this);
    students_.push_back(student);
    return student;
}

const std::vector<std::shared_ptr<Student>>& Group::GetStudents() const {
    return students_;
}

size_t Group::GetStudentCount() const {
    return students_.size();
}

std::shared_ptr<Student> Group::FindStudentByRecordBook(const std::string& recordBook) const {
    const std::string target = FormatUtils::Trim(recordBook);
    for (const std::shared_ptr<Student>& student : students_) {
        if (FormatUtils::EqualsIgnoreCase(student->GetRecordBook(), target)) {
            return student;
        }
    }
    return nullptr;
}

std::shared_ptr<Student> Group::FindStudentByFullName(const std::string& fullName) const {
    const std::string target = FormatUtils::Trim(fullName);
    if (target.empty()) {
        return nullptr;
    }
    for (const std::shared_ptr<Student>& student : students_) {
        if (FormatUtils::ContainsIgnoreCase(student->GetFullName(), target)) {
            return student;
        }
    }
    return nullptr;
}

double Group::GetAverageMark() const {
    if (students_.empty()) {
        return 0.0;
    }
    double sum = 0.0;
    for (const std::shared_ptr<Student>& student : students_) {
        sum += student->GetAverageMark();
    }
    return sum / static_cast<double>(students_.size());
}

std::string Group::GetShortInfo() const {
    const std::string code = (specialty_ != nullptr) ? specialty_->GetCode() : "---";
    return number_ + " (" + code + ")";
}

std::string Group::GetInfo() const {
    const std::string specialtyName = (specialty_ != nullptr) ? specialty_->GetName() : "не определена";
    return "Группа: " + number_
        + " | специальность: " + specialtyName
        + " | студентов: " + std::to_string(students_.size());
}
