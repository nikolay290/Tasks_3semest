#include "FormatUtils.h"

namespace {
	/**
	 * @brief Перевести символ латиницы или кириллицы в нижний регистр
	 * @param symbol - символ в кодировке UTF-8
	 * @return символ в нижнем регистре; по умолчанию возвращает сам символ
	 */
	char toLowerAscii(const char symbol) {
		if (symbol >= 'A' && symbol <= 'Z') {
			return static_cast<char>(symbol - 'A' + 'a');
		}
		return symbol;
	}

	/**
	 * @brief Разобрать длину UTF-8-последовательности из её первого байта
	 * @param firstByte - первый байт символа
	 * @return длина последовательности в байтах; 1, если последовательность
	 *         некорректна
	 */
	std::size_t utf8SequenceLength(const unsigned char firstByte) {
		if (firstByte < 0x80) {
			return 1;
		}
		if ((firstByte & 0xE0) == 0xC0) {
			return 2;
		}
		if ((firstByte & 0xF0) == 0xE0) {
			return 3;
		}
		if ((firstByte & 0xF8) == 0xF0) {
			return 4;
		}
		return 1;
	}

	/**
	 * @brief Привести весь UTF-8-текст к нижнему регистру
	 * @param text - исходная строка
	 * @return строка, в которой латинские и кириллические буквы заменены
	 *         на их нижние аналоги
	 */
	std::string toLowerUtf8(const std::string& text) {
		std::string result;
		result.reserve(text.length());
		for (std::size_t index = 0; index < text.length();) {
			const unsigned char firstByte = static_cast<unsigned char>(text[index]);
			const std::size_t length = utf8SequenceLength(firstByte);
			if (length == 1) {
				result += toLowerAscii(text[index]);
				++index;
				continue;
			}
			if (index + length > text.length()) {
				break;
			}
			if (length == 2) {
				const unsigned int codePoint =
					(static_cast<unsigned int>(firstByte) & 0x1F) << 6 |
					(static_cast<unsigned char>(text[index + 1]) & 0x3F);
				if (codePoint >= 0x410 && codePoint <= 0x42F) {
					const unsigned int lowerCase = codePoint + 0x20;
					result += static_cast<char>(0xC0 | (lowerCase >> 6));
					result += static_cast<char>(0x80 | (lowerCase & 0x3F));
				} else if (codePoint == 0x401) {
					result += static_cast<char>(0xD1);
					result += static_cast<char>(0x91);
				} else {
					result.append(text, index, length);
				}
				index += length;
				continue;
			}
			result.append(text, index, length);
			index += length;
		}
		return result;
	}
}

std::string FormatUtils::formatDecimal(const double value) {
	const long long scaled = static_cast<long long>(value * 100.0 + (value >= 0 ? 0.5 : -0.5));
	const long long wholePart = scaled / 100;
	const long long fractionPart = scaled % 100 < 0 ? -(scaled % 100) : scaled % 100;
	std::string result = std::to_string(wholePart) + ".";
	result += fractionPart < 10 ? "0" + std::to_string(fractionPart) : std::to_string(fractionPart);
	return result;
}

bool FormatUtils::equalsIgnoreCase(const std::string& a, const std::string& b) {
	return toLowerUtf8(a) == toLowerUtf8(b);
}

bool FormatUtils::containsIgnoreCase(const std::string& haystack, const std::string& needle) {
	if (needle.empty()) {
		return true;
	}
	return toLowerUtf8(haystack).find(toLowerUtf8(needle)) != std::string::npos;
}

std::string FormatUtils::toInitials(const std::string& fullName) {
	std::vector<std::string> words;
	std::string currentWord;
	for (std::size_t index = 0; index <= fullName.length(); ++index) {
		if (index == fullName.length() || fullName[index] == ' ') {
			const std::string word = trim(currentWord);
			if (!word.empty()) {
				words.push_back(word);
			}
			currentWord.clear();
			continue;
		}
		currentWord += fullName[index];
	}
	if (words.empty()) {
		return "";
	}
	std::string result = words[0];
	for (std::size_t index = 1; index < words.size(); ++index) {
		result += " ";
		result.append(words[index], 0, utf8SequenceLength(static_cast<unsigned char>(words[index][0])));
		result += ".";
	}
	return result;
}

std::string FormatUtils::trim(const std::string& text) {
	std::size_t begin = 0;
	std::size_t end = text.length();
	while (begin < end && text[begin] == ' ') {
		++begin;
	}
	while (end > begin && text[end - 1] == ' ') {
		--end;
	}
	return text.substr(begin, end - begin);
}

std::string FormatUtils::join(const std::vector<std::string>& items, const std::string& separator) {
	std::string result;
	for (std::size_t index = 0; index < items.size(); ++index) {
		if (index > 0) {
			result += separator;
		}
		result += items[index];
	}
	return result;
}
