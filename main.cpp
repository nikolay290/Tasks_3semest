//
// main.cpp
// Демонстрационная консольная программа для варианта задания "ВУЗ".
//
// Задание 1 (общее, единое для всех вариантов): создать коллекцию
// объектов базового типа, заполнить её объектами типов-наследников,
// проитерировать её как коллекцию базового класса и вывести информацию
// о каждом объекте в std::cout.
//
// Задания варианта "ВУЗ":
//   1) Выдавать информацию о студенте по № зачетной книжки, по ФИО.
//   2) Выдавать список предметов, читаемых данной кафедрой.
//   3) Выдавать список преподавателей, проводящих занятие в данной группе.
//   4) Выдавать список групп, обучающихся на данной специальности.
//
// Структура файла: сначала объявления (прототипы) всех вспомогательных
// функций с их документацией, затем main(), и уже после main() -
// реализации (тела) этих функций в том же порядке.
//
// ВАЖНО: именно здесь, а не в библиотеке, выполняется весь ввод-вывод
// через std::cout / std::cin. Библиотека классов не зависит от потоков.
//
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#endif

#include "VuzLib.h"

namespace {

    /**
     * @brief Пункты главного меню. Значения соответствуют тому, что
     *        пользователь вводит с клавиатуры. PrintMenu() выводит эти
     *        же числа на экран через приведение значений enum к int,
     *        а не отдельными захардкоженными цифрами в строке - так
     *        текст меню и код обработки выбора не могут разойтись.
     */
    enum class MainMenuChoice {
        Exit = 0,                        ///< "0 - Выход"
        ShowAllPeople = 1,               ///< "1 - Все люди ВУЗа (полиморфный обход)"
        StudentInfo = 2,                 ///< "2 - Информация о студенте" (задание 1)
        DepartmentDisciplines = 3,       ///< "3 - Предметы кафедры" (задание 2)
        GroupTeachers = 4,               ///< "4 - Преподаватели группы" (задание 3)
        SpecialtyGroups = 5,             ///< "5 - Группы специальности" (задание 4)
        ShowAllLessons = 6,              ///< "6 - Расписание занятий"
    };

#ifdef _WIN32
    /**
     * @brief Переключает кодовые страницы ввода/вывода консоли Windows
     *        в UTF-8. На платформах, отличных от Windows, функция не вызывается.
     */
    void EnableUtf8Console();
#endif

    /** @brief Печатает горизонтальную разделительную линию в std::cout. */
    void PrintDivider();

    /**
     * @brief Возвращает фиксированную "текущую" дату демо-программы.
     *        Захардкожена (а не взята из системных часов), чтобы вывод
     *        был воспроизводим при каждом запуске.
     * @return Опорная дата, используемая во всей демонстрации.
     */
    Date Today();

    /**
     * @brief Формирует демонстрационный ВУЗ: специальности, кафедры,
     *        дисциплины, группы, студентов, преподавателей и занятия.
     * @return Экземпляр University, готовый к использованию в демо-меню.
     */
    University CreateDemoUniversity();

    /**
     * @brief Формирует демонстрационную коллекцию объектов БАЗОВОГО типа
     *        Person, заполняя её объектами типов-наследников Student и
     *        Teacher. Это общее требование задания 1.
     * @param university ВУЗ-источник данных.
     * @return Вектор умных указателей на Person (студенты + преподаватели).
     */
    std::vector<std::shared_ptr<Person>> CreateDemoPeople(const University& university);

    // ---- Задание 1 (общее): полиморфный обход коллекции базового типа ----

    /**
     * @brief Проходит коллекцию людей через указатели БАЗОВОГО класса
     *        (Person) и печатает информацию о каждом элементе в std::cout.
     *        GetRole()/GetInfo()/IsStudent() виртуальные, поэтому
     *        вызывается реализация конкретного класса-наследника, хотя
     *        статический тип каждого элемента - "указатель на Person":
     *        это и есть требование задания 1 о полиморфном обходе.
     * @param people Коллекция для вывода.
     */
    void PrintAllPeoplePolymorphically(const std::vector<std::shared_ptr<Person>>& people);

    // ---- Задание 2: информация о студенте по № зачётной книжки и по ФИО ----

    /**
     * @brief Обработчик пункта меню "Информация о студенте": считывает
     *        с консоли номер зачётной книжки, печатает найденного
     *        студента, затем предлагает найти студентов по ФИО.
     * @param university ВУЗ, к которому выполняется запрос.
     */
    void ShowStudentInfo(const University& university);

    // ---- Задание 3: список предметов кафедры ----

    /**
     * @brief Обработчик пункта меню "Предметы кафедры": считывает
     *        название кафедры и печатает читаемые ею дисциплины.
     * @param university ВУЗ, к которому выполняется запрос.
     */
    void ShowDepartmentDisciplines(const University& university);

    // ---- Задание 4: список преподавателей группы ----

    /**
     * @brief Обработчик пункта меню "Преподаватели группы": считывает
     *        номер группы и печатает ведущих в ней преподавателей.
     * @param university ВУЗ, к которому выполняется запрос.
     */
    void ShowGroupTeachers(const University& university);

    // ---- Задание 5: список групп специальности ----

    /**
     * @brief Обработчик пункта меню "Группы специальности": считывает
     *        код специальности и печатает обучающиеся на ней группы.
     * @param university ВУЗ, к которому выполняется запрос.
     */
    void ShowSpecialtyGroups(const University& university);

    /**
     * @brief Печатает полное расписание занятий ВУЗа.
     * @param university ВУЗ, расписание которого выводится.
     */
    void ShowAllLessons(const University& university);

    /** @brief Печатает в std::cout пункты главного меню. */
    void PrintMenu();

} // namespace

/**
 * @brief Точка входа программы. Формирует демонстрационный ВУЗ и
 *        полиморфную коллекцию людей, печатает полиморфный обход
 *        коллекции, требуемый заданием 1, затем запускает интерактивное
 *        текстовое меню, реализующее четыре задания варианта "ВУЗ".
 * @return 0 при нормальном завершении.
 */
int main() {
#ifdef _WIN32
    EnableUtf8Console();
#endif

    const University university = CreateDemoUniversity();
    const std::vector<std::shared_ptr<Person>> people = CreateDemoPeople(university);

    std::cout << "=== " << university.GetName() << " ===\n";
    std::cout << university.GetStatistics() << "\n";

    // Сразу показываем полиморфный обход коллекции (общее требование
    // задания 1 - "коллекция базового типа, заполненная наследниками").
    PrintAllPeoplePolymorphically(people);

    bool running = true;
    while (running) {
        PrintMenu();
        int choice = -1;
        if (!(std::cin >> choice)) {
            break; // конец ввода (например, при перенаправлении из файла)
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (static_cast<MainMenuChoice>(choice)) {
        case MainMenuChoice::ShowAllPeople:
            PrintAllPeoplePolymorphically(people);
            break;
        case MainMenuChoice::StudentInfo:
            ShowStudentInfo(university);
            break;
        case MainMenuChoice::DepartmentDisciplines:
            ShowDepartmentDisciplines(university);
            break;
        case MainMenuChoice::GroupTeachers:
            ShowGroupTeachers(university);
            break;
        case MainMenuChoice::SpecialtyGroups:
            ShowSpecialtyGroups(university);
            break;
        case MainMenuChoice::ShowAllLessons:
            ShowAllLessons(university);
            break;
        case MainMenuChoice::Exit:
            running = false;
            break;
        default:
            std::cout << "Неизвестный пункт меню.\n";
            break;
        }
    }

    std::cout << "Работа программы завершена.\n";
    return 0;
}

// ============================================================================
// Реализации функций, объявленных выше (см. их документацию у прототипов).
// ============================================================================

namespace {

#ifdef _WIN32
    void EnableUtf8Console() {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    }
#endif

    void PrintDivider() {
        std::cout << "------------------------------------------------------------\n";
    }

    Date Today() {
        return Date(2026, 9, 29);
    }

    University CreateDemoUniversity() {
        University university;
        university.SetName("Российский  университет транспорта");

        // --- Специальности ---
        std::shared_ptr<Specialty> informatics =
            university.AddSpecialty("09.02.07", "Информационные системы и технологии");
        std::shared_ptr<Specialty> economics =
            university.AddSpecialty("38.03.01", "Экономика");

        // --- Кафедры ---
        std::shared_ptr<Department> appliedMath = university.AddDepartment("Прикладная математика");
        std::shared_ptr<Department> generalPhysics = university.AddDepartment("Общая физика");
        std::shared_ptr<Department> computerScience = university.AddDepartment("Компьютерные науки");

        // --- Дисциплины, которые читают кафедры ---
        std::shared_ptr<Discipline> discreteMath = appliedMath->AddDiscipline("Дискретная математика");
        appliedMath->AddDiscipline("Математический анализ");
        generalPhysics->AddDiscipline("Общая физика");
        std::shared_ptr<Discipline> programming = computerScience->AddDiscipline("Программирование");
        std::shared_ptr<Discipline> databases = computerScience->AddDiscipline("Базы данных");

        // --- Группы ---
        std::shared_ptr<Group> group1I = university.AddGroup("1И", informatics.get());
        std::shared_ptr<Group> group2I = university.AddGroup("2И", informatics.get());
        std::shared_ptr<Group> group1E = university.AddGroup("1Э", economics.get());

        // --- Преподаватели (занятия ведут преподаватели с разных кафедр) ---
        std::shared_ptr<Teacher> ivanov = university.AddTeacher("Иванов Иван Иванович",
            Date(1972, 3, 14), "профессор", "доктор физ.-мат. наук", appliedMath.get());
        std::shared_ptr<Teacher> petrov = university.AddTeacher("Петров Пётр Петрович",
            Date(1980, 7, 2), "доцент", "канд. физ.-мат. наук", appliedMath.get());
        std::shared_ptr<Teacher> sidorov = university.AddTeacher("Сидоров Сидор Сидорович",
            Date(1985, 11, 30), "старший преподаватель", "", generalPhysics.get());
        std::shared_ptr<Teacher> kuznetsova = university.AddTeacher("Кузнецова Кузьма Петровна",
            Date(1978, 5, 19), "доцент", "канд. тех. наук", computerScience.get());

        // --- Студенты ---
        std::shared_ptr<Student> smirnov = group1I->AddStudent("Смирнов Алексей Николаевич",
            Date(2005, 4, 12), "17-142", 2);
        group1I->AddStudent("Волков Дмитрий Олегович", Date(2005, 9, 1), "17-143", 2);
        group1I->AddStudent("Морозова Елена Викторовна", Date(2004, 12, 25), "17-144", 3);
        group2I->AddStudent("Лебедев Сергей Павлович", Date(2004, 6, 7), "18-201", 3);
        group1E->AddStudent("Новикова Ирина Андреевна", Date(2003, 2, 17), "16-011", 4);

        smirnov->AddMark(5.0);
        smirnov->AddMark(4.0);
        group1I->GetStudents()[1]->AddMark(4.0);
        group1I->GetStudents()[1]->AddMark(3.0);
        group1I->GetStudents()[2]->AddMark(5.0);
        group1I->GetStudents()[2]->AddMark(4.0);
        group2I->GetStudents()[0]->AddMark(4.0);
        group1E->GetStudents()[0]->AddMark(5.0);

        // --- Занятия. Лабораторные работы ведут ДВА преподавателя. ---
        university.AddLesson(LessonType::Lecture, discreteMath.get(), group1I.get(),
            ivanov.get(), nullptr, "ауд. 25", Date(2026, 9, 1));
        university.AddLesson(LessonType::Lab, discreteMath.get(), group1I.get(),
            ivanov.get(), petrov.get(), "ауд. 12", Date(2026, 9, 3));
        university.AddLesson(LessonType::Lab, programming.get(), group1I.get(),
            kuznetsova.get(), sidorov.get(), "ауд. 41", Date(2026, 9, 5));
        university.AddLesson(LessonType::Practical, programming.get(), group2I.get(),
            kuznetsova.get(), nullptr, "ауд. 42", Date(2026, 9, 2));
        university.AddLesson(LessonType::Lecture, databases.get(), group2I.get(),
            kuznetsova.get(), nullptr, "ауд. 43", Date(2026, 9, 4));
        university.AddLesson(LessonType::Lecture, programming.get(), group1E.get(),
            sidorov.get(), nullptr, "ауд. 44", Date(2026, 9, 2));

        return university;
    }

    std::vector<std::shared_ptr<Person>> CreateDemoPeople(const University& university) {
        // Требование задания 1: коллекция объектов БАЗОВОГО типа Person,
        // заполняемая объектами типов-наследников (Student, Teacher).
        // GetAllPeople() возвращает shared_ptr<Person>, внутри которых лежат
        // объекты производных классов - именно они и выводятся полиморфно.
        return university.GetAllPeople();
    }

    void PrintAllPeoplePolymorphically(const std::vector<std::shared_ptr<Person>>& people) {
        PrintDivider();
        std::cout << "Задание 1. Полиморфный обход коллекции базового типа Person:\n";
        PrintDivider();

        size_t students = 0;
        size_t teachers = 0;

        for (const std::shared_ptr<Person>& person : people) {
            // Статический тип person - shared_ptr<Person> (базовый класс),
            // но вызовы виртуальные, поэтому для каждого объекта
            // выполняется его собственная реализация.
            std::cout << person->GetInfo() << "\n";

            if (person->IsStudent()) {
                ++students;
            }
            else if (person->IsTeacher()) {
                ++teachers;
            }
        }

        std::cout << "---\n";
        std::cout << "Всего в коллекции: " << people.size()
            << " (студентов: " << students << ", преподавателей: " << teachers << ")\n";
    }

    void ShowStudentInfo(const University& university) {
        // -- Информация по номеру зачётной книжки --
        std::cout << "Введите номер зачетной книжки: ";
        std::string recordBook;
        std::getline(std::cin, recordBook);

        std::shared_ptr<Student> byRecordBook = university.FindStudentByRecordBook(recordBook);
        if (byRecordBook == nullptr) {
            std::cout << "Студент с зачетной книжкой \"" << recordBook << "\" не найден.\n";
        }
        else {
            std::cout << "Найден студент:\n  " << byRecordBook->GetInfo() << "\n";
        }

        // -- Информация по ФИО --
        std::cout << "\nВведите ФИО (можно часть, например \"Волков\"): ";
        std::string fullName;
        std::getline(std::cin, fullName);

        const std::vector<std::shared_ptr<Student>> found = university.FindStudentsByFullName(fullName);
        if (found.empty()) {
            std::cout << "Студенты с ФИО, содержащим \"" << fullName << "\", не найдены.\n";
        }
        else {
            std::cout << "Найдено студентов: " << found.size() << "\n";
            for (const std::shared_ptr<Student>& student : found) {
                std::cout << "  " << student->GetInfo() << "\n";
            }
        }
    }

    void ShowDepartmentDisciplines(const University& university) {
        std::cout << "Введите название кафедры: ";
        std::string name;
        std::getline(std::cin, name);

        std::shared_ptr<Department> department = university.FindDepartmentByName(name);
        if (department == nullptr) {
            std::cout << "Кафедра \"" << name << "\" не найдена.\n";
            return;
        }

        const std::vector<std::shared_ptr<Discipline>> disciplines =
            university.GetDisciplinesByDepartment(department.get());

        std::cout << "Предметы, читаемые кафедрой \"" << department->GetName() << "\":\n";
        if (disciplines.empty()) {
            std::cout << "  (список пуст)\n";
        }
        else {
            for (const std::shared_ptr<Discipline>& discipline : disciplines) {
                std::cout << "  - " << discipline->GetInfo() << "\n";
            }
        }
    }

    void ShowGroupTeachers(const University& university) {
        std::cout << "Введите номер группы: ";
        std::string number;
        std::getline(std::cin, number);

        std::shared_ptr<Group> group = university.FindGroupByNumber(number);
        if (group == nullptr) {
            std::cout << "Группа \"" << number << "\" не найдена.\n";
            return;
        }

        const std::vector<std::shared_ptr<Teacher>> teachers = university.GetTeachersByGroup(group.get());
        std::cout << "Преподаватели, проводящие занятия в группе " << group->GetNumber() << ":\n";
        if (teachers.empty()) {
            std::cout << "  (список пуст)\n";
        }
        else {
            for (const std::shared_ptr<Teacher>& teacher : teachers) {
                std::cout << "  - " << teacher->GetInfo() << "\n";
            }
        }
    }

    void ShowSpecialtyGroups(const University& university) {
        std::cout << "Введите код специальности: ";
        std::string code;
        std::getline(std::cin, code);

        std::shared_ptr<Specialty> specialty = university.FindSpecialtyByCode(code);
        if (specialty == nullptr) {
            std::cout << "Специальность с кодом \"" << code << "\" не найдена.\n";
            return;
        }

        const std::vector<std::shared_ptr<Group>> groups = university.GetGroupsBySpecialty(specialty.get());
        std::cout << "Группы, обучающиеся на специальности \"" << specialty->GetName() << "\":\n";
        if (groups.empty()) {
            std::cout << "  (список пуст)\n";
        }
        else {
            for (const std::shared_ptr<Group>& group : groups) {
                std::cout << "  - " << group->GetInfo() << "\n";
            }
        }
    }

    void ShowAllLessons(const University& university) {
        std::cout << "Расписание занятий (на " << Today().ToString() << "):\n";
        for (const std::shared_ptr<Lesson>& lesson : university.GetLessons()) {
            std::cout << "  - " << lesson->GetInfo() << "\n";
        }
    }

    void PrintMenu() {
        PrintDivider();
        std::cout << static_cast<int>(MainMenuChoice::ShowAllPeople)
            << " - Все люди ВУЗа (полиморфный обход коллекции)\n";
        std::cout << static_cast<int>(MainMenuChoice::StudentInfo)
            << " - Информация о студенте (Задание 1)\n";
        std::cout << static_cast<int>(MainMenuChoice::DepartmentDisciplines)
            << " - Список предметов кафедры (Задание 2)\n";
        std::cout << static_cast<int>(MainMenuChoice::GroupTeachers)
            << " - Список преподавателей группы (Задание 3)\n";
        std::cout << static_cast<int>(MainMenuChoice::SpecialtyGroups)
            << " - Список групп специальности (Задание 4)\n";
        std::cout << static_cast<int>(MainMenuChoice::ShowAllLessons)
            << " - Расписание занятий\n";
        std::cout << static_cast<int>(MainMenuChoice::Exit) << " - Выход\n";
        std::cout << "Ваш выбор: ";
    }

} // namespace
