#ifndef PP_INCLUDED_BASE_H
#define PP_INCLUDED_BASE_H

#include "lib/filesystem.h"

#include "lang/types.h"

struct pp_included_base {
	virtual ~pp_included_base();
	virtual lang::ssource_type& get_ssource() = 0;

	fs::path directory;
	std::string name;
};

#endif // PP_INCLUDED_BASE_H
