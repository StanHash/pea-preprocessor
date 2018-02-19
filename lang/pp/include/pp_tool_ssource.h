#ifndef PP_TOOL_SSOURCE_H
#define PP_TOOL_SSOURCE_H

#include <queue>

#include "lang/pp/include/pp_included_base.h"
#include "lang/config.h"

class pp_tool_ssource : public lang::ssource_type, public pp_included_base {
public:
	pp_tool_ssource(const lang::config* pConfig,
		const fs::path& toolBinary,
		const fs::path& wd,
		const std::vector<std::string>& arguments
	);

	lang::string_type get_string() { lang::string_type str = std::move(mData.front()); mData.pop(); return str; }
	bool at_end() const { return mData.empty(); }

	lang::ssource_type& get_ssource() final { return *this; }

private:
	void _exec_tool(
		const fs::path& toolBinary,
		const fs::path& wd,
		const std::vector<std::string>& arguments
	);

private:
	std::queue<lang::string_type> mData;
};

#endif // PP_TOOL_SSOURCE_H
