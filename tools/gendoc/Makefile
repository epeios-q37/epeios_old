# 'gendoc' Makefile by Claude L. Simon (csimon@webmails.com)
# Builds executable for the 'gendoc' software.
# Requires GNU make (http://www.gnu.org/).
# Copyright (C) 2000,2001 Claude SIMON (csimon@webmail.com).
#
# This file is part of the Epeios project (http://epeios.org/).
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

name = gendoc
copt =
# Epeios general debugging features
# copt += -DXXX_DBG
# Required for multitasking
#copt += -DCPE_MT
lopt = 
libs = ttr tol flw stf err ctn mmi btr uym fam que tsrcpr xtf str xmltag\
       fil xmlval xmldcm xmlcor txmtbl

ifndef EPEIOS_SRC
	src=
else
	src=$(EPEIOS_SRC)
endif

ifndef EPEIOS_ENV
	env=gnu
else
	env=$(EPEIOS_ENV)
endif

ifeq ("$(env)","windows")
# compilation options
co = -q -c -w-aus -w-inl
# linkage options
lo = -q -e$(name)
endif

ifeq ("$(env)","gnu")
co = -c
# Generates debugging data.
#co += -g
# General optimization.
co += -O
# To use with gprof
#co += -pg
lo = -o $(name).exe
# needed when pthread are used.
#lo += -lpthread
endif

ifeq ("$(env)","beos")
co = -c -g
lo = -o $(name)
endif

ifeq ("$(env)","windows")
ox = obj
cc = bcc32
dss = ;
dst = :
larg = $(lo) *.$(ox)	# Because Windows command size limited.
endif

ifeq ("$(env)","gnu")
ox = o
cc = g++ 
larg = $(lo) $(objs)
dss = :
endif

ifeq ("$(EPEIOS_OS)","linux")
dst = :
endif

ifeq ("$(EPEIOS_OS)","cygwin")
dst = ;
endif

ifeq ("$(env)","beos")
ox = o
cc = g++
dss = :
dst = :
larg = $(lo) $(objs)
endif

vpath %.cpp $(subst $(ds),:,$(src))

srcd = $(subst $(ds), ,$(src))

co += $(copt) $(srcd:%=-I%) $(EPEIOS_COPT)
lo += $(EPEIOS_LOPT) $(lopt)

objs = $(name).$(ox) $(libs:=.$(ox))

$(name).exe: $(objs)
	$(cc) $(larg)
	
%.$(ox): %.cpp
	$(cc) $(co) $<
