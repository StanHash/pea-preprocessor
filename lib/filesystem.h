#ifndef WRAPPER_FILESYSTEM_H
#define WRAPPER_FILESYSTEM_H

// This is just a wrapper around either boost::filesystem (for C++11 & C++14) or std::filesystem (C++17 +)

// Check for C++17
#if __cplusplus >= 201703L
	#include <filesystem>
	namespace fs = std::filesystem;
#else
	#include <boost/filesystem.hpp>
	namespace fs = boost::filesystem;
#endif

#endif // WRAPPER_FILESYSTEM_H
