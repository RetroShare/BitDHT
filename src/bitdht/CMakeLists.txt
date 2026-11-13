## ------------------------------------------------------------------------- ##
 # src/bitdht/CMakeLists.txt
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

# 3.1 : target_sources()
# 3.13: CMP0076
cmake_minimum_required(VERSION 3.13...4.4)

target_sources(${PROJECT_NAME}
	${PUBLIC_FILESET_HEADERS_FILES}
	bdaccount.h
	bdconnection.h
	bdfilter.h
	bdfriendlist.h
	bdhash.h
	bdhistory.h
	bdiface.h
	bdmanager.h
	bdmsgs.h
	bdnode.h
	bdobj.h
	bdpeer.h
	bdquery.h
	bdquerymgr.h
	bdstddht.h
	bdstore.h
	bencode.h

	${PRIVATE_FILESET_SOURCES_FILES}
	bdaccount.cc
	bdconnection.cc
	bdfilter.cc
	bdfriendlist.cc
	bdhash.cc
	bdhistory.cc
	bdmanager.cc
	bdmsgs.cc
	bdnode.cc
	bdobj.cc
	bdpeer.cc
	bdquery.cc
	bdquerymgr.cc
	bdstddht.cc
	bdstore.cc
	bencode.c
)

if(BD_EMBED_BDBOOT)
	target_compile_definitions(${PROJECT_NAME} PRIVATE BD_EMBED_BDBOOT)

	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/bdboot_embed.cc
		COMMAND ${CMAKE_COMMAND}
			-DBDBOOT_SOURCE=${BD_BOOT_FILE}
			-DOUTPUT=${CMAKE_CURRENT_BINARY_DIR}/bdboot_embed.cc
			-P ${CMAKE_CURRENT_SOURCE_DIR}/bdboot_embed.cmake
			--
		DEPENDS
			${CMAKE_CURRENT_SOURCE_DIR}/bdboot_embed.cmake
			${BD_BOOT_FILE}
		COMMENT "Generating bdboot_embed.cc"
	)
	add_custom_target(bdboot_embed.cc-target
		DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/bdboot_embed.cc
	)
	add_dependencies(${PROJECT_NAME} bdboot_embed.cc-target)

	target_sources(${PROJECT_NAME}
		${PRIVATE_FILESET_private_HEADERS_FILES}
		bdboot_embed.hh

		${PRIVATE_FILESET_SOURCES_FILES}
		${CMAKE_CURRENT_BINARY_DIR}/bdboot_embed.cc
	)
endif()
