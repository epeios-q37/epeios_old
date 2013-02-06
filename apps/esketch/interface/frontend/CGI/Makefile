# 'esketchbkd' Makefile by Claude SIMON (develop@logiplus.fr)
# Builds backend dynamic library for the 'eSketch' software.
# Requires GNU make (http://www.gnu.org/).
# Copyright (C) 2011 Claude SIMON.

# $Id$

name = esketchcgi
copt =
# Epeios general debugging features
copt += -DXXX_DBG
# Required for multitasking
# copt += -DCPE_MT -DMTK_KEEP
# Required when compiling a library.
copt += -DCPE_LIBRARY
lopt = 

# epeios/core
mods = btr cdgb64 cio dlbrry dtfptb err fil flw flx fnm lcl mmi que rgstry
mods += sck ssnmng str tht tol ttr uym xml xpp

# epeios/cgi
mods += cgiarg cgigat

# epeios/csd
mods += csdbnc csdlec csducl csdsnc

# epeios/fbl
mods += fblcmd fblcst fblfrb fblfrd fblovl fbltyp

# epeios/frd
mods += frdkrn frdrgy

# epeios/scl
mods += sclrgstry sclerror scllocale scldaemon sclmisc

# /esketch/processing/backend
mods += 

src = :..:../..:../../..

# Don't modify line below.
os := $(shell uname -o 2>/dev/null || uname -s)


##################################
# Specific to Cygwin environment #
##################################

ifeq ("$(os)","Cygwin")

	lo = -shared -W1,-soname,$(name).so -o $(name).so 
	lo +=

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

	lo = -shared -W1,-soname,$(name).so -o $(name).so 
	lo +=


endif

##########################################


#################################
# Specific to MinGW environment #
#################################

ifeq ("$(os)","Msys")

	target = $(name).dll

	co += -D__MSVCRT_VERSION__=0x0800 -fpermissive

	lo += -Wl,--kill-at -shared
	lo += -m32

	libs += -lwsock32 -lmsvcr100

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
	src += :../epeios
else
	src += :$(EPEIOS_SRC)
endif

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
