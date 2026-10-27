#include "Group.h"
#include "Specialty.h"

#include "Teacher.h"

#include <stdexcept>

#include "FormatUtils.h"

namespace {
	/**
	 * @brief Построить облегчённую копию дисциплины: название и кафедра без
	 *        списка преподавателей. Так преподаватель знает читаемые
	 *        дисциплины, но копии преподавателей не вкладываются друг в
	 *        друга бесконечно.
	 * @param discipline - исходная дисциплина
	 * @return дисциплина с тем же названием и кафедрой без преподавателей
	 */
	Discipline makeLightDiscipline(const Discipline& discipline) {
		const Department department(discipline.getDepartment().getName());
		return Discipline(discipline.getName(), department);
	}
}

Teacher::Teacher(const std::string& fullName, const Date& birthDate, const std::string& position,
	const std::string& degree, const Department& department, const int id)
	: Person(fullName, birthDate, id), position(FormatUtils::trim(position)),
	degree(FormatUtils::trim(degree)), department(department) {
	if (this->position.empty()) {
		throw std::invalid_argument("Должность преподавателя не может быть пустой");
	}
}

const std::string& Teacher::getPosition() const noexcept {return position;}

const std::string& Teacher::getDegree() const noexcept {return degree;}

const Department& Teacher::getDepartment() const noexcept {return department;}

void Teacher::addDiscipline(const Discipline& discipline) {
	for (const Discipline& added : disciplines) {
		if (added == discipline) {
			return;
		}
	}
	disciplines.push_back(makeLightDiscipline(discipline));
}

const std::vector<Discipline>& Teacher::getDisciplines() const noexcept {return disciplines;}

std::size_t Teacher::getDisciplineCount() const noexcept {return disciplines.size();}

std::string Teacher::getRole() const {return "Преподаватель";}

std::string Teacher::getSpecificInfo() const {
	std::string result = "Должность: " + position + " | Кафедра: " + department.getName()
		+ " | Дисциплин: " + std::to_string(disciplines.size());
	if (!degree.empty()) {
		result += " | Учёная степень: " + degree;
	}
	return result;
}

bool Teacher::isTeacher() const {return true;}

bool Teacher::operator==(const Teacher& other) const noexcept {
	return id == other.id &&
		FormatUtils::equalsIgnoreCase(position, other.position) &&
		FormatUtils::equalsIgnoreCase(degree, other.degree) &&
		department == other.department;
}

bool Teacher::operator!=(const Teacher& other) const noexcept {return !(*this == other);}
