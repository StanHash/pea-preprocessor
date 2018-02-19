#ifndef PP_EA_NOCOMMENT_SSOURCE_H
#define PP_EA_NOCOMMENT_SSOURCE_H

#include "lang/types.h"

class pp_nocomment_ssource : public lang::ssource_type {
public:
	pp_nocomment_ssource(lang::ssource_type& source);

	lang::string_type get_string() override;
	bool at_end() const override;

private:
	lang::string_type get_string_raw();

private:
	static const lang::string_type msLineCommentOpener;
	static const lang::string_type msMultilineCommentOpener;
	static const lang::string_type msMultilineCommentEnder;

private:
	lang::ssource_type* const mpSource;
	int mMultilineCommentDepth = 0;
};

#endif // PP_EA_NOCOMMENT_SSOURCE_H
