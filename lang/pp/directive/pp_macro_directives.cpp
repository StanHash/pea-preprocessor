#include "pp_macro_directives.h"

#include "lib/error/sscan_error.h"

#include "lang/error/macro_error.h"
#include "lang/pp/macro/macro_scanning.h"

void pp_directive_define::handle(lang::sscanner_type& scanner) {
	lang::string_type format;

	complete_macro_signature signature = sscan::macro::read_complete_signature(scanner);

	if (sscan::scan_whitespaces(scanner), scanner.match('"'))
		format = sscan::read_string_literal(scanner);
	else
		format = lang::string_type(scanner.it(), scanner.end());

	if (signature.argument_count()) {
		if (mpConfig->pp.macro.strictMacroFormat)
			format = macro::format_format_strict(format, signature.arguments());
		else
			format = macro::format_format_simple(format, signature.arguments());
	}

	macro_signature actualSignature(std::move(signature));
	if (!mpLibrary->define_macro(actualSignature, format))
		throw macro_error(actualSignature, "already exists (couldn't define) (consider using #ifndef)");
}

void pp_directive_undef::handle(lang::sscanner_type& scanner) {
	macro_signature signature = sscan::macro::read_signature(scanner);

	if (!mpLibrary->undefine_macro(signature))
		throw macro_error(signature, "doesn't exists (couldn't undefine) (consider using #ifdef)");
}
