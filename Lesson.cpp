//
// Lesson.cpp
// Реализация класса Lesson.
//
#include "Lesson.h"
#include "Discipline.h"
#include "Group.h"
#include "Teacher.h"
#include "FormatUtils.h"

#include <stdexcept>

const char* ToString(LessonType type) {
    switch (type) {
    case LessonType::Lecture:   return "Лекция";
    case LessonType::Practical: return "Практика";
    case LessonType::Lab:       return "Лабораторная работа";
    }
    return "Неизвестное занятие";
}

void Lesson::Validate(LessonType type, const Discipline* discipline, const Group* group,
    const Teacher* teacher, const Teacher* secondTeacher) {
    if (discipline == nullptr) {
        throw std::invalid_argument("Lesson: у занятия должна быть дисциплина");
    }
    if (group == nullptr) {
        throw std::invalid_argument("Lesson: у занятия должна быть группа");
    }
    if (teacher == nullptr) {
        throw std::invalid_argument("Lesson: у занятия должен быть преподаватель");
    }
    if (secondTeacher != nullptr && secondTeacher == teacher) {
        throw std::invalid_argument("Lesson: второй преподаватель должен отличаться от основного");
    }

    // Предметная особенность ВУЗа: лабораторные работы ведут два преподавателя.
    if (type == LessonType::Lab && secondTeacher == nullptr) {
        throw std::invalid_argument("Lesson: лабораторную работу ведут два преподавателя");
    }
    if (type != LessonType::Lab && secondTeacher != nullptr) {
        throw std::invalid_argument("Lesson: второй преподаватель положен только на лабораторной работе");
    }
}

Lesson::Lesson(LessonType type, const Discipline* discipline, const Group* group,
    const Teacher* teacher, const Teacher* secondTeacher, const std::string& room, Date date)
    : type_(type)
    , discipline_(discipline)
    , group_(group)
    , teacher_(teacher)
    , secondTeacher_(secondTeacher)
    , room_(room)
    , date_(date)
{
    Validate(type, discipline, group, teacher, secondTeacher);
    room_ = FormatUtils::Trim(room_);
}

LessonType Lesson::GetType() const {
    return type_;
}

const Discipline* Lesson::GetDiscipline() const {
    return discipline_;
}

const Group* Lesson::GetGroup() const {
    return group_;
}

const Teacher* Lesson::GetTeacher() const {
    return teacher_;
}

const Teacher* Lesson::GetSecondTeacher() const {
    return secondTeacher_;
}

std::vector<const Teacher*> Lesson::GetTeachers() const {
    std::vector<const Teacher*> result;
    result.push_back(teacher_);
    if (secondTeacher_ != nullptr) {
        result.push_back(secondTeacher_);
    }
    return result;
}

size_t Lesson::GetTeacherCount() const {
    return (secondTeacher_ == nullptr) ? 1u : 2u;
}

const std::string& Lesson::GetRoom() const {
    return room_;
}

const Date& Lesson::GetDate() const {
    return date_;
}

bool Lesson::IsTaughtBy(const Teacher* teacher) const {
    if (teacher == nullptr) {
        return false;
    }
    return teacher == teacher_ || teacher == secondTeacher_;
}

std::string Lesson::GetShortInfo() const {
    return std::string(ToString(type_)) + ": " + discipline_->GetName();
}

std::string Lesson::GetInfo() const {
    std::vector<std::string> teacherNames;
    for (const Teacher* teacher : GetTeachers()) {
        teacherNames.push_back(FormatUtils::ToInitials(teacher->GetFullName())
            + " (" + teacher->GetPosition() + ")");
    }

    return "Занятие: " + GetShortInfo()
        + " | группа: " + group_->GetNumber()
        + " | преподаватели: " + FormatUtils::Join(teacherNames, ", ")
        + " | аудитория: " + (room_.empty() ? "не указана" : room_)
        + " | дата: " + date_.ToString();
}
