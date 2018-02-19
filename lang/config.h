#ifndef LANG_CONFIG_H
#define LANG_CONFIG_H

#include <vector>

#include "types.h"
#include "lib/filesystem.h"

namespace lang {

struct config {
	struct {
		bool emulate = false;

		fs::path input;
		fs::path output;

		struct {
			bool force                   = false; // forces some config values on or off when true

			bool enableDirectiveAliases  = true;
			bool enableBinary            = true;

			lang::string_type binaryToolName = LANG_LITERAL "bin2ea";
			std::vector<std::string> binaryToolArguments = {
				"%%",
				"-byte",
				"--to-stdout"
			};
		} eaCompat;

		struct {
			bool enable            = true; // disables #define, #undef, #if[n]def & macro expansion if false

			bool strictMacroFormat = false; // how "#define Macro(A, B) AB" is handled // TODO: better desc

			std::vector<lang::string_type> extraDefines;
		} macro;

		struct {
			bool enable = true; // disables #else, #endif, #if & direvatives if false
		} condition;

		struct {
			bool enable             = true; // disables #include if false

			bool ignoreMissingFiles = false; // skips over included missing files

			bool enableRootRelative = false; // Relative to the main file's directory
			unsigned int maxDepth   = 100; // Maximum include depth (to prevent infinite include loops)

			std::vector<fs::path> extraIncludeDirectories;
		} include;

		struct {
			bool enable             = true; // disables #include_tool & direvatives if false

			bool ignoreMissingTools = false; // skips over tool calls to missing tools

			bool enableSystemTools  = false; // looks in PATH for extra tools
			bool enableLocalTools   = false; // looks in current directory for extra tools

			bool addToStdout        = true; // adds --to-stdout to tool arguments

			std::vector<fs::path> extraToolDirectories; // list of extra directories containing tools
		} tool;

		struct {
			bool enable              = false; // Enables Makefile dependency output

			bool noExtraIncludes     = true; // Won't add non-local includes to dependency list
			bool noMissingFiles      = true; // Won't add include files that are missing // TODO: make this relevant
			bool noToolArguments     = true; // Won't add files referenced in tool call arguments

			bool outputIsTarget      = true;  // Adds Makefile output (pp.makedep.target) to Makefile target
			bool addPhonyTargetFiles = false; // Adds empty targets for each dependent file (except the main one)

			fs::path output; // Makefile output (empty for stdout)
			fs::path target; // Dependency target
		} makedep;
	} pp;
};

} // namespace lang

#endif // LANG_CONFIG_H
