# 'expp' Makefile by Claude SIMON (http://zeusw.org/intl/contact.html)
# Builds executable for the 'expp' software.
# Requires GNU make (http://www.gnu.org/).
# Copyright (C) 2011 Claude SIMON
#
#   This file is part of 'expp'.
#
#    'expp' is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    'expp' is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with 'expp'.  If not, see <http://www.gnu.org/licenses/>.

# $Id$

name = expp
copt =
# Epeios general debugging features
#copt += -DXXX_DBG
# Required for multitasking
#copt += -DCPE_MT
# For using <setjmp.h> instead of C++ exceptions.
# copt += -DERR_JMPUSE
mods = cio clnarg epsmsc err fil flw fnm mmi str tol ttr uym xml xpp lcl rgstry cdgb64
mods += global registry

os := $(shell uname -o 2>/dev/null || uname -s)

src =

ifndef EPEIOS_SRC
	src += ../epeios
else
	src += $(EPEIOS_SRC)
endif

##################################
# Specific to Cygwin environment #
##################################

ifeq ("$(os)","Cygwin")
	env = gnu
	#co += -m32
	#lo += -m32
	libs =

	# To use as Windows service.
	#mods += wintol
endif

##########################################
# Specific for the GNU/Linux environment #
##########################################

ifeq ("$(os)","GNU/Linux")
	env = gnu
	#co += -m32
	#lo += -m32
	libs = 

	# For sound (i.e. MIDI)-related functions.
	#libs += -lasound
endif

#########################
# For Mac OS environment #
#########################

ifeq ("$(os)","Darwin")
	env = gnu
	#co += -m32
	#lo += -m32
	libs =
endif

##################################
# Common to all GNU environment. #
##################################

ifeq ("$(env)","gnu")
	lo += -o $(name)

	# Generates debugging data.
	#co += -g

	# General optimization.
	co += -O

	# To use with gprof
	#co += -pg
endif

######################
# Don't modify below #
######################

ifeq ("$(env)","gnu")
	co := -c $(co)
	ox = o
	cc = g++ 
	larg = $(lo) $(objs)
	ds = :
endif

vpath %.cpp $(subst $(ds),:,$(src))

srcd = $(subst $(ds), ,$(src))

co += $(copt) $(srcd:%=-I%) $(EPEIOS_COPT)
lo += $(EPEIOS_LOPT)

objs = $(name).$(ox) $(mods:=.$(ox)) 

$(name):  $(objs)
	$(cc) $(larg) $(libs)
	
%.$(ox): %.cpp
	$(cc) $(co) $<
