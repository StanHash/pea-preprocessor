#ifndef FILE_STRING_SOURCE_H
#define FILE_STRING_SOURCE_H

#include <fstream>

#include "lib/ssource/stream_string_source.h"
#include "lib/string_building.h"

template<typename CharType>
class basic_file_string_source : public basic_stream_string_source<CharType> {
public:
	basic_file_string_source(const char* fileName)
		: basic_stream_string_source<CharType>(mFileStream) {
		mFileStream.open(fileName);

		if (!mFileStream.is_open())
			throw std::runtime_error(string_builder("couldn't open \"", fileName, "\"")());
	}

	basic_file_string_source(const std::string& fileName)
		: basic_file_string_source<CharType>(fileName.c_str()) {}

	~basic_file_string_source() { mFileStream.close(); }

private:
	std::basic_ifstream<CharType> mFileStream;
};

#endif // FILE_STRING_SOURCE_H
