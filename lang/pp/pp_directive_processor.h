#ifndef PP_DIRECTIVE_PROCESSOR_H
#define PP_DIRECTIVE_PROCESSOR_H

#include <map>

#include "lang/pp/directive/pp_directive_base.h"

class pp_directive_processor {
public:
	/*!
	 * \brief process_directive
	 * \param string The string to process
	 * \return true if processed a directive
	 */
	bool process_directive(const lang::string_type& string);

	virtual bool in_selection() const = 0;

protected:
	std::map<lang::string_type, pp_directive_base*>& directives() { return mDirectives; }
	const std::map<lang::string_type, pp_directive_base*>& directives() const { return mDirectives; }

private:
	std::map<lang::string_type, pp_directive_base*> mDirectives;
};

#endif // PP_DIRECTIVE_PROCESSOR_H
