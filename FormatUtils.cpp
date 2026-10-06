//
// FormatUtils.cpp
// Реализация вспомогательных функций форматирования.
//
#include "FormatUtils.h"

#include <cmath>
#include <vector>

namespace FormatUtils {

    namespace {
        // Таблица однобуквенных цифр - используется для сборки целой части
        // числа поразрядно, без потоков и без <iomanip>.
        const char* const kOneDigit[10] = {
            "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
        };

        // Таблица двухзначных представлений 0..99 - используется для
        // дробной части (с ведущим нулём) и для месяца/дня в датах.
        const char* const kTwoDigits[100] = {
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

        /**
         * @brief Декодирует один UTF-8 символ по индексу @p i в @p s,
         *        записывая кодовую точку в @p cp и сдвигая @p i на длину
         *        символа (1..4 байта).
         */
        unsigned int DecodeUtf8(const std::string& s, size_t& i) {
            const unsigned char c = static_cast<unsigned char>(s[i]);
            size_t extra = 0;
            unsigned int cp = 0;

            if (c < 0x80) {                 // ASCII
                cp = c;
                extra = 0;
            }
            else if ((c & 0xE0) == 0xC0) {  // 110xxxxx
                cp = c & 0x1Fu;
                extra = 1;
            }
            else if ((c & 0xF0) == 0xE0) {  // 1110xxxx
                cp = c & 0x0Fu;
                extra = 2;
            }
            else if ((c & 0xF8) == 0xF0) {  // 11110xxx
                cp = c & 0x07u;
                extra = 3;
            }
            else {
                // Некорректный старший байт - трактуем как отдельный символ.
                ++i;
                return 0xFFFDu;
            }

            ++i;
            for (size_t k = 0; k < extra && i < s.size(); ++k, ++i) {
                cp = (cp << 6) | (static_cast<unsigned char>(s[i]) & 0x3Fu);
            }
            return cp;
        }

        /**
         * @brief Приводит кодовую точку Unicode к строчному регистру
         *        (только для кириллицы и латиницы, которых достаточно для
         *        предметной области). Остальные символы не меняются.
         */
        unsigned int ToLowerCodePoint(unsigned int cp) {
            if (cp >= 0x0410 && cp <= 0x042F) return cp + 0x20; // А-Я -> а-я
            if (cp == 0x0401) return 0x0451;                     // Ё -> ё
            if (cp >= 0x0041 && cp <= 0x005A) return cp + 0x20; // A-Z -> a-z
            return cp;
        }
    }

    std::string FormatDecimal(const double value) {
        // Округляем до сотых и печатаем целую и дробную части раздельно,
        // чтобы избежать отрицательного нуля и обхода через потоки.
        long long cents = static_cast<long long>(std::llround(value * 100.0));

        std::string sign;
        if (cents < 0) {
            sign = "-";
            cents = -cents;
        }

        const long long whole = cents / 100;
        const long long frac = cents % 100;

        std::string wholePart;
        if (whole == 0) {
            wholePart = "0";
        }
        else {
            long long w = whole;
            while (w > 0) {
                wholePart = kOneDigit[w % 10] + wholePart;
                w /= 10;
            }
        }

        return sign + wholePart + "." + kTwoDigits[frac];
    }

    bool EqualsIgnoreCase(const std::string& a, const std::string& b) {
        if (a.size() != b.size()) {
            return false;
        }

        size_t i = 0;
        size_t j = 0;
        while (i < a.size() && j < b.size()) {
            const unsigned int ca = ToLowerCodePoint(DecodeUtf8(a, i));
            const unsigned int cb = ToLowerCodePoint(DecodeUtf8(b, j));
            if (ca != cb) {
                return false;
            }
        }
        return i == a.size() && j == b.size();
    }

    bool ContainsIgnoreCase(const std::string& haystack, const std::string& needle) {
        if (needle.empty()) {
            return true;
        }
        if (needle.size() > haystack.size()) {
            return false;
        }

        const size_t n = haystack.size() - needle.size();
        for (size_t start = 0; start <= n; ++start) {
            size_t i = start;
            size_t j = 0;
            bool matched = true;
            while (j < needle.size()) {
                const unsigned int ch = ToLowerCodePoint(DecodeUtf8(haystack, i));
                const unsigned int cn = ToLowerCodePoint(DecodeUtf8(needle, j));
                if (ch != cn) {
                    matched = false;
                    break;
                }
            }
            if (matched) {
                return true;
            }
        }
        return false;
    }

    std::string ToInitials(const std::string& fullName) {
        const std::string trimmed = Trim(fullName);
        if (trimmed.empty()) {
            return "";
        }

        // Разбиваем по пробелам без потоков.
        std::vector<std::string> words;
        std::string current;
        for (char c : trimmed) {
            if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
                if (!current.empty()) {
                    words.push_back(current);
                    current.clear();
                }
            }
            else {
                current.push_back(c);
            }
        }
        if (!current.empty()) {
            words.push_back(current);
        }

        std::string result = words[0];
        for (size_t i = 1; i < words.size(); ++i) {
            // Первая буква следующей части - это один UTF-8 символ (1..4 байта).
            size_t pos = 0;
            DecodeUtf8(words[i], pos);
            result += " " + words[i].substr(0, pos) + ".";
        }
        return result;
    }

    std::string Trim(const std::string& text) {
        size_t begin = 0;
        while (begin < text.size() && (text[begin] == ' ' || text[begin] == '\t'
            || text[begin] == '\n' || text[begin] == '\r')) {
            ++begin;
        }
        size_t end = text.size();
        while (end > begin && (text[end - 1] == ' ' || text[end - 1] == '\t'
            || text[end - 1] == '\n' || text[end - 1] == '\r')) {
            --end;
        }
        return text.substr(begin, end - begin);
    }

    std::string Join(const std::vector<std::string>& items, const std::string& separator) {
        std::string result;
        for (size_t i = 0; i < items.size(); ++i) {
            if (i != 0) {
                result += separator;
            }
            result += items[i];
        }
        return result;
    }
}
