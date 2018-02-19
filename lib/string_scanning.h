#ifndef STRING_SCANNING_H
#define STRING_SCANNING_H

#include <string>
#include <cstring>

#include "lib/scanner.h"
#include "lib/char_utility.h"

/*!
 * utility functions for messing with scanners on char ranges (aka strings)
 */
namespace sscan {

template<typename IteratorType>
IteratorType scan_identifier(basic_range_scanner<IteratorType>& scanner) {
	if (scanner.match_if(char_utility<typename std::iterator_traits<IteratorType>::value_type>::is_alpha_or_underscore))
		while (scanner.consume_if(char_utility<typename std::iterator_traits<IteratorType>::value_type>::is_alphanum_or_underscore));

	return scanner.it();
}

template<typename IteratorType>
IteratorType scan_number_or_identifier(basic_range_scanner<IteratorType>& scanner) {
	while (scanner.consume_if(char_utility<typename std::iterator_traits<IteratorType>::value_type>::is_alphanum_or_underscore));
	return scanner.it();
}

template<typename IteratorType>
std::basic_string<typename std::iterator_traits<IteratorType>::value_type> read_identifier(basic_range_scanner<IteratorType>& scanner) {
	IteratorType start = scanner.it();
	return std::basic_string<typename std::iterator_traits<IteratorType>::value_type>(start, scan_identifier(scanner));
}

template<typename IteratorType>
std::basic_string<typename std::iterator_traits<IteratorType>::value_type> read_number_or_identifier(basic_range_scanner<IteratorType>& scanner) {
	IteratorType start = scanner.it();
	return std::basic_string<typename std::iterator_traits<IteratorType>::value_type>(start, scan_number_or_identifier(scanner));
}

template<typename IteratorType>
std::basic_string<typename std::iterator_traits<IteratorType>::value_type> read_string_literal(basic_range_scanner<IteratorType>& scanner) {
	std::basic_string<typename std::iterator_traits<IteratorType>::value_type> result;

	if (scanner.consume('"')) {
		result.reserve(std::distance(scanner.it(), scanner.end()));

		typename std::iterator_traits<IteratorType>::value_type chr = 0;
		bool escaped = false;

		while (chr = scanner.consume(), chr && (chr != '"' || escaped)) {
			if (escaped) {
				escaped = false;
				switch (chr) {
				case 't':
					result.push_back('\t');
					break;
				case 'n':
					result.push_back('\n');
					break;
				default:
					result.push_back(chr);
					break;
				}
			} else if (chr == '\\')
				escaped = true;
			else
				result.push_back(chr);
		}
	}

	return result;
}

template<typename IteratorType>
IteratorType scan_whitespaces(basic_range_scanner<IteratorType>& scanner) {
	while (scanner.consume_if(char_utility<typename std::iterator_traits<IteratorType>::value_type>::is_space));
	return scanner.it();
}

template<typename IteratorType, typename CharType>
bool match_cstring(basic_range_scanner<IteratorType>& scanner, const CharType* cstring) {
	return scanner.match_range(cstring, cstring + std::strlen(cstring));
}

template<typename IteratorType, typename CharType>
bool consume_cstring(basic_range_scanner<IteratorType>& scanner, const CharType* cstring) {
	return scanner.consume_range(cstring, cstring + std::strlen(cstring));
}

template<typename CharType>
basic_range_scanner<const CharType*> make_cstring_scanner(const CharType* cstring) {
	// TODO: generic wrapper around strlen/wcslen maybe
	return basic_range_scanner<const CharType*>(cstring, cstring + std::strlen(cstring));
}

} // namespace sscan

template<typename CharType>
using basic_string_scanner = basic_range_scanner<typename std::basic_string<CharType>::const_iterator>;

template<typename CharType>
using basic_cstring_scanner = basic_range_scanner<const CharType*>;

using string_scanner  = basic_string_scanner<char>;
using wstring_scanner = basic_string_scanner<wchar_t>;

using cstring_scanner  = basic_cstring_scanner<char>;
using wcstring_scanner = basic_cstring_scanner<wchar_t>;

#endif // STRING_SCANNING_H
