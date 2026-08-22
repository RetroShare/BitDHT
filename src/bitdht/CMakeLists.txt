# ------------------------------------------------------------------------ *\
# src/bitdht/CMakeLists.txt
# This file is part of BitDHT
#
# Copyright (C) 2026      David Bears <dbear4q@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
# ------------------------------------------------------------------------ */

target_sources(${PROJECT_NAME}
  PUBLIC FILE_SET HEADERS FILES
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

  PRIVATE
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
