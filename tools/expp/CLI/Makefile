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
mods += sclmisc scllocale sclerror sclrgstry scltool 
mods += locale registry

# Don't modify line below.
os := $(shell uname -o 2>/dev/null || uname -s)


##################################
# Specific to Cygwin environment #
##################################

ifeq ("$(os)","Cygwin")

	libs =

endif

##################################


##########################################
# Specific for the GNU/Linux environment #
##########################################

ifeq ("$(os)","GNU/Linux")

	libs = 

	# For sound (i.e. MIDI)-related functions.
	#libs += -lasound

endif

##########################################


#################################
# Specific to MinGW environment #
#################################

ifeq ("$(os)","Msys")

	libs =

endif

##################################


##########################
# For Mac OS environment #
##########################

ifeq ("$(os)","Darwin")

	libs =

endif

##########################


##############################
# Common to all environment. #
##############################

# Generates debugging data.
#co += -g

# General optimization.
co += -O

# To use with gprof
#co += -pg

# To force 32 bits target (incompatible with below options).
co += -m32
lo += -m32

# To force 64 bits target (incompatible with above options).
#co += -m64
#lo += -m64


###############################
###############################

##### Don't modify below ######

###############################
###############################

##################################
# Specific to Cygwin environment #
##################################

ifeq ("$(os)","Cygwin")

endif

##################################


##########################################
# Specific for the GNU/Linux environment #
##########################################

ifeq ("$(os)","GNU/Linux")

endif

##########################################


##################################
# Specific to MinGW environment #
##################################

ifeq ("$(os)","Msys")

endif

##################################


#########################
# For Mac OS environment #
#########################

ifeq ("$(os)","Darwin")

endif

#########################


##################################
# Common to all GNU environment. #
##################################

ifndef EPEIOS_SRC
	src += ../epeios
else
	src += $(EPEIOS_SRC)
endif

lo += -o $(name)
co := -c $(co)
ox = o
cc = g++ 
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
