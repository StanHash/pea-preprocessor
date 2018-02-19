#include "pp_binary_directives.h"

#include "lang/pp/include/pp_tool_ssource.h"
#include "lang/pp/include/pp_binary_tool_ssource.h"
#include "lang/error/include_error.h"
#include "lang/error/tool_error.h"

#include "lib/error/sscan_error.h"

void pp_directive_include_binary::handle(lang::sscanner_type& scanner) {
	// TODO: merge shared code with pp_directive_include in some base class

	lang::string_type fname;

	if (scanner.match('"'))
		fname = sscan::read_string_literal(scanner);
	else
		fname.append(scanner.it(), scanner.end());

	bool isLocal  = true;
	fs::path file = mpIncluder->find_local_file_path(fname);

	if (file.empty()) {
		isLocal = false;
		file = mpIncluder->find_extra_file_path(fname);
	}

	// end of code shared with pp_directive_include

	fs::path binTool = mpIncluder->find_tool_path(mpConfig->pp.eaCompat.binaryToolName);

	if (binTool.empty())
		throw std::runtime_error(string_builder(
			"binary tool ('",
			mpConfig->pp.eaCompat.binaryToolName,
			"') couldn't be found"
		)());

	std::vector<std::string> binArguments = mpConfig->pp.eaCompat.binaryToolArguments;

	for (auto& arg : binArguments)
		if (arg == "%%")
			arg = fs::absolute(file).string();

	if (file.size()) {
		mpIncluder->include<pp_tool_ssource>(!isLocal,
			mpConfig,
			binTool,
			mpIncluder->current_directory(),
			binArguments
		);
	} else if (!mpConfig->pp.include.ignoreMissingFiles)
		throw no_include_error(fname);

	if (mpObserver)
		mpObserver->on_include_file((file.empty() ? (mpIncluder->current_directory() / fname) : file), mpIncluder->in_local());
}

void pp_directive_include_binary_tool::handle(lang::sscanner_type& scanner) {
	// TODO: merge shared code with pp_directive_include_tool in some base class

	lang::string_type toolName;
	std::vector<lang::string_type> arguments;

	if (scanner.match('"'))
		toolName = sscan::read_string_literal(scanner);
	else
		toolName = sscan::read_identifier(scanner);

	if (toolName.empty())
		throw sscan_error(scanner, "expected valid tool name");

	while (sscan::scan_whitespaces(scanner), !scanner.at_end()) {
		lang::string_type arg;

		if (scanner.match('"'))
			arg = sscan::read_string_literal(scanner);
		else
			arg = sscan::read_identifier(scanner);

		if (arg.empty())
			throw sscan_error(scanner, "expected valid tool argument");

		arguments.push_back(arg);
	}

	fs::path toolPath = mpIncluder->find_tool_path(toolName);
	fs::path binTool  = mpIncluder->find_tool_path(mpConfig->pp.eaCompat.binaryToolName);

	if (binTool.empty())
		throw std::runtime_error(string_builder(
			"binary tool ('",
			mpConfig->pp.eaCompat.binaryToolName,
			"') couldn't be found"
		)());

	fs::path wd = mpIncluder->current_directory();

	if (toolPath.size()) {
		mpIncluder->include<pp_binary_tool_ssource>(false,
			mpConfig,
			binTool,
			toolPath,
			wd,
			arguments
		);
	} else if (!mpConfig->pp.tool.ignoreMissingTools)
		throw no_tool_error(toolName);

	if (mpObserver)
		mpObserver->on_include_tool(toolName, wd, arguments);
}
