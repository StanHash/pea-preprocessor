#ifndef SCANNER_H
#define SCANNER_H

#include <iterator>

template<typename IteratorType>
class basic_range_scanner {
public:
	using value_type = typename std::iterator_traits<IteratorType>::value_type;

public:
	basic_range_scanner(IteratorType start, IteratorType end)
		: mCursor(start), mBegin(start), mEnd(end) {}

	void reset() { mCursor = mBegin; }

	IteratorType begin() const { return mBegin; }
	IteratorType end() const { return mEnd; }

	IteratorType& it() { return mCursor; }
	IteratorType it() const { return mCursor; }

	bool at_end() const { return mCursor == mEnd; }

	value_type peek() const { return (at_end() ? 0 : *mCursor); }

	value_type match(value_type chr) const {
		return ((at_end() || (*mCursor) != chr) ? 0 : chr);
	}

	template<typename OtherIteratorType>
	bool match_range(OtherIteratorType start, OtherIteratorType end) const {
		if (std::distance(start, end) > std::distance(mCursor, mEnd))
			return false;

		for (IteratorType it = mCursor; start != end; ++it, ++start)
			if ((*start) != (*it))
				return false;

		return true;
	}

	template<typename RangeType>
	bool match_range(const RangeType& range) {
		return match_range(std::begin(range), std::end(range));
	}

	template<typename FuncType>
	value_type match_if(FuncType func) const {
		value_type chr = peek();
		return (chr && func(chr) ? chr : 0);
	}

	value_type consume() {
		return (at_end() ? 0 : (*mCursor++));
	}

	value_type consume(value_type chr) {
		if (match(chr) == chr)
			return (*mCursor++);
		return 0;
	}

	template<typename OtherIteratorType>
	bool consume_range(OtherIteratorType start, OtherIteratorType end) {
		if (match_range<OtherIteratorType>(start, end))
			return ((std::advance(mCursor, std::distance(start, end))), true);
		return false;
	}

	template<typename RangeType>
	bool consume_range(const RangeType& range) {
		return consume_range(std::begin(range), std::end(range));
	}

	template<typename FuncType>
	value_type consume_if(FuncType func) {
		if (value_type chr = match_if(func))
			return (*mCursor++);
		return 0;
	}

private:
	IteratorType mCursor;
	IteratorType mBegin, mEnd;
};

// helper function
template<typename IteratorType>
basic_range_scanner<IteratorType> make_scanner(IteratorType start, IteratorType end) {
	return basic_range_scanner<IteratorType>(start, end);
}

// helperer function
template<typename RangeType>
basic_range_scanner<typename RangeType::const_iterator> make_scanner(const RangeType& range) {
	return basic_range_scanner<typename RangeType::const_iterator>(std::begin(range), std::end(range));
}

#endif // SCANNER_H
