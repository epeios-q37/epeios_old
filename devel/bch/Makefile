# 'bch' Makefile by Claude L. Simon (simon@epeios.org)
# Builds test executable for the 'bch' library.
# Requires GNU make (http://www.gnu.org/).
# Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org).
#
# This file is part of the Epeios (http://www.epeios.org/) project.
# 
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, go to http://www.fsf.org/
# or write to the:
#
#                       Free Software Foundation, Inc.,
#          59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#

# $Id$

name = bch
copt =
# Epeios general debugging features
copt += -DXXX_DBG
# Required for multitasking
#copt += -DCPE_MT
lopt = 
libs = ttr tol flw stf err



ifeq ("$(EPEIOS_OS)","windows")
# object file extension
ox = obj
# compiler/linker binary
cc = bcc32
# compilation options
co = -q -c -w-aus
# linkage options
lo = -q -e$(name)
ds = ;
endif

ifeq ("$(EPEIOS_OS)","linux")
ox = o
cc = g++ 
co = -c
# Generates debugging data.
co += -g
# General optimization.
#co += -O
# To use with gprof
#co += -pg
lo = -o $(name)
# needed when pthread are used.
#lo += -lglib -lgthread
ds = :
endif

ifeq ("$(EPEIOS_OS)","beos")
ox = o
cc = g++
co = -c -g
lo = -o $(name)
ds = :
endif

vpath %.cpp $(subst $(ds),:,$(EPEIOS_SRC))

srcd = $(subst $(ds), ,$(EPEIOS_SRC))

co += $(copt) $(srcd:%=-I%) $(EPEIOS_COPT)
lo += $(EPEIOS_LOPT) $(lopt)

objs = $(name).$(ox) $(name)_test.$(ox) $(libs:=.$(ox))

$(name):  $(objs)
	$(cc) $(lo) $(objs)
	
%.$(ox): %.cpp
	$(cc) $(co) $< 
