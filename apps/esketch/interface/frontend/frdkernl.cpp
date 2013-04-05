/*
	'frdkernl.cpp' by Claude SIMON (http://zeusw.org/).

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

#include "frdkernl.h"
#include "flf.h"
#include "flx.h"
#include "lcl.h"
#include "sktfbc.h"

using namespace fbltyp;

using namespace frdkernl;
using xml::writer_;

#define CASE( n )\
	case m##n:\
	return "FRD_" #n;\
	break

static const char *frdkernl::GetLabel( frdkernl::message__ Message )
{
	switch ( Message ) {
	CASE( NotImplementedYet );
	default:
		ERRCcp();
		break;
	}

	return NULL;	// To avois a 'warning'.
}

