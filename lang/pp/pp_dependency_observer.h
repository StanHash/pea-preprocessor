#ifndef PP_DEPENDENCY_TREE_H
#define PP_DEPENDENCY_TREE_H

#include <vector>
#include <map>

#include "lib/filesystem.h"

#include "lang/config.h"
#include "lang/pp/pp_include_observer.h"

class pp_dependency_observer : public pp_include_observer {
public:
	pp_dependency_observer(lang::config* pConfig)
		: mpConfig(pConfig) {}

	void on_include_file(const fs::path& filePath, bool isLocal) final;
	void on_include_tool(const fs::path&, const fs::path& wd, const std::vector<std::string>& arguments) final;

	void generate_makefile(std::basic_ostream<lang::char_type>& output) const;

	void add_dependency(const fs::path& path);
	void add_dependency(fs::path&& path);

private:
	std::vector<fs::path> mFiles;
	lang::config* mpConfig;
};

#endif // PP_DEPENDENCY_TREE_H
