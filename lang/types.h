#ifndef TYPES_H
#define TYPES_H

#include "lib/ssource/string_source.h"
#include "lib/string_scanning.h"
// #include "lib/filesystem.h"

// let's (not) use the system pathname char type
// using char_type = fs::path::value_type;

// lang::string_type is the string type for the processed strings

namespace lang {

//#define LANG_LITERAL L""
//using char_type = wchar_t;

#define LANG_LITERAL u8""
using char_type = char;

// SLITERAL macro usage: LANG_LITERAL "String"

using string_type   = std::basic_string<char_type>;
using ssource_type  = basic_string_source<char_type>;
using sscanner_type = basic_string_scanner<char_type>;

} // namespace lang

#endif // TYPES_H
