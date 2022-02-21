# RetroShare decentralized communication platform
#
# Copyright (C) 2021-2022  Gioacchino Mazzurco <gio@retroshare.cc>
# Copyright (C) 2021-2022  Asociación Civil Altermundi <info@altermundi.net>
#
# SPDX-License-Identifier: CC0-1.0

cmake_minimum_required (VERSION 2.8.12)
project(bitdht)

set(
	RS_DATA_DIR
	"${CMAKE_INSTALL_PREFIX}/share/retroshare"
	CACHE STRING
	"Path where to install RetroShare system wide data" )

file(
	GLOB BITDHT_SOURCES
	src/bitdht/*.c src/bitdht/*.cc src/udp/*.cc src/util/*.cc )

add_library(${PROJECT_NAME} ${BITDHT_SOURCES})

target_include_directories(
	${PROJECT_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src )

install(
	FILES src/bitdht/bdboot.txt
	DESTINATION ${RS_DATA_DIR} )
