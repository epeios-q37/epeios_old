# 'egeckocom' Makefile by $AUTHOR_NAME$ ($AUTHOR_EMAIL$)
# Builds XPCOM component for the 'egeckocom' software.
# Requires GNU make (http://www.gnu.org/).
# Copyright (C) $COPYRIGHT_DATES$ $COPYRIGHT_OWNER$.
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

name = emobdacom
copt =
# Epeios general debugging features
copt += -DXXX_DBG
# Required for multitasking
copt += -DCPE_MT -DMTK_KEEP
# Required when compiling a library.
# copt += -DNSXPCM_BKD
copt += -DCPE_LIBRARY
lopt =
mods += csdbnc csddlc
mods += dtfptb err fil flw fnm lcl mmi mtk nsxpcm rgstry sck str tht tol ttr uym xml xtf
mods += bkdacc bkdcmd bkdcst bkdrac bkdtpm 
mods += kernel registry
mods += frdbcknd frdkernl frdrgstry frdtrnsnt
mods += ui ui_base ui_bksdb ui_dbs_f ui_dbsdb ui_lst_v ui_main ui_rcd_f ui_rcd_v ui_struct

# To adapt !
arm-sdk=/home/csimon/xulrunner-sdk-arm
#i686-sdk=/home/csimon/xulrunner-sdk-i686
i686-sdk=/cygdrive/h/xulrunner-sdk


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
#co += -DMTX_NO_ATOMIC_OPERATIONS
co += -fshort-wchar
co += -include "mozilla-config.h"
ifeq ("$(DEB_BUILD_ARCH)","armel")
sdk=$(arm-sdk)
else
sdk=$(i686-sdk)
endif
co += -I$(sdk)/include
co += -I$(sdk)/include/xpcom
co += -I$(sdk)/include/content
co += -I$(sdk)/include/dom
co += -I$(sdk)/sdk/include
co += -I$(sdk)/include/layout
co += -I$(sdk)/include/exthandler
src += ../..
# Generates debugging data.
co += -g
# We want a library.
co += -fPIC
# General optimization.
#co += -O
# To use with gprof
#co += -pg
# lo = -shared -W1,-soname,$(name).so -o $(name).so 
lo = -shared -o $(name).so 
lo += -L$(sdk)/lib -L$(sdk)/bin -Wl,-soname,$(name).so,-rpath-link,$(sdk)/bin -lxpcomglue_s -lxpcom -lnspr4
# needed when pthread are used.
libs += -lpthread
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

ifeq ("$(env)","macosxu")
co = -c
# co += -O
co += -fPIC
co += -isysroot /Developer/SDKs/MacOSX10.4u.sdk
co += -arch i386
co += -arch ppc
lo = -o $(name).dylib
lo += -Wl,-syslibroot,/Developer/SDKs/MacOSX10.4u.sdk -dynamiclib
lo += -arch i386
lo += -arch ppc
lo += -L/csimon/mozilla/dist/lib -lnspr4 -lembed_base_s -lplc4 -lplds4 -lxpcom -lxpcomglue_s -lxpcom_core
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

$(name).so: $(objs)
	$(cc) $(objs) $(lo) $(libs) 
	
%.$(ox): %.cpp
	$(cc) $(co) $< 
