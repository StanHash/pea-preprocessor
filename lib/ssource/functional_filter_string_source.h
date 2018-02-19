#ifndef FUNCTIONAL_FILTER_STRING_SOURCE_H
#define FUNCTIONAL_FILTER_STRING_SOURCE_H

#include <functional>

#include "filter_string_source.h"

template<typename CharType>
class basic_functional_filter_string_source : public basic_filter_string_source<CharType> {
public:
	using filter_function_type = std::function<std::basic_string<CharType>(std::basic_string<CharType>&&)>;

public:
	template<typename FunctionType>
	basic_functional_filter_string_source(FunctionType function, basic_string_source<CharType>& source)
		: basic_filter_string_source<CharType>(source), mFilterFunction(function) {}

protected:
	std::basic_string<CharType> filter_string(std::basic_string<CharType>&& in) override final {
		return mFilterFunction(std::move(in));
	}

private:
	filter_function_type mFilterFunction;
};

using functional_filter_string_source  = basic_functional_filter_string_source<char>;
using functional_filter_wstring_source = basic_functional_filter_string_source<wchar_t>;

#endif // FUNCTIONAL_FILTER_STRING_SOURCE_H
