#ifndef MACRO_ERROR_H
#define MACRO_ERROR_H

#include <stdexcept>
#include <cstring>

#include "lang/pp/macro/macro.h"

class macro_error : public std::runtime_error {
public:
	macro_error(const macro_signature& m, const char* error)
		: std::runtime_error(make_error_string(m, error)) {}

	template<typename CharType>
	macro_error(const macro_signature& m, const std::basic_string<CharType>& error)
		: std::runtime_error(make_error_string(m, std::string(error.begin(), error.end()).c_str())) {}

private:
	std::string make_error_string(const macro_signature& m, const char* error) {
		std::string result;
		result.reserve(1 + m.name().size() + (m.argument_count() ? (m.argument_count() + 1) : 0) + std::strlen(error));

		result.append(m.name().begin(), m.name().end());

		if (m.argument_count()) {
			result.append(1, '(');
			result.append(m.argument_count()-1, ',');
			result.append(1, ')');
		}

		result.append(1, ' ');
		result.append(error);

		return result;
	}
};

#endif // MACRO_ERROR_H
