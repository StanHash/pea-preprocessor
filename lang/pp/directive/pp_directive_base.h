#ifndef PP_DIRECTIVE_BASE_H
#define PP_DIRECTIVE_BASE_H

#include "lang/types.h"

class pp_directive_base {
public:
	virtual ~pp_directive_base() {}

	virtual void handle(lang::sscanner_type&) = 0;
	virtual bool ignores_selection() const { return false; }
};

#endif // PP_DIRECTIVE_BASE_H
