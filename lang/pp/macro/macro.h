#ifndef MACRO_H
#define MACRO_H

#include <vector>

#include "lang/pp/macro/macro_signature.h"

class macro {
	friend class macro_library;

public:
	const lang::string_type& name() const { return mpSignature ? mpSignature->name() : msDummy; }
	const lang::string_type& format() const { return mpFormat ? *mpFormat : msDummy; }
	unsigned int argument_count() const { return mpSignature->argument_count(); }

	lang::string_type expand(const std::vector<lang::string_type>& arguments);

public:
	static lang::string_type format_format_simple(const lang::string_type& format, const std::vector<lang::string_type>& arguments);
	static lang::string_type format_format_strict(const lang::string_type& format, const std::vector<lang::string_type>& arguments);

protected:
	macro(const macro_signature* sig, const lang::string_type* fmt)
		: mpSignature(sig), mpFormat(fmt) {}

private:
	const macro_signature* mpSignature;
	const lang::string_type* mpFormat;

	static const lang::string_type msDummy;
};

#endif // MACRO_H
