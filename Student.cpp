#include "Student.h"

#include <stdexcept>

#include "FormatUtils.h"

namespace {
	/**
	 * @brief Построить лёгкую копию группы: номер и специальность без
	 *        списка студентов. Так студент знает свою группу, но копии
	 *        студентов не вкладываются друг в друга бесконечно.
	 * @param group - исходная группа
	 * @return группа с тем же номером и специальностью без студентов
	 */
	Group makeLightGroup(const Group& group) {
		const Specialty& specialty = group.getSpecialty();
		const Department department(specialty.getDepartment().getName());
		const Specialty lightSpecialty(specialty.getCode(), specialty.getName(), department);
		return Group(group.getNumber(), lightSpecialty);
	}
}

Student::Student(const std::string& fullName, const Date& birthDate, const std::string& recordBook,
	const unsigned int course, const Group& studentGroup, const int id)
	: Person(fullName, birthDate, id), recordBook(FormatUtils::trim(recordBook)), course(course),
	group(makeLightGroup(studentGroup)) {
	if (this->recordBook.empty()) {
		throw std::invalid_argument("Номер зачётной книжки не может быть пустым");
	}
	if (course < 1) {
		throw std::invalid_argument("Номер курса должен быть не меньше 1");
	}
}

const std::string& Student::getRecordBook() const noexcept {return recordBook;}

unsigned int Student::getCourse() const noexcept {return course;}

const Group& Student::getGroup() const noexcept {return group;}

void Student::setGroup(const Group& newGroup) {
	group = makeLightGroup(newGroup);
}

void Student::addMark(const double mark) {
	if (mark < 2.0 || mark > 5.0) {
		throw std::invalid_argument("Оценка должна быть в диапазоне от 2 до 5");
	}
	marks.push_back(mark);
}

const std::vector<double>& Student::getMarks() const noexcept {return marks;}

std::size_t Student::getMarkCount() const noexcept {return marks.size();}

bool Student::hasMarks() const noexcept {return !marks.empty();}

double Student::getAverageMark() const noexcept {
	if (marks.empty()) {
		return 0.0;
	}
	double sum = 0.0;
	for (const double mark : marks) {
		sum += mark;
	}
	return sum / static_cast<double>(marks.size());
}

std::string Student::getRole() const {return "Студент";}

std::string Student::getSpecificInfo() const {
	std::string result = "Зачётная книжка: " + recordBook +
		" | Курс: " + std::to_string(course) +
		" | Группа: " + group.getNumber() +
		" | Оценок: " + std::to_string(marks.size());
	if (hasMarks()) {
		result += " | Средний балл: " + FormatUtils::formatDecimal(getAverageMark());
	}
	return result;
}

bool Student::isStudent() const {return true;}

bool Student::operator==(const Student& other) const noexcept {
	return id == other.id && recordBook == other.recordBook;
}

bool Student::operator!=(const Student& other) const noexcept {return !(*this == other);}
