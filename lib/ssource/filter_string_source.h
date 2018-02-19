#ifndef FILTER_STRING_SOURCE_H
#define FILTER_STRING_SOURCE_H

#include "string_source.h"

template<typename CharType>
class basic_filter_string_source : public basic_string_source<CharType> {
public:
	basic_filter_string_source(basic_string_source<CharType>& source)
		: mpSource(&source) {}

	std::basic_string<CharType> get_string() override { return filter_string(mpSource->get_string()); }
	bool at_end() const override { return mpSource->at_end(); }

protected:
	virtual std::basic_string<CharType> filter_string(std::basic_string<CharType>&&) = 0;

private:
	basic_string_source<CharType>* const mpSource;
};

using filter_string_source    = basic_filter_string_source<char>;
using filter_wstring_source   = basic_filter_string_source<wchar_t>;
using filter_u16string_source = basic_filter_string_source<char16_t>;
using filter_u32string_source = basic_filter_string_source<char32_t>;

#endif // FILTER_STRING_SOURCE_H
