cmake_minimum_required(VERSION 3.0.0)

# Project name
project(FPSWalker)

# Find all source files
file (GLOB_RECURSE SRC_FILES
	source/*.cpp
	headers/*.h
)
add_executable (FPSWalker ${SRC_FILES})
include_directories("headers/")

# Set startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT FPSWalker)
# Group all cmake targets into the one folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set Release build if not define others
if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE Release)
endif()

# Organize project files
function (assign_source_group)
	foreach (source IN ITEMS ${ARGN})
		if (IS_ABSOLUTE ${source})
			file(RELATIVE_PATH source_rel ${CMAKE_CURRENT_SOURCE_DIR} ${source})
		else()
			set(source_rel ${source})
		endif()
		get_filename_component(source_path ${source_rel} PATH)
		string(REPLACE "/" "\\" source_path_msvc ${source_path})
		source_group(${source_path_msvc} FILES ${source})
	endforeach()
endfunction (assign_source_group)
 
assign_source_group(${SRC_FILES})

