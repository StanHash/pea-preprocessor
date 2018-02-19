#ifndef STRING_BUILDING_H
#define STRING_BUILDING_H

#include <string>
#include <vector>

template<typename CharType>
struct basic_string_builder {
	basic_string_builder() {}

	template<typename... Args>
	basic_string_builder(Args&& ... args) {
		append(std::forward<Args>(args)...);
	}

	template<typename Type, typename... Args>
	void append(Type type, Args&& ... args) {
		append(std::to_string(type));
		append(std::forward<Args>(args)...);
	}

	template<typename... Args>
	void append(const CharType* str, Args&& ... args) {
		append(str);
		append(std::forward<Args>(args)...);
	}

	template<typename... Args>
	void append(const std::basic_string<CharType>& str, Args&& ... args) {
		append(str);
		append(std::forward<Args>(args)...);
	}

	template<typename... Args>
	void append(std::basic_string<CharType>&& str, Args&& ... args) {
		append(std::move(str));
		append(std::forward<Args>(args)...);
	}

	void append(const CharType* str) {
		mStringList.push_back(std::basic_string<CharType>(str));
	}

	void append(const std::basic_string<CharType>& str) {
		mStringList.push_back(str);
	}

	void append(std::basic_string<CharType>&& str) {
		mStringList.push_back(std::move(str));
	}

	std::basic_string<CharType> operator () () {
		std::basic_string<CharType> result;
		unsigned int size = 0;

		for (auto& str : mStringList)
			size += str.size();

		result.reserve(size);

		for (auto& str : mStringList)
			result.append(str);

		return result;
	}

private:
	std::vector<std::basic_string<CharType>> mStringList;
};

using string_builder    = basic_string_builder<char>;
using wstring_builder   = basic_string_builder<wchar_t>;
using u16string_builder = basic_string_builder<char16_t>;
using u32string_builder = basic_string_builder<char32_t>;

#endif // STRING_BUILDING_H
