/*
	'sktinf.h' by Claude SIMON (http://zeusw.org/).

    This file is part of 'eSketch' software.

    'eSketch' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eSketch' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eSketch'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

// eSKeTch INFormations.

#ifndef SKTINF__INC
# define SKTINF__INC

# include "epsmsc.h"

# define SKTINF_VERSION 					"0.1.0"
# define SKTINF_COPYRIGHT_YEARS				"2012"
# define SKTINF_LC_AFFIX					"esketch"
# define SKTINF_UC_AFFIX					"ESKETCH"
# define SKTINF_MC_AFFIX					"eSketch"
# define SKTINF_UC_SHORT					"SKT"
# define SKTINF_LC_SHORT					"skt"
# define SKTINF_WEBSITE_URL					"http://zeusw.org/intl/" SKTINF_LC_AFFIX "/"
# define SKTINF_AFFILIATED_PROJECT_NAME		"Epeios"
# define SKTINF_AFFILIATED_PROJECT_URL		EPSMSC_EPEIOS_URL
# define SKTINF_AFFILIATED_SOFTWARE_NAME	SKTINF_MC_AFFIX " (V" SKTINF_VERSION ")"
# define SKTINF_AFFILIATED_SOFTWARE_URL		SKTINF_WEBSITE_URL
# define SKTINF_DEPENDENCE					EPSMSC_EPEIOS_FRAMEWORK_DEPENDENCE
# define SKTINF_COPYRIGHT					EPSMSC_COPYRIGHT( SKTINF_COPYRIGHT_YEARS )
# define SKTINF_AUTHOR_NAME					EPSMSC_AUTHOR_NAME
# define SKTINF_AUTHOR_CONTACT				EPSMSC_AUTHOR_CONTACT

#endif
