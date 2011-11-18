# 'edaemon' Makefile by Claude SIMON (http://zeusw.org/intl/contact.html)
# Builds executable for the 'edaemon' software.
# Requires GNU make (http://www.gnu.org/).
# Copyright (C) 2011 Claude SIMON
#
#    This file is part of 'edaemon'.
#
#   'edaemon' is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    'edaemon' is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with 'edaemon'.  If not, see <http://www.gnu.org/licenses/>.
#
# $Id$

name = edaemon
copt =
# Epeios general debugging features
copt += -DXXX_DBG
# Required for multitasking
copt += -DCPE_MT
# For using <setjmp.h> instead of C++ exceptions.
# copt += -DERR_JMPUSE
copt += -DCSD_NO_SERVICE_HANDLING # Will no more be necessary when 'eService' will be in function.

# epeios/core
mods = cdgb64 cio clnarg dlbrry dtfptb epsmsc err
mods += fil flw fnm lcl lst mmi mtk
mods += rgstry sck str tht tol ttr uym xml
mods += xpp

# epeios/csd
mods += csdbns csdlec csdsns

# epeios/scl
mods += scllocale sclrgstry sclmisc scltool

# edaemon
mods += locale registry

os := $(shell uname -o 2>/dev/null || uname -s)

src =

ifndef EPEIOS_SRC
	src += epeios
else
	src += $(EPEIOS_SRC)
endif

##################################
# Specific to Cygwin environment #
##################################

ifeq ("$(os)","Cygwin")
	env = gnu
	
	co +=
	
	lo +=
	
	libs =
	
endif

##########################################
# Specific for the GNU/Linux environment #
##########################################

ifeq ("$(os)","GNU/Linux")
	env = gnu
	
	co +=
	
	lo +=
	
	libs = 
	# For sound (i.e. MIDI)-related functions.
	#libs += -lasound
	# For pthread-related functions.
	#libs += -lpthread
	
endif

##################################################
# Specific for the Linux environment (N900/N810) #
##################################################

ifeq ("$(os)","Linux")
	env = gnu
	
	co +=
	
	lo +=
	
	libs = 
	# For sound (i.e. MIDI)-related functions.
	#libs += -lasound
	# For pthread-related functions.
	#libs += -lpthread
	
endif

#########################
# For Mac OS environment #
#########################

ifeq ("$(os)","Darwin")
	env = gnu
	
	lo +=
	
	libs =
	
endif

##################################
# Common to all GNU environment. #
##################################

ifeq ("$(env)","gnu")
	lo = -o $(name)

	# Generates debugging data.
	co += -g

	# General optimization.
	#co += -O

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
