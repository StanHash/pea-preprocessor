#ifndef STRING_SOURCE_H
#define STRING_SOURCE_H

#include <string>

template<typename CharType>
class basic_string_source {
public:
	using value_type = CharType;
	using string_type = std::basic_string<CharType>;

public:
	virtual ~basic_string_source() {}

	virtual string_type get_string() = 0;
	virtual bool at_end() const = 0;

	inline bool get_string(string_type& out) { if (at_end()) return false; out = get_string(); return true; }
};

using string_source    = basic_string_source<char>;
using wstring_source   = basic_string_source<wchar_t>;
using u16string_source = basic_string_source<char16_t>;
using u32string_source = basic_string_source<char32_t>;

template<class StringSourceType>
class counting_string_source : public StringSourceType {
	static_assert(
		std::is_base_of<basic_string_source<typename StringSourceType::value_type>, StringSourceType>::value,
		"counting_string_source<SourceType>: SourceType must be derived from basic_string_source"
	);

public:
	template<typename... Args>
	counting_string_source(Args&& ... args)
		: StringSourceType(std::forward<Args>(args)...), mGetCount(0) {}

	typename StringSourceType::string_type get_string() override { return (mGetCount++, StringSourceType::get_string()); }

	std::size_t get_count() const { return mGetCount; }

private:
	std::size_t mGetCount;
};

#endif // STRING_SOURCE_H
