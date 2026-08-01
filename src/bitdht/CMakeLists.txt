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

target_sources(${PROJECT_NAME} PRIVATE
  bdaccount.cc
  bdaccount.h
  bdconnection.cc
  bdconnection.h
  bdfilter.cc
  bdfilter.h
  bdfriendlist.cc
  bdfriendlist.h
  bdhash.cc
  bdhash.h
  bdhistory.cc
  bdhistory.h
  bdiface.h
  bdmanager.cc
  bdmanager.h
  bdmsgs.cc
  bdmsgs.h
  bdnode.cc
  bdnode.h
  bdobj.cc
  bdobj.h
  bdpeer.cc
  bdpeer.h
  bdquery.cc
  bdquery.h
  bdquerymgr.cc
  bdquerymgr.h
  bdstddht.cc
  bdstddht.h
  bdstore.cc
  bdstore.h
  bencode.c
  bencode.h
)
