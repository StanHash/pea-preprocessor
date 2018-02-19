#ifndef TOOL_ERROR_H
#define TOOL_ERROR_H

#include <stdexcept>

#include "lib/string_building.h"

class tool_error : public std::runtime_error {
public:
	tool_error(const char* error)
		: std::runtime_error(string_builder("tool didn't execute successfully:\n", error)()) {}
};

#endif // TOOL_ERROR_H
