/*
	Test source for the 'xmllpr' library by Claude SIMON (http://zeusw.org/intl/contact.html).
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xmllpr.h"

#include "err.h"
#include "cio.h"
#include "fil.h"

using cio::cin;
using cio::cout;
using cio::cerr;

class callback__
: public xmllpr::callback__
{
protected:
	virtual void XMLLPRTag(
		const str::string_ &Prefix,
		const str::string_ &Name )
	{
		cout << "Tag : " << Prefix << txf::tab << Name << txf::nl;
	}
	virtual void XMLLPRValue( const str::string_ &Value )
	{
		cout << "Value : " << Value << txf::nl;
	}
	virtual void XMLLPRAttribute(
		const str::string_ &Prefix,
		const str::string_ &Name,
		const str::string_ &Value )
	{
		cout << "Attribute : " << Prefix << txf::tab << Name << txf::tab << Value << txf::nl;
	}
	virtual void XMLLPRTagClosed( void )
	{
		cout << "Tag closed" << txf::nl;
	}
	virtual void XMLLPRError(
		xmllpr::location__ Line,
		xmllpr::location__ Column )
	{
		cout << "ERROR : " << Line << ", " << Column << txf::nl;
	}
};

void Generic( int argc, char *argv[] )
{
ERRProlog
	fil::file_iflow___ FFlow;
	xtf::extended_text_iflow__ XFlow;
	callback__ Callback;
	xmllpr::parser Parser;
ERRBegin
	FFlow.Init( "h:\\cvs\\lgsb\\apps\\gesbib\\frontend\\api.xml" );
//	FFlow.Init( "essai.xml" );
	FFlow.EOFD( XTF_EOXT );

	XFlow.Init( FFlow );

	Parser.Init();

	Parser.Parse( XFlow, Callback );
ERRErr
ERREnd
ERREpilog
}

int main( int argc, char *argv[] )
{
ERRFProlog
ERRFBegin
	cout << "Test of library " << XMLLPRTutor.Name << ' ' << __DATE__" "__TIME__"\n";

	switch( argc ) {
	case 1:
		Generic( argc, argv );
		break;
	case 2:
		if ( !strcmp( argv[1], "/i" ) )
		{
			TTR.Advertise( cout );
			break;
		}
	default:
		cout << txf::sync;
		cerr << "\nBad arguments.\n";
		cout << "Usage: " << XMLLPRTutor.Name << " [/i]\n\n";
		ERRi();
	}

ERRFErr
ERRFEnd
ERRFEpilog
	return ERRExitValue;
}
