#ifndef PP_EA_SELECTION_STACK_H
#define PP_EA_SELECTION_STACK_H

#include <vector>

class pp_selection_stack {
public:
	// ifdef, ifndef, if, etc
	void push_conditional(bool selected);

	// else
	bool flip_conditional();

	// endif
	bool pop_conditional();

	// test
	bool in_selection() const;

private:
	std::vector<bool> mStack;
};

#endif // PP_EA_SELECTION_STACK_H
