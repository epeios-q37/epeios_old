# 'netspy' Makefile by Claude L. Simon (simon@epeios.org)
# Builds executable for the 'netspy' software.
# Requires GNU make (http://www.gnu.org/).
# Copyright (C) 2001 Claude L. SIMON.
#
# This file is part of the Epeios project (http://www.epeios.org/).
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

name = netspy
copt =
lopt = 
# Needed by multitasking software.
copt += -DCPE_MT
# To activate Epeios general debugging features.
copt += -DXXX_DBG
# Speed up thread andling (used with -DCPE_MTK).
# copt += -DMTK_KEEP
libs = ttr tol flw stf err uym fam clnarg epsmsc mmi ctn srv sck clt mtk

ifndef EPEIOS_SRC
	epeios_src=
else
	epeios_src=$(EPEIOS_SRC)
endif

ifndef EPEIOS_OS
	epeios_os=linux
else
	epeios_os=$(EPEIOS_OS)
endif


ifeq ("$(epeios_os)","windows")
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

ifeq ("$(epeios_os)","linux")
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
lo += -lglib -lgthread
ds = :
endif

ifeq ("$(epeios_os)","beos")
ox = o
cc = g++
co = -c -g
lo = -o $(name)
ds = :
endif

vpath %.cpp $(subst $(ds),:,$(epeios_src))

srcd = $(subst $(ds), ,$(epeios_src))

co += $(copt) $(srcd:%=-I%) $(EPEIOS_COPT)
lo += $(EPEIOS_LOPT) $(lopt)

objs = $(name).$(ox) $(libs:=.$(ox))

$(name):  $(objs)
	$(cc) $(lo) $(objs)
	
%.$(ox): %.cpp
	$(cc) $(co) $< 
