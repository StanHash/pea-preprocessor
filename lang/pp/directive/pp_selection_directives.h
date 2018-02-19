#ifndef PP_SELECTION_DIRECTIVES_H
#define PP_SELECTION_DIRECTIVES_H

#include "lang/pp/directive/pp_directive_base.h"
#include "lang/pp/pp_selection_stack.h"

struct pp_directive_else : public pp_directive_base {
public:
	pp_directive_else(pp_selection_stack& stack)
		: mpStack(&stack) {}

	void handle(lang::sscanner_type&) final;
	bool ignores_selection() const final;

private:
	pp_selection_stack* const mpStack;
};

struct pp_directive_endif : public pp_directive_base {
public:
	pp_directive_endif(pp_selection_stack& stack)
		: mpStack(&stack) {}

	void handle(lang::sscanner_type&) final;
	bool ignores_selection() const final;

private:
	pp_selection_stack* const mpStack;
};

class pp_selection_directives {
public:
	pp_selection_directives(pp_selection_stack& stack)
		: mDirectiveElse(stack), mDirectiveEndif(stack) {}

	pp_directive_base* get_else() { return &mDirectiveElse; }
	pp_directive_endif* get_endif() { return &mDirectiveEndif; }

private:
	pp_directive_else mDirectiveElse;
	pp_directive_endif mDirectiveEndif;
};

#endif // PP_SELECTION_DIRECTIVES_H
