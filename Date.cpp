//
// Date.cpp
// Реализация класса Date. Не использует потоки ввода-вывода: все
// преобразования выполняются средствами std::string и арифметики.
//
#include "Date.h"

#include <stdexcept>

Date::Date(int year, int month, int day)
    : year_(year)
    , month_(month)
    , day_(day)
{
    Validate(year, month, day);
}

Date Date::Parse(const std::string& isoDate) {
    // Строгий разбор формата "ГГГГ-ММ-ДД": ровно 10 символов,
    // разделители '-' на позициях 4 и 7, остальные символы - цифры.
    if (isoDate.size() != 10 || isoDate[4] != '-' || isoDate[7] != '-') {
        throw std::invalid_argument("Date: строка \"" + isoDate + "\" не соответствует формату ГГГГ-ММ-ДД");
    }
    for (size_t i = 0; i < isoDate.size(); ++i) {
        if (i == 4 || i == 7) {
            continue; // разделители проверены выше
        }
        if (isoDate[i] < '0' || isoDate[i] > '9') {
            throw std::invalid_argument("Date: строка \"" + isoDate + "\" содержит нецифровой символ");
        }
    }

    const int year = std::stoi(isoDate.substr(0, 4));
    const int month = std::stoi(isoDate.substr(5, 2));
    const int day = std::stoi(isoDate.substr(8, 2));
    return Date(year, month, day);
}

int Date::GetYear() const {
    return year_;
}

int Date::GetMonth() const {
    return month_;
}

int Date::GetDay() const {
    return day_;
}

std::string Date::ToString() const {
    // Ручное форматирование с ведущими нулями, чтобы не тянуть <iomanip>
    // и не зависеть от локали/потоков.
    static const char* kOneDigit[] = {
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
    };

    static const char* kTwoDigits[] = {
        "00","01","02","03","04","05","06","07","08","09",
        "10","11","12","13","14","15","16","17","18","19",
        "20","21","22","23","24","25","26","27","28","29",
        "30","31","32","33","34","35","36","37","38","39",
        "40","41","42","43","44","45","46","47","48","49",
        "50","51","52","53","54","55","56","57","58","59",
        "60","61","62","63","64","65","66","67","68","69",
        "70","71","72","73","74","75","76","77","78","79",
        "80","81","82","83","84","85","86","87","88","89",
        "90","91","92","93","94","95","96","97","98","99"
    };

    std::string yearPart;
    int y = year_;
    if (y < 0) {
        yearPart = "-";
        y = -y;
    }
    // Минимум 4 цифры года, ведущие нули при необходимости.
    std::string digits;
    if (y == 0) {
        digits = "0";
    }
    while (y > 0) {
        digits = kOneDigit[y % 10] + digits;
        y /= 10;
    }
    while (digits.size() < 4) {
        digits = "0" + digits;
    }
    yearPart += digits;

    return yearPart + "-" + kTwoDigits[month_] + "-" + kTwoDigits[day_];
}

long Date::ToDayNumber() const {
    // Формула Юлианского дня (для григорианского календаря), приведённая
    // к эпохе 0000-03-01. Точности для наших задач более чем достаточно.
    long a = (14 - month_) / 12;
    long y = year_ + 4800 - a;
    long m = month_ + 12 * a - 3;
    return day_ + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
}

long Date::DaysUntil(const Date& other) const {
    return other.ToDayNumber() - ToDayNumber();
}

bool Date::operator==(const Date& other) const {
    return year_ == other.year_ && month_ == other.month_ && day_ == other.day_;
}

bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

bool Date::operator<(const Date& other) const {
    if (year_ != other.year_) return year_ < other.year_;
    if (month_ != other.month_) return month_ < other.month_;
    return day_ < other.day_;
}

bool Date::operator<=(const Date& other) const {
    return !(other < *this);
}

bool Date::operator>(const Date& other) const {
    return other < *this;
}

bool Date::operator>=(const Date& other) const {
    return !(*this < other);
}

void Date::Validate(int year, int month, int day) {
    if (month < 1 || month > 12) {
        throw std::invalid_argument("Date: месяц вне диапазона 1..12");
    }
    if (day < 1 || day > DaysInMonth(year, month)) {
        throw std::invalid_argument("Date: день вне диапазона для указанного месяца");
    }
}

int Date::DaysInMonth(int year, int month) {
    static const int kDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && IsLeapYear(year)) {
        return 29;
    }
    return kDays[month - 1];
}

bool Date::IsLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
