#ifndef PP_BINARY_DIRECTIVES_H
#define PP_BINARY_DIRECTIVES_H

#include "lang/pp/directive/pp_directive_base.h"
#include "lang/pp/ssource/pp_including_ssource.h"
#include "lang/pp/pp_include_observer.h"
#include "lang/config.h"

struct pp_directive_include_binary : public pp_directive_base {
	pp_directive_include_binary(
		pp_including_ssource& includingSource,
		const lang::config* pConfig,
		pp_include_observer& obv
	) : mpIncluder(&includingSource), mpConfig(pConfig), mpObserver(&obv) {}

	void handle(lang::sscanner_type& scanner) final;

private:
	const lang::config* const mpConfig;
	pp_including_ssource* const mpIncluder;
	pp_include_observer* const mpObserver;
};

struct pp_directive_include_binary_tool : public pp_directive_base {
	pp_directive_include_binary_tool(
		pp_including_ssource& includingSource,
		const lang::config* pConfig,
		pp_include_observer& obv
	) : mpIncluder(&includingSource), mpConfig(pConfig), mpObserver(&obv) {}

	void handle(lang::sscanner_type& scanner) final;

private:
	const lang::config* const mpConfig;
	pp_including_ssource* const mpIncluder;
	pp_include_observer* const mpObserver;
};

class pp_binary_directives {
public:
	pp_binary_directives(
		pp_including_ssource& includer,
		const lang::config* pConfig,
		pp_include_observer& obv
	) : mDirectiveIncludeBinary(includer, pConfig, obv),
		mDirectiveIncludeBinaryTool(includer, pConfig, obv) {}

	pp_directive_base* get_include_binary()      { return &mDirectiveIncludeBinary; }
	pp_directive_base* get_include_binary_tool() { return &mDirectiveIncludeBinaryTool; }

private:
	pp_directive_include_binary      mDirectiveIncludeBinary;
	pp_directive_include_binary_tool mDirectiveIncludeBinaryTool;
};

#endif // PP_BINARY_DIRECTIVES_H
