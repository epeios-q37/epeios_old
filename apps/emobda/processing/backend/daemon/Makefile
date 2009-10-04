# 'emobdadmn' Makefile by Claude SIMON ($http://zeusw.org/$)
# Builds executable for the 'emobdadmn' software.
# Requires GNU make (http://www.gnu.org/).
# Copyright (C) 2009 Claude SIMON.
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

# $$Id$$

name = emobdadmn
copt =
# Epeios general debugging features
copt += -DXXX_DBG
# Required for multitasking
copt += -DCPE_MT -DMTK_KEEP
#copt += -DMTX_NO_ATOMIC_OPERATIONS
#copt += -DCPE_64_BITS_FORBIDDEN
lopt = 
mods = btr cio clnarg dtfptb epsmsc err fil flm flw fnm
mods += idxbtr lcl lst mmi mtk que rgstry sck str tht tol ttr
mods += txf uym xml xtf
mods += dbsbsc dbsdct dbsidx dbstbl 
mods += csdbns csdsns
mods += bkdcore bkdmnger bkdrgstry
mods += bkdcmd bkdcst bkddsc bkdmng bkdrrm bkdtpm
mods += mbdbsc mbddsc mbdeng mbdfld mbdidx mbdmng mbdstr mbdtbl
mods += registry

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

ifeq ("$(env)","windows")
# compilation options
co = -q -c -w-aus -w-inl
# linkage options
lo = -q -e$(name)
endif

ifeq ("$(env)","gnu")
co = -c
# Generates debugging data.
#co += -g
# General optimization.
#co += -O
lo += -o $(name)
# needed when pthread are used.
libs += -lpthread
src += ..
src += ../..
# To use with gprof (both lines)
#co += -pg
#lo += -pg

ifeq ("$(OSTYPE)","linux-gnu")
else
endif
libs += -lm
endif

ifeq ("$(env)","beos")
co = -c -g
lo = -o $(name)
endif

ifeq ("$(env)","windows")
ox = obj
cc = bcc32
ds = ;
lrfs = @LinkResponseFile
endif

ifeq ("$(env)","gnu")
ox = o
cc = g++ 
ds = :
lrfs = `cat LinkResponseFile`
endif

ifeq ("$(env)","beos")
ox = o
cc = g++
ds = :
lrfs = `cat LinkResponseFile`
endif

vpath %.cpp $(subst $(ds),:,$(src))

srcd = $(subst $(ds), ,$(src))

co += $(copt) $(srcd:%=-I%) $(EPEIOS_COPT)
lo += $(EPEIOS_LOPT) $(lopt)

objs = $(name).$(ox) $(mods:=.$(ox))

$(name):  $(objs)
	$(cc) $(lo) $(objs) $(libs)
	
%.$(ox): %.cpp
	$(cc) $(co) $< 
