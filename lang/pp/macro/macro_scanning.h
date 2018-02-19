#ifndef MACRO_SCANNING_H
#define MACRO_SCANNING_H

#include <vector>

#include "lib/error/sscan_error.h"
#include "lib/string_scanning.h"

#include "macro_signature.h"

namespace sscan {
namespace macro {

namespace _ {

template<typename IteratorType, typename FuncType>
void for_each_define_argument(basic_range_scanner<IteratorType>& scanner, FuncType func) {
	if (scanner.consume('(')) {
		while (sscan::scan_whitespaces(scanner), !scanner.at_end()) {
			// NOTE: using scan_number_or_identifier instead of scan_identifier for compatibility with the imfamously broken EAstdlib (specifically FE8 Definitions.txt)

			auto it = scanner.it();
			func(it, sscan::scan_number_or_identifier(scanner));

			if (sscan::scan_whitespaces(scanner), !scanner.consume(','))
				break; // no comma, end of list
		}

		if (!scanner.consume(')'))
			throw sscan_error(scanner, "Wrong macro definition (expected ',' or ')')");
	}
}

template<typename IteratorType>
std::vector<std::basic_string<typename std::iterator_traits<IteratorType>::value_type>> read_define_arguments(basic_range_scanner<IteratorType>& scanner) {
	std::vector<std::basic_string<typename std::iterator_traits<IteratorType>::value_type>> result;

	for_each_define_argument(scanner, [&result] (IteratorType a, IteratorType b) {
		result.emplace_back(a, b);
	});

	return result;
}

template<typename IteratorType>
unsigned int count_define_arguments(basic_range_scanner<IteratorType>& scanner) {
	unsigned int i = 0;

	for_each_define_argument(scanner, [&i] (IteratorType, IteratorType) {
		++i;
	});

	return i;
}

} // namespace _

template<typename IteratorType>
std::vector<std::basic_string<typename std::iterator_traits<IteratorType>::value_type>> call_arguments(basic_range_scanner<IteratorType>& scanner) {
	std::vector<std::basic_string<typename std::iterator_traits<IteratorType>::value_type>> result;

	// argument parsing for macro invocation is more complex than for macro definition
	// because expressions like "SomeMacro(SomeOtherMacro(Stuff, Keks), Thing)" needs to be valid

	if (scanner.consume('(')) {
		while (!scanner.at_end()) {
			sscan::scan_whitespaces(scanner);
			unsigned int ignoreDepth = 0;

			if (scanner.match('"')) {
				result.push_back(sscan::read_string_literal(scanner));
				sscan::scan_whitespaces(scanner);
			} else {
				auto itStart = scanner.it();

				while (!scanner.at_end()) {
					if (scanner.consume('(')) {
						++ignoreDepth;
						continue;
					}

					if (ignoreDepth && scanner.consume(')')) {
						--ignoreDepth;
						continue;
					}

					if (!ignoreDepth)
						if (scanner.match(',') || scanner.match(')'))
							break;

					scanner.consume();
				}

				result.emplace_back(itStart, scanner.it());

				// trim ending whitespaces
				while (result.back().size() && char_utility<typename std::iterator_traits<IteratorType>::value_type>::is_space(result.back().back()))
					result.back().pop_back();
			}

			if (scanner.consume(','))
				continue;

			if (scanner.consume(')'))
				break;

			throw sscan_error(scanner, "Wrong macro invocation (expected ',' or ')')");
		}
	}

	return result;
}

template<typename IteratorType>
complete_macro_signature read_complete_signature(basic_range_scanner<IteratorType>& scanner) {
	lang::string_type name;
	std::vector<lang::string_type> args;

	// NOTE: using scan_number_or_identifier instead of scan_identifier for compatibility with the imfamously broken EAstdlib (specifically FE8 Definitions.txt)

	if ((name = sscan::read_number_or_identifier(scanner)).empty())
		throw sscan_error(scanner, "Wrong macro name (expected identifier)");

	if ((args = _::read_define_arguments(scanner)).size() > 100)
		throw sscan_error(scanner, "Too many arguments (sorry)");

	return complete_macro_signature(std::move(name), std::move(args));
}

template<typename IteratorType>
macro_signature read_signature(basic_range_scanner<IteratorType>& scanner) {
	lang::string_type name;
	unsigned int argc;

	// NOTE: using scan_number_or_identifier instead of scan_identifier for compatibility with the imfamously broken EAstdlib (specifically FE8 Definitions.txt)

	if ((name = sscan::read_number_or_identifier(scanner)).empty())
		throw sscan_error(scanner, "Wrong macro name (expected identifier)");

	if ((argc = _::count_define_arguments(scanner)) > 100)
		throw sscan_error(scanner, "Too many arguments (sorry)");

	return macro_signature(std::move(name), argc);
}

} // namespace macro
} // namespace sscan

#endif // MACRO_SCANNING_H
