#ifndef MACRO_SIGNATURE_H
#define MACRO_SIGNATURE_H

#include <vector>

#include "lang/types.h"

class complete_macro_signature {
	friend class macro_signature;

public:
	complete_macro_signature(const lang::string_type& name, const std::vector<lang::string_type>& args)
		: mName(name), mArguments(args) {}

	complete_macro_signature(const lang::string_type& name, std::vector<lang::string_type>&& args)
		: mName(name), mArguments(std::move(args)) {}

	complete_macro_signature(lang::string_type&& name, const std::vector<lang::string_type>& args)
		: mName(std::move(name)), mArguments(args) {}

	complete_macro_signature(lang::string_type&& name, std::vector<lang::string_type>&& args)
		: mName(std::move(name)), mArguments(std::move(args)) {}

	template<typename IteratorType>
	complete_macro_signature(const lang::string_type& name, IteratorType argBegin, IteratorType argEnd)
		: mName(name), mArguments(argBegin, argEnd) {}

	template<typename IteratorType>
	complete_macro_signature(lang::string_type&& name, IteratorType argBegin, IteratorType argEnd)
		: mName(std::move(name)), mArguments(argBegin, argEnd) {}

	const lang::string_type& name() const { return mName; }
	unsigned int argument_count() const { return mArguments.size(); }

	const std::vector<lang::string_type>& arguments() const { return mArguments; }

private:
	lang::string_type mName;
	std::vector<lang::string_type> mArguments;
};

class macro_signature {
	friend class macro_library;

public:
	macro_signature(const lang::string_type& name, int argc)
		: mName(name), mArgCount(argc) {}

	macro_signature(lang::string_type&& name, int argc)
		: mName(std::move(name)), mArgCount(argc) {}

	macro_signature(const complete_macro_signature& signature)
		: mName(signature.mName), mArgCount(signature.mArguments.size()) {}

	macro_signature(complete_macro_signature&& signature)
		: mName(std::move(signature.mName)), mArgCount(signature.mArguments.size()) {}

	const lang::string_type& name() const { return mName; }
	unsigned int argument_count() const { return mArgCount; }

public:
	static bool compare(const macro_signature& left, const macro_signature& right);

private:
	lang::string_type mName;
	unsigned int mArgCount;
};

#endif // MACRO_SIGNATURE_H
