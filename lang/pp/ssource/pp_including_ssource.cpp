#include "pp_including_ssource.h"

#include <algorithm>

#include <boost/process.hpp>

#include "lib/ssource/file_string_source.h"
#include "lib/string_building.h"

/*
 * I am ashamed of the poor code quality found in this file
 */

lang::string_type pp_including_ssource::get_string() {
	cleanup_stack();
	++mStack.back().line;

	return mStack.back().pIncluded->get_ssource().get_string();
}

bool pp_including_ssource::at_end() const {
	for (const auto& source : mStack)
		if (!source.pIncluded->get_ssource().at_end())
			return false;
	return true;
}

fs::path pp_including_ssource::find_local_file_path(const fs::path& fileName) const {
	fs::path result;

	// Current Directory Relative
	if (mStack.size())
		result = mStack.back().pIncluded->directory / (fileName);
	else
		result = fileName;

	if (fs::exists(result))
		return result;

	// Main Directory Relative
	if (mpConfig->pp.include.enableRootRelative && (mStack.size() > 2))
		result = mStack.front().pIncluded->directory / (fileName);

	if (fs::exists(result))
		return result;

	return fs::path();
}

fs::path pp_including_ssource::find_extra_file_path(const fs::path& fileName) const {
	fs::path result;

	if (mpConfig->pp.include.extraIncludeDirectories.empty())
		return result;

	auto it = mpConfig->pp.include.extraIncludeDirectories.begin();

	do {
		result = (*it) / (fileName);

		if (fs::exists(result))
			return result;
	} while (++it != mpConfig->pp.include.extraIncludeDirectories.end());

	return fs::path();
}

fs::path pp_including_ssource::find_tool_path(const fs::path& toolName) const {
	std::vector<fs::path> searchPaths(mpConfig->pp.tool.extraToolDirectories);

	if (mpConfig->pp.tool.enableSystemTools) {
		const auto& sysPath = boost::this_process::path();
		std::copy(sysPath.begin(), sysPath.end(), std::back_inserter(searchPaths));
	}

	if (mpConfig->pp.tool.enableLocalTools && mStack.size())
		searchPaths.push_back(mStack.back().pIncluded->directory);

	// TODO: maybe take the toolName path prefix and append it to the listed paths?
	// Then bp::search_path with toolName.filename()

	return boost::process::search_path(toolName, searchPaths);
}

bool pp_including_ssource::in_local() const {
	return (mStack.size() ? mStack.back().local : true);
}

fs::path pp_including_ssource::current_directory() const {
	if (mStack.size())
		return mStack.back().pIncluded->directory;
	return fs::path();
}

std::string pp_including_ssource::current_source_name() const {
	if (mStack.size())
		return mStack.back().pIncluded->name;
	return "<no source>";
}

unsigned int pp_including_ssource::current_source_line() const {
	if (mStack.size())
		return mStack.back().line;
	return 0;
}

void pp_including_ssource::cleanup_stack() {
	while (mStack.size() && mStack.back().pIncluded->get_ssource().at_end())
		mStack.pop_back();
}
