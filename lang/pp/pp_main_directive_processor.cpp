#include "pp_main_directive_processor.h"

pp_main_directive_processor::pp_main_directive_processor(
	const lang::config* pConfig,
	pp_including_ssource& includer,
	pp_include_observer& incObserver,
	macro_library& library
) : mpConfig(pConfig),
	  mMacroDirectives(library, pConfig),
	  mSelectionDirectives(mSelectionStack),
	  mConditionalMacroDirectives(library, mSelectionStack),
	  mIncludeDirectives(includer, pConfig, incObserver),
	  mBinaryDirectives(includer, pConfig, incObserver) {
	if (mpConfig->pp.macro.enable) {
		directives().insert({ LANG_LITERAL "define", mMacroDirectives.get_define() });
		directives().insert({ LANG_LITERAL "undef",  mMacroDirectives.get_undef() });

		if (mpConfig->pp.condition.enable) {
			directives().insert({ LANG_LITERAL "ifdef",  mConditionalMacroDirectives.get_ifdef() });
			directives().insert({ LANG_LITERAL "ifndef", mConditionalMacroDirectives.get_ifndef() });
		}
	}

	if (mpConfig->pp.condition.enable) {
		directives().insert({ LANG_LITERAL "else",  mSelectionDirectives.get_else() });
		directives().insert({ LANG_LITERAL "endif", mSelectionDirectives.get_endif() });
	}

	if (mpConfig->pp.include.enable) {
		directives().insert({ LANG_LITERAL "include", mIncludeDirectives.get_include() });

		if (mpConfig->pp.eaCompat.enableBinary) {
			directives().insert({ LANG_LITERAL "incbin", mBinaryDirectives.get_include_binary() });
		}
	}

	if (mpConfig->pp.tool.enable) {
		directives().insert({ LANG_LITERAL "include_tool", mIncludeDirectives.get_include_tool() });

		if (mpConfig->pp.eaCompat.enableDirectiveAliases) {
			directives().insert({ LANG_LITERAL "inctext",   mIncludeDirectives.get_include_tool() });
			directives().insert({ LANG_LITERAL "inctevent", mIncludeDirectives.get_include_tool() });
		}

		if (mpConfig->pp.eaCompat.enableBinary) {
			directives().insert({ LANG_LITERAL "incext", mBinaryDirectives.get_include_binary_tool() });
		}
	}
}
