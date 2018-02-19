#include "pp_conditional_macro_directives.h"

#include "lang/pp/macro/macro_scanning.h"

void pp_directive_ifdef::handle(lang::sscanner_type& argument) {
	mpStack->push_conditional(mpLibrary->is_macro_defined(sscan::macro::read_signature(argument)));
}

bool pp_directive_ifdef::ignores_selection() const {
	return true;
}

void pp_directive_ifndef::handle(lang::sscanner_type& argument) {
	mpStack->push_conditional(!mpLibrary->is_macro_defined(sscan::macro::read_signature(argument)));
}

bool pp_directive_ifndef::ignores_selection() const {
	return true;
}
