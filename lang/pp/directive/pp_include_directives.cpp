#include "pp_include_directives.h"

#include "lib/string_scanning.h"
#include "lib/error/sscan_error.h"

#include "lang/pp/include/pp_included_file_ssource.h"
#include "lang/pp/include/pp_tool_ssource.h"
#include "lang/error/include_error.h"

void pp_directive_include::handle(lang::sscanner_type& scanner) {
	lang::string_type fname;

	if (scanner.match('"'))
		fname = sscan::read_string_literal(scanner);
	else
		fname.append(scanner.it(), scanner.end());

	bool isLocal  = true;
	fs::path file = mpIncludingSource->find_local_file_path(fname);

	if (file.empty()) {
		isLocal = false;
		file = mpIncludingSource->find_extra_file_path(fname);
	}

	if (file.size()) {
		mpIncludingSource->include<pp_included_file_ssource>(!isLocal, file);
	} else if (!mpConfig->pp.include.ignoreMissingFiles)
		throw no_include_error(fname);

	if (mpObserver)
		mpObserver->on_include_file((file.empty() ? (mpIncludingSource->current_directory() / fname) : file), mpIncludingSource->in_local());
}

void pp_directive_include_tool::handle(lang::sscanner_type& scanner) {
	lang::string_type toolName;
	std::vector<std::string> arguments;

	// TODO: ability to use temporary files instead of stdout
	// for example "#include_tool SomeTool SomeInput -o %%"
	// would create a temporary file and replace '%%' by its path
	// then wait for the program to end before reading from that file

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

		arguments.emplace_back(arg.begin(), arg.end());
	}

	fs::path toolPath = mpIncludingSource->find_tool_path(toolName);
	fs::path wd       = mpIncludingSource->current_directory();

	if (toolPath.size()) {
		mpIncludingSource->include<pp_tool_ssource>(false,
			mpConfig,
			toolPath,
			wd,
			arguments
		);
	} else if (!mpConfig->pp.tool.ignoreMissingTools)
		throw no_tool_error(toolName);

	if (mpObserver)
		mpObserver->on_include_tool(toolName, wd, arguments);
}
