#ifndef PP_CONDITIONAL_MACRO_DIRECTIVES_H
#define PP_CONDITIONAL_MACRO_DIRECTIVES_H

#include "lang/pp/directive/pp_directive_base.h"
#include "lang/pp/macro/macro_library.h"
#include "lang/pp/pp_selection_stack.h"

struct pp_directive_ifdef : public pp_directive_base {
	pp_directive_ifdef(macro_library& library, pp_selection_stack& stack)
		: mpLibrary(&library), mpStack(&stack) {}

	void handle(lang::sscanner_type& argument) final;
	bool ignores_selection() const final;

private:
	macro_library* const mpLibrary;
	pp_selection_stack* const mpStack;
};

struct pp_directive_ifndef : public pp_directive_base {
	pp_directive_ifndef(macro_library& library, pp_selection_stack& stack)
		: mpLibrary(&library), mpStack(&stack) {}

	void handle(lang::sscanner_type& argument) final;
	bool ignores_selection() const final;

private:
	macro_library* const mpLibrary;
	pp_selection_stack* const mpStack;
};

class pp_conditional_macro_directives {
public:
	pp_conditional_macro_directives(macro_library& library, pp_selection_stack& stack)
		: mDirectiveIfdef(library, stack), mDirectiveIfndef(library, stack) {}

	pp_directive_base* get_ifdef() { return &mDirectiveIfdef; }
	pp_directive_base* get_ifndef() { return &mDirectiveIfndef; }

private:
	pp_directive_ifdef mDirectiveIfdef;
	pp_directive_ifndef mDirectiveIfndef;
};

#endif // PP_CONDITIONAL_MACRO_DIRECTIVES_H
