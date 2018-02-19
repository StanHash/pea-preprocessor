#include "main_args.h"

#include <functional>

#include "lib/string_scanning.h"
#include "lib/error/sscan_error.h"

#include "option/parameter_interface.h"

namespace args {

// TODO: move all of those elsewhere maybe

using argument_scanner = basic_range_scanner<const char* const*>;

static argument_scanner make_argument_scanner(int argc, const char* const* argv) {
	return argument_scanner(std::next(argv), std::next(argv, argc));
}

namespace ascan {

static void ensure_has_next(const argument_scanner& scanner) {
	if (scanner.at_end())
		throw std::runtime_error("reached end of program arguments unexpectedly");
}

} // namespace ascan

template<typename ConfigType>
struct option_definition {
	const char* name;
	const char* usage;
	const char* desc;
	std::function<void(ConfigType&, argument_scanner&)> handler;
};

static const option_definition<lang::config> programOptions[] {
	{
		"o", "-o <file>", "sets output (defaults to stdout)", [] (lang::config& config, argument_scanner& scanner) {
			ascan::ensure_has_next(scanner);
			config.pp.output = scanner.consume();
		}
	}, {
		"E", "-E", "disables output", [] (lang::config& config, argument_scanner& scanner) {
			config.pp.emulate = true;
		}
	}, {
		"D", "-D <name>", "adds preprocessor definition", [] (lang::config& config, argument_scanner& scanner) {
			ascan::ensure_has_next(scanner);
			const char* value = scanner.consume();
			config.pp.macro.extraDefines.emplace_back(value, value + std::strlen(value));
		}
	}, {
		"I", "-I <directory>", "adds extra directory to search included files in", [] (lang::config& config, argument_scanner& scanner) {
			ascan::ensure_has_next(scanner);
			config.pp.include.extraIncludeDirectories.emplace_back(scanner.consume());
		}
	}, {
		"T", "-T <directory>", "adds extra directory to seatch tools in", [] (lang::config& config, argument_scanner& scanner) {
			ascan::ensure_has_next(scanner);
			config.pp.tool.extraToolDirectories.emplace_back(scanner.consume());
		}
	}, {
		"M", "-M", "enables generation of dependency Makefile (implies -E)", [] (lang::config& config, argument_scanner&) {
			config.pp.emulate = true;
			config.pp.makedep.enable = true;
			config.pp.makedep.noExtraIncludes = false;
		}
	}, {
		"MM", "-MM", "same as -M; doesn't add included files from extra directories (see -I)", [] (lang::config& config, argument_scanner&) {
			config.pp.emulate = true;
			config.pp.makedep.enable = true;
			config.pp.makedep.noExtraIncludes = true;
		}
	}, {
		"MD", "-MD", "enables generation of dependency Makefile as side-effect of regular preprocessing (doesn't imply -E)", [] (lang::config& config, argument_scanner&) {
			config.pp.emulate = false;
			config.pp.makedep.enable = true;
			config.pp.makedep.noExtraIncludes = false;
		}
	}, {
		"MMD", "-MMD", "same as -MD; doesn't add included files from extra directories (see -I)", [] (lang::config& config, argument_scanner&) {
			config.pp.emulate = false;
			config.pp.makedep.enable = true;
			config.pp.makedep.noExtraIncludes = true;
		}
	}, {
		"MG", "-MG", "when generating dependency Makefile, adds missing included files to dependency", [] (lang::config& config, argument_scanner&) {
			config.pp.makedep.noMissingFiles = false;
			config.pp.include.ignoreMissingFiles = true;
		}
	}, {
		"MP", "-MP", "when generating dependency Makefile, adds phony targets for each file listed (except the main input)", [] (lang::config& config, argument_scanner&) {
			config.pp.makedep.addPhonyTargetFiles = true;
		}
	}, {
		"MF", "-MF <file>", "when generating dependency Makefile, sets output (defaults to stdout)", [] (lang::config& config, argument_scanner& scanner) {
			ascan::ensure_has_next(scanner);
			config.pp.makedep.output = fs::path(scanner.consume());
		}
	}, {
		"MT", "-MT <name>", "when generating dependency Makefile, sets rule target", [] (lang::config& config, argument_scanner& scanner) {
			ascan::ensure_has_next(scanner);
			config.pp.makedep.target = fs::path(scanner.consume());
		}
	}
};

static parameter_interface make_parameter_interface(lang::config& config) {
	parameter_interface result;

	result.register_parameter("pp.emulate", parameter_interface::make_bool_handler(config.pp.emulate));
	result.register_parameter("pp.input",   parameter_interface::make_path_handler(config.pp.input));
	result.register_parameter("pp.output",  parameter_interface::make_path_handler(config.pp.output));

	result.register_parameter("pp.eaCompat.force",                  parameter_interface::make_bool_handler(config.pp.eaCompat.force));
	result.register_parameter("pp.eaCompat.enableDirectiveAliases", parameter_interface::make_bool_handler(config.pp.eaCompat.enableDirectiveAliases));
	result.register_parameter("pp.eaCompat.enableBinary",           parameter_interface::make_bool_handler(config.pp.eaCompat.enableBinary));
	result.register_parameter("pp.eaCompat.binaryToolName",         parameter_interface::make_string_handler(config.pp.eaCompat.binaryToolName));
	result.register_parameter("pp.eaCompat.binaryToolArguments",    parameter_interface::make_string_list_handler(config.pp.eaCompat.binaryToolArguments));

	result.register_parameter("pp.macro.enable",            parameter_interface::make_bool_handler(config.pp.macro.enable));
	result.register_parameter("pp.macro.extraDefines",      parameter_interface::make_string_list_handler(config.pp.macro.extraDefines));
	result.register_parameter("pp.macro.strictMacroFormat", parameter_interface::make_bool_handler(config.pp.macro.strictMacroFormat));

	result.register_parameter("pp.condition.enable", parameter_interface::make_bool_handler(config.pp.condition.enable));

	result.register_parameter("pp.include.enable",                  parameter_interface::make_bool_handler(config.pp.include.enable));
	result.register_parameter("pp.include.ignoreMissingFiles",      parameter_interface::make_bool_handler(config.pp.include.ignoreMissingFiles));
	result.register_parameter("pp.include.enableRootRelative",      parameter_interface::make_bool_handler(config.pp.include.enableRootRelative));
	result.register_parameter("pp.include.maxDepth",                parameter_interface::make_int_handler(config.pp.include.maxDepth));
	result.register_parameter("pp.include.extraIncludeDirectories", parameter_interface::make_path_list_handler(config.pp.include.extraIncludeDirectories));

	result.register_parameter("pp.tool.enable",               parameter_interface::make_bool_handler(config.pp.tool.enable));
	result.register_parameter("pp.tool.ignoreMissingTools",   parameter_interface::make_bool_handler(config.pp.tool.ignoreMissingTools));
	result.register_parameter("pp.tool.enableSystemTools",    parameter_interface::make_bool_handler(config.pp.tool.enableSystemTools));
	result.register_parameter("pp.tool.enableLocalTools",     parameter_interface::make_bool_handler(config.pp.tool.enableLocalTools));
	result.register_parameter("pp.tool.addToStdout",          parameter_interface::make_bool_handler(config.pp.tool.addToStdout));
	result.register_parameter("pp.tool.extraToolDirectories", parameter_interface::make_path_list_handler(config.pp.tool.extraToolDirectories));

	result.register_parameter("pp.makedep.enable",              parameter_interface::make_bool_handler(config.pp.makedep.enable));
	result.register_parameter("pp.makedep.noExtraIncludes",     parameter_interface::make_bool_handler(config.pp.makedep.noExtraIncludes));
	result.register_parameter("pp.makedep.noMissingFiles",      parameter_interface::make_bool_handler(config.pp.makedep.noMissingFiles));
	result.register_parameter("pp.makedep.noToolArguments",     parameter_interface::make_bool_handler(config.pp.makedep.noToolArguments));
	result.register_parameter("pp.makedep.outputIsTarget",      parameter_interface::make_bool_handler(config.pp.makedep.outputIsTarget));
	result.register_parameter("pp.makedep.addPhonyTargetFiles", parameter_interface::make_bool_handler(config.pp.makedep.addPhonyTargetFiles));
	result.register_parameter("pp.makedep.target",              parameter_interface::make_path_handler(config.pp.makedep.target));
	result.register_parameter("pp.makedep.output",              parameter_interface::make_path_handler(config.pp.makedep.output));

	return result;
}

bool should_show_help(int argc, char** argv) {
	if (argc < 2)
		return true;

	for (auto it = std::next(argv); it != std::next(argv, argc); ++it)
		if ((std::strcmp(*it, "-h") == 0) || (std::strcmp(*it, "--help") == 0))
			return true;

	return false;
}

void show_help(std::ostream& out, const char* programName) {
	out << "Usage:" << std::endl;
	out << "  " << programName << " <input> [options]..." << std::endl;

	lang::config config; // default

	std::size_t usageColSize = 0;

	for (const auto& option : programOptions)
		usageColSize = std::max<std::size_t>(std::strlen(option.usage), usageColSize);

	out << std::endl;
	out << "Options:" << std::endl;

	for (const auto& option : programOptions) {
		out << "  " << option.usage
			<< std::string(usageColSize - std::strlen(option.usage), ' ')
			<< " - " << option.desc
			<< std::endl;
	}

	out << std::endl;
	out << "In addition to using the above options, you can also directly modify the underlying configuration using the following argument format:" << std::endl;
	out << "  -.<parameter><operation><value>" << std::endl;

	out << std::endl;
	out << "Where <parameter> is the name of the parameter (see below for a complete list), <operation> either '=' or '+=', and <value> the value you wish to set/add" << std::endl;

	out << std::endl;
	out << "Examples:" << std::endl;
	out << "  -.pp.tool.enableSystemTools=true" << std::endl;
	out << "  -.pp.include.extraIncludeDirectories+=/usr/include" << std::endl;

	out << std::endl;
	out << "Default parameter values:" << std::endl;

	parameter_interface paramInterface = make_parameter_interface(config);
	paramInterface.print_values(out);
}

static void handle_parameter_operation_argument(parameter_interface& interface, cstring_scanner& scanner) {
	auto itStart = scanner.it();

	while (scanner.consume_if([] (char chr) -> bool {
		return std::isalnum(chr) || chr == '.' || chr == '_';
	}));

	std::string name(itStart, scanner.it());
	sscan::scan_whitespaces(scanner);

	if (scanner.consume('=')) {
		interface.set_value(name, std::string(sscan::scan_whitespaces(scanner), scanner.end()));
		return;
	}

	if (sscan::consume_cstring(scanner, "+=")) {
		interface.add_value(name, std::string(sscan::scan_whitespaces(scanner), scanner.end()));
		return;
	}

	throw sscan_error(scanner, "Unknown parameter operation");
}

static void handle_program_option_argument(const char* option, lang::config& config, argument_scanner& scanner) {
	auto it = std::find_if(std::begin(programOptions), std::end(programOptions), [&option] (const option_definition<lang::config>& optionDefinition) {
		return std::strcmp(optionDefinition.name, option) == 0;
	});

	if (it == std::end(programOptions))
		throw std::runtime_error(string_builder("unknown program option '-", option, "'")());

	try {
		(it->handler)(config, scanner);
	} catch (const std::exception& e) {
		throw std::runtime_error(string_builder(" option \'-", it->name, "\' - ", e.what())());
	}
}

void args_to_config(lang::config& config, int argc, char** argv) {
	parameter_interface interface = make_parameter_interface(config);
	argument_scanner argsScanner = make_argument_scanner(argc, argv);

	while (!argsScanner.at_end()) {
		auto scanner = sscan::make_cstring_scanner(argsScanner.consume());

		if (sscan::consume_cstring(scanner, "-."))
			handle_parameter_operation_argument(interface, scanner);
		else if (scanner.consume('-'))
			handle_program_option_argument(scanner.it(), config, argsScanner);
		else
			config.pp.input = scanner.begin();
	}
}

} // namespace args
