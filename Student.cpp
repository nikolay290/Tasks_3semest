//
// Student.cpp
// Реализация типа-наследника Student.
//
#include "Student.h"
#include "Group.h"
#include "Specialty.h"
#include "FormatUtils.h"

#include <stdexcept>

Student::Student(const std::string& fullName, Date birthDate, const std::string& recordBook,
    unsigned course, const Group* group)
    : Person(fullName, birthDate)      // инициализация базового класса
    , recordBook_(recordBook)
    , course_(course)
    , group_(group)
{
    if (FormatUtils::Trim(recordBook_).empty()) {
        throw std::invalid_argument("Student: номер зачётной книжки не может быть пустым");
    }
    if (course == 0) {
        throw std::invalid_argument("Student: номер курса должен быть больше нуля");
    }
    if (group == nullptr) {
        throw std::invalid_argument("Student: студент должен состоять в группе");
    }
    recordBook_ = FormatUtils::Trim(recordBook_);
}

const std::string& Student::GetRecordBook() const {
    return recordBook_;
}

unsigned Student::GetCourse() const {
    return course_;
}

const Group* Student::GetGroup() const {
    return group_;
}

void Student::AddMark(const double mark) {
    if (mark < 1.0 || mark > 5.0) {
        throw std::invalid_argument("Student: оценка должна быть в диапазоне от 1 до 5");
    }
    marks_.push_back(mark);
}

double Student::GetAverageMark() const {
    if (marks_.empty()) {
        return 0.0;
    }
    double sum = 0.0;
    for (double mark : marks_) {
        sum += mark;
    }
    return sum / static_cast<double>(marks_.size());
}

bool Student::IsStudent() const {
    return true;
}

std::string Student::GetRole() const {
    return "Студент";
}

std::string Student::GetInfo() const {
    const Specialty* specialty = group_->GetSpecialty();
    const std::string specialtyName = (specialty != nullptr) ? specialty->GetName() : "не определена";

    return "Студент: " + GetFullName()
        + " | зачётная книжка: " + recordBook_
        + " | курс: " + std::to_string(course_)
        + " | группа: " + group_->GetNumber()
        + " | специальность: " + specialtyName
        + " | средний балл: " + FormatUtils::FormatDecimal(GetAverageMark());
}

std::string Student::GetShortInfo() const {
    return FormatUtils::ToInitials(GetFullName()) + " (гр. " + group_->GetNumber() + ")";
}

std::string Student::GetSpecificInfo() const {
    return "зачётная книжка " + recordBook_ + ", курс " + std::to_string(course_);
}
