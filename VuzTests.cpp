//
// VuzTests.cpp
// Все модульные тесты проекта на встроенном в Visual Studio фреймворке
// Microsoft CppUnitTest:
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
#include "CppUnitTest.h"
#include "VuzLib.h"

#include <algorithm>
#include <memory>
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
            Date d(2004, 3, 15);
            Assert::AreEqual(2004, d.GetYear());
            Assert::AreEqual(3, d.GetMonth());
            Assert::AreEqual(15, d.GetDay());
        }

        /** @brief Parse() извлекает компоненты из строки формата ГГГГ-ММ-ДД. */
        TEST_METHOD(Parse_ValidIsoString_ReturnsCorrectComponents)
        {
            Date d = Date::Parse("2004-03-15");
            Assert::AreEqual(2004, d.GetYear());
            Assert::AreEqual(3, d.GetMonth());
            Assert::AreEqual(15, d.GetDay());
        }

        /** @brief Parse() отклоняет строку в неверном формате. */
        TEST_METHOD(Parse_WrongFormat_ThrowsInvalidArgument)
        {
            Assert::ExpectException<std::invalid_argument>([]() {
                Date::Parse("15-03-2004");
                });
        }

        /** @brief Конструктор отклоняет месяц вне диапазона 1..12. */
        TEST_METHOD(Constructor_InvalidMonth_Throws)
        {
            Assert::ExpectException<std::invalid_argument>([]() {
                Date invalid(2004, 13, 1);
                (void)invalid;
                });
        }

        /** @brief Конструктор отклоняет день, которого нет в этом месяце. */
        TEST_METHOD(Constructor_InvalidDayForMonth_Throws)
        {
            Assert::ExpectException<std::invalid_argument>([]() {
                Date invalid(2004, 2, 30); // 2004 - високосный, но в феврале 29 дней
                (void)invalid;
                });
        }

        /** @brief 29 февраля допустимо в високосном году. */
        TEST_METHOD(LeapYear_February29_IsValid)
        {
            Date d(2004, 2, 29);
            Assert::AreEqual(29, d.GetDay());
        }

        /** @brief DaysUntil() возвращает ноль для одной и той же даты. */
        TEST_METHOD(DaysUntil_SameDate_ReturnsZero)
        {
            Date d(2026, 9, 29);
            Assert::AreEqual(0L, d.DaysUntil(d));
        }

        /** @brief DaysUntil() возвращает 7 через ровно неделю. */
        TEST_METHOD(DaysUntil_OneWeekLater_ReturnsSeven)
        {
            Date start(2026, 9, 22);
            Date end(2026, 9, 29);
            Assert::AreEqual(7L, start.DaysUntil(end));
        }

        /** @brief DaysUntil() корректен при переходе через границу года. */
        TEST_METHOD(DaysUntil_AcrossYearBoundary_IsCorrect)
        {
            Date start(2025, 12, 30);
            Date end(2026, 1, 2);
            Assert::AreEqual(3L, start.DaysUntil(end));
        }

        /** @brief Операторы сравнения работают корректно. */
        TEST_METHOD(ComparisonOperators_WorkAsExpected)
        {
            Date a(2026, 1, 1);
            Date b(2026, 1, 2);
            Assert::IsTrue(a < b);
            Assert::IsTrue(b > a);
            Assert::IsTrue(a != b);
            Assert::IsFalse(a == b);
            Assert::IsTrue(a == Date(2026, 1, 1));
            Assert::IsTrue(a <= Date(2026, 1, 1));
            Assert::IsTrue(b >= Date(2026, 1, 1));
        }

        /** @brief ToString() дополняет месяц и день ведущим нулём. */
        TEST_METHOD(ToString_FormatsWithLeadingZeros)
        {
            Date d(2026, 3, 5);
            Assert::AreEqual(std::string("2026-03-05"), d.ToString());
        }
    };

    // ==== Тесты базового класса Person и полиморфного обхода ====

    TEST_CLASS(PersonTests)
    {
    public:
        /** @brief Конструктор Person отклоняет пустое ФИО. */
        TEST_METHOD(Constructor_EmptyFullName_Throws)
        {
            Assert::ExpectException<std::invalid_argument>([]() {
                Student s("", Date(2005, 1, 1), "17-001", 1, nullptr);
                (void)s;
                });
        }

        /** @brief GetShortInfo() базового класса возвращает инициалы. */
        TEST_METHOD(GetShortInfo_ReturnsInitials)
        {
            Specialty specialty("09.02.07", "Информационные системы");
            Group group("1И", &specialty);
            Student s("Иванов Иван Иванович", Date(2005, 1, 1), "17-001", 1, &group);

            // Фамилия + инициалы + группа.
            Assert::AreEqual(std::string("Иванов И. И. (гр. 1И)"), s.GetShortInfo());
        }

        /** @brief GetAge() считает полные прожитые годы. */
        TEST_METHOD(GetAge_CalculatesFullYears)
        {
            Specialty specialty("09.02.07", "ИСиТ");
            Group group("1И", &specialty);
            Student s("Петров Пётр Петрович", Date(2005, 6, 15), "17-002", 1, &group);

            Assert::AreEqual(20, s.GetAge(Date(2026, 6, 14))); // день рождения ещё не наступил
            Assert::AreEqual(21, s.GetAge(Date(2026, 6, 15))); // в день рождения
            Assert::AreEqual(0, s.GetAge(Date(2000, 1, 1)));   // дата рождения не наступила
        }

        /**
         * @brief ГЛАВНЫЙ ТЕСТ задания 1: коллекция указателей на базовый
         *        тип Person обходится полиморфно - вызываются реализации
         *        GetRole() конкретных наследников.
         */
        TEST_METHOD(PolymorphicCollection_ReturnsDerivedRoles)
        {
            Specialty specialty("09.02.07", "ИСиТ");
            Group group("1И", &specialty);
            Department department("Прикладная математика");

            std::shared_ptr<Student> student =
                group.AddStudent("Сидоров Сидор Сидорович", Date(2005, 1, 1), "17-003", 1);
            std::shared_ptr<Teacher> teacher =
                std::make_shared<Teacher>("Кузнецов Кузьма Кузьмич", Date(1978, 5, 19),
                    "доцент", "канд. тех. наук", &department);

            // Коллекция объектов БАЗОВОГО типа, заполненная наследниками.
            std::vector<std::shared_ptr<Person>> people;
            people.push_back(student);
            people.push_back(teacher);

            Assert::AreEqual<size_t>(2, people.size());

            // Статический тип - Person, но вызовы виртуальные.
            Assert::AreEqual(std::string("Студент"), people[0]->GetRole());
            Assert::AreEqual(std::string("Преподаватель"), people[1]->GetRole());

            Assert::IsTrue(people[0]->IsStudent());
            Assert::IsFalse(people[0]->IsTeacher());
            Assert::IsTrue(people[1]->IsTeacher());
            Assert::IsFalse(people[1]->IsStudent());

            // GetInfo() тоже различается у наследников.
            const std::string studentInfo = people[0]->GetInfo();
            const std::string teacherInfo = people[1]->GetInfo();
            Assert::IsTrue(studentInfo.find("зачётная книжка") != std::string::npos);
            Assert::IsTrue(teacherInfo.find("Прикладная математика") != std::string::npos);
        }
    };

    // ==== Тесты класса Student ====

    TEST_CLASS(StudentTests)
    {
    public:
        /** @brief Корректно созданный студент возвращает свои поля. */
        TEST_METHOD(ConstructedStudent_ReturnsFields)
        {
            Specialty specialty("09.02.07", "Информационные системы и технологии");
            Group group("1И", &specialty);
            Student s("Смирнов Алексей Николаевич", Date(2005, 4, 12), "17-142", 2, &group);

            Assert::AreEqual(std::string("Смирнов Алексей Николаевич"), s.GetFullName());
            Assert::AreEqual(std::string("17-142"), s.GetRecordBook());
            Assert::AreEqual(2u, s.GetCourse());
            Assert::IsTrue(s.GetGroup() == &group);
            Assert::AreEqual(std::string("Студент"), s.GetRole());
        }

        /** @brief Конструктор отклоняет пустую зачётную книжку. */
        TEST_METHOD(Constructor_EmptyRecordBook_Throws)
        {
            Specialty specialty("09.02.07", "ИСиТ");
            Group group("1И", &specialty);
            Assert::ExpectException<std::invalid_argument>([&]() {
                Student s("Иванов И. И.", Date(2005, 1, 1), "   ", 1, &group);
                (void)s;
                });
        }

        /** @brief Конструктор отклоняет нулевой курс и nullptr-группу. */
        TEST_METHOD(Constructor_InvalidCourseOrGroup_Throws)
        {
            Specialty specialty("09.02.07", "ИСиТ");
            Group group("1И", &specialty);
            Assert::ExpectException<std::invalid_argument>([&]() {
                Student s("Иванов И. И.", Date(2005, 1, 1), "17-004", 0, &group);
                (void)s;
                });
            Assert::ExpectException<std::invalid_argument>([&]() {
                Student s("Иванов И. И.", Date(2005, 1, 1), "17-005", 1, nullptr);
                (void)s;
                });
        }

        /** @brief GetAverageMark() усредняет оценки; без оценок - 0.0. */
        TEST_METHOD(AverageMark_AveragesMarks)
        {
            Specialty specialty("09.02.07", "ИСиТ");
            Group group("1И", &specialty);
            Student s("Иванов И. И.", Date(2005, 1, 1), "17-006", 1, &group);

            Assert::AreEqual(0.0, s.GetAverageMark());
            s.AddMark(5.0);
            s.AddMark(4.0);
            s.AddMark(3.0);
            Assert::AreEqual(4.0, s.GetAverageMark());
        }

        /** @brief AddMark() отклоняет оценки вне диапазона 1..5. */
        TEST_METHOD(AddMark_OutOfRange_Throws)
        {
            Specialty specialty("09.02.07", "ИСиТ");
            Group group("1И", &specialty);
            Student s("Иванов И. И.", Date(2005, 1, 1), "17-007", 1, &group);

            Assert::ExpectException<std::invalid_argument>([&]() { s.AddMark(0.0); });
            Assert::ExpectException<std::invalid_argument>([&]() { s.AddMark(6.0); });
            Assert::ExpectException<std::invalid_argument>([&]() { s.AddMark(-1.0); });
        }
    };

    // ==== Тесты класса Teacher ====

    TEST_CLASS(TeacherTests)
    {
    public:
        /** @brief Корректно созданный преподаватель возвращает свои поля. */
        TEST_METHOD(ConstructedTeacher_ReturnsFields)
        {
            Department department("Прикладная математика");
            Teacher t("Иванов Иван Иванович", Date(1972, 3, 14),
                "профессор", "доктор физ.-мат. наук", &department);

            Assert::AreEqual(std::string("профессор"), t.GetPosition());
            Assert::AreEqual(std::string("доктор физ.-мат. наук"), t.GetDegree());
            Assert::IsTrue(t.GetDepartment() == &department);
            Assert::AreEqual(std::string("Преподаватель"), t.GetRole());
        }

        /** @brief Конструктор отклоняет пустую должность и nullptr-кафедру. */
        TEST_METHOD(Constructor_InvalidArguments_Throws)
        {
            Department department("Физика");
            Assert::ExpectException<std::invalid_argument>([&]() {
                Teacher t("Иванов И. И.", Date(1972, 1, 1), "  ", "", &department);
                (void)t;
                });
            Assert::ExpectException<std::invalid_argument>([&]() {
                Teacher t("Иванов И. И.", Date(1972, 1, 1), "доцент", "", nullptr);
                (void)t;
                });
        }

        /** @brief Пустая учёная степень допустима и отображается как "нет". */
        TEST_METHOD(EmptyDegree_IsAllowed)
        {
            Department department("Физика");
            Teacher t("Сидоров С. С.", Date(1985, 1, 1), "ассистент", "", &department);

            Assert::AreEqual(std::string(""), t.GetDegree());
            Assert::IsTrue(t.GetInfo().find("степень: нет") != std::string::npos);
        }

        /** @brief AddGroup() не добавляет одну группу дважды. */
        TEST_METHOD(AddGroup_SameGroupTwice_ReturnsFalseSecondTime)
        {
            Department department("Физика");
            Specialty specialty("09.02.07", "ИСиТ");
            Group group("1И", &specialty);
            Teacher t("Сидоров С. С.", Date(1985, 1, 1), "ассистент", "", &department);

            Assert::IsTrue(t.AddGroup(&group));
            Assert::IsFalse(t.AddGroup(&group));
            Assert::AreEqual<size_t>(1, t.GetGroups().size());
            Assert::IsTrue(t.TeachesInGroup(&group));
            Assert::IsFalse(t.TeachesInGroup(nullptr));
        }
    };

    // ==== Тесты классов Group и Specialty ====

    TEST_CLASS(GroupSpecialtyTests)
    {
    public:
        /** @brief Группа возвращает номер, специальность и число студентов. */
        TEST_METHOD(Group_ReturnsFields)
        {
            Specialty specialty("09.02.07", "Информационные системы и технологии");
            Group group("1И", &specialty);
            group.AddStudent("Иванов И. И.", Date(2005, 1, 1), "17-001", 1);
            group.AddStudent("Петров П. П.", Date(2005, 1, 1), "17-002", 1);

            Assert::AreEqual(std::string("1И"), group.GetNumber());
            Assert::IsTrue(group.GetSpecialty() == &specialty);
            Assert::AreEqual<size_t>(2, group.GetStudentCount());
            Assert::AreEqual(std::string("1И (09.02.07)"), group.GetShortInfo());
        }

        /** @brief Конструктор группы отклоняет пустой номер и nullptr. */
        TEST_METHOD(Group_InvalidArguments_Throws)
        {
            Specialty specialty("09.02.07", "ИСиТ");
            Assert::ExpectException<std::invalid_argument>([&]() {
                Group g("   ", &specialty);
                (void)g;
                });
            Assert::ExpectException<std::invalid_argument>([]() {
                Group g("1И", nullptr);
                (void)g;
                });
        }

        /** @brief Группа отклоняет дублирующуюся зачётную книжку. */
        TEST_METHOD(AddStudent_DuplicateRecordBook_Throws)
        {
            Specialty specialty("09.02.07", "ИСиТ");
            Group group("1И", &specialty);
            group.AddStudent("Иванов И. И.", Date(2005, 1, 1), "17-001", 1);

            Assert::ExpectException<std::invalid_argument>([&]() {
                group.AddStudent("Петров П. П.", Date(2005, 1, 1), "17-001", 1);
                });
        }

        /** @brief Поиск по зачётной книжке игнорирует регистр и пробелы. */
        TEST_METHOD(FindStudentByRecordBook_IgnoresCaseAndSpaces)
        {
            Specialty specialty("09.02.07", "ИСиТ");
            Group group("1И", &specialty);
            group.AddStudent("Иванов И. И.", Date(2005, 1, 1), "17-001", 1);

            Assert::IsNotNull(group.FindStudentByRecordBook("17-001").get());
            Assert::IsNotNull(group.FindStudentByRecordBook("  17-001  ").get());
            Assert::IsNull(group.FindStudentByRecordBook("99-999").get());
        }

        /** @brief GetAverageMark() группы усредняет баллы её студентов. */
        TEST_METHOD(GroupAverageMark_IsCorrect)
        {
            Specialty specialty("09.02.07", "ИСиТ");
            Group group("1И", &specialty);
            std::shared_ptr<Student> a = group.AddStudent("Иванов И. И.", Date(2005, 1, 1), "17-001", 1);
            std::shared_ptr<Student> b = group.AddStudent("Петров П. П.", Date(2005, 1, 1), "17-002", 1);
            a->AddMark(5.0); // средний 5.0
            a->AddMark(4.0);
            b->AddMark(3.0); // средний 3.0
            // (4.5 + 3.0) / 2 = 3.75
            Assert::AreEqual(3.75, group.GetAverageMark());
        }

        /** @brief Пустая группа имеет средний балл 0.0. */
        TEST_METHOD(EmptyGroup_AverageMarkIsZero)
        {
            Specialty specialty("09.02.07", "ИСиТ");
            Group group("1И", &specialty);
            Assert::AreEqual(0.0, group.GetAverageMark());
        }

        /** @brief Специальность возвращает код, название и число групп. */
        TEST_METHOD(Specialty_ReturnsFields)
        {
            Specialty specialty("09.02.07", "Информационные системы и технологии");
            Group group1("1И", &specialty);
            Group group2("2И", &specialty);

            Assert::AreEqual(std::string("09.02.07"), specialty.GetCode());
            Assert::AreEqual(std::string("Информационные системы и технологии"), specialty.GetName());
            Assert::AreEqual<size_t>(2, specialty.GetGroupCount());
        }

        /** @brief Специальность отклоняет пустые код и название. */
        TEST_METHOD(Specialty_InvalidArguments_Throws)
        {
            Assert::ExpectException<std::invalid_argument>([]() {
                Specialty s("  ", "ИСиТ");
                (void)s;
                });
            Assert::ExpectException<std::invalid_argument>([]() {
                Specialty s("09.02.07", "   ");
                (void)s;
                });
        }

        /**
         * @brief Конструктор Group сам регистрирует группу у специальности,
         *        поэтому повторный явный AddGroup() возвращает false
         *        и не создаёт дубликатов.
         */
        TEST_METHOD(Specialty_AddGroup_SameGroupTwice_ReturnsFalse)
        {
            Specialty specialty("09.02.07", "ИСиТ");

            // Создание группы уже зарегистрировало её у специальности.
            Group group("1И", &specialty);
            Assert::AreEqual<size_t>(1, specialty.GetGroupCount());

            Assert::IsFalse(specialty.AddGroup(&group));
            Assert::IsFalse(specialty.AddGroup(&group));
            Assert::AreEqual<size_t>(1, specialty.GetGroupCount());
        }

        /** @brief Повторная регистрация nullptr отклоняется. */
        TEST_METHOD(Specialty_AddGroup_Nullptr_Throws)
        {
            Specialty specialty("09.02.07", "ИСиТ");
            Assert::ExpectException<std::invalid_argument>([&]() {
                specialty.AddGroup(nullptr);
                });
        }

        /**
         * @brief Группы, созданные напрямую (без University), тоже
         *        попадают в список своей специальности - проверка
         *        инварианта "создание группы регистрирует её".
         */
        TEST_METHOD(Specialty_Constructor_RegistersGroupAutomatically)
        {
            Specialty specialty("09.02.07", "ИСиТ");
            Group group1("1И", &specialty);
            Group group2("2И", &specialty);

            Assert::AreEqual<size_t>(2, specialty.GetGroupCount());
            Assert::IsTrue(specialty.GetGroups()[0] == &group1);
            Assert::IsTrue(specialty.GetGroups()[1] == &group2);
        }
    };

    // ==== Тесты классов Department и Discipline ====

    TEST_CLASS(DepartmentTests)
    {
    public:
        /** @brief Кафедра возвращает название, число дисциплин и преподавателей. */
        TEST_METHOD(Department_ReturnsFields)
        {
            Department department("Прикладная математика");
            department.AddDiscipline("Дискретная математика");
            department.AddDiscipline("Математический анализ");

            Assert::AreEqual(std::string("Прикладная математика"), department.GetName());
            Assert::AreEqual<size_t>(2, department.GetDisciplineCount());
            Assert::AreEqual<size_t>(0, department.GetTeacherCount());
        }

        /** @brief Конструктор кафедры отклоняет пустое название. */
        TEST_METHOD(Department_EmptyName_Throws)
        {
            Assert::ExpectException<std::invalid_argument>([]() {
                Department d("   ");
                (void)d;
                });
        }

        /** @brief Кафедра отклоняет пустую и дублирующуюся дисциплину. */
        TEST_METHOD(AddDiscipline_EmptyOrDuplicate_Throws)
        {
            Department department("Физика");
            department.AddDiscipline("Общая физика");

            Assert::ExpectException<std::invalid_argument>([&]() {
                department.AddDiscipline("   ");
                });
            Assert::ExpectException<std::invalid_argument>([&]() {
                department.AddDiscipline("общая физика"); // тот же, другой регистр
                });
        }

        /** @brief Поиск дисциплины по названию игнорирует регистр. */
        TEST_METHOD(FindDisciplineByName_IgnoresCaseAndSpaces)
        {
            Department department("Физика");
            std::shared_ptr<Discipline> discipline = department.AddDiscipline("Общая физика");

            Assert::IsTrue(department.FindDisciplineByName("общая физика") == discipline);
            Assert::IsTrue(department.FindDisciplineByName("  ОБЩАЯ ФИЗИКА  ") == discipline);
            Assert::IsNull(department.FindDisciplineByName("Математика").get());
        }

        /** @brief Дисциплина знает свою кафедру. */
        TEST_METHOD(Discipline_BelongsToDepartment)
        {
            Department department("Компьютерные науки");
            std::shared_ptr<Discipline> discipline = department.AddDiscipline("Программирование");

            Assert::IsTrue(discipline->GetDepartment() == &department);
            Assert::IsTrue(discipline->IsReadByOwnDepartment());
            Assert::AreEqual(std::string("Программирование"), discipline->GetName());
        }

        /** @brief Конструктор дисциплины отклоняет пустое название и nullptr. */
        TEST_METHOD(Discipline_InvalidArguments_Throws)
        {
            Department department("Физика");
            Assert::ExpectException<std::invalid_argument>([&]() {
                Discipline d("   ", &department);
                (void)d;
                });
            Assert::ExpectException<std::invalid_argument>([]() {
                Discipline d("Физика", nullptr);
                (void)d;
                });
        }
    };

    // ==== Тесты класса Lesson ====

    TEST_CLASS(LessonTests)
    {
    private:
        // Общая подготовка данных для тестов занятий.
        struct Fixture {
            University university;
            std::shared_ptr<Specialty> specialty;
            std::shared_ptr<Department> department;
            std::shared_ptr<Discipline> discipline;
            std::shared_ptr<Group> group;
            std::shared_ptr<Teacher> first;
            std::shared_ptr<Teacher> second;

            Fixture() {
                specialty = university.AddSpecialty("09.02.07", "ИСиТ");
                department = university.AddDepartment("Прикладная математика");
                discipline = department->AddDiscipline("Дискретная математика");
                group = university.AddGroup("1И", specialty.get());
                first = university.AddTeacher("Иванов И. И.", Date(1972, 1, 1),
                    "профессор", "", department.get());
                second = university.AddTeacher("Петров П. П.", Date(1980, 1, 1),
                    "доцент", "", department.get());
            }
        };

    public:
        /** @brief Лекция создаётся с одним преподавателем. */
        TEST_METHOD(Lecture_HasSingleTeacher)
        {
            Fixture f;
            std::shared_ptr<Lesson> lesson = f.university.AddLesson(LessonType::Lecture,
                f.discipline.get(), f.group.get(), f.first.get(), nullptr, "ауд. 1", Date(2026, 9, 1));

            Assert::AreEqual<size_t>(1, lesson->GetTeacherCount());
            Assert::IsNull(lesson->GetSecondTeacher());
            Assert::IsTrue(lesson->GetTeacher() == f.first.get());
        }

        /** @brief Лабораторная работа требует двух преподавателей. */
        TEST_METHOD(Lab_RequiresTwoTeachers)
        {
            Fixture f;
            // Без второго преподавателя - исключение.
            Assert::ExpectException<std::invalid_argument>([&]() {
                f.university.AddLesson(LessonType::Lab, f.discipline.get(), f.group.get(),
                    f.first.get(), nullptr, "ауд. 1", Date(2026, 9, 1));
                });

            std::shared_ptr<Lesson> lesson = f.university.AddLesson(LessonType::Lab,
                f.discipline.get(), f.group.get(), f.first.get(), f.second.get(), "ауд. 1", Date(2026, 9, 1));

            Assert::AreEqual<size_t>(2, lesson->GetTeacherCount());
            Assert::IsTrue(lesson->GetSecondTeacher() == f.second.get());
        }

        /** @brief Второй преподаватель допустим только на лабораторной. */
        TEST_METHOD(NonLab_RejectsSecondTeacher)
        {
            Fixture f;
            Assert::ExpectException<std::invalid_argument>([&]() {
                f.university.AddLesson(LessonType::Lecture, f.discipline.get(), f.group.get(),
                    f.first.get(), f.second.get(), "ауд. 1", Date(2026, 9, 1));
                });
        }

        /** @brief Второй преподаватель должен отличаться от основного. */
        TEST_METHOD(SameTeacherTwice_Throws)
        {
            Fixture f;
            Assert::ExpectException<std::invalid_argument>([&]() {
                f.university.AddLesson(LessonType::Lab, f.discipline.get(), f.group.get(),
                    f.first.get(), f.first.get(), "ауд. 1", Date(2026, 9, 1));
                });
        }

        /** @brief Занятие без дисциплины, группы или преподавателя недопустимо. */
        TEST_METHOD(MissingRequiredArgument_Throws)
        {
            Fixture f;
            Assert::ExpectException<std::invalid_argument>([&]() {
                f.university.AddLesson(LessonType::Lecture, nullptr, f.group.get(),
                    f.first.get(), nullptr, "ауд. 1", Date(2026, 9, 1));
                });
            Assert::ExpectException<std::invalid_argument>([&]() {
                f.university.AddLesson(LessonType::Lecture, f.discipline.get(), nullptr,
                    f.first.get(), nullptr, "ауд. 1", Date(2026, 9, 1));
                });
            Assert::ExpectException<std::invalid_argument>([&]() {
                f.university.AddLesson(LessonType::Lecture, f.discipline.get(), f.group.get(),
                    nullptr, nullptr, "ауд. 1", Date(2026, 9, 1));
                });
        }

        /** @brief IsTaughtBy() узнаёт обоих преподавателей занятия. */
        TEST_METHOD(IsTaughtBy_RecognizesBothTeachers)
        {
            Fixture f;
            std::shared_ptr<Lesson> lesson = f.university.AddLesson(LessonType::Lab,
                f.discipline.get(), f.group.get(), f.first.get(), f.second.get(), "ауд. 1", Date(2026, 9, 1));

            Assert::IsTrue(lesson->IsTaughtBy(f.first.get()));
            Assert::IsTrue(lesson->IsTaughtBy(f.second.get()));
            Assert::IsFalse(lesson->IsTaughtBy(nullptr));
        }

        /** @brief ToString() для LessonType даёт русские названия. */
        TEST_METHOD(LessonType_ToString_ReturnsRussianNames)
        {
            Assert::AreEqual(std::string("Лекция"), std::string(ToString(LessonType::Lecture)));
            Assert::AreEqual(std::string("Практика"), std::string(ToString(LessonType::Practical)));
            Assert::AreEqual(std::string("Лабораторная работа"), std::string(ToString(LessonType::Lab)));
        }
    };

    // ==== Тесты класса University и четырёх запросов варианта ====

    TEST_CLASS(UniversityTests)
    {
    private:
        // Полностью заполненный ВУЗ для проверки запросов.
        struct Fixture {
            University university;
            std::shared_ptr<Specialty> specialtyInfo;
            std::shared_ptr<Specialty> specialtyEcon;
            std::shared_ptr<Department> appliedMath;
            std::shared_ptr<Department> computerScience;
            std::shared_ptr<Discipline> discreteMath;
            std::shared_ptr<Discipline> programming;
            std::shared_ptr<Group> group1I;
            std::shared_ptr<Group> group2I;
            std::shared_ptr<Group> group1E;
            std::shared_ptr<Teacher> ivanov;
            std::shared_ptr<Teacher> petrov;
            std::shared_ptr<Teacher> kuznetsova;

            Fixture() {
                university.SetName("Тестовый университет");

                specialtyInfo = university.AddSpecialty("09.02.07",
                    "Информационные системы и технологии");
                specialtyEcon = university.AddSpecialty("38.03.01", "Экономика");

                appliedMath = university.AddDepartment("Прикладная математика");
                computerScience = university.AddDepartment("Компьютерные науки");

                discreteMath = appliedMath->AddDiscipline("Дискретная математика");
                appliedMath->AddDiscipline("Математический анализ");
                programming = computerScience->AddDiscipline("Программирование");

                group1I = university.AddGroup("1И", specialtyInfo.get());
                group2I = university.AddGroup("2И", specialtyInfo.get());
                group1E = university.AddGroup("1Э", specialtyEcon.get());

                ivanov = university.AddTeacher("Иванов Иван Иванович", Date(1972, 3, 14),
                    "профессор", "доктор наук", appliedMath.get());
                petrov = university.AddTeacher("Петров Пётр Петрович", Date(1980, 7, 2),
                    "доцент", "", appliedMath.get());
                kuznetsova = university.AddTeacher("Кузнецова Кузьма Петровна", Date(1978, 5, 19),
                    "доцент", "канд. тех. наук", computerScience.get());

                group1I->AddStudent("Смирнова Анна Сергеевна", Date(2005, 1, 1), "17-001", 2);
                group1I->AddStudent("Волков Дмитрий Олегович", Date(2005, 2, 2), "17-002", 2);
                group2I->AddStudent("Лебедев Сергей Павлович", Date(2004, 3, 3), "18-003", 3);
                group1E->AddStudent("Новикова Ирина Андреевна", Date(2003, 4, 4), "16-004", 4);

                university.AddLesson(LessonType::Lecture, discreteMath.get(), group1I.get(),
                    ivanov.get(), nullptr, "ауд. 1", Date(2026, 9, 1));
                university.AddLesson(LessonType::Lab, discreteMath.get(), group1I.get(),
                    ivanov.get(), petrov.get(), "ауд. 2", Date(2026, 9, 2));
                university.AddLesson(LessonType::Lab, programming.get(), group1I.get(),
                    kuznetsova.get(), petrov.get(), "ауд. 3", Date(2026, 9, 3));
                university.AddLesson(LessonType::Lecture, programming.get(), group2I.get(),
                    kuznetsova.get(), nullptr, "ауд. 4", Date(2026, 9, 1));
            }
        };

    public:
        // ---- Общее ----

        /** @brief Пустой ВУЗ согласован: все коллекции пусты, поиск даёт nullptr. */
        TEST_METHOD(EmptyUniversity_IsConsistent)
        {
            University empty;
            Assert::AreEqual<size_t>(0, empty.GetGroups().size());
            Assert::AreEqual<size_t>(0, empty.GetAllPeople().size());
            Assert::AreEqual<size_t>(0, empty.GetAllStudents().size());
            Assert::IsNull(empty.FindStudentByRecordBook("любой").get());
            Assert::IsNull(empty.FindDepartmentByName("любая").get());
            Assert::IsNull(empty.FindSpecialtyByCode("любой").get());
            Assert::IsNull(empty.FindGroupByNumber("любая").get());
        }

        /** @brief SetName() отклоняет пустое название. */
        TEST_METHOD(SetName_Empty_Throws)
        {
            University u;
            Assert::ExpectException<std::invalid_argument>([&]() { u.SetName("   "); });
        }

        /** @brief Дубликаты (специальность, кафедра, группа) отклоняются. */
        TEST_METHOD(DuplicateObjects_Throw)
        {
            Fixture f;
            Assert::ExpectException<std::invalid_argument>([&]() {
                f.university.AddSpecialty("09.02.07", "Дубликат");
                });
            Assert::ExpectException<std::invalid_argument>([&]() {
                f.university.AddDepartment("прикладная математика");
                });
            Assert::ExpectException<std::invalid_argument>([&]() {
                f.university.AddGroup("1И", f.specialtyInfo.get());
                });
        }

        /** @brief AddGroup/AddTeacher отклоняют nullptr. */
        TEST_METHOD(NullArguments_Throw)
        {
            Fixture f;
            Assert::ExpectException<std::invalid_argument>([&]() {
                f.university.AddGroup("9И", nullptr);
                });
            Assert::ExpectException<std::invalid_argument>([&]() {
                f.university.AddTeacher("Иванов И. И.", Date(1970, 1, 1), "доцент", "", nullptr);
                });
        }

        // ---- Запрос 1: информация о студенте по № зачётной книжки и по ФИО ----

        /** @brief Поиск студента по номеру зачётной книжки. */
        TEST_METHOD(Query1_FindStudentByRecordBook)
        {
            Fixture f;
            std::shared_ptr<Student> student = f.university.FindStudentByRecordBook("17-002");
            Assert::IsNotNull(student.get());
            Assert::AreEqual(std::string("Волков Дмитрий Олегович"), student->GetFullName());
            Assert::AreEqual(std::string("1И"), student->GetGroup()->GetNumber());

            // Регистронезависимо и с пробелами.
            Assert::IsNotNull(f.university.FindStudentByRecordBook("  17-002  ").get());
            // Несуществующий номер.
            Assert::IsNull(f.university.FindStudentByRecordBook("99-999").get());
        }

        /** @brief Поиск студентов по ФИО (по вхождению подстроки). */
        TEST_METHOD(Query1_FindStudentsByFullName)
        {
            Fixture f;
            Assert::AreEqual<size_t>(1, f.university.FindStudentsByFullName("Смирнова").size());
            Assert::AreEqual<size_t>(1, f.university.FindStudentsByFullName("СМИРНОВА").size());
            Assert::AreEqual<size_t>(1, f.university.FindStudentsByFullName("Анна Сергеевна").size());
            Assert::AreEqual<size_t>(0, f.university.FindStudentsByFullName("Нет Такого").size());
            Assert::AreEqual<size_t>(0, f.university.FindStudentsByFullName("").size());
        }

        /** @brief Одинаковые ФИО в разных группах находятся оба. */
        TEST_METHOD(Query1_SameFullNameInDifferentGroups_FindsBoth)
        {
            Fixture f;
            f.group1E->AddStudent("Волков Дмитрий Олегович", Date(2003, 1, 1), "16-005", 4);
            const std::vector<std::shared_ptr<Student>> found =
                f.university.FindStudentsByFullName("Волков Дмитрий Олегович");
            Assert::AreEqual<size_t>(2, found.size());
        }

        // ---- Запрос 2: список предметов, читаемых кафедрой ----

        /** @brief Список дисциплин кафедры. */
        TEST_METHOD(Query2_GetDisciplinesByDepartment)
        {
            Fixture f;
            const std::vector<std::shared_ptr<Discipline>> applied =
                f.university.GetDisciplinesByDepartment(f.appliedMath.get());
            Assert::AreEqual<size_t>(2, applied.size());

            const std::vector<std::shared_ptr<Discipline>> cs =
                f.university.GetDisciplinesByDepartment(f.computerScience.get());
            Assert::AreEqual<size_t>(1, cs.size());
            Assert::AreEqual(std::string("Программирование"), cs[0]->GetName());

            // Неизвестная кафедра и nullptr дают пустой список.
            Assert::AreEqual<size_t>(0, f.university.GetDisciplinesByDepartment(nullptr).size());
        }

        /** @brief Возвращаются только дисциплины своей кафедры. */
        TEST_METHOD(Query2_ReturnsOnlyOwnDisciplines)
        {
            Fixture f;
            for (const std::shared_ptr<Discipline>& discipline :
                f.university.GetDisciplinesByDepartment(f.appliedMath.get())) {
                Assert::IsTrue(discipline->GetDepartment() == f.appliedMath.get());
            }
        }

        // ---- Запрос 3: список преподавателей, ведущих занятия в группе ----

        /** @brief Список преподавателей группы включает обоих ведущих лабораторной. */
        TEST_METHOD(Query3_GetTeachersByGroup_IncludesLabCoTeachers)
        {
            Fixture f;
            // 1И: лекция (Иванов), лаб. (Иванов + Петров), лаб. (Кузнецова + Петров).
            const std::vector<std::shared_ptr<Teacher>> teachers =
                f.university.GetTeachersByGroup(f.group1I.get());

            Assert::AreEqual<size_t>(3, teachers.size()); // без повторов

            const std::vector<std::string> names = {
                teachers[0]->GetFullName(), teachers[1]->GetFullName(), teachers[2]->GetFullName() };
            Assert::IsTrue(std::find(names.begin(), names.end(),
                std::string("Иванов Иван Иванович")) != names.end());
            Assert::IsTrue(std::find(names.begin(), names.end(),
                std::string("Петров Пётр Петрович")) != names.end());
            Assert::IsTrue(std::find(names.begin(), names.end(),
                std::string("Кузнецова Кузьма Петровна")) != names.end());
        }

        /** @brief В группе без занятий список преподавателей пуст. */
        TEST_METHOD(Query3_GroupWithoutLessons_ReturnsEmpty)
        {
            Fixture f;
            std::shared_ptr<Group> empty = f.university.AddGroup("1П", f.specialtyInfo.get());
            Assert::AreEqual<size_t>(0, f.university.GetTeachersByGroup(empty.get()).size());
            Assert::AreEqual<size_t>(0, f.university.GetTeachersByGroup(nullptr).size());
        }

        /** @brief Новые занятия отражаются в списке преподавателей группы. */
        TEST_METHOD(Query3_ReflectsNewlyAddedLessons)
        {
            Fixture f;
            std::shared_ptr<Group> group = f.university.AddGroup("1Л", f.specialtyEcon.get());
            Assert::AreEqual<size_t>(0, f.university.GetTeachersByGroup(group.get()).size());

            f.university.AddLesson(LessonType::Lab, f.programming.get(), group.get(),
                f.kuznetsova.get(), f.ivanov.get(), "ауд. 5", Date(2026, 9, 5));

            Assert::AreEqual<size_t>(2, f.university.GetTeachersByGroup(group.get()).size());
        }

        // ---- Запрос 4: список групп на специальности ----

        /** @brief Список групп по специальности. */
        TEST_METHOD(Query4_GetGroupsBySpecialty)
        {
            Fixture f;
            const std::vector<std::shared_ptr<Group>> infoGroups =
                f.university.GetGroupsBySpecialty(f.specialtyInfo.get());
            Assert::AreEqual<size_t>(2, infoGroups.size());

            const std::vector<std::shared_ptr<Group>> econGroups =
                f.university.GetGroupsBySpecialty(f.specialtyEcon.get());
            Assert::AreEqual<size_t>(1, econGroups.size());
            Assert::AreEqual(std::string("1Э"), econGroups[0]->GetNumber());

            Assert::AreEqual<size_t>(0, f.university.GetGroupsBySpecialty(nullptr).size());
        }

        /** @brief Новая группа появляется в списке своей специальности. */
        TEST_METHOD(Query4_NewGroup_AppearsInSpecialty)
        {
            Fixture f;
            f.university.AddGroup("3И", f.specialtyInfo.get());
            Assert::AreEqual<size_t>(3, f.university.GetGroupsBySpecialty(f.specialtyInfo.get()).size());
            Assert::AreEqual<size_t>(3, f.specialtyInfo->GetGroupCount());
        }

        // ---- Дополнительные ----

        /** @brief GetAllPeople() возвращает людей как БАЗОВЫЙ тип Person. */
        TEST_METHOD(GetAllPeople_ReturnsBaseTypePointers)
        {
            Fixture f;
            // 4 студента + 3 преподавателя.
            const std::vector<std::shared_ptr<Person>> people = f.university.GetAllPeople();
            Assert::AreEqual<size_t>(7, people.size());

            size_t students = 0;
            size_t teachers = 0;
            for (const std::shared_ptr<Person>& person : people) {
                if (person->IsStudent()) {
                    ++students;
                }
                if (person->IsTeacher()) {
                    ++teachers;
                }
                // Виртуальный вызов через базовый тип.
                Assert::IsFalse(person->GetInfo().empty());
            }
            Assert::AreEqual<size_t>(4, students);
            Assert::AreEqual<size_t>(3, teachers);
        }

        /** @brief GetLessonsByGroup() возвращает занятия конкретной группы. */
        TEST_METHOD(GetLessonsByGroup_ReturnsGroupLessons)
        {
            Fixture f;
            Assert::AreEqual<size_t>(3, f.university.GetLessonsByGroup(f.group1I.get()).size());
            Assert::AreEqual<size_t>(1, f.university.GetLessonsByGroup(f.group2I.get()).size());
            Assert::AreEqual<size_t>(0, f.university.GetLessonsByGroup(nullptr).size());
        }

        /** @brief GetStatistics() содержит все счётчики. */
        TEST_METHOD(GetStatistics_ContainsCounters)
        {
            Fixture f;
            const std::string stats = f.university.GetStatistics();
            Assert::IsTrue(stats.find("Тестовый университет") != std::string::npos);
            Assert::IsTrue(stats.find("специальностей: 2") != std::string::npos);
            Assert::IsTrue(stats.find("кафедр: 2") != std::string::npos);
            Assert::IsTrue(stats.find("групп: 3") != std::string::npos);
            Assert::IsTrue(stats.find("студентов: 4") != std::string::npos);
            Assert::IsTrue(stats.find("преподавателей: 3") != std::string::npos);
            Assert::IsTrue(stats.find("занятий: 4") != std::string::npos);
        }
    };

    // ==== Тесты вспомогательных функций ====

    TEST_CLASS(FormatUtilsTests)
    {
    public:
        /** @brief FormatDecimal() печатает два знака после точки. */
        TEST_METHOD(FormatDecimal_TwoFractionDigits)
        {
            Assert::AreEqual(std::string("123.50"), FormatUtils::FormatDecimal(123.5));
            Assert::AreEqual(std::string("4.00"), FormatUtils::FormatDecimal(4.0));
            Assert::AreEqual(std::string("3.75"), FormatUtils::FormatDecimal(3.75));
        }

        /** @brief EqualsIgnoreCase() работает с кириллицей. */
        TEST_METHOD(EqualsIgnoreCase_HandlesCyrillic)
        {
            Assert::IsTrue(FormatUtils::EqualsIgnoreCase("Физика", "физика"));
            Assert::IsTrue(FormatUtils::EqualsIgnoreCase("ЁЖИК", "ёжик"));
            Assert::IsFalse(FormatUtils::EqualsIgnoreCase("Физика", "Физика1"));
        }

        /** @brief ContainsIgnoreCase() ищет подстроку без учёта регистра. */
        TEST_METHOD(ContainsIgnoreCase_FindsSubstring)
        {
            Assert::IsTrue(FormatUtils::ContainsIgnoreCase("Смирнова Анна", "смирнова"));
            Assert::IsTrue(FormatUtils::ContainsIgnoreCase("Смирнова Анна", "АННА"));
            Assert::IsTrue(FormatUtils::ContainsIgnoreCase("Смирнова Анна", ""));
            Assert::IsFalse(FormatUtils::ContainsIgnoreCase("Смирнова Анна", "Петров"));
        }

        /** @brief ToInitials() сокращает имя до "Фамилия И. О.". */
        TEST_METHOD(ToInitials_CreatesInitials)
        {
            Assert::AreEqual(std::string("Иванов И. И."), FormatUtils::ToInitials("Иванов Иван Иванович"));
            Assert::AreEqual(std::string("Петров П."), FormatUtils::ToInitials("Петров Пётр"));
            Assert::AreEqual(std::string("Сидоров"), FormatUtils::ToInitials("  Сидоров  "));
            Assert::AreEqual(std::string(""), FormatUtils::ToInitials("   "));
        }

        /** @brief Trim() убирает крайние пробелы. */
        TEST_METHOD(Trim_RemovesEdgeSpaces)
        {
            Assert::AreEqual(std::string("текст"), FormatUtils::Trim("   текст  "));
            Assert::AreEqual(std::string(""), FormatUtils::Trim("   "));
        }

        /** @brief Join() склеивает элементы через разделитель. */
        TEST_METHOD(Join_ConcatenatesItems)
        {
            const std::vector<std::string> items = { "Физика", "Математика" };
            Assert::AreEqual(std::string("Физика, Математика"), FormatUtils::Join(items, ", "));
            Assert::AreEqual(std::string(""), FormatUtils::Join({}, ", "));
        }
    };

} // namespace VuzTests
