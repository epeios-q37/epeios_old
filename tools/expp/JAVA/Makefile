# 'jexpp' Makefile by Claude SIMON (http://zeusw.org/intl/contact.html)
# Builds library for the 'XPPInputStream' JAVA component.
# Requires GNU make (http://www.gnu.org/).
# Copyright (C) 2011 Claude SIMON
#
#   'jexpp' is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    'jexpp' is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with 'jexpp'.  If not, see <http://www.gnu.org/licenses/>.
#
# $Id$

name = jexpp
copt =
# Epeios general debugging features
copt += -DXXX_DBG
# Required for multitasking
# copt += -DCPE_MT
# For using <setjmp.h> instead of C++ exceptions.
# copt += -DERR_JMPUSE
copt += -DCPE_JAVA
copt += -DCPE_LIBRARY
# copt += -U__CYGWIN__ -U__CYGWIN32__
# copt += -DCPE_CYGWIN -DIOP_USE_STANDARD_IO
copt += -DCPE_ST

# epeios/core
mods += cdgb64 ctn err cio
mods += fil flf flw fnm lcl
mods += rgstry str tht
mods += tol ttr xml xpp

# Don't modify line below.
os := $(shell uname -o 2>/dev/null || uname -s)

ifndef EPEIOS_SRC
	src = :../epeios
else
	src = :$(EPEIOS_SRC)
endif

jdk = $(JDK)

src += :"$(jdk)/include"

##############################
# Common to all environment. #
##############################

cc = g++ 

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

##############################


##################################
# Specific to Cygwin environment #
##################################

ifeq ("$(os)","Cygwin")

	target = $(name).dll

endif

##################################


##########################################
# Specific for the GNU/Linux environment #
##########################################

ifeq ("$(os)","GNU/Linux")

	# For sound (i.e. MIDI)-related functions.
	#libs += -lasound

endif

##########################################


##################################
# Specific to MinGW environment #
##################################

ifeq ("$(os)","Msys")

	target = $(name).dll

	src += :"$(jdk)/include/win32"

	lo += -Wl,--kill-at -shared
	lo += -m32

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

lo += -o $(target)
co := -c $(co)
ox = o
larg = $(lo) $(objs)

vpath %.cpp $(src)

co += $(copt) $(subst :, -I,$(src)) $(EPEIOS_COPT)
lo += $(EPEIOS_LOPT)

objs = $(name).o $(mods:=.o) 

$(target):  $(objs)
	$(cc) $(larg) $(libs)
	
%.$(ox): %.cpp
	$(cc) $(co) $<
