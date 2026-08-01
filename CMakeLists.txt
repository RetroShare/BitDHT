# RetroShare decentralized communication platform
#
# Copyright (C) 2021-2022  Gioacchino Mazzurco <gio@retroshare.cc>
# Copyright (C) 2026       David Bears <dbear4q@gmail.com>
#
# SPDX-License-Identifier: CC0-1.0

cmake_minimum_required(VERSION 3.15)
project(bitdht)

if(NOT DEFINED RS_DATA_DIR)
	include(GNUInstallDirs)
	set(RS_DATA_DIR "${CMAKE_INSTALL_DATADIR}/retroshare")
endif()

add_library(${PROJECT_NAME})

target_include_directories(
	${PROJECT_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(WIN32)
	target_compile_definitions(bitdht PRIVATE UNICODE _UNICODE)

	# Some files trip strict GCC >= 15 diagnostics that are now errors by
	# default; downgrade them to warnings for now.
	if(
		CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
		CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
	)
		target_compile_options(bitdht PRIVATE
			-Wno-error=incompatible-pointer-types
			-Wno-error=int-conversion
			-Wno-error=implicit-function-declaration
		)
	endif()
endif()


add_library(bitdht:bitdht ALIAS bitdht)

add_subdirectory(src)

# TODO: some things to polish the cmake integration
#     - install the library itself to support dynamic linking
#     - install headers
#     - install a config file
#     - create a build-tree config file
#     - config version files for install and build trees

install(
	FILES src/bitdht/bdboot.txt
	DESTINATION ${RS_DATA_DIR}
)
