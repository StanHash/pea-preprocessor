#include "pp_directive_processor.h"

#include "lib/string_scanning.h"
#include "lang/error/directive_error.h"

bool pp_directive_processor::process_directive(const lang::string_type& string) {
	lang::sscanner_type scanner = make_scanner(string);

	sscan::scan_whitespaces(scanner);

	if (!scanner.consume('#'))
		return false; // it isn't directive

	// Reading name
	sscan::scan_whitespaces(scanner);
	lang::string_type directiveName = sscan::read_identifier(scanner);

	auto it = directives().find(directiveName);

	if (it == directives().end())
		return false; // it isn't a known directive

	if (!in_selection() && !it->second->ignores_selection())
		return false; // it isn't time to directive

	// Reading argument
	sscan::scan_whitespaces(scanner);

	try {
		it->second->handle(scanner);
	} catch (const std::exception& e) {
		throw directive_error(directiveName, e.what());
	}

	return true;
}
