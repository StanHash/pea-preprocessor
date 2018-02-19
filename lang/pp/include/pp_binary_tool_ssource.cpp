#include "pp_binary_tool_ssource.h"

#include <algorithm>

#include <boost/asio.hpp>
#include <boost/process.hpp>

#include "lang/error/tool_error.h"

pp_binary_tool_ssource::pp_binary_tool_ssource(
	const lang::config* pConfig,
	const fs::path& binaryToolBinary,
	const fs::path& toolBinary,
	const fs::path& wd,
	const std::vector<std::string>& arguments
) : mpConfig(pConfig) {
	std::vector<std::string> realArguments(arguments.begin(), arguments.end());

	if (mpConfig->pp.tool.addToStdout)
		realArguments.push_back("--to-stdout");

	string_builder builder(
		"[", binaryToolBinary.filename().string(), "] ",
		toolBinary.filename().string()
	);

	for (const std::string& arg : arguments)
		builder.append(" ", arg);

	name = builder();
	directory = wd;

	_exec_tool(binaryToolBinary, toolBinary, wd, realArguments);
}

void pp_binary_tool_ssource::_exec_tool(
	const boost::filesystem::path& binaryToolBinary,
	const boost::filesystem::path& toolBinary,
	const boost::filesystem::path& wd,
	const std::vector<std::string>& arguments
) {
	// See pp_tool_ssource, but worse
	// TODO ^

	try {
		std::string data;

		{
			std::future<std::string> out;
			std::future<std::string> toolErr;
			std::future<std::string> binErr;

			// TODO: better
			fs::path tmpFileName = fs::temp_directory_path() / "__eacpp__bintemp";

			if (fs::exists(tmpFileName))
				fs::remove(tmpFileName);

			{
				boost::asio::io_service ios;
				boost::process::child c(
					toolBinary,
					arguments,
					boost::process::start_dir = wd,
					boost::process::std_in.close(),
					boost::process::std_out > tmpFileName,
					boost::process::std_err > toolErr,
					ios
				);

				ios.run();
			}

			std::string errString = toolErr.get();

			if (errString.size())
				throw std::runtime_error(errString);

			{
				std::vector<std::string> binaryArguments = mpConfig->pp.eaCompat.binaryToolArguments;

				for (auto& arg : binaryArguments)
					if (arg == "%%")
						arg = fs::absolute(tmpFileName).string();

				boost::asio::io_service ios;
				boost::process::child b(
					binaryToolBinary,
					binaryArguments,
					boost::process::start_dir = wd,
					boost::process::std_in.close(),
					boost::process::std_out > out,
					boost::process::std_err > binErr,
					ios
				);

				ios.run();
			}

			fs::remove(tmpFileName);

			errString = binErr.get();

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
