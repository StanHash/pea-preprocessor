#include "pp_selection_stack.h"

void pp_selection_stack::push_conditional(bool selected) {
	mStack.push_back(selected);
}

bool pp_selection_stack::flip_conditional() {
	if (mStack.empty())
		return false;

	mStack.back() = !mStack.back();
	return true;
}

bool pp_selection_stack::pop_conditional() {
	if (mStack.empty())
		return false;

	mStack.pop_back();
	return true;
}

bool pp_selection_stack::in_selection() const {
	for (bool selection : mStack)
		if (!selection)
			return false;
	return true;
}
