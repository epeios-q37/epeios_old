# 'edemo' Makefile by Claude SIMON (http://zeusw.org/intl/contact.html)
# Builds executable for the 'edemo' component.
# Requires GNU make (http://www.gnu.org/).
# Copyright (C) 2011 Claude SIMON
#
#    This file is part of 'edemo'.
#
#   'edemo' is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    'edemo' is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with 'edemo'.  If not, see <http://www.gnu.org/licenses/>.
#
# $Id$

ifndef JDK
	jdk = $(HOME)/jdk
else
	jdk = $(JDK)

endif
name = jexpp
copt =
# Epeios general debugging features
copt += -DXXX_DBG
# Required for multitasking
# copt += -DCPE_MT
# For using <setjmp.h> instead of C++ exceptions.
# copt += -DERR_JMPUSE
copt += -DCPE_LIBRARY

# epeios/core
mods += cdgb64 ctn err 
mods += fil flf flw fnm lcl mmi
mods += rgstry str tht
mods += tol ttr uym xml xpp

os := $(shell uname -o 2>/dev/null || uname -s)

src = $(jdk)/include

ifndef EPEIOS_SRC
	src += epeios
else
	src += $(EPEIOS_SRC)
endif

src += 

##################################
# Specific to Cygwin environment #
##################################

ifeq ("$(os)","Cygwin")
	env = gnu
	
	co += -g -m32 "-D__int64=long long" 
	
	lo += -m32 -o $(name).dll -s -shared
	
	libs =

	src += $(jdk)/include/win32
	
	libs += -Wl,--subsystem,windows,--kill-at
endif

##########################################
# Specific for the GNU/Linux environment #
##########################################

ifeq ("$(os)","GNU/Linux")
	env = gnu
	
#	co += -std=c++0x
	co +=  -static -lc
	
	lo += -shared -W1,-soname,lib$(name).so -o lib$(name).so 
	
	libs = 
	# For sound (i.e. MIDI)-related functions.
	#libs += -lasound
	# For pthread-related functions.
	#libs += -lpthread

	libs += -static -lc
	
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

	co += -m32

	lo = -m32 -dynamiclib -o lib$(name).jnilib
	libs += -framework JavaVM
	
endif

##################################
# Common to all GNU environment. #
##################################

ifeq ("$(env)","gnu")
#	lo = -o $(name)

	# Generates debugging data.
	co += -g

	# General optimization.
	#co += -O

	# To use with gprof
	#co += -pg

	#co += -fshort-wchar

#	libs += -lm
	
endif

######################
# Don't modify below #
######################

ifeq ("$(env)","gnu")
	co := -c $(co)
	ox = o

	ifndef EPEIOS_CC
		cc += g++
	else
		cc += $(EPEIOS_CC)
	endif

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
