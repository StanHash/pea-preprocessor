#ifndef PP_INCLUDING_SSOURCE_H
#define PP_INCLUDING_SSOURCE_H

#include "lib/filesystem.h"
#include "lib/ssource/string_source.h"

#include "lang/config.h"
#include "lang/pp/include/pp_included_base.h"
#include "lang/error/include_error.h"

class pp_including_ssource : public lang::ssource_type {
private:
	struct included_source {
		std::unique_ptr<pp_included_base> pIncluded;
		unsigned int line;
		bool local;
	};

public:
	pp_including_ssource(const lang::config* pConfig)
		: mpConfig(pConfig) {}

	lang::string_type get_string() final;
	bool at_end() const final;

	fs::path find_local_file_path(const fs::path& fileName) const;
	fs::path find_extra_file_path(const fs::path& fileName) const;

	fs::path find_tool_path(const fs::path& toolName) const;

	template<typename IncludedType, typename... Args>
	void include(bool forceNonLocal, Args&& ... args) {
		if (mStack.size() > mpConfig->pp.include.maxDepth)
			throw include_depth_error();

		mStack.push_back({
			.pIncluded = std::unique_ptr<pp_included_base>(new IncludedType(std::forward<Args>(args)...)),
			.line      = 0,
			.local     = (forceNonLocal ? false : (mStack.size() ? mStack.back().local : true))
		});
	}

	bool in_local() const;
	fs::path current_directory() const;
	std::string current_source_name() const;
	unsigned int current_source_line() const;

private:
	void cleanup_stack();

private:
	std::vector<included_source> mStack;

	const lang::config*  mpConfig   = nullptr;
};

#endif // PP_INCLUDING_SSOURCE_H
