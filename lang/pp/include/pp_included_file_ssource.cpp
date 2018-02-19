#include "pp_included_file_ssource.h"

pp_included_file_ssource::pp_included_file_ssource(const fs::path& file)
	: mSource(fs::absolute(file).string()) {
	name      = fs::path(file).filename().string();
	directory = fs::path(file).remove_filename();
}
