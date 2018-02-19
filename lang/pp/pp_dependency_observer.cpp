#include "pp_dependency_observer.h"

void pp_dependency_observer::on_include_file(const fs::path& filePath, bool isLocal) {
	if (mpConfig->pp.makedep.noMissingFiles && !fs::exists(filePath))
		return;

	if (mpConfig->pp.makedep.noExtraIncludes && !isLocal)
		return;

	add_dependency(filePath);
}

void pp_dependency_observer::on_include_tool(const fs::path&, const fs::path& wd, const std::vector<std::string>& arguments) {
	if (mpConfig->pp.makedep.noToolArguments)
		return;

	for (const std::string& arg : arguments) {
		if (arg.empty() || (arg[0] == '-'))
			continue;

		fs::path path = wd / arg;

		if (fs::exists(path))
			add_dependency(std::move(path));
	}
}

void pp_dependency_observer::generate_makefile(std::basic_ostream<lang::char_type>& output) const {
	output << mpConfig->pp.makedep.target.string<lang::string_type>();

	if (mpConfig->pp.makedep.outputIsTarget && mpConfig->pp.makedep.output.size())
		output << ' ' << mpConfig->pp.makedep.output.string<lang::string_type>();

	output << ':';

	for (auto file : mFiles)
		output << ' ' << file.string<lang::string_type>();

	output << std::endl;

	if (mpConfig->pp.makedep.addPhonyTargetFiles) {
		fs::path main(mpConfig->pp.input);

		output << std::endl;

		for (auto file : mFiles)
			if (file != main)
				output << file.string<lang::string_type>() << ":" << std::endl;
	}
}

void pp_dependency_observer::add_dependency(const fs::path& path) {
	if (std::find(mFiles.begin(), mFiles.end(), path) == mFiles.end())
		mFiles.emplace_back(path);
}

void pp_dependency_observer::add_dependency(fs::path&& path) {
	if (std::find(mFiles.begin(), mFiles.end(), path) == mFiles.end())
		mFiles.push_back(std::move(path));
}
