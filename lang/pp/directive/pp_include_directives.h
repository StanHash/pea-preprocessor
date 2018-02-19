#ifndef PP_INCLUDE_DIRECTIVES_H
#define PP_INCLUDE_DIRECTIVES_H

#include "lang/pp/directive/pp_directive_base.h"
#include "lang/pp/ssource/pp_including_ssource.h"
#include "lang/pp/pp_include_observer.h"
#include "lang/pp/pp_include_observer.h"

struct pp_directive_include : public pp_directive_base {
	pp_directive_include(
		pp_including_ssource& includingSource,
		const lang::config* pConfig,
		pp_include_observer& obv
	) : mpIncludingSource(&includingSource), mpConfig(pConfig), mpObserver(&obv) {}

	void handle(lang::sscanner_type& scanner) final;

private:
	pp_including_ssource* const mpIncludingSource;
	const lang::config* const mpConfig;
	pp_include_observer* const mpObserver;
};

struct pp_directive_include_tool : public pp_directive_base {
	pp_directive_include_tool(
		pp_including_ssource& includingSource,
		const lang::config* pConfig,
		pp_include_observer& obv
	) : mpIncludingSource(&includingSource), mpConfig(pConfig), mpObserver(&obv) {}

	void handle(lang::sscanner_type& scanner) final;

private:
	pp_including_ssource* const mpIncludingSource;
	const lang::config* const mpConfig;
	pp_include_observer* const mpObserver;
};

class pp_include_directives {
public:
	pp_include_directives(
		pp_including_ssource& includingSource,
		const lang::config* pConfig,
		pp_include_observer& obv
	) : mDirectiveInclude(includingSource, pConfig, obv),
		mDirectiveIncludeTool(includingSource, pConfig, obv) {}

	pp_directive_base* get_include() { return &mDirectiveInclude; }
	pp_directive_base* get_include_tool() { return &mDirectiveIncludeTool; }

private:
	pp_directive_include mDirectiveInclude;
	pp_directive_include_tool mDirectiveIncludeTool;
};

#endif // PP_INCLUDE_DIRECTIVES_H
