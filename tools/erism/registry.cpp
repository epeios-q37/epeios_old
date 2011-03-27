/*
	'cnfgrtn' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of th 'erism' tool.
	Copyright (C) 2011 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.*/
// $Id$


#include "registry.h"

#define ROOT	""

#define S "/"

rgstry::multi_level_registry registry::Registry;

const char *registry::path::Language = "Language";
const char *registry::path::MIDIImplementationsFileName = "MIDIImplementationsFileName";
const char *registry::path::LocaleFileName = "LocaleFileName";
const char *registry::path::DefaultDeviceID = "DefaultDeviceID";

