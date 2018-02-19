#include "macro_library.h"

#include <algorithm>

#include "lang/pp/macro/macro_scanning.h"

bool macro_library::define_macro(const lang::string_type& name, int argc, const lang::string_type& format) {
	auto it = find(name, argc);

	if (it != mLibrary.end())
		return false;

	mLibrary.insert({
		macro_signature(name, argc),
		format
	});

	return true;
}

bool macro_library::define_macro(const lang::string_type& name, int argc, lang::string_type&& format) {
	auto it = find(name, argc);

	if (it != mLibrary.end())
		return false;

	mLibrary.insert({
		macro_signature(name, argc),
		std::move(format)
	});

	return true;
}

bool macro_library::define_macro(lang::string_type&& name, int argc, const lang::string_type& format) {
	auto it = find(name, argc);

	if (it != mLibrary.end())
		return false;

	mLibrary.insert({
		macro_signature(std::move(name), argc),
		format
	});

	return true;
}

bool macro_library::define_macro(lang::string_type&& name, int argc, lang::string_type&& format) {
	auto it = find(name, argc);

	if (it != mLibrary.end())
		return false;

	mLibrary.insert({
		macro_signature(std::move(name), argc),
		std::move(format)
	});

	return true;
}

bool macro_library::define_macro(macro_signature&& signature, lang::string_type&& format) {
	auto it = mLibrary.find(signature);

	if (it != mLibrary.end())
		return false;

	mLibrary.insert({
		std::move(signature),
		std::move(format)
	});

	return true;
}

bool macro_library::define_macro(macro_signature&& signature, const lang::string_type& format) {
	auto it = mLibrary.find(signature);

	if (it != mLibrary.end())
		return false;

	mLibrary.insert({
		std::move(signature),
		format
	});

	return true;
}

bool macro_library::define_macro(const macro_signature& signature, lang::string_type&& format) {
	auto it = mLibrary.find(signature);

	if (it != mLibrary.end())
		return false;

	mLibrary.insert({
		signature,
		std::move(format)
	});

	return true;
}

bool macro_library::define_macro(const macro_signature& signature, const lang::string_type& format) {
	auto it = mLibrary.find(signature);

	if (it != mLibrary.end())
		return false;

	mLibrary.insert({
		signature,
		format
	});

	return true;
}

bool macro_library::undefine_macro(const lang::string_type& name) {
	return undefine_macro(name, 0);
}

bool macro_library::undefine_macro(const lang::string_type& name, int argc) {
	auto it = find(name, argc);

	if (it == mLibrary.end())
		return false;

	mLibrary.erase(it);
	return true;
}

bool macro_library::undefine_macro(const macro_signature& signature) {
	auto it = mLibrary.find(signature);

	if (it == mLibrary.end())
		return false;

	mLibrary.erase(it);
	return true;
}

bool macro_library::is_macro_defined(const lang::string_type& name) const {
	return is_macro_defined(name, 0);
}

bool macro_library::is_macro_defined(const lang::string_type &name, int argc) const {
	return find(name, argc) != mLibrary.end();
}

bool macro_library::is_macro_defined(const macro_signature& signature) const {
	return mLibrary.find(signature) != mLibrary.end();
}

macro macro_library::get_macro(const lang::string_type& name) const {
	return get_macro(name, 0);
}

macro macro_library::get_macro(const lang::string_type& name, int argc) const {
	auto it = find(name, argc);

	if (it == mLibrary.end())
		return macro(nullptr, nullptr);

	return macro(&it->first, &it->second);
}

lang::string_type macro_library::expand(const lang::string_type& line) const {
	lang::string_type result(line);
	expand_private(result);

	return result;
}

lang::string_type macro_library::expand(lang::string_type&& line) const {
	lang::string_type result(std::move(line));
	expand_private(result);

	return result;
}

std::map<macro_signature, lang::string_type, decltype(&macro_signature::compare)>::iterator macro_library::find(const lang::string_type& name, int argc) {
	return std::find_if(mLibrary.begin(), mLibrary.end(), [&name, argc] (const std::pair<macro_signature, lang::string_type>& p) {
		return p.first.argument_count() == argc && p.first.name() == name;
	});
}

std::map<macro_signature, lang::string_type, decltype(&macro_signature::compare)>::const_iterator macro_library::find(const lang::string_type& name, int argc) const {
	return std::find_if(mLibrary.begin(), mLibrary.end(), [&name, argc] (const std::pair<macro_signature, lang::string_type>& p) {
		return p.first.argument_count() == argc && p.first.name() == name;
	});
}

void macro_library::expand_private(lang::string_type& toExpand) const {
	auto scanner = make_scanner(toExpand);

	while (!scanner.at_end()) {
		while (!scanner.match_if(char_utility<lang::char_type>::is_alpha_or_underscore)) {
			scanner.consume();

			if (scanner.at_end())
				break;
		}

		auto itStart = scanner.it();

		auto name = sscan::read_identifier(scanner);
		auto args = sscan::macro::call_arguments(scanner);

		auto itMacro = find(name, args.size());

		if (itMacro == mLibrary.end())
			continue; // skip

		for (auto& arg : args)
			expand_private(arg);

		// Since we are going to modify the string, iterators may be invalidated
		// So we get the current position now and we rebuild the scanner from it later
		unsigned int pos = std::distance(toExpand.cbegin(), itStart);

		toExpand.replace(itStart, scanner.it(), macro(&itMacro->first, &itMacro->second).expand(args));

		scanner = make_scanner(std::next(toExpand.cbegin(), pos), toExpand.cend());
	}
}
