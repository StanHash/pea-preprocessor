#ifndef MACRO_LIBRARY_H
#define MACRO_LIBRARY_H

#include <map>

#include "lang/pp/macro/macro.h"
#include "lang/pp/macro/macro_signature.h"

class macro_library {
public:
	macro_library()
		: mLibrary(macro_signature::compare) {}

	inline bool define_macro(macro_signature&& signature)                                    { return define_macro(std::move(signature), LANG_LITERAL ""); }
	inline bool define_macro(const macro_signature& signature)                               { return define_macro(signature, LANG_LITERAL ""); }
	inline bool define_macro(const lang::string_type& name)                                  { return define_macro(name, 0, LANG_LITERAL ""); }
	inline bool define_macro(const lang::string_type& name, const lang::string_type& format) { return define_macro(name, 0, format); }
	inline bool define_macro(const lang::string_type& name, lang::string_type&& format)      { return define_macro(name, 0, std::move(format)); }
	inline bool define_macro(lang::string_type&& name)                                       { return define_macro(std::move(name), 0, LANG_LITERAL ""); }
	inline bool define_macro(lang::string_type&& name, const lang::string_type& format)      { return define_macro(std::move(name), 0, format); }
	inline bool define_macro(lang::string_type&& name, lang::string_type&& format)           { return define_macro(std::move(name), 0, std::move(format)); }

	bool define_macro(const lang::string_type& name, int argc, const lang::string_type& format);
	bool define_macro(const lang::string_type& name, int argc, lang::string_type&& format);
	bool define_macro(lang::string_type&& name, int argc, const lang::string_type& format);
	bool define_macro(lang::string_type&& name, int argc, lang::string_type&& format);
	bool define_macro(macro_signature&& signature, lang::string_type&& format);
	bool define_macro(macro_signature&& signature, const lang::string_type& format);
	bool define_macro(const macro_signature& signature, lang::string_type&& format);
	bool define_macro(const macro_signature& signature, const lang::string_type& format);

	bool undefine_macro(const lang::string_type& name);
	bool undefine_macro(const lang::string_type& name, int argc);
	bool undefine_macro(const macro_signature& signature);

	bool is_macro_defined(const lang::string_type& name) const;
	bool is_macro_defined(const lang::string_type& name, int argc) const;
	bool is_macro_defined(const macro_signature& signature) const;

	lang::string_type expand(const lang::string_type& line) const;
	lang::string_type expand(lang::string_type&& line) const;

	macro get_macro(const lang::string_type& name) const;
	macro get_macro(const lang::string_type& name, int argc) const;

protected:
	std::map<macro_signature, lang::string_type, decltype(&macro_signature::compare)>::iterator find(const lang::string_type& name, int argc);
	std::map<macro_signature, lang::string_type, decltype(&macro_signature::compare)>::const_iterator find(const lang::string_type& name, int argc) const;

	void expand_private(lang::string_type& toExpand) const;

private:
	std::map<macro_signature, lang::string_type, decltype(&macro_signature::compare)> mLibrary;
};

#endif // MACRO_LIBRARY_H
