#ifndef INCLUDE_ERROR_H
#define INCLUDE_ERROR_H

#include <stdexcept>

#include "lib/string_building.h"

#include "lang/types.h"

class no_include_error : public std::runtime_error {
public:
	no_include_error(const char* fileName)
		: std::runtime_error(make_error_message(fileName)) {}

	template<typename CharType>
	no_include_error(const std::basic_string<CharType>& fileName)
		: std::runtime_error(make_error_message(std::string(fileName.begin(), fileName.end()))) {}

private:
	std::string make_error_message(const std::string& fileName) {
		return string_builder("file \"", fileName, "\" couldn't be found")();
	}
};

class no_tool_error : public std::runtime_error {
public:
	no_tool_error(const char* toolName)
		: std::runtime_error(make_error_message(toolName)) {}

	template<typename CharType>
	no_tool_error(const std::basic_string<CharType>& toolName)
		: std::runtime_error(make_error_message(std::string(toolName.begin(), toolName.end()))) {}

private:
	std::string make_error_message(const std::string& toolName) {
		return string_builder("tool \"", toolName, "\" couldn't be found")();
	}
};

class include_depth_error : public std::runtime_error {
public:
	include_depth_error()
		: std::runtime_error("Reached maximum include depth") {}
};

#endif // INCLUDE_ERROR_H
