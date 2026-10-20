//
// VuzTests.cpp
//   - TEST_CLASS(DateTests)             - класс Date;
//   - TEST_CLASS(PersonTests)           - базовый класс Person и
//                                         полиморфный обход коллекции;
//   - TEST_CLASS(StudentTests)          - класс Student;
//   - TEST_CLASS(TeacherTests)          - класс Teacher;
//   - TEST_CLASS(GroupSpecialtyTests)   - классы Group и Specialty;
//   - TEST_CLASS(DepartmentTests)       - классы Department и Discipline;
//   - TEST_CLASS(LessonTests)           - класс Lesson;
//   - TEST_CLASS(UniversityTests)       - класс University и все четыре
//                                         запроса варианта "ВУЗ";
//   - TEST_CLASS(FormatUtilsTests)      - вспомогательные функции.
//
// Библиотека не использует умные указатели: объектами владеют контейнеры
// std::vector внутри University и Group, а методы возвращают ссылки на
// готовые объекты либо не владеющие указатели на них.
//
#include "CppUnitTest.h"
#include "VuzLib.h"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace VuzTests
{
    // ==== Тесты класса Date ====

    TEST_CLASS(DateTests)
    {
    public:
        /** @brief Конструктор корректно сохраняет компоненты даты. */
        TEST_METHOD(Constructor_ValidDate_KeepsComponents)
        {
            const Date date(2004, 3, 15);

            Assert::AreEqual(2004, date.getYear());
            Assert::AreEqual(3, date.getMonth());
            Assert::AreEqual(15, date.getDay());
        }

        /** @brief Разбор строки корректного формата даёт верную дату. */
        TEST_METHOD(Parse_ValidIsoString_ReturnsCorrectComponents)
        {
            const Date date = Date::parse("2004-03-15");

            Assert::AreEqual(2004, date.getYear());
            Assert::AreEqual(3, date.getMonth());
            Assert::AreEqual(15, date.getDay());
        }

        /** @brief Строка неверного формата отклоняется. */
        TEST_METHOD(Parse_WrongFormat_Throws)
        {
            Assert::ExpectException<std::invalid_argument>([] { Date::parse("15.03.2004"); });
            Assert::ExpectException<std::invalid_argument>([] { Date::parse("2004-3-15"); });
            Assert::ExpectException<std::invalid_argument>([] { Date::parse(""); });
        }

        /** @brief Недопустимый месяц отклоняется. */
        TEST_METHOD(Constructor_InvalidMonth_Throws)
        {
            Assert::ExpectException<std::invalid_argument>([] { Date(2026, 13, 1); });
            Assert::ExpectException<std::invalid_argument>([] { Date(2026, 0, 1); });
        }

        /** @brief Недопустимый для месяца день отклоняется. */
        TEST_METHOD(Constructor_InvalidDayForMonth_Throws)
        {
            Assert::ExpectException<std::invalid_argument>([] { Date(2026, 4, 31); });
            Assert::ExpectException<std::invalid_argument>([] { Date(2026, 2, 30); });
        }

        /** @brief 29 февраля допустимо только в високосный год. */
        TEST_METHOD(LeapYear_February29_IsValid)
        {
            const Date leapDay(2024, 2, 29);

            Assert::AreEqual(29, leapDay.getDay());
            Assert::ExpectException<std::invalid_argument>([] { Date(2026, 2, 29); });
            Assert::ExpectException<std::invalid_argument>([] { Date(1900, 2, 29); });
            const Date oldLeapDay(2000, 2, 29);
            Assert::AreEqual(29, oldLeapDay.getDay());
        }

        /** @brief Разница даты с самой собой равна нулю. */
        TEST_METHOD(DaysUntil_SameDate_ReturnsZero)
        {
            const Date date(2026, 5, 10);

            Assert::AreEqual(0, date.daysUntil(date));
        }

        /** @brief Разница через неделю равна семи дням. */
        TEST_METHOD(DaysUntil_OneWeekLater_ReturnsSeven)
        {
            const Date date(2026, 1, 1);

            Assert::AreEqual(7, date.daysUntil(Date(2026, 1, 8)));
        }

        /** @brief Разница через границу года считается верно. */
        TEST_METHOD(DaysUntil_AcrossYearBoundary_IsCorrect)
        {
            const Date date(2025, 12, 25);

            Assert::AreEqual(11, date.daysUntil(Date(2026, 1, 5)));
        }

        /** @brief Операторы сравнения работают по календарному порядку. */
        TEST_METHOD(ComparisonOperators_WorkAsExpected)
        {
            const Date earlier(2025, 1, 1);
            const Date later(2026, 1, 1);
            const Date same(2025, 1, 1);

            Assert::IsTrue(earlier < later);
            Assert::IsTrue(earlier <= later);
            Assert::IsTrue(earlier <= same);
            Assert::IsTrue(later > earlier);
            Assert::IsTrue(earlier == same);
            Assert::IsTrue(earlier != later);
        }

        /** @brief Дата форматируется с ведущими нулями. */
        TEST_METHOD(ToString_FormatsWithLeadingZeros)
        {
            const Date date(2004, 3, 5);

            Assert::AreEqual(std::string("2004-03-05"), date.toString());
        }
    };

    // ==== Тесты класса Person ====

    TEST_CLASS(PersonTests)
    {
    public:
        /** @brief Пустое ФИО отклоняется конструктором. */
        TEST_METHOD(Constructor_EmptyFullName_Throws)
        {
            Assert::ExpectException<std::invalid_argument>(
                [] { Student("", Date(2005, 1, 1), "17-142", 2); });
            Assert::ExpectException<std::invalid_argument>(
                [] { Student("   ", Date(2005, 1, 1), "17-142", 2); });
        }

        /** @brief Краткое представление сокращает имя и отчество. */
        TEST_METHOD(GetShortInfo_ReturnsInitials)
        {
            const Student student("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2);

            Assert::AreEqual(std::string("Смирнов А. Н. (2005-04-12)"), student.getShortInfo());
        }

        /** @brief Возраст считается в полных годах. */
        TEST_METHOD(GetAge_CalculatesFullYears)
        {
            const Student student("Смирнов Алексей Николаевич", Date(2000, 1, 1), "17-142", 2);

            Assert::AreEqual(26, student.getAge(Date(2026, 1, 1)));
            Assert::AreEqual(25, student.getAge(Date(2025, 12, 31)));
        }

        /** @brief Каждый человек получает собственный id. */
        TEST_METHOD(EachPerson_GetsUniqueId)
        {
            const Student first("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2);
            const Student second("Волков Дмитрий Андреевич", Date(2005, 9, 30), "17-143", 2);

            Assert::AreNotEqual(first.getId(), second.getId());
        }

        /** @brief Явно переданный id сохраняется, а счётчик продвигается. */
        TEST_METHOD(Constructor_ExplicitId_IsKept)
        {
            const Student student("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2, 500);
            const Student follower("Волков Дмитрий Андреевич", Date(2005, 9, 30), "17-143", 2);

            Assert::AreEqual(500, student.getId());
            Assert::IsTrue(follower.getId() > 500);
        }

        /** @brief Пустое ФИО нельзя задать и setter'ом. */
        TEST_METHOD(SetFullName_WithSpacesOnly_Throws)
        {
            Student student("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2);

            Assert::ExpectException<std::invalid_argument>([&] { student.setFullName("  "); });
        }

        /** @brief Коллекция базового типа заполняется объектами-наследниками. */
        TEST_METHOD(PolymorphicCollection_ReturnsDerivedRoles)
        {
            const Specialty specialty("09.02.07", "Информатика");
            Group group("1И", specialty);
            group.addStudent(Student("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2));
            const Teacher teacher("Иванов Иван Иванович", Date(1975, 3, 12), "профессор",
                "доктор наук", Department("Прикладная математика"));

            std::vector<const Person*> people;
            for (const Student& student : group.getStudents())
            {
                people.push_back(&student);
            }
            people.push_back(&teacher);

            Assert::AreEqual(std::size_t(2), people.size());
            Assert::IsTrue(people[0]->isStudent());
            Assert::IsFalse(people[0]->isTeacher());
            Assert::IsTrue(people[1]->isTeacher());
            Assert::IsFalse(people[1]->isStudent());
            Assert::AreEqual(std::string("Студент"), people[0]->getRole());
            Assert::AreEqual(std::string("Преподаватель"), people[1]->getRole());
        }

        /** @brief Студент и преподаватель с разными id не равны. */
        TEST_METHOD(CompareStudentAndTeacher_DifferentPeople_AreNotEqual)
        {
            const Student student("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2);
            const Teacher teacher("Иванов Иван Иванович", Date(1975, 3, 12), "профессор",
                "доктор наук", Department("Прикладная математика"));
            const Person& personStudent = student;
            const Person& personTeacher = teacher;

            Assert::IsTrue(personStudent != personTeacher);
            Assert::IsFalse(personStudent == personTeacher);
        }
    };

    // ==== Тесты класса Student ====

    TEST_CLASS(StudentTests)
    {
    public:
        /** @brief Пустой номер зачётной книжки отклоняется. */
        TEST_METHOD(Constructor_EmptyRecordBook_Throws)
        {
            Assert::ExpectException<std::invalid_argument>(
                [] { Student("Смирнов Алексей Николаевич", Date(2005, 4, 12), "", 2); });
        }

        /** @brief Нулевой курс отклоняется. */
        TEST_METHOD(Constructor_ZeroCourse_Throws)
        {
            Assert::ExpectException<std::invalid_argument>(
                [] { Student("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 0); });
        }

        /** @brief Корректная оценка сохраняется. */
        TEST_METHOD(AddMark_ValidMark_IsStored)
        {
            Student student("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2);

            student.addMark(5.0);
            student.addMark(3.0);

            Assert::AreEqual(std::size_t(2), student.getMarkCount());
            Assert::IsTrue(student.hasMarks());
            Assert::AreEqual(5.0, student.getMarks().at(0));
            Assert::AreEqual(3.0, student.getMarks().at(1));
        }

        /** @brief Оценка вне диапазона 2-5 отклоняется. */
        TEST_METHOD(AddMark_OutOfRange_Throws)
        {
            Student student("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2);

            Assert::ExpectException<std::invalid_argument>([&] { student.addMark(1.0); });
            Assert::ExpectException<std::invalid_argument>([&] { student.addMark(5.5); });
        }

        /** @brief Средний балл считается по всем оценкам. */
        TEST_METHOD(GetAverageMark_CalculatesMean)
        {
            Student student("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2);
            student.addMark(5.0);
            student.addMark(4.0);

            Assert::AreEqual(4.5, student.getAverageMark());
        }

        /** @brief Без оценок средний балл равен нулю. */
        TEST_METHOD(GetAverageMark_NoMarks_ReturnsZero)
        {
            const Student student("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2);

            Assert::AreEqual(0.0, student.getAverageMark());
            Assert::IsFalse(student.hasMarks());
        }

        /** @brief Сведения роли содержат номер книжки и курс. */
        TEST_METHOD(GetSpecificInfo_ContainsRecordBookAndCourse)
        {
            const Student student("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2);
            const std::string info = student.getSpecificInfo();

            Assert::IsTrue(info.find("17-142") != std::string::npos);
            Assert::IsTrue(info.find("Курс: 2") != std::string::npos);
        }

        /** @brief Геттеры возвращают данные, переданные конструктором. */
        TEST_METHOD(Getters_ReturnConstructorValues)
        {
            const Student student("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 3);

            Assert::AreEqual(std::string("17-142"), student.getRecordBook());
            Assert::AreEqual(3u, student.getCourse());
            Assert::AreEqual(std::string("Смирнов Алексей Николаевич"), student.getFullName());
        }
    };

    // ==== Тесты класса Teacher ====

    TEST_CLASS(TeacherTests)
    {
    public:
        /** @brief Пустые ФИО или должность отклоняются. */
        TEST_METHOD(Constructor_EmptyPosition_Throws)
        {
            Assert::ExpectException<std::invalid_argument>(
                [] { Teacher("", Date(1975, 3, 12), "профессор", "", Department("Кафедра")); });
            Assert::ExpectException<std::invalid_argument>(
                [] { Teacher("Иванов И. И.", Date(1975, 3, 12), "  ", "", Department("Кафедра")); });
        }

        /** @brief Преподаватель хранит свою кафедру значением. */
        TEST_METHOD(GetDepartment_ReturnsItsDepartment)
        {
            const Teacher teacher("Иванов Иван Иванович", Date(1975, 3, 12), "профессор",
                "доктор наук", Department("Прикладная математика"));

            Assert::AreEqual(std::string("Прикладная математика"), teacher.getDepartment().getName());
        }

        /** @brief Пустая учёная степень допустима. */
        TEST_METHOD(Constructor_EmptyDegree_IsAllowed)
        {
            const Teacher teacher("Иванов Иван Иванович", Date(1975, 3, 12), "доцент",
                "", Department("Прикладная математика"));

            Assert::AreEqual(std::string("доцент"), teacher.getPosition());
            Assert::AreEqual(std::string(""), teacher.getDegree());
        }

        /** @brief Сведения роли содержат должность и кафедру. */
        TEST_METHOD(GetSpecificInfo_ContainsPositionAndDepartment)
        {
            const Teacher teacher("Иванов Иван Иванович", Date(1975, 3, 12), "профессор",
                "доктор наук", Department("Прикладная математика"));
            const std::string info = teacher.getSpecificInfo();

            Assert::IsTrue(info.find("профессор") != std::string::npos);
            Assert::IsTrue(info.find("Прикладная математика") != std::string::npos);
        }

        /** @brief Роль преподавателя определяется корректно. */
        TEST_METHOD(IsTeacher_TrueWhileIsStudent_False)
        {
            const Teacher teacher("Иванов Иван Иванович", Date(1975, 3, 12), "профессор",
                "доктор наук", Department("Прикладная математика"));

            Assert::IsTrue(teacher.isTeacher());
            Assert::IsFalse(teacher.isStudent());
        }
    };

    // ==== Тесты классов Group и Specialty ====

    TEST_CLASS(GroupSpecialtyTests)
    {
    public:
        /** @brief Пустой код специальности отклоняется. */
        TEST_METHOD(Specialty_EmptyCode_Throws)
        {
            Assert::ExpectException<std::invalid_argument>(
                [] { Specialty("", "Информатика"); });
        }

        /** @brief Пустое название специальности отклоняется. */
        TEST_METHOD(Specialty_EmptyName_Throws)
        {
            Assert::ExpectException<std::invalid_argument>(
                [] { Specialty("09.02.07", "   "); });
        }

        /** @brief Краткое представление специальности содержит код и название. */
        TEST_METHOD(Specialty_GetShortInfo_ContainsCodeAndName)
        {
            const Specialty specialty("09.02.07", "Информатика");

            Assert::AreEqual(std::string("09.02.07 - Информатика"), specialty.getShortInfo());
        }

        /** @brief Пустой номер группы отклоняется. */
        TEST_METHOD(Group_EmptyNumber_Throws)
        {
            const Specialty specialty("09.02.07", "Информатика");

            Assert::ExpectException<std::invalid_argument>(
                [&] { Group("", specialty); });
        }

        /** @brief Группа хранит специальность, переданную конструктором. */
        TEST_METHOD(Group_KnowsItsSpecialty)
        {
            const Specialty specialty("09.02.07", "Информатика");
            const Group group("1И", specialty);

            Assert::AreEqual(std::string("1И"), group.getNumber());
            Assert::AreEqual(std::string("09.02.07"), group.getSpecialty().getCode());
        }

        /** @brief Добавленные студенты попадают в список группы. */
        TEST_METHOD(AddStudent_GrowsStudentList)
        {
            const Specialty specialty("09.02.07", "Информатика");
            Group group("1И", specialty);

            group.addStudent(Student("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2));
            group.addStudent(Student("Волков Дмитрий Андреевич", Date(2005, 9, 30), "17-143", 2));

            Assert::AreEqual(std::size_t(2), group.getStudentCount());
        }

        /** @brief Повторное добавление студента с той же книжкой отклоняется. */
        TEST_METHOD(AddStudent_DuplicateRecordBook_Throws)
        {
            const Specialty specialty("09.02.07", "Информатика");
            Group group("1И", specialty);
            group.addStudent(Student("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2));

            Assert::ExpectException<std::invalid_argument>(
                [&] { group.addStudent(Student("Волков Дмитрий Андреевич", Date(2005, 9, 30), "17-142", 2)); });
        }

        /** @brief Поиск студента по номеру книжки работает без учёта регистра. */
        TEST_METHOD(FindStudentByRecordBook_ReturnsStudent)
        {
            const Specialty specialty("09.02.07", "Информатика");
            Group group("1И", specialty);
            group.addStudent(Student("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2));

            Assert::IsTrue(group.findStudentByRecordBook("17-142"));
            const std::vector<Student> found = group.getStudentByRecordBook("17-142");
            Assert::AreEqual(std::size_t(1), found.size());
            Assert::AreEqual(std::string("Смирнов Алексей Николаевич"), found.at(0).getFullName());
        }

        /** @brief Неизвестный номер книжки не находится. */
        TEST_METHOD(FindStudentByRecordBook_WrongNumber_ReturnsNull)
        {
            const Specialty specialty("09.02.07", "Информатика");
            Group group("1И", specialty);

            Assert::IsFalse(group.findStudentByRecordBook("99-999"));
            Assert::IsTrue(group.getStudentByRecordBook("99-999").empty());
        }

        /** @brief Средний балл группы усредняет оценки всех студентов. */
        TEST_METHOD(GetAverageMark_AggregatesAllStudents)
        {
            const Specialty specialty("09.02.07", "Информатика");
            Group group("1И", specialty);
            Student first("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2);
            first.addMark(5.0);
            Student second("Волков Дмитрий Андреевич", Date(2005, 9, 30), "17-143", 2);
            second.addMark(4.0);
            second.addMark(4.0);
            group.addStudent(first);
            group.addStudent(second);

            Assert::AreEqual(13.0 / 3.0, group.getAverageMark());
        }

        /** @brief Информация о группе содержит специальность и число студентов. */
        TEST_METHOD(Group_GetInfo_ContainsSpecialtyAndStudentCount)
        {
            const Specialty specialty("09.02.07", "Информатика");
            Group group("1И", specialty);
            group.addStudent(Student("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2));
            const std::string info = group.getInfo();

            Assert::IsTrue(info.find("1И") != std::string::npos);
            Assert::IsTrue(info.find("09.02.07") != std::string::npos);
            Assert::IsTrue(info.find("Студентов: 1") != std::string::npos);
        }
    };

    // ==== Тесты классов Department и Discipline ====

    TEST_CLASS(DepartmentTests)
    {
    public:
        /** @brief Пустое название кафедры отклоняется. */
        TEST_METHOD(Department_EmptyName_Throws)
        {
            Assert::ExpectException<std::invalid_argument>([] { Department(""); });
        }

        /** @brief Кафедры с одинаковыми названиями равны. */
        TEST_METHOD(Department_EqualNames_AreEqual)
        {
            const Department first("Прикладная математика");
            const Department second("Прикладная математика");
            const Department third("Информационные системы");

            Assert::IsTrue(first == second);
            Assert::IsTrue(first != third);
        }

        /** @brief Краткое представление дисциплины равно её названию. */
        TEST_METHOD(Discipline_GetShortInfo_IsItsName)
        {
            const Discipline discipline("Дискретная математика", Department("Прикладная математика"));

            Assert::AreEqual(std::string("Дискретная математика"), discipline.getShortInfo());
        }

        /** @brief Дисциплина помнит кафедру, переданную конструктором. */
        TEST_METHOD(Discipline_KnowsItsDepartment)
        {
            const Discipline discipline("Дискретная математика", Department("Прикладная математика"));

            Assert::AreEqual(std::string("Прикладная математика"), discipline.getDepartment().getName());
        }

        /** @brief Дисциплины разных кафедр не равны. */
        TEST_METHOD(Discipline_DifferentDepartments_AreNotEqual)
        {
            const Discipline first("Дискретная математика", Department("Прикладная математика"));
            const Discipline second("Дискретная математика", Department("Информационные системы"));

            Assert::IsTrue(first != second);
        }

        /** @brief Пустое название дисциплины отклоняется. */
        TEST_METHOD(Discipline_EmptyName_Throws)
        {
            Assert::ExpectException<std::invalid_argument>(
                [] { Discipline("", Department("Прикладная математика")); });
        }
    };

    // ==== Тесты класса Lesson ====

    TEST_CLASS(LessonTests)
    {
    private:
        static Specialty makeSpecialty()
        {
            return Specialty("09.02.07", "Информатика");
        }

    public:
        /** @brief Лабораторную работу без второго преподавателя вести нельзя. */
        TEST_METHOD(Lab_WithoutSecondTeacher_Throws)
        {
            const Discipline discipline("Дискретная математика", Department("Прикладная математика"));
            const Group group("1И", makeSpecialty());
            const Teacher teacher("Иванов Иван Иванович", Date(1975, 3, 12), "профессор",
                "доктор наук", Department("Прикладная математика"));

            Assert::ExpectException<std::invalid_argument>(
                [&] { Lesson(LessonType::Lab, discipline, group, teacher, nullptr, "12", Date(2026, 9, 3)); });
        }

        /** @brief Два одинаковых преподавателя для лабораторной недопустимы. */
        TEST_METHOD(Lab_WithSameTeacherTwice_Throws)
        {
            const Discipline discipline("Дискретная математика", Department("Прикладная математика"));
            const Group group("1И", makeSpecialty());
            const Teacher teacher("Иванов Иван Иванович", Date(1975, 3, 12), "профессор",
                "доктор наук", Department("Прикладная математика"));

            Assert::ExpectException<std::invalid_argument>(
                [&] { Lesson(LessonType::Lab, discipline, group, teacher, &teacher, "12", Date(2026, 9, 3)); });
        }

        /** @brief Второй преподаватель нужен только для лабораторной. */
        TEST_METHOD(Lecture_WithSecondTeacher_Throws)
        {
            const Discipline discipline("Дискретная математика", Department("Прикладная математика"));
            const Group group("1И", makeSpecialty());
            const Teacher first("Иванов Иван Иванович", Date(1975, 3, 12), "профессор",
                "доктор наук", Department("Прикладная математика"));
            const Teacher second("Петров Пётр Петрович", Date(1982, 7, 25), "доцент",
                "кандидат наук", Department("Прикладная математика"));

            Assert::ExpectException<std::invalid_argument>(
                [&] { Lesson(LessonType::Lecture, discipline, group, first, &second, "25", Date(2026, 9, 1)); });
        }

        /** @brief Лабораторная хранит обоих преподавателей. */
        TEST_METHOD(Lab_WithTwoTeachers_StoresBoth)
        {
            const Discipline discipline("Дискретная математика", Department("Прикладная математика"));
            const Group group("1И", makeSpecialty());
            const Teacher first("Иванов Иван Иванович", Date(1975, 3, 12), "профессор",
                "доктор наук", Department("Прикладная математика"));
            const Teacher second("Петров Пётр Петрович", Date(1982, 7, 25), "доцент",
                "кандидат наук", Department("Прикладная математика"));

            const Lesson lesson(LessonType::Lab, discipline, group, first, &second, "12", Date(2026, 9, 3));

            Assert::AreEqual(std::size_t(2), lesson.getTeacherCount());
            Assert::IsTrue(lesson.isTaughtBy(first));
            Assert::IsTrue(lesson.isTaughtBy(second));
        }

        /** @brief Лекция хранит ровно одного преподавателя. */
        TEST_METHOD(Lecture_WithOneTeacher_StoresOne)
        {
            const Discipline discipline("Дискретная математика", Department("Прикладная математика"));
            const Group group("1И", makeSpecialty());
            const Teacher teacher("Иванов Иван Иванович", Date(1975, 3, 12), "профессор",
                "доктор наук", Department("Прикладная математика"));

            const Lesson lesson(LessonType::Lecture, discipline, group, teacher, nullptr, "25", Date(2026, 9, 1));

            Assert::AreEqual(std::size_t(1), lesson.getTeacherCount());
            Assert::AreEqual(std::string("Лекция"), toString(lesson.getType()));
            Assert::AreEqual(std::string("1И"), lesson.getGroupNumber());
        }

        /** @brief Чужой преподаватель не считается ведущим занятие. */
        TEST_METHOD(IsTaughtBy_UnrelatedTeacher_ReturnsFalse)
        {
            const Discipline discipline("Дискретная математика", Department("Прикладная математика"));
            const Group group("1И", makeSpecialty());
            const Teacher teacher("Иванов Иван Иванович", Date(1975, 3, 12), "профессор",
                "доктор наук", Department("Прикладная математика"));
            const Teacher stranger("Петров Пётр Петрович", Date(1982, 7, 25), "доцент",
                "кандидат наук", Department("Прикладная математика"));
            const Lesson lesson(LessonType::Lecture, discipline, group, teacher, nullptr, "25", Date(2026, 9, 1));

            Assert::IsFalse(lesson.isTaughtBy(stranger));
        }

        /** @brief Занятия с разными преподавателями не равны. */
        TEST_METHOD(OperatorEqual_DifferentTeacher_ReturnsFalse)
        {
            const Discipline discipline("Дискретная математика", Department("Прикладная математика"));
            const Group group("1И", makeSpecialty());
            const Teacher first("Иванов Иван Иванович", Date(1975, 3, 12), "профессор",
                "доктор наук", Department("Прикладная математика"));
            const Teacher second("Петров Пётр Петрович", Date(1982, 7, 25), "доцент",
                "кандидат наук", Department("Прикладная математика"));
            const Lesson firstLesson(LessonType::Lecture, discipline, group, first, nullptr,
                "25", Date(2026, 9, 1));
            const Lesson secondLesson(LessonType::Lecture, discipline, group, second, nullptr,
                "25", Date(2026, 9, 1));

            Assert::IsFalse(firstLesson == secondLesson);
            Assert::IsTrue(firstLesson != secondLesson);
        }

        /** @brief Полностью совпадающие занятия равны. */
        TEST_METHOD(OperatorEqual_SameData_ReturnsTrue)
        {
            const Discipline discipline("Дискретная математика", Department("Прикладная математика"));
            const Group group("1И", makeSpecialty());
            const Teacher teacher("Иванов Иван Иванович", Date(1975, 3, 12), "профессор",
                "доктор наук", Department("Прикладная математика"));
            const Lesson firstLesson(LessonType::Lecture, discipline, group, teacher, nullptr,
                "25", Date(2026, 9, 1));
            const Lesson secondLesson(LessonType::Lecture, discipline, group, teacher, nullptr,
                "25", Date(2026, 9, 1));

            Assert::IsTrue(firstLesson == secondLesson);
        }

        /** @brief Лабораторные с одинаковым составом преподавателей равны. */
        TEST_METHOD(OperatorEqual_LabWithSwappedTeachers_ReturnsTrue)
        {
            const Discipline discipline("Дискретная математика", Department("Прикладная математика"));
            const Group group("1И", makeSpecialty());
            const Teacher first("Иванов Иван Иванович", Date(1975, 3, 12), "профессор",
                "доктор наук", Department("Прикладная математика"));
            const Teacher second("Петров Пётр Петрович", Date(1982, 7, 25), "доцент",
                "кандидат наук", Department("Прикладная математика"));
            const Lesson firstLesson(LessonType::Lab, discipline, group, first, &second,
                "12", Date(2026, 9, 3));
            const Lesson secondLesson(LessonType::Lab, discipline, group, second, &first,
                "12", Date(2026, 9, 3));

            Assert::IsTrue(firstLesson == secondLesson);
        }

        /** @brief Описание занятия содержит вид, дисциплину и группу. */
        TEST_METHOD(GetInfo_ContainsTypeDisciplineAndGroup)
        {
            const Discipline discipline("Дискретная математика", Department("Прикладная математика"));
            const Group group("1И", makeSpecialty());
            const Teacher teacher("Иванов Иван Иванович", Date(1975, 3, 12), "профессор",
                "доктор наук", Department("Прикладная математика"));
            const Lesson lesson(LessonType::Lecture, discipline, group, teacher, nullptr, "12", Date(2026, 9, 3));
            const std::string info = lesson.getInfo();

            Assert::IsTrue(info.find("Лекция") != std::string::npos);
            Assert::IsTrue(info.find("Дискретная математика") != std::string::npos);
            Assert::IsTrue(info.find("1И") != std::string::npos);
            Assert::IsTrue(info.find("2026-09-03") != std::string::npos);
        }
    };

    // ==== Тесты класса University ====

    TEST_CLASS(UniversityTests)
    {
    private:
        static University makeUniversity()
        {
            University university("Тестовый ВУЗ");

            const Department appliedMath("Прикладная математика");
            const Department informationSystems("Информационные системы");
            university.addDepartment(appliedMath);
            university.addDepartment(informationSystems);

            const Specialty informatics("09.02.07", "Информатика");
            const Specialty economics("38.02.01", "Экономика");
            university.addSpecialty(informatics);
            university.addSpecialty(economics);

            const Discipline discreteMath("Дискретная математика", appliedMath);
            const Discipline linearAlgebra("Линейная алгебра", appliedMath);
            const Discipline databases("Базы данных", informationSystems);
            university.addDiscipline(discreteMath);
            university.addDiscipline(linearAlgebra);
            university.addDiscipline(databases);

            const Teacher ivanov("Иванов Иван Иванович", Date(1975, 3, 12), "профессор",
                "доктор наук", appliedMath);
            const Teacher petrov("Петров Пётр Петрович", Date(1982, 7, 25), "доцент",
                "кандидат наук", appliedMath);
            const Teacher kuznetsova("Кузнецова Мария Сергеевна", Date(1988, 11, 3), "доцент",
                "кандидат наук", informationSystems);
            university.addTeacher(ivanov);
            university.addTeacher(petrov);
            university.addTeacher(kuznetsova);

            Group group1I("1И", informatics);
            Group group2I("2И", informatics);
            Group group1E("1Э", economics);

            Student smirnov("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2);
            smirnov.addMark(5.0);
            smirnov.addMark(4.0);
            group1I.addStudent(smirnov);

            Student orlova("Орлова Елена Викторовна", Date(2004, 12, 8), "16-101", 3);
            orlova.addMark(5.0);
            group2I.addStudent(orlova);

            Student morozov("Морозов Павел Олегович", Date(2006, 2, 17), "18-201", 1);
            morozov.addMark(3.0);
            group1E.addStudent(morozov);

            university.addGroup(group1I);
            university.addGroup(group2I);
            university.addGroup(group1E);

            university.addLesson(Lesson(LessonType::Lecture, discreteMath, group1I, ivanov,
                nullptr, "25", Date(2026, 9, 1)));
            university.addLesson(Lesson(LessonType::Lab, discreteMath, group1I, petrov, &ivanov,
                "12", Date(2026, 9, 3)));
            university.addLesson(Lesson(LessonType::Practical, linearAlgebra, group2I, petrov,
                nullptr, "25", Date(2026, 9, 2)));

            return university;
        }

    public:
        /** @brief Повторное добавление специальности с тем же кодом отклоняется. */
        TEST_METHOD(AddSpecialty_DuplicateCode_Throws)
        {
            University university("Тестовый ВУЗ");
            university.addSpecialty(Specialty("09.02.07", "Информатика"));

            Assert::ExpectException<std::invalid_argument>(
                [&] { university.addSpecialty(Specialty("09.02.07", "Информатика")); });
        }

        /** @brief Повторное добавление кафедры отклоняется. */
        TEST_METHOD(AddDepartment_DuplicateName_Throws)
        {
            University university("Тестовый ВУЗ");
            university.addDepartment(Department("Прикладная математика"));

            Assert::ExpectException<std::invalid_argument>(
                [&] { university.addDepartment(Department("Прикладная математика")); });
        }

        /** @brief Повторное добавление группы отклоняется. */
        TEST_METHOD(AddGroup_DuplicateNumber_Throws)
        {
            University university("Тестовый ВУЗ");
            const Specialty informatics("09.02.07", "Информатика");
            university.addGroup(Group("1И", informatics));

            Assert::ExpectException<std::invalid_argument>(
                [&] { university.addGroup(Group("1И", informatics)); });
        }

        /** @brief Занятие с неизвестным преподавателем отклоняется. */
        TEST_METHOD(AddLesson_UnknownTeacher_Throws)
        {
            University university("Тестовый ВУЗ");
            const Specialty informatics("09.02.07", "Информатика");
            const Discipline discipline("Дискретная математика", Department("Прикладная математика"));
            const Group group("1И", informatics);
            const Teacher stranger("Иванов Иван Иванович", Date(1975, 3, 12), "профессор",
                "доктор наук", Department("Прикладная математика"));
            university.addGroup(group);

            Assert::ExpectException<std::invalid_argument>(
                [&] { university.addLesson(Lesson(LessonType::Lecture, discipline, group, stranger,
                    nullptr, "25", Date(2026, 9, 1))); });
        }

        /** @brief Занятие в незарегистрированной группе отклоняется. */
        TEST_METHOD(AddLesson_UnknownGroup_Throws)
        {
            University university = makeUniversity();
            const Specialty informatics("09.02.07", "Информатика");
            const Group unknownGroup("99З", informatics);
            const Discipline discipline("Дискретная математика", Department("Прикладная математика"));
            const Teacher teacher = university.getTeachers().at(0);

            Assert::ExpectException<std::invalid_argument>(
                [&] { university.addLesson(Lesson(LessonType::Lecture, discipline, unknownGroup,
                    teacher, nullptr, "25", Date(2026, 9, 1))); });
        }

        /** @brief Название ВУЗа не может быть пустым. */
        TEST_METHOD(SetName_Empty_Throws)
        {
            University university("Тестовый ВУЗ");

            Assert::ExpectException<std::invalid_argument>([&] { university.setName("   "); });
        }

        /** @brief Студент находится по номеру зачётной книжки в любой группе. */
        TEST_METHOD(FindStudentByRecordBook_FindsAcrossGroups)
        {
            const University university = makeUniversity();

            Assert::IsTrue(university.findStudentByRecordBook("16-101"));
            Assert::IsFalse(university.findStudentByRecordBook("99-999"));
            const std::vector<Student> students = university.getStudentByRecordBook("16-101");
            Assert::AreEqual(std::size_t(1), students.size());
            Assert::AreEqual(std::string("Орлова Елена Викторовна"), students.at(0).getFullName());
            Assert::IsTrue(university.getStudentByRecordBook("99-999").empty());
        }

        /** @brief Поиск по части ФИО регистронезависим. */
        TEST_METHOD(FindStudentsByFullName_MatchesPartially)
        {
            const University university = makeUniversity();

            const std::vector<Student> students = university.findStudentsByFullName("смирнов");

            Assert::AreEqual(std::size_t(1), students.size());
            Assert::AreEqual(std::string("17-142"), students.at(0).getRecordBook());
        }

        /** @brief Кафедра возвращает только свои дисциплины. */
        TEST_METHOD(GetDisciplinesByDepartment_ReturnsOnlyItsDisciplines)
        {
            const University university = makeUniversity();

            const std::vector<Discipline> disciplines =
                university.getDisciplinesByDepartment(Department("Прикладная математика"));

            Assert::AreEqual(std::size_t(2), disciplines.size());
            Assert::AreEqual(std::string("Дискретная математика"), disciplines.at(0).getName());
            Assert::AreEqual(std::string("Линейная алгебра"), disciplines.at(1).getName());
        }

        /** @brief Преподаватели группы определяются по её занятиям. */
        TEST_METHOD(GetTeachersByGroup_ReturnsTeachersFromLessons)
        {
            const University university = makeUniversity();

            const std::vector<Teacher> teachers =
                university.getTeachersByGroup(university.getGroups().at(0));

            Assert::AreEqual(std::size_t(2), teachers.size());
        }

        /** @brief Одинаковые преподаватели не задваиваются в списке группы. */
        TEST_METHOD(GetTeachersByGroup_DeduplicatesTeachers)
        {
            const University university = makeUniversity();

            const std::vector<Teacher> teachers =
                university.getTeachersByGroup(university.getGroups().at(0));

            for (std::size_t i = 0; i < teachers.size(); ++i)
            {
                for (std::size_t j = i + 1; j < teachers.size(); ++j)
                {
                    Assert::AreNotEqual(teachers.at(i).getId(), teachers.at(j).getId());
                }
            }
        }

        /** @brief Группа без занятий не имеет преподавателей. */
        TEST_METHOD(GetTeachersByGroup_GroupWithoutLessons_ReturnsEmpty)
        {
            const University university = makeUniversity();

            const std::vector<Teacher> teachers =
                university.getTeachersByGroup(university.getGroups().at(2));

            Assert::AreEqual(std::size_t(0), teachers.size());
        }

        /** @brief Все группы специальности возвращаются одним запросом. */
        TEST_METHOD(GetGroupsBySpecialty_ReturnsAllItsGroups)
        {
            const University university = makeUniversity();

            const std::vector<Group> groups =
                university.getGroupsBySpecialty(Specialty("09.02.07", "Информатика"));

            Assert::AreEqual(std::size_t(2), groups.size());
            Assert::AreEqual(std::string("1И"), groups.at(0).getNumber());
            Assert::AreEqual(std::string("2И"), groups.at(1).getNumber());
        }

        /** @brief В ВУЗе есть и студенты, и преподаватели. */
        TEST_METHOD(GetAllStudentsAndTeachers_ReturnSeparateCollections)
        {
            const University university = makeUniversity();

            const std::vector<Student> students = university.getAllStudents();
            const std::vector<Teacher> teachers = university.getAllTeachers();

            Assert::AreEqual(std::size_t(3), students.size());
            Assert::AreEqual(std::size_t(3), teachers.size());
        }

        /** @brief Коллекции возвращаются копиями и не зависят от ВУЗа. */
        TEST_METHOD(GetAllStudents_ReturnsIndependentCopies)
        {
            University university = makeUniversity();
            std::vector<Student> students = university.getAllStudents();
            const std::size_t marksBefore = students.at(0).getMarkCount();

            students.at(0).addMark(2.0);
            const std::size_t marksInUniversity =
                university.getAllStudents().at(0).getMarkCount();

            Assert::AreEqual(marksBefore, marksInUniversity);
        }

        /** @brief Статистика содержит счётчики объектов ВУЗа. */
        TEST_METHOD(GetStatistics_ContainsCounts)
        {
            const University university = makeUniversity();
            const std::string statistics = university.getStatistics();

            Assert::IsTrue(statistics.find("Групп: 3") != std::string::npos);
            Assert::IsTrue(statistics.find("Студентов: 3") != std::string::npos);
            Assert::IsTrue(statistics.find("Преподавателей: 3") != std::string::npos);
            Assert::IsTrue(statistics.find("Занятий: 3") != std::string::npos);
        }

        /** @brief Поиск преподавателя по ФИО работает. */
        TEST_METHOD(FindTeacherByFullName_ReturnsTeacher)
        {
            const University university = makeUniversity();

            Assert::IsTrue(university.findTeacherByFullName("кузнецова мария сергеевна"));
            Assert::IsFalse(university.findTeacherByFullName("Сидоров Сидор Сидорович"));
            const std::vector<Teacher> teachers = university.getTeacherByFullName("кузнецова мария сергеевна");
            Assert::AreEqual(std::size_t(1), teachers.size());
            Assert::AreEqual(std::string("Информационные системы"), teachers.at(0).getDepartment().getName());
        }

        /** @brief Неизвестный код специальности не находится. */
        TEST_METHOD(FindSpecialtyByCode_Unknown_ReturnsNull)
        {
            const University university = makeUniversity();

            Assert::IsFalse(university.findSpecialtyByCode("00.00.00"));
            Assert::IsTrue(university.findSpecialtyByCode("09.02.07"));
            Assert::IsTrue(university.findDepartmentByName("Информационные системы"));
            Assert::IsFalse(university.findDepartmentByName("Неизвестная кафедра"));
            Assert::IsTrue(university.findGroupByNumber("1И"));
            Assert::IsFalse(university.findGroupByNumber("99З"));
        }
    };

    // ==== Тесты вспомогательных функций ====

    TEST_CLASS(FormatUtilsTests)
    {
    public:
        /** @brief Число форматируется с двумя знаками после запятой. */
        TEST_METHOD(FormatDecimal_TwoDigits)
        {
            Assert::AreEqual(std::string("4.50"), FormatUtils::formatDecimal(4.5));
            Assert::AreEqual(std::string("4.67"), FormatUtils::formatDecimal(14.0 / 3.0));
            Assert::AreEqual(std::string("5.00"), FormatUtils::formatDecimal(5.0));
        }

        /** @brief Сравнение строк регистронезависимо для кириллицы. */
        TEST_METHOD(EqualsIgnoreCase_HandlesCyrillic)
        {
            Assert::IsTrue(FormatUtils::equalsIgnoreCase("ПРИКЛАДНАЯ", "прикладная"));
            Assert::IsFalse(FormatUtils::equalsIgnoreCase("Прикладная", "Информатика"));
        }

        /** @brief Подстрока находится регистронезависимо. */
        TEST_METHOD(ContainsIgnoreCase_FindsPart)
        {
            Assert::IsTrue(FormatUtils::containsIgnoreCase("Смирнов Алексей", "смирнов"));
            Assert::IsFalse(FormatUtils::containsIgnoreCase("Смирнов Алексей", "Петров"));
        }

        /** @brief Пустая подстрока всегда считается найденной. */
        TEST_METHOD(ContainsIgnoreCase_EmptyNeedle_True)
        {
            Assert::IsTrue(FormatUtils::containsIgnoreCase("Смирнов Алексей", ""));
        }

        /** @brief ФИО сокращается до вида "Фамилия И. О.". */
        TEST_METHOD(ToInitials_ShortensNameAndPatronymic)
        {
            Assert::AreEqual(std::string("Смирнов А. Н."),
                FormatUtils::toInitials("Смирнов  Алексей   Николаевич"));
        }

        /** @brief Крайние пробелы удаляются. */
        TEST_METHOD(Trim_RemovesEdgeSpaces)
        {
            Assert::AreEqual(std::string("Прикладная"), FormatUtils::trim("   Прикладная   "));
            Assert::AreEqual(std::string(""), FormatUtils::trim("     "));
        }

        /** @brief Склейка использует переданный разделитель. */
        TEST_METHOD(Join_WithSeparator)
        {
            const std::vector<std::string> items{ "ауд. 12", "ауд. 25" };

            Assert::AreEqual(std::string("ауд. 12; ауд. 25"), FormatUtils::join(items, "; "));
        }

        /** @brief Склейка пустой коллекции даёт пустую строку. */
        TEST_METHOD(Join_Empty_ReturnsEmpty)
        {
            const std::vector<std::string> items;

            Assert::AreEqual(std::string(""), FormatUtils::join(items, "; "));
        }
    };
}
