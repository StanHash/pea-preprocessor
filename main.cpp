#include <iostream>
#include <fstream>

#include "lang/config.h"

#include "lang/pp/ssource/pp_including_ssource.h"
#include "lang/pp/ssource/pp_nocomment_ssource.h"

#include "lang/pp/include/pp_included_file_ssource.h"

#include "lang/pp/pp_dependency_observer.h"

#include "lang/pp/pp_main_directive_processor.h"
#include "lang/error/include_error.h"

#include "main_args.h"

/*
 *
 * THE MASTER GENERAL BORING TODO LIST
 * In order of priority
 *
 * DONE: #incbin & #incext
 * TODO: "#define __strict" & "#define __sticky" - to overwrite pp.macro.strictMacroFormat
 * TODO: pp.include.skipMissingFiles - to skip missing files
 * TODO: "#include __optional" & "#include __required"- to overwrite pp.include.skipMissingFiles
 * TODO: "#if[n]def __<something>" - to not have to check for a specific parameter count
 * TODO; "#undef __<something>" - to undefine all macros of that name (regardless of argc)
 * TODO: some option to allow newline escapes (using character '\' maybe probably)
 * TODO: "#if <expression>" for generic conditions
 * TODO: inline directives (proposed syntax: #{<directive>})
 *
 */

template<typename CharType>
std::basic_ostream<CharType>& get_cout() { throw 0; }

template<> std::basic_ostream<wchar_t>& get_cout<wchar_t>() { return std::wcout; }
template<> std::basic_ostream<char>&    get_cout<char>()    { return std::cout; }

template<typename FuncType>
static void for_ostream_of(const fs::path& outFile, FuncType func) {
	if (outFile.empty())
		func(get_cout<lang::char_type>());
	else {
		std::basic_ofstream<lang::char_type> fStream;
		fStream.open(outFile.native());

		if (!fStream.is_open())
			throw std::runtime_error(std::string("Couldn't open file for write: ").append(outFile.string()));

		func(fStream);
	}
}

int main(int argc, char** argv) {
	if (args::should_show_help(argc, argv)) {
		args::show_help(std::cout, argv[0]);
		return 0;
	}

	lang::config config;

	try {
		args::args_to_config(config, argc, argv);
	} catch (const std::exception& e) {
		std::cerr << "ERROR - argument scanning failed - " << e.what() << std::endl;
		return 1;
	}

	if (config.pp.eaCompat.force) {
		config.pp.eaCompat.enableBinary           = true; // #incbin, #incext
		config.pp.eaCompat.enableDirectiveAliases = true; // #inctext, #inctevent

		config.pp.macro.strictMacroFormat         = false; // #define Macro(a, b) abracadabra; Marco(0, 1) -> 01r0c0d01r0

		config.pp.tool.addToStdout                = true; // #incext implies --to-stdout

		config.pp.include.enableRootRelative      = false;
		config.pp.tool.enableLocalTools           = false;
		config.pp.tool.enableSystemTools          = false;
	}

	macro_library library;
	pp_dependency_observer dependencyMaker(&config);

	pp_including_ssource mainSource(&config);
	pp_nocomment_ssource source(mainSource);

	pp_main_directive_processor dp(
		&config,
		mainSource,
		dependencyMaker,
		library
	);

	try {
		for (const auto& define : config.pp.macro.extraDefines)
			library.define_macro(define);

		if (config.pp.input.empty())
			throw std::runtime_error("no input");

		if (!fs::exists(config.pp.input))
			throw no_include_error(config.pp.input.c_str());

		dependencyMaker.add_dependency(config.pp.input);
		mainSource.include<pp_included_file_ssource>(false, config.pp.input);
	} catch (const std::exception& e) {
		std::cerr << "ERROR - " << e.what() << std::endl;
		return 1;
	}

	try {
		if (config.pp.emulate) {
			while (!source.at_end())
				dp.process_directive(source.get_string());
		} else {
			for_ostream_of(config.pp.output, [&dp, &source, &library] (std::basic_ostream<lang::char_type>& out) {
				while (!source.at_end()) {
					lang::string_type str = source.get_string();

					if (dp.process_directive(str))
						continue;

					if (dp.in_selection())
						out << library.expand(std::move(str)) << std::endl;
				}
			});
		}

		if (config.pp.makedep.enable) {
			for_ostream_of(config.pp.makedep.output, [&dependencyMaker] (std::basic_ostream<lang::char_type>& out) {
				dependencyMaker.generate_makefile(out);
			});
		}
	} catch (const std::exception& e) {
		std::cerr << "[";

		if (mainSource.current_directory().size())
			std::cerr << mainSource.current_directory().string() << ":";

		lang::string_type srcName = mainSource.current_source_name();

		std::cerr << std::string(srcName.begin(), srcName.end())
				  << "(" << mainSource.current_source_line() << ")] ERROR - "
				  << e.what() << std::endl;

		return 1;
	}

	return 0;
}
