#include "pp_nocomment_ssource.h"

#include "lib/string_scanning.h"

const lang::string_type pp_nocomment_ssource::msLineCommentOpener(LANG_LITERAL"//");
const lang::string_type pp_nocomment_ssource::msMultilineCommentOpener(LANG_LITERAL"/*");
const lang::string_type pp_nocomment_ssource::msMultilineCommentEnder(LANG_LITERAL"*/");

pp_nocomment_ssource::pp_nocomment_ssource(lang::ssource_type& source)
	: mpSource(&source) {}

lang::string_type pp_nocomment_ssource::get_string() {
	lang::string_type result;

	do
		result = get_string_raw();
	while (result.empty() && !mpSource->at_end());

	return result;
}

bool pp_nocomment_ssource::at_end() const {
	return mpSource->at_end();
}

lang::string_type pp_nocomment_ssource::get_string_raw() {
	lang::string_type source = mpSource->get_string();
	auto scanner = make_scanner(source);

	lang::string_type result;
	result.reserve(source.size());

	while (!scanner.at_end()) {
		if (mMultilineCommentDepth) {
			if (scanner.consume_range(msMultilineCommentOpener))
				mMultilineCommentDepth++;
			else if (scanner.consume_range(msMultilineCommentEnder))
				mMultilineCommentDepth--;
			else
				scanner.consume();
		} else {
			if (scanner.consume_range(msLineCommentOpener))
				break; // start of line comment: the end

			if (scanner.consume_range(msMultilineCommentOpener))
				mMultilineCommentDepth++; // start of multiline comment
			else
				result.push_back(scanner.consume());
		}
	}

	while (result.size() && char_utility<lang::char_type>::is_space(result.back()))
		result.pop_back();

	return result;
}
