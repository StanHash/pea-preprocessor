#ifndef PP_MACRO_DIRECTIVES_H
#define PP_MACRO_DIRECTIVES_H

#include "lang/pp/directive/pp_directive_base.h"
#include "lang/pp/macro/macro_library.h"
#include "lang/config.h"

struct pp_directive_define : public pp_directive_base {
	pp_directive_define(macro_library& library, const lang::config* pConfig)
		: mpLibrary(&library), mpConfig(pConfig) {}

	void handle(lang::sscanner_type& scanner) final;

private:
	macro_library* const mpLibrary;
	const lang::config* const mpConfig;
};

struct pp_directive_undef : public pp_directive_base {
	pp_directive_undef(macro_library& library)
		: mpLibrary(&library) {}

	void handle(lang::sscanner_type& scanner) final;

private:
	macro_library* const mpLibrary;
};

class pp_macro_directives {
public:
	pp_macro_directives(macro_library& library, const lang::config* pConfig)
		: mDefineDirective(library, pConfig), mUndefDirective(library) {}

	pp_directive_base* get_define() { return &mDefineDirective; }
	pp_directive_base* get_undef()  { return &mUndefDirective; }

private:
	pp_directive_define mDefineDirective;
	pp_directive_undef  mUndefDirective;
};

#endif // PP_MACRO_DIRECTIVES_H
