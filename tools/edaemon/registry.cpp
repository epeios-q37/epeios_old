/*
	'registry' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of th 'edaemon' tool.
	Copyright (C) 2011 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    This file is part of 'edaemon'.

    'edaemon' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'edaemon' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'edaemon'.  If not, see <http://www.gnu.org/licenses/>.
*/	
// $Id$


#include "registry.h"

using namespace registry;

using rgstry::entry___;

static entry___ Log_( "Log", sclrgstry::Parameters );
entry___ registry::LogFileName( "FileName", Log_ );
entry___ registry::LogMode( "@Mode", Log_ );

static entry___ Modules_( "Modules" );
static entry___ ModuleFree_( "Module", Modules_ );
entry___ registry::ModuleId( "@id", ModuleFree_ );
static entry___ Module_( RGSTRY_TAGGING_ATTRIBUTE( "id" ), ModuleFree_ );
entry___ registry::ModuleFileName( "FileName", Module_ );
entry___ registry::ModuleService( "Service", Module_ );
entry___ registry::ModuleConnectionType( "ConnectionType", Module_ );

