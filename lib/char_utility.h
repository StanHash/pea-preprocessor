#ifndef CHAR_UTILITY_H
#define CHAR_UTILITY_H

#include <cctype>

template<typename CharType>
struct char_utility {
	using value_type = CharType;

	static bool is_space(value_type chr) { return std::isspace(chr); }
	static bool is_alpha(value_type chr) { return std::isalpha(chr); }
	static bool is_digit(value_type chr) { return std::isdigit(chr); }
	static bool is_alphanum(value_type chr) { return std::isalnum(chr); }
	static bool is_alpha_or_underscore(value_type chr) { return chr == '_' || std::isalpha(chr); }
	static bool is_alphanum_or_underscore(value_type chr) { return chr == '_' || std::isalnum(chr); }
};

#endif // CHAR_UTILITY_H
