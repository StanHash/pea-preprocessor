#ifndef PP_INCLUDED_FILE_SSOURCE_H
#define PP_INCLUDED_FILE_SSOURCE_H

#include "lib/ssource/file_string_source.h"

#include "lang/pp/include/pp_included_base.h"

class pp_included_file_ssource : public pp_included_base {
public:
	pp_included_file_ssource(const fs::path& fileName);

	lang::ssource_type& get_ssource() final { return mSource; }

private:
	basic_file_string_source<lang::char_type> mSource;
};

#endif // PP_INCLUDED_FILE_SSOURCE_H
