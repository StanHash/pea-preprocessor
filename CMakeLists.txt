project(pea)
cmake_minimum_required(VERSION 3.1)

# Setting C++ Standard to C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(SOURCE_LIST
    main.cpp

	main_args.h
	main_args.cpp

	lib/filesystem.h

	lib/char_utility.h

	lib/scanner.h
	lib/string_scanning.h

	lib/string_building.h

	lib/error/sscan_error.h

	lib/ssource/string_source.h
	lib/ssource/stream_string_source.h
	lib/ssource/filter_string_source.h
	lib/ssource/functional_filter_string_source.h
	lib/ssource/string_string_source.h
	lib/ssource/file_string_source.h

	option/parameter_interface.h
	option/parameter_interface.cpp

	lang/config.h
	lang/types.h

	lang/error/include_error.h
	lang/error/directive_error.h
	lang/error/macro_error.h
	lang/error/tool_error.h

	lang/pp/macro/macro_scanning.h

	lang/pp/macro/macro_signature.h
	lang/pp/macro/macro_signature.cpp

	lang/pp/macro/macro.h
	lang/pp/macro/macro.cpp

	lang/pp/macro/macro_library.h
	lang/pp/macro/macro_library.cpp

	lang/pp/include/pp_included_base.h
	lang/pp/include/pp_included_base.cpp

	lang/pp/include/pp_included_file_ssource.h
	lang/pp/include/pp_included_file_ssource.cpp

	lang/pp/include/pp_tool_ssource.h
	lang/pp/include/pp_tool_ssource.cpp

	lang/pp/include/pp_binary_tool_ssource.h
	lang/pp/include/pp_binary_tool_ssource.cpp

	lang/pp/ssource/pp_nocomment_ssource.h
	lang/pp/ssource/pp_nocomment_ssource.cpp

	lang/pp/ssource/pp_including_ssource.h
	lang/pp/ssource/pp_including_ssource.cpp

	lang/pp/directive/pp_directive_base.h

	lang/pp/directive/pp_macro_directives.h
	lang/pp/directive/pp_macro_directives.cpp

	lang/pp/directive/pp_selection_directives.h
	lang/pp/directive/pp_selection_directives.cpp

	lang/pp/directive/pp_conditional_macro_directives.h
	lang/pp/directive/pp_conditional_macro_directives.cpp

	lang/pp/directive/pp_include_directives.h
	lang/pp/directive/pp_include_directives.cpp

	lang/pp/directive/pp_binary_directives.h
	lang/pp/directive/pp_binary_directives.cpp

	lang/pp/pp_selection_stack.h
	lang/pp/pp_selection_stack.cpp

	lang/pp/pp_directive_processor.h
	lang/pp/pp_directive_processor.cpp

	lang/pp/pp_include_observer.h
	lang/pp/pp_include_observer.cpp

	lang/pp/pp_dependency_observer.h
	lang/pp/pp_dependency_observer.cpp

	lang/pp/pp_main_directive_processor.h
	lang/pp/pp_main_directive_processor.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_LIST})

# this is to ensure inclusion relative to base directory is allowed
include_directories(${CMAKE_SOURCE_DIR})

# link statically
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

# find, include & link boost (filesystem & process)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system filesystem)

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

	target_link_libraries(${PROJECT_NAME} -lpthread)

	if (WIN32)
		target_link_libraries(${PROJECT_NAME} -lws2_32)
	endif()
endif()
