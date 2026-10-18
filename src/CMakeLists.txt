## ------------------------------------------------------------------------- ##
 # src/CMakeLists.txt
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

cmake_minimum_required(VERSION 3.0...4.4)

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.23)
	target_sources(${PROJECT_NAME}
		PUBLIC FILE_SET HEADERS
		BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}

		PRIVATE FILE_SET private_HEADERS TYPE HEADERS
		BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	)
	set(PUBLIC_FILESET_HEADERS_FILES PUBLIC FILE_SET HEADERS FILES)
	set(PRIVATE_FILESET_private_HEADERS_FILES
		PRIVATE FILE_SET private_HEADERS FILES
	)
else()
	set(PUBLIC_FILESET_HEADERS_FILES PRIVATE)
	set(PRIVATE_FILESET_private_HEADERS_FILES PRIVATE)
	target_include_directories(${PROJECT_NAME}
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
	)
endif()

if(CMAKE_VERSION VERSION_GREATER_EQUAL 4.4)
	target_sources(${PROJECT_NAME}
		PRIVATE FILE_SET SOURCES
	)
	set(PRIVATE_FILESET_SOURCES_FILES PRIVATE FILE_SET SOURCES FILES)
else()
	set(PRIVATE_FILESET_SOURCES_FILES PRIVATE)
endif()

add_subdirectory(bitdht)
add_subdirectory(udp)
add_subdirectory(util)
