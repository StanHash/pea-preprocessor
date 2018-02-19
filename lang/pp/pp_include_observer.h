#ifndef PP_INCLUDE_HANDLER_H
#define PP_INCLUDE_HANDLER_H

#include <vector>

#include "lib/filesystem.h"

class pp_include_observer {
public:
	virtual ~pp_include_observer();

	virtual void on_include_file(const fs::path& filePath, bool isLocal) = 0;
	virtual void on_include_tool(const fs::path& toolPath, const fs::path& wd, const std::vector<std::string>& arguments) = 0;
};

#endif // PP_INCLUDE_HANDLER_H
