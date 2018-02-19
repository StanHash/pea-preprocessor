#ifndef PP_MAIN_DIRECTIVE_PROCESSOR_H
#define PP_MAIN_DIRECTIVE_PROCESSOR_H

#include "lang/config.h"
#include "lang/pp/pp_directive_processor.h"
#include "lang/pp/directive/pp_macro_directives.h"
#include "lang/pp/directive/pp_selection_directives.h"
#include "lang/pp/directive/pp_conditional_macro_directives.h"
#include "lang/pp/directive/pp_include_directives.h"
#include "lang/pp/directive/pp_binary_directives.h"

class pp_main_directive_processor : public pp_directive_processor {
public:
	pp_main_directive_processor(
		const lang::config* pConfig,
		pp_including_ssource& includer,
		pp_include_observer& incObserver,
		macro_library& library
	);

	bool in_selection() const { return mSelectionStack.in_selection(); }

private:
	const lang::config* const mpConfig;

	pp_selection_stack mSelectionStack;

	pp_macro_directives mMacroDirectives;
	pp_selection_directives mSelectionDirectives;
	pp_conditional_macro_directives mConditionalMacroDirectives;
	pp_include_directives mIncludeDirectives;
	pp_binary_directives mBinaryDirectives;
};

#endif // PP_MAIN_DIRECTIVE_PROCESSOR_H
