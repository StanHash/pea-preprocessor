#ifndef MAIN_ARGS_H
#define MAIN_ARGS_H

#include "lang/config.h"

namespace args {

// TODO: better?

bool should_show_help(int argc, char** argv);
void show_help(std::ostream& out, const char* programName);
void args_to_config(lang::config& config, int argc, char** argv);

} // namespace args

#endif // MAIN_ARGS_H
