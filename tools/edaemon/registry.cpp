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

# define ROOT	""

# define S "/"

const char *registry::paths::Service = ROOT "Service";

#define LOG_FILE_NAME	ROOT "LogFileName" S
const char *registry::paths::log::FileName = LOG_FILE_NAME;
const char *registry::paths::log::Mode = LOG_FILE_NAME "@Mode";

#define MODULES	ROOT "Modules" S
#define RAW_MODULE MODULES "Module" S
#define RAW_MODULE_ID "id"
#define MODULE RAW_MODULE "[" RAW_MODULE_ID "=\"" REGISTRY_TAG_MARKER_STRING "1\"]" S

const char *registry::paths::modules::module::Id = RAW_MODULE "@" RAW_MODULE_ID;
const char *registry::paths::modules::module::ConnectionType = RAW_MODULE "@ConnectionType";
const char *registry::paths::modules::module::Service = MODULE "Service";
const char *registry::paths::modules::module::FileName = MODULE "FileName";