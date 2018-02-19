#include "pp_tool_ssource.h"

#include <algorithm>

#include <boost/asio.hpp>
#include <boost/process.hpp>

#include "lib/string_scanning.h"

#include "lang/error/tool_error.h"

pp_tool_ssource::pp_tool_ssource(
	const lang::config* pConfig,
	const fs::path& toolBinary,
	const fs::path& wd,
	const std::vector<std::string>& arguments
) {
	std::vector<std::string> realArguments(arguments.begin(), arguments.end());

	if (pConfig->pp.tool.addToStdout)
		realArguments.push_back("--to-stdout");

	string_builder builder(toolBinary.filename().string());

	for (const std::string& arg : arguments)
		builder.append(" ", arg);

	name = builder();
	directory = wd;

	_exec_tool(toolBinary, wd, realArguments);
}

void pp_tool_ssource::_exec_tool(
	const fs::path& toolBinary,
	const fs::path& wd,
	const std::vector<std::string>& arguments
) {
	// Welcome to me being bad at code
	// I really want to be able to make this a bit better
	// Like by having the whole thing work in parallel
	// And reading output progressively such as with a standard stream
	// So that we don't have to wait for the whole tool process to end
	// Before reading the first bit of output it gave us
	// But apparently (according to the Boost.Process doc), that's _unsafe_
	// And I'm probably too new to this asio stuff to know how to pull it off that way
	// Maybe one day
	// TODO ^

	try {
		std::string data;

		{
			boost::asio::io_service ios;

			std::future<std::string> out;
			std::future<std::string> err;

			boost::process::child c(
				toolBinary,
				arguments,
				boost::process::start_dir = wd,
				boost::process::std_in.close(),
				boost::process::std_out > out,
				boost::process::std_err > err,
				ios
			);

			ios.run();

			std::string errString = err.get();

			if (errString.size())
				throw std::runtime_error(errString);

			data = out.get();
		}

		auto scanner = make_scanner(data);

		while (!scanner.at_end()) {
			auto itStart = scanner.it();

			// read line
			while (!scanner.at_end() && !scanner.consume('\n')) // TODO: too many hardcoded newlines
				scanner.consume();

			// emplace line
			mData.emplace(itStart, scanner.it());

			// trim blank characters at the end of the line
			while (mData.back().size() && char_utility<lang::char_type>::is_space(mData.back().back()))
				mData.back().pop_back();
		}
	} catch (const std::exception& e) {
		throw tool_error(e.what());
	}
}
