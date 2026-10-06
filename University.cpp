//
// University.cpp
// Реализация класса University.
//
#include "University.h"
#include "Department.h"
#include "Discipline.h"
#include "Group.h"
#include "Lesson.h"
#include "Person.h"
#include "Specialty.h"
#include "Student.h"
#include "Teacher.h"
#include "FormatUtils.h"

#include <algorithm>
#include <stdexcept>

University::University()
    : name_("ВУЗ")
{
}

University::~University() = default;

// ---- Создание объектов ----

std::shared_ptr<Specialty> University::AddSpecialty(const std::string& code, const std::string& name) {
    if (FindSpecialtyByCode(code) != nullptr) {
        throw std::invalid_argument("University: специальность с кодом " + code + " уже существует");
    }
    auto specialty = std::make_shared<Specialty>(code, name);
    specialties_.push_back(specialty);
    return specialty;
}

std::shared_ptr<Department> University::AddDepartment(const std::string& name) {
    if (FindDepartmentByName(name) != nullptr) {
        throw std::invalid_argument("University: кафедра \"" + name + "\" уже существует");
    }
    auto department = std::make_shared<Department>(name);
    departments_.push_back(department);
    return department;
}

std::shared_ptr<Group> University::AddGroup(const std::string& number, Specialty* specialty) {
    if (specialty == nullptr) {
        throw std::invalid_argument("University: группа должна относиться к специальности");
    }
    if (FindGroupByNumber(number) != nullptr) {
        throw std::invalid_argument("University: группа с номером " + number + " уже существует");
    }

    auto group = std::make_shared<Group>(number, specialty);
    groups_.push_back(group);
    // Регистрация группы в списке её специальности выполняется
    // конструктором Group - здесь дублировать её не нужно.
    return group;
}

std::shared_ptr<Teacher> University::AddTeacher(const std::string& fullName, Date birthDate,
    const std::string& position, const std::string& degree, Department* department) {
    if (department == nullptr) {
        throw std::invalid_argument("University: преподаватель должен работать на кафедре");
    }

    auto teacher = std::make_shared<Teacher>(fullName, birthDate, position, degree, department);
    teachers_.push_back(teacher);
    department->AddTeacher(teacher.get());
    return teacher;
}

std::shared_ptr<Lesson> University::AddLesson(LessonType type, const Discipline* discipline,
    const Group* group, Teacher* teacher, Teacher* secondTeacher,
    const std::string& room, Date date) {
    auto lesson = std::make_shared<Lesson>(type, discipline, group, teacher, secondTeacher, room, date);
    lessons_.push_back(lesson);

    // Регистрируем у преподавателей, в каких группах они ведут занятия -
    // это нужно третьему запросу ("список преподавателей, проводящих
    // занятия в данной группе"). Teacher хранит не владеющие указатели
    // на группы, которые живут дольше преподавателей.
    if (teacher != nullptr) {
        teacher->AddGroup(group);
    }
    if (secondTeacher != nullptr) {
        secondTeacher->AddGroup(group);
    }
    return lesson;
}

// ---- Общий доступ ----

const std::string& University::GetName() const {
    return name_;
}

void University::SetName(const std::string& name) {
    if (FormatUtils::Trim(name).empty()) {
        throw std::invalid_argument("University: название вуза не может быть пустым");
    }
    name_ = FormatUtils::Trim(name);
}

const std::vector<std::shared_ptr<Specialty>>& University::GetSpecialties() const {
    return specialties_;
}

const std::vector<std::shared_ptr<Department>>& University::GetDepartments() const {
    return departments_;
}

const std::vector<std::shared_ptr<Group>>& University::GetGroups() const {
    return groups_;
}

const std::vector<std::shared_ptr<Teacher>>& University::GetTeachers() const {
    return teachers_;
}

const std::vector<std::shared_ptr<Lesson>>& University::GetLessons() const {
    return lessons_;
}

// ---- Запрос 1 ----

std::shared_ptr<Student> University::FindStudentByRecordBook(const std::string& recordBook) const {
    for (const std::shared_ptr<Group>& group : groups_) {
        std::shared_ptr<Student> student = group->FindStudentByRecordBook(recordBook);
        if (student != nullptr) {
            return student;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Student>> University::FindStudentsByFullName(const std::string& fullName) const {
    std::vector<std::shared_ptr<Student>> result;
    const std::string target = FormatUtils::Trim(fullName);
    if (target.empty()) {
        return result;
    }

    for (const std::shared_ptr<Group>& group : groups_) {
        for (const std::shared_ptr<Student>& student : group->GetStudents()) {
            if (FormatUtils::ContainsIgnoreCase(student->GetFullName(), target)) {
                result.push_back(student);
            }
        }
    }
    return result;
}

// ---- Запрос 2 ----

std::vector<std::shared_ptr<Discipline>> University::GetDisciplinesByDepartment(const Department* department) const {
    std::vector<std::shared_ptr<Discipline>> result;
    if (department == nullptr) {
        return result;
    }

    for (const std::shared_ptr<Department>& candidate : departments_) {
        if (candidate.get() == department) {
            return candidate->GetDisciplines();
        }
    }
    return result;
}

// ---- Запрос 3 ----

std::vector<std::shared_ptr<Teacher>> University::GetTeachersByGroup(const Group* group) const {
    std::vector<std::shared_ptr<Teacher>> result;
    if (group == nullptr) {
        return result;
    }

    // Проходим по занятиям группы: так учитываются и вторые преподаватели
    // лабораторных работ, а повторы исключаются сравнением указателей.
    for (const std::shared_ptr<Lesson>& lesson : lessons_) {
        if (lesson->GetGroup() != group) {
            continue;
        }
        for (const Teacher* teacher : lesson->GetTeachers()) {
            const bool alreadyPresent = std::any_of(
                result.begin(), result.end(),
                [teacher](const std::shared_ptr<Teacher>& item) { return item.get() == teacher; });
            if (!alreadyPresent) {
                for (const std::shared_ptr<Teacher>& owned : teachers_) {
                    if (owned.get() == teacher) {
                        result.push_back(owned);
                        break;
                    }
                }
            }
        }
    }
    return result;
}

// ---- Запрос 4 ----

std::vector<std::shared_ptr<Group>> University::GetGroupsBySpecialty(const Specialty* specialty) const {
    std::vector<std::shared_ptr<Group>> result;
    if (specialty == nullptr) {
        return result;
    }

    for (const std::shared_ptr<Group>& group : groups_) {
        if (group->GetSpecialty() == specialty) {
            result.push_back(group);
        }
    }
    return result;
}

// ---- Дополнительные запросы ----

std::shared_ptr<Department> University::FindDepartmentByName(const std::string& name) const {
    const std::string target = FormatUtils::Trim(name);
    for (const std::shared_ptr<Department>& department : departments_) {
        if (FormatUtils::EqualsIgnoreCase(department->GetName(), target)) {
            return department;
        }
    }
    return nullptr;
}

std::shared_ptr<Specialty> University::FindSpecialtyByCode(const std::string& code) const {
    const std::string target = FormatUtils::Trim(code);
    for (const std::shared_ptr<Specialty>& specialty : specialties_) {
        if (FormatUtils::EqualsIgnoreCase(specialty->GetCode(), target)) {
            return specialty;
        }
    }
    return nullptr;
}

std::shared_ptr<Group> University::FindGroupByNumber(const std::string& number) const {
    const std::string target = FormatUtils::Trim(number);
    for (const std::shared_ptr<Group>& group : groups_) {
        if (FormatUtils::EqualsIgnoreCase(group->GetNumber(), target)) {
            return group;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Lesson>> University::GetLessonsByGroup(const Group* group) const {
    std::vector<std::shared_ptr<Lesson>> result;
    if (group == nullptr) {
        return result;
    }
    for (const std::shared_ptr<Lesson>& lesson : lessons_) {
        if (lesson->GetGroup() == group) {
            result.push_back(lesson);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Person>> University::GetAllPeople() const {
    // Именно этот метод формирует полиморфную коллекцию БАЗОВОГО типа
    // Person, которую затем обходит main.cpp как коллекцию базового класса.
    std::vector<std::shared_ptr<Person>> people;

    for (const std::shared_ptr<Group>& group : groups_) {
        for (const std::shared_ptr<Student>& student : group->GetStudents()) {
            // shared_ptr<Student> преобразуется в shared_ptr<Person>
            // (Student наследуется публично от Person).
            people.push_back(student);
        }
    }
    for (const std::shared_ptr<Teacher>& teacher : teachers_) {
        people.push_back(teacher);
    }
    return people;
}

std::vector<std::shared_ptr<Student>> University::GetAllStudents() const {
    std::vector<std::shared_ptr<Student>> result;
    for (const std::shared_ptr<Group>& group : groups_) {
        for (const std::shared_ptr<Student>& student : group->GetStudents()) {
            result.push_back(student);
        }
    }
    return result;
}

std::string University::GetStatistics() const {
    size_t studentCount = 0;
    for (const std::shared_ptr<Group>& group : groups_) {
        studentCount += group->GetStudentCount();
    }

    return "ВУЗ: " + name_
        + " | специальностей: " + std::to_string(specialties_.size())
        + " | кафедр: " + std::to_string(departments_.size())
        + " | групп: " + std::to_string(groups_.size())
        + " | студентов: " + std::to_string(studentCount)
        + " | преподавателей: " + std::to_string(teachers_.size())
        + " | занятий: " + std::to_string(lessons_.size());
}
