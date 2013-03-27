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
mods = cio clnarg epsmsc err fil flw fnm str tol ttr
mods += ias sdr uys xml xpp lcl rgstry cdgb64 ags cpe
mods += sclmisc scllocale sclerror sclrgstry scltool 
mods += locale registry

# Don't modify line below.
os := $(shell uname -o 2>/dev/null || uname -s)


##############################
# Common to all environment. #
##############################

# Generates debugging data.
#co += -g

# General optimization.
co += -O

# To use with gprof
#co += -pg

lo += -static

###############################


##########################
# For Cygwin environment #
##########################

ifeq ("$(os)","Cygwin")

	libs =

# By default, vanilla Cygwin compiler ('g++') is used.
# Resulting application uses the Cygwin POSIX Layer.
# Only 32 bits application can be made.
# For 64 bits and/or application using
# directly the MS CRT, see below.

# The MinGW compiler under Cygwin. 
# To build 64 bits application.
# Uses directly the MS CRT.
	#cc = x86_64-w64-mingw32-g++

# THe MinGW compiler under Cygwin.
# For 32 bits application using directly the MS CRT.
	#cc = i686-w64-mingw32-g++

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

# By décommenting below line, you can compile
# for 32 bits architectrures instead of
# the default 64 bits one.
	#bitness = 32

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

endif

##################################


##########################################
# Specific for the GNU/Linux environment #
##########################################

ifeq ("$(os)","GNU/Linux")

	co += -DGNULINUX

endif

##########################################


##################################
# Specific to MinGW environment #
##################################

ifeq ("$(os)","Msys")

	co += -DMSYS

endif

##################################


#########################
# For Mac OS environment #
#########################

ifeq ("$(os)","Darwin")

	co += -DDARWIN

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

ifndef cc
	cc = g++
endif

ifdef bitness
	co += -m$(bitness)
	lo += -m$(bitness)
endif

lo += -o $(name)
co := -c $(co)
ox = o

larg = $(lo) $(objs)
ds = :

vpath %.cpp $(subst $(ds),:,$(src))

srcd = $(subst $(ds), ,$(src))

co += $(copt) $(srcd:%=-I%) $(EPEIOS_COPT)
lo += $(EPEIOS_LOPT)

objs = $(name).$(ox) $(mods:=.$(ox)) 

$(name):  $(objs)
	$(cc) $(larg) $(libs)
	
%.$(ox): %.cpp
	$(cc) $(co) $<
