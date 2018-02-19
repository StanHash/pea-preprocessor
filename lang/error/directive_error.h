#ifndef DIRECTIVE_ERROR_H
#define DIRECTIVE_ERROR_H

#include <stdexcept>

#include "lib/string_building.h"

class directive_error : public std::runtime_error {
public:
	template<typename CharType>
	directive_error(const std::basic_string<CharType>& directive, const char* error)
		: std::runtime_error(make_error_string(std::string(directive.begin(), directive.end()).c_str(), error)) {}

private:
	std::string make_error_string(const char* directive, const char* error) const {
		return string_builder("directive \"", directive, "\" failed - ", error)();
	}
};

#endif // DIRECTIVE_ERROR_H
