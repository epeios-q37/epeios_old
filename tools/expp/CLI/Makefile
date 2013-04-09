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
mods = cio clnarg epsmsc err fil flw fnm str tol ttr bso
mods += ias sdr uys xml xpp lcl rgstry cdgb64 ags cpe
mods += sclmisc scllocale sclerror sclrgstry scltool 
mods += locale registry

# Don't modify line below.
os := $(shell uname -o 2>/dev/null || uname -s)


##############################
# Common to all environment. #
##############################

# Generates debugging data.
#CXXFLAGS += -g

# General optimization.
CXXFLAGS += -O

# To use with gprof
#CXXFLAGS += -pg

###############################


##########################
# For Cygwin environment #
##########################

ifeq ("$(os)","Cygwin")

# By default, vanilla Cygwin compiler ('g++') is used.
# Resulting application uses the Cygwin POSIX Layer.
# Currently, only 32 bits application can be made.
# For 64 bits and/or application using
# directly the MS CRT, see below.

# Use of the MinGW compiler under Cygwin. 
# To build 64 bits application.
# Uses directly the MS CRT.
	#arch=x64

# The MinGW compiler under Cygwin.
# For 32 bits application using directly the MS CRT.
	#arch=x86

	libs =

endif

##########################


#############################
# For GNU/Linux environment #
#############################

ifeq ("$(os)","GNU/Linux")

	libs = 

# For sound (i.e. MIDI)-related functions.
	#libs += -lasound

endif

#############################


#########################
# For MinGW environment #
#########################

ifeq ("$(os)","Msys")

	libs =

endif

#########################


##########################
# For Mac OS environment #
##########################

ifeq ("$(os)","Darwin")

	libs =

endif

##########################


###################################
###################################
##### DON'T MODIFY BELOW !!! ######
###################################
###################################

##########################
# For Cygwin environment #
##########################

ifeq ("$(os)","Cygwin")

	co += -DCYGWIN

	LDFLAGS += -static

	ifeq ("$(arch)","x86")
		CXX = i686-w64-mingw32-g++
	else ifeq ("$(arch)","x64")
		CXX = x86_64-w64-mingw32-g++
	endif
endif

##################################


##########################################
# Specific for the GNU/Linux environment #
##########################################

ifeq ("$(os)","GNU/Linux")

	co += -DGNULINUX

	LDFLAGS += -static

	ifeq ("$(arch)","x86")
		co += -m32
		lo += -m32
	else ifeq ("$(arch)","x64")
		co += -m64
		lo += -m64
	endif

endif

##########################################


##################################
# Specific to MinGW environment #
##################################

ifeq ("$(os)","Msys")

	co += -DMSYS

	LDFLAGS += -static

	ifeq ("$(arch)","x86")
		co += -m32
		lo += -m32
	else ifeq ("$(arch)","x64")
		co += -m64
		lo += -m64
	endif

endif

##################################


#########################
# For Mac OS environment #
#########################

ifeq ("$(os)","Darwin")

	co += -DDARWIN

	#LDFLAGS += -static	// Doesn't work.

	ifeq ("$(arch)","x86")
		co += -m32
		lo += -m32
	else ifeq ("$(arch)","x64")
		co += -m64
		lo += -m64
	endif

endif

#########################


##################################
# Common to all GNU environment. #
##################################

##################################

ifndef EPEIOS_SRC
	src += ../epeios
else
	src += $(EPEIOS_SRC)
endif

ox = o
ds = :

vpath %.cpp $(subst $(ds),:,$(src))

srcd = $(subst $(ds), ,$(src))

co := -c $(co) $(copt) $(srcd:%=-I%)
lo += -o $(name) $(objs)

objs = $(name).$(ox) $(mods:=.$(ox)) 

$(name): $(objs)
	$(CXX) $(LDFLAGS) $(lo) $(libs)
	
%.$(ox): %.cpp
	$(CXX) $(CPPFLAGS) $(co) $(CXXFLAGS) $<
