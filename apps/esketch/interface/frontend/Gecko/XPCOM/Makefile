# 'esketchgck' Project
#    by Claude SIMON (http://zeusw.org/intl/contact.html).
#
# Requires GNU make (http://www.gnu.org/software/make/).
#
#	Copyright (C) 2013 Claude SIMON.
#
#	This file is part of 'ESketch'.
#
#    'ESketch' is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    'ESketch' is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with 'ESketch'.  If not, see <http://www.gnu.org/licenses/>.

name = esketchgck


# Epeios general debugging features
#copt += -DXXX_DBG
# Required for multitasking
#copt += -DE_MT
# For using <setjmp.h> instead of C++ exceptions.
#copt += -DERR_JMPUSE
	




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
	
# Required for Gecko.
copt += -DE_GECKO

IA_32=x86
x86_64=x64
Cygwin=Cygwin
MinGW=Msys
GNULinux=GNU/Linux
MacOS=Darwin
os=$(shell uname -o 2>/dev/null || uname -s)
  
gdk = $(GDK)
      

##########################
# For Cygwin environment #
##########################

ifeq ("$(os)","$(Cygwin)")

# By default, vanilla Cygwin compiler ('g++') is used.
# Resulting application uses the Cygwin POSIX Layer.
# Currently, only 32 bits application can be made.
# For 64 bits and/or application using
# directly the MS CRT, see below.

# Use of the MinGW compiler under Cygwin. 
# To build 64 bits application.
# Uses directly the MS CRT.
	#arch=$(X86_64)

# Use of the MinGW compiler under Cygwin.
# For 32 bits application using directly the MS CRT.
	#arch=$(IA_32)

endif

##########################
		

#########################
# For MinGW environment #
#########################

ifeq ("$(os)", "$(MinGW)")

endif

#########################
		

#############################
# For GNU/Linux environment #
#############################

ifeq ("$(os)","$(GNULinux)")

endif

#############################
	

##########################
# For MacOS environment #
##########################

ifeq ("$(os)","$(MacOS)")

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

ifeq ("$(os)","$(Cygwin)")

	co += -DCYGWIN

	LDFLAGS += -static

	ifeq ("$(arch)","$(IA_32)")
		CXX = i686-w64-mingw32-g++
	else ifeq ("$(arch)","$(x86_64)")
		CXX = x86_64-w64-mingw32-g++
    
	endif
	
	target=$(name).dll
	lo += -Wl,--kill-at -shared

endif

##########################
		

#########################
# For MinGW environment #
#########################

ifeq ("$(os)", "$(MinGW)")

	co += -DMSYS

	LDFLAGS += -static

	ifeq ("$(arch)","x86")
		co += -m32
		lo += -m32
	else ifeq ("$(arch)","x64")
		co += -m64
		lo += -m64
	endif

	target=$(name).dll
	lo += -Wl,--kill-at -shared
endif

#########################
		

#############################
# For GNU/Linux environment #
#############################

ifeq ("$(os)","$(GNULinux)")

	co += -DGNULINUX

	ifeq ("$(arch)","x86")
		co += -m32
		lo += -m32
	else ifeq ("$(arch)","x64")
		co += -m64
		lo += -m64
	endif
	target=lib$(name).so
	lo += -shared
	co += -fPIC
endif

#############################
		

##########################
# For MacOS environment #
##########################

ifeq ("$(os)","$(MacOS)")

	co += -DDARWIN

	#LDFLAGS += -static	// Doesn't work.

	ifeq ("$(arch)","x86")
		co += -m32
		lo += -m32
	else ifeq ("$(arch)","x64")
		co += -m64
		lo += -m64
	endif
	target=lib$(name).dylib
	lo += -shared
	co += -fPIC

endif

##########################
		
src += :../..
src += :../../../..

ifndef EPEIOS_SRC
	src += :../epeios
else
	src += :$(EPEIOS_SRC)
endif
    
src += :"$(gdk)/include"
      
ox = o
ds = :

vpath %.cpp $(subst $(ds),:,$(src))

srcd = $(subst $(ds), ,$(src))

co := -c $(co) $(copt) $(subst :, -I,$(src))
lo += -o $(target) $(objs)

objs = $(name).$(ox) $(mods:=.$(ox))

$(target): $(objs)
	$(CXX) $(LDFLAGS) $(lo) $(libs)

%.$(ox): %.cpp
	$(CXX) $(CPPFLAGS) $(co) $(CXXFLAGS) $<
		