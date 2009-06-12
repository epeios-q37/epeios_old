/*
	Header for the 'emobdacgid' module by Claude SIMON.

	 This file is part of 'emobda' software, which iss part
	 of the Epeios project (http://zeusw.org/epeios/).

    'emobda' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'emobda' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'emobda'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#ifndef EMOBDACGID__INC
#define EMOBDACGID__INC

#include "epsmsc.h"

#define NAME			"emobdacgid"
#define VERSION			"0.0.1"
#define COPYRIGHT_YEARS	"2009"
#define DESCRIPTION		"EMobDa CGI frontend"
#define INFO			EPSMSC_EPEIOS_TEXT
#define AUTHOR_NAME		EPSMSC_AUTHOR_NAME
#define AUTHOR_CONTACT	EPSMSC_AUTHOR_CONTACT
#define HELP			EPSMSC_HELP_INVITATION( NAME )
#define COPYRIGHT		"Copyright (c) " COPYRIGHT_YEARS " " AUTHOR_NAME " (" AUTHOR_CONTACT ")."
#define CVS_DETAILS		("$Id$\b " + 5)

enum exit_value__ {
	evArguments = 2,
	evInitialization,
};


#endif
