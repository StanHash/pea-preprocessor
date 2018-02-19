#ifndef STRING_STRING_SOURCE_H
#define STRING_STRING_SOURCE_H

#include "stream_string_source.h"

template<typename CharType>
class basic_string_string_source : public basic_stream_string_source<CharType> {
public:
	basic_string_string_source(const std::basic_string<CharType>& str)
		: stream_string_source(mStringStream), mString(str), mStringStream(mString) {}

	basic_string_string_source(std::basic_string<CharType>&& str)
		: stream_string_source(mStringStream), mString(std::move(str)), mStringStream(mString) {}

	basic_string_string_source(const std::basic_string<CharType>& str, CharType delim)
		: stream_string_source(mStringStream, delim), mString(str), mStringStream(mString) {}

	basic_string_string_source(std::basic_string<CharType>&& str, CharType delim)
		: stream_string_source(mStringStream, delim), mString(std::move(str)), mStringStream(mString) {}

private:
	std::basic_string<CharType> mString;
	std::basic_stringstream<CharType> mStringStream;
};

using string_string_source    = basic_string_string_source<char>;
using string_wstring_source   = basic_string_string_source<wchar_t>;
using string_u16string_source = basic_string_string_source<char16_t>;
using string_u32string_source = basic_string_string_source<char32_t>;

#endif // STRING_STRING_SOURCE_H
