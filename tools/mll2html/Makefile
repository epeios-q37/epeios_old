# 'mll2html' Makefile by Claude L. Simon (csimon@webmails.com)
# Builds executable for the 'mll2html' software.
# Requires GNU make (http://www.gnu.org/).
# Copyright (C) 2000,2001 Claude SIMON (csimon@webmails.com).
#
# This file is part of the Epeios project (http://epeios.org/).
# This file is also part of the GNU project (http://www.gnu.org/).
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

name = mll2html
copt =
# Epeios general debugging features
copt += -DXXX_DBG
# Required for multitasking
#copt += -DCPE_MT
lopt = 
libs = ttr tol flw stf err uym fam clnarg epsmsc mmi ctn str fil gnumll tagdtc\
       txmpgh xtf txf epeios tym

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

ifeq ("$(EPEIOS_OS)","windows")
# compilation options
co = -q -c -w-aus -w-inl
# linkage options
lo = -q -e$(name)
endif

ifeq ("$(EPEIOS_OS)","linux")
co = -c
# Generates debugging data.
co += -g
# General optimization.
#co += -O
# To use with gprof
#co += -pg
lo = -o $(name)
# needed when pthread are used.
#lo += -lpthread
endif

ifeq ("$(EPEIOS_OS)","beos")
co = -c -g
lo = -o $(name)
endif

#

ifeq ("$(EPEIOS_OS)","windows")
ox = obj
cc = bcc32
ds = ;
larg = $(lo) *.$(ox)	# Because Windows command size limited.
endif

ifeq ("$(EPEIOS_OS)","linux")
ox = o
cc = g++ 
ds = :
larg = $(lo) $(objs)
endif

ifeq ("$(EPEIOS_OS)","beos")
ox = o
cc = g++
ds = :
larg = $(lo) $(objs)
endif

vpath %.cpp $(subst $(ds),:,$(EPEIOS_SRC))

srcd = $(subst $(ds), ,$(EPEIOS_SRC))

co += $(copt) $(srcd:%=-I%) $(EPEIOS_COPT)
lo += $(EPEIOS_LOPT) $(lopt)

objs = $(name).$(ox) $(libs:=.$(ox))

$(name):  $(objs)
	$(cc) $(larg)
	
%.$(ox): %.cpp
	$(cc) $(co) $<