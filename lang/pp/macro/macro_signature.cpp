#include "macro_signature.h"

bool macro_signature::compare(const macro_signature& left, const macro_signature& right) {
	if (left.mName < right.mName)
		return true;

	if (right.mName < left.mName)
		return false;

	return left.mArgCount < right.mArgCount;
}
