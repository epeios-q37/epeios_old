# 'gendoc' Makefile by Claude L. Simon (epeios@epeios.org).
# Builds executable for the 'gendoc' software.
# Requires GNU make (http://www.gnu.org/).
# Copyright (C) 2000 Claude L. Simon.
#
# This file is part of the Epeios project (http://www.Epeios.org/).
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
# along with this program; if not, go to http://www.fsf.org or
# write to the:
#
#                       Free Software Foundation, Inc.,
#          59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#

# $Id$

name = gendoc
copt = -DXXX_DBG
lopt = 
libs = ttr tol flw stf err

ifeq ("$(EPEIOS_OS)","windows")
ox = obj
cc = bcc32
co = -q -c -w-aus
lo = -q -e$(name)
endif

ifeq ("$(EPEIOS_OS)","linux")
ox = o
cc = g++ 
co = -c -g
lo = -o $(name)
endif

ifeq ("$(EPEIOS_OS)","beos")
ox = o
cc = g++
co = -c
lo = -o $(name)
endif

co += -I$(EPEIOS_SRC) -I$(EPEIOS__SRC) $(EPEIOS_COPT) $(copt)
lo += $(EPEIOS_LOPT) $(lopt)

objs = $(name).$(ox) $(libs:=.$(ox))

$(name):  $(objs)
	$(cc) $(lo) $(objs)
	
%.$(ox): %.cpp
	$(cc) $(co) $< 

%.$(ox): $(EPEIOS_SRC)/%.cpp
	$(cc) $(co) $<

%.$(ox): $(EPEIOS__SRC)/%.cpp
	$(cc) $(co) $<
