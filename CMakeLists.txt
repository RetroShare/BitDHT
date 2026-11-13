## ------------------------------------------------------------------------- ##
 # CMakeLists.txt
 # This file is part of BitDHT.
 #
 # Copyright (C) 2026      David Bears <dbear4q@gmail.com>
 #
 # SPDX-License-Identifier: AGPL-3.0-or-later
 #
 # This program is free software: you can redistribute it and/or modify
 # it under the terms of the GNU Affero General Public License as
 # published by the Free Software Foundation, either version 3 of the
 # License, or (at your option) any later version.
 #
 # This program is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 # GNU Affero General Public License for more details.
 #
 # You should have received a copy of the GNU Affero General Public License
 # along with this program.  If not, see <https://www.gnu.org/licenses/>.
## ------------------------------------------------------------------------- ##

# 3.1 : configure_package_config_file(INSTALL_PREFIX)
# 3.7 : if(VERSION_GREATER_EQUAL)
# 3.11: add_library() ... target_sources()
# 3.11: configure_package_config_file(COMPATIBILITY SameMinorVersion)
cmake_minimum_required(VERSION 3.11...4.4)

project(bitdht
	VERSION 0.0.0
	LANGUAGES CXX C
)

include(GNUInstallDirs)
set(PROJECT_NAME_PRETTY "BitDHT")
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
set(BD_INSTALL_DATADIR "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}")
set(BD_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")

set(BD_EMBED_BDBOOT_DEFAULT FALSE)
if(WIN32)
	# Windows prefers self-contained libraries
	set(BD_EMBED_BDBOOT_DEFAULT TRUE)
endif()
option(BD_EMBED_BDBOOT
	"embed the bdboot file into the library"
	${BD_EMBED_BDBOOT_DEFAULT}
)


### Dependencies

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/mk/cmake)


### Version

if(DEFINED RS_VERSION_SUFFIX AND NOT (RS_VERSION_SUFFIX STREQUAL ""))
	set(PROJECT_VERSION_FULL "${PROJECT_VERSION}-${RS_VERSION_SUFFIX}")
else()
	include(GetGitRevisionDescription)
	git_describe_working_tree(PROJECT_VERSION_FULL --match "v*.*.*")
	if(PROJECT_VERSION_FULL)
		string(SUBSTRING "${PROJECT_VERSION_FULL}" 1 -1 PROJECT_VERSION_FULL)
		string(FIND "${PROJECT_VERSION_FULL}" ${PROJECT_VERSION} pos)
		if(NOT pos EQUAL 0)
			message(WARNING "Version mismatch!"
				" ${PROJECT_NAME_PRETTY} version is ${PROJECT_VERSION},"
				" but git describe returned ${PROJECT_VERSION_FULL}."
			)
		endif()
	else()
		set(PROJECT_VERSION_FULL "${PROJECT_VERSION}-unknown")
	endif()
endif()
message(STATUS "${PROJECT_NAME_PRETTY} version: ${PROJECT_VERSION_FULL}")


### Targets

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set(BD_BOOT_FILE "${PROJECT_SOURCE_DIR}/src/bitdht/bdboot.txt")

if(WIN32)
	target_link_libraries(${PROJECT_NAME} "-lws2_32" "-lwsock32")
	target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)

	# Some files trip strict GCC >= 15 diagnostics that are now errors by
	# default; downgrade them to warnings for now.
	if(
		CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
		CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
	)
		target_compile_options(${PROJECT_NAME} PRIVATE
			-Wno-error=incompatible-pointer-types
			-Wno-error=int-conversion
			-Wno-error=implicit-function-declaration
		)
	endif()
endif()


### Subdirectories

add_subdirectory(src)


### Installation

set(BUILD_CONFIG_DIR "${PROJECT_BINARY_DIR}/cmake")
set(INSTALL_CONFIG_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.23)
	set(FILESET_INSTALL
		FILE_SET HEADERS DESTINATION "${BD_INSTALL_INCLUDEDIR}"
	)
else()
	set(FILESET_INSTALL INCLUDES DESTINATION ${BD_INSTALL_INCLUDEDIR})
	get_target_property(${PROJECT_NAME}_SOURCES ${PROJECT_NAME} SOURCES)
	foreach(source ${${PROJECT_NAME}_SOURCES})
		string(TOLOWER ${source} sourceLower)
		if(NOT sourceLower MATCHES "\\.(h|hh|h\\+\\+|hpp)\$")
			continue()
		endif()
		foreach(base_ ${PROJECT_SOURCE_DIR}/src ${PROJECT_BINARY_DIR}/src)
			string(FIND ${source} ${base_}/ basePos)
			if(basePos EQUAL 0)
				set(base ${base_})
				break()
			endif()
		endforeach()
		if(NOT basePos EQUAL 0)
			message(SEND_ERROR "couldn't find base for header ${source}")
			continue()
		endif()

		get_filename_component(dest ${source} DIRECTORY)
		file(RELATIVE_PATH dest ${base} ${dest})
		install(FILES ${source}
			DESTINATION ${BD_INSTALL_INCLUDEDIR}/${dest}
			COMPONENT ${PROJECT_NAME}
		)
	endforeach()
endif()

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}_export
	COMPONENT ${PROJECT_NAME}
  ${FILESET_INSTALL}
)


install(
	FILES "${BD_BOOT_FILE}"
	DESTINATION "${BD_INSTALL_DATADIR}"
	COMPONENT ${PROJECT_NAME}
)

# config package for install tree
install(EXPORT ${PROJECT_NAME}_export
  DESTINATION ${INSTALL_CONFIG_DIR}
  FILE ${PROJECT_NAME}-targets.cmake
  NAMESPACE ${PROJECT_NAME}::
	COMPONENT ${PROJECT_NAME}
)
include(CMakePackageConfigHelpers)
set(BD_INSTALL_BOOT_FILE "${BD_INSTALL_DATADIR}/bdboot.txt")
configure_package_config_file(
	mk/cmake/${PROJECT_NAME_LOWER}-config.cmake.in
  ${BUILD_CONFIG_DIR}/${PROJECT_NAME_LOWER}-config-install.cmake
  INSTALL_DESTINATION ${INSTALL_CONFIG_DIR}
	PATH_VARS BD_INSTALL_BOOT_FILE
)
write_basic_package_version_file(
	${BUILD_CONFIG_DIR}/${PROJECT_NAME_LOWER}-config-version-install.cmake
	COMPATIBILITY SameMinorVersion
)
foreach(config config config-version)
	install(FILES
		${BUILD_CONFIG_DIR}/${PROJECT_NAME_LOWER}-${config}-install.cmake
		DESTINATION ${INSTALL_CONFIG_DIR}
		RENAME ${PROJECT_NAME_LOWER}-${config}.cmake
		COMPONENT ${PROJECT_NAME}
	)
endforeach()

# config package for build tree
export(EXPORT ${PROJECT_NAME}_export
  FILE cmake/${PROJECT_NAME}-targets.cmake
  NAMESPACE ${PROJECT_NAME}::
)
include(CMakePackageConfigHelpers)
set(PACKAGE_BD_INSTALL_BOOT_FILE ${BD_BOOT_FILE})
configure_package_config_file(
	mk/cmake/${PROJECT_NAME_LOWER}-config.cmake.in
  ${BUILD_CONFIG_DIR}/${PROJECT_NAME_LOWER}-config.cmake
  INSTALL_DESTINATION cmake
	INSTALL_PREFIX ${PROJECT_BINARY_DIR}
)
write_basic_package_version_file(
	${BUILD_CONFIG_DIR}/${PROJECT_NAME_LOWER}-config-version.cmake
	COMPATIBILITY SameMinorVersion
)

# config package for CMAKE_FIND_PACKAGE_REDIRECTS_DIR
if(CMAKE_VERSION VERSION_LESS 3.24)
	set(CMAKE_FIND_PACKAGE_REDIRECTS_DIR ${CMAKE_BINARY_DIR}/pkgRedirects)
endif()
configure_file(
	mk/cmake/${PROJECT_NAME}-config_redirects.cmake.in
  "${CMAKE_FIND_PACKAGE_REDIRECTS_DIR}/${PROJECT_NAME}-config.cmake"
	@ONLY
)
