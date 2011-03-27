# 'erism' Makefile by Claude SIMON (http://zeusw.org/intl/contact.html)
# Builds executable for the 'erism' software.
# Requires GNU make (http://www.gnu.org/).
# Copyright (C) 2011 Claude SIMON
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

name = erism
copt =
# Epeios general debugging features
copt += -DXXX_DBG
# Required for multitasking
copt += -DCPE_MT
lopt = 

mods = cdgb64 cio clnarg epsmsc err fil flf flw fnm fwf lcl lst mmi
mods += rgstry str tht tol ttr uym xml xpp
mods += mscmdd mscmdf mscmdm mscrmi
mods += common get identity registry set 

OS := $(shell uname -o)

ifndef EPEIOS_SRC
	src=
else
	src=$(EPEIOS_SRC)
endif

ifndef EPEIOS_ENV
	env=gnu
else
	env=$(EPEIOS_ENV)
endif

###################################
# For non-GNU Windows environment #
###################################

ifeq ("$(env)","windows")
# compilation options
co = -q -c -w-aus -w-inl
# linkage options
lo = -q -e$(name)
libs = 
endif

##################################
# Common to all GNU environment. #
##################################

ifeq ("$(env)","gnu")
co = -c
# Generates debugging data.
# co += -g
# General optimization.
co += -O
# To use with gprof
#co += -pg
lo = -o $(name)
libs =
endif

##########################################
# Specific for the GNU/Linux environment #
##########################################

ifeq ("$(OS)","GNU/Linux")
co +=
lo +=
libs += -lasound
endif

##################################
# Specific to Cygwin environment #
##################################

ifeq ("$(OS)","Cygwin")
co +=
lo +=
libs += -lwinmm
endif

########################
# For BeOS environment #
########################

ifeq ("$(env)","beos")
co = -c -g
lo = -o $(name)
libs =
endif

######################
# Don't modify below #
######################

ifeq ("$(env)","windows")
ox = obj
cc = bcc32
ds = ;
larg = $(lo) *.$(ox)	# Because Windows command size limited.
endif

ifeq ("$(env)","gnu")
ox = o
cc = g++ 
larg = $(lo) $(objs)
ds = :
endif

ifeq ("$(env)","beos")
ox = o
cc = g++
ds = :
larg = $(lo) $(objs)
endif

vpath %.cpp $(subst $(ds),:,$(EPEIOS_SRC))

srcd = $(subst $(ds), ,$(EPEIOS_SRC))

co += $(copt) $(srcd:%=-I%) $(EPEIOS_COPT)
lo += $(EPEIOS_LOPT) $(lopt)

objs = $(name).$(ox) $(mods:=.$(ox)) 

$(name):  $(objs)
	$(cc) $(larg) $(libs)
	
%.$(ox): %.cpp
	$(cc) $(co) $<
