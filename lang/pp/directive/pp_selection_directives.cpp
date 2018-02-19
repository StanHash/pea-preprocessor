#include "pp_selection_directives.h"

#include <stdexcept>

void pp_directive_else::handle(lang::sscanner_type&) {
	if (!mpStack->flip_conditional())
		throw std::runtime_error("else directive encountered without any matching opening condition");
}

bool pp_directive_else::ignores_selection() const {
	return true;
}

void pp_directive_endif::handle(lang::sscanner_type&) {
	if (!mpStack->pop_conditional())
		throw std::runtime_error("endif directive encountered without any matching opening condition");
}

bool pp_directive_endif::ignores_selection() const {
	return true;
}
