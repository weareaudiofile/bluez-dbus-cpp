##
# Copyright Audiofile LLC 2019 - 2020.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
##

# executable
add_executable (example
	example.cpp )

target_include_directories( example PUBLIC 	
	$<BUILD_INTERFACE:${BLUEZ_HEADER_DIR}>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )

target_link_libraries( example PRIVATE bluez-dbus-cpp)

# Set output directory for binaries
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/)

target_compile_options(example PRIVATE -Wmissing-field-initializers)
