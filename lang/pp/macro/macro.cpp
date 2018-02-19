#include "macro.h"

#include <algorithm>
#include <array>

#include "lib/string_scanning.h"

const lang::string_type macro::msDummy;

lang::string_type macro::expand(const std::vector<lang::string_type>& arguments) {
	lang::string_type result(*mpFormat);
	result.reserve(result.size() * 2);

	for (unsigned int i=0; i<arguments.size(); ++i) {
		std::array<lang::char_type, 4> searchValue {
			'#',
			(lang::char_type)('0' + (i / 10)),
			(lang::char_type)('0' + (i % 10)),
			'#'
		};

		auto it = result.begin();
		while ((it = std::search(result.begin(), result.end(), searchValue.begin(), searchValue.end())) != result.end())
			result.replace(it, std::next(it, searchValue.size()), arguments[i]);
	}

	return result;
}

lang::string_type macro::format_format_simple(const lang::string_type& format, const std::vector<lang::string_type>& arguments) {
	lang::string_type result(format);

	for (unsigned int i=0; i<arguments.size(); ++i) {
		if (arguments[i].empty())
			continue;

		std::array<lang::char_type, 4> replaceValue {
			'#',
			(lang::char_type)('0' + (i / 10)),
			(lang::char_type)('0' + (i % 10)),
			'#'
		};

		auto it = result.begin();
		while ((it = std::search(result.begin(), result.end(), arguments[i].begin(), arguments[i].end())) != result.end())
			result.replace(it, std::next(it, arguments[i].size()), replaceValue.begin(), replaceValue.end());
	}

	return result;
}

lang::string_type macro::format_format_strict(const lang::string_type& format, const std::vector<lang::string_type>& arguments) {
	lang::string_type result(format);
	auto scanner = make_scanner(result.begin(), result.end());

	while (!scanner.at_end()) {
		while (!scanner.match_if(char_utility<lang::char_type>::is_alpha_or_underscore))
			if (!scanner.consume())
				return result;

		auto itStart = scanner.it();
		auto itEnd   = sscan::scan_identifier(scanner);

		for (unsigned int i=0; i<arguments.size(); ++i) {
			if (std::distance(itStart, itEnd) != std::distance(arguments[i].begin(), arguments[i].end()))
				continue;

			if (!std::equal(itStart, itEnd, arguments[i].begin()))
				continue;

			std::array<lang::char_type, 4> replaceValue {
				'#',
				(lang::char_type)('0' + (i / 10)),
				(lang::char_type)('0' + (i % 10)),
				'#'
			};

			auto scanPosition = itStart - result.begin();
			result.replace(itStart, itEnd, replaceValue.begin(), replaceValue.end());
			scanner = make_scanner(std::next(result.begin(), scanPosition), result.end());
			break;
		}
	}

	return result;
}
