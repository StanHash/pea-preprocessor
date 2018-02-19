#ifndef SSCAN_ERROR_H
#define SSCAN_ERROR_H

#include <stdexcept>

#include "lib/string_scanning.h"
#include "lib/string_building.h"

class sscan_error : public std::runtime_error {
public:
	template<typename IteratorType>
	sscan_error(basic_range_scanner<IteratorType> scanner)
		: std::runtime_error(string_builder("failed to scan:\n", make_scan_error_string(scanner))()) {}

	template<typename IteratorType>
	sscan_error(basic_range_scanner<IteratorType> scanner, const std::string& str)
		: std::runtime_error(string_builder("failed to scan:\n", make_scan_error_string(scanner), "\n\t", str)()) {}

	template<typename IteratorType>
	sscan_error(basic_range_scanner<IteratorType> scanner, const char* str)
		: std::runtime_error(string_builder("failed to scan:\n", make_scan_error_string(scanner), "\n\t", str)()) {}

	template<typename IteratorType>
	static std::string make_scan_error_string(basic_range_scanner<IteratorType> scanner) {
		std::string result;

		std::string::size_type size = std::distance(scanner.begin(), scanner.end());
		result.reserve(size * 2 + 4);

		result.append(1, '\t');
		result.append(scanner.begin(), scanner.end());

		result.append("\n\t");
		result.append(size+1, ' ');
		result[3 + size + std::distance(scanner.begin(), scanner.it())] = '^';

		return result;
	}
};

#endif // SSCAN_ERROR_H
