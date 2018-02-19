#ifndef STREAM_STRING_SOURCE_H
#define STREAM_STRING_SOURCE_H

#include <istream>

#include "string_source.h"

template<typename CharType>
class basic_stream_string_source : public basic_string_source<CharType> {
public:
	using istream_type = std::basic_istream<CharType>;
public:
	basic_stream_string_source(istream_type &input)
		: mpInput(&input), mPartDelimiter('\n') {}

	basic_stream_string_source(istream_type& input, CharType delim)
		: mpInput(&input), mPartDelimiter(delim) {}

	std::basic_string<CharType> get_string() override {
		std::basic_string<CharType> result;
		return std::getline(*mpInput, result, mPartDelimiter), result;
	}

	bool at_end() const override { return !mpInput || !(*mpInput) || mpInput->eof(); }

	void set_part_separator(CharType chr) { mPartDelimiter = chr; }
	CharType part_separator() const { return mPartDelimiter; }

private:
	istream_type* mpInput = nullptr;
	CharType mPartDelimiter = '\n';
};

using stream_string_source    = basic_stream_string_source<char>;
using stream_wstring_source   = basic_stream_string_source<wchar_t>;
using stream_u16string_source = basic_stream_string_source<char16_t>;
using stream_u32string_source = basic_stream_string_source<char32_t>;

#endif // STREAM_STRING_SOURCE_H
