/*
	'mscmdd' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'mscmdd' header file ('mscmdd.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (http://zeusw.org/intl/contact.html).
$_RAW_$
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

#define MSCMDD__COMPILATION

#include "mscmdd.h"

class mscmddtutor
: public ttr_tutor
{
public:
	mscmddtutor( void )
	: ttr_tutor( MSCMDD_NAME )
	{
#ifdef MSCMDD_DBG
		Version = MSCMDD_VERSION "\b\bD $";
#else
		Version = MSCMDD_VERSION;
#endif
		Owner = MSCMDD_OWNER;
		Date = "$Date$";
	}
	virtual ~mscmddtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace mscmdd;

static void Convert_(
	const WCHAR *WString,
	description_ &String )
{
	while ( *WString ) {
		String.Append( (char)*WString );

		WString++;
	}

}

bso::ulong__ mscmdd::GetMidiInDeviceDescriptions( descriptions_ &Descriptions )
{
	bso::ulong__ Count;
ERRProlog
	MIDIINCAPS InCaps;
	bso::ulong__ Counter = 0;
	description Description;
ERRBegin
	Count =  midiInGetNumDevs();

	while ( Counter < Count ) {
		midiInGetDevCaps( Counter++, &InCaps, sizeof( InCaps ) );

		Description.Init();

		Convert_( InCaps.szPname, Description );

		Descriptions.Append( Description );
	}
ERRErr
ERREnd
ERREpilog
	return Count;
}

bso::ulong__ mscmdd::GetMidiOutDeviceDescriptions( descriptions_ &Descriptions )
{
	bso::ulong__ Count;
ERRProlog
	MIDIOUTCAPS OutCaps;
	bso::ulong__ Counter = 0;
	description Description;
ERRBegin
	Count =  midiOutGetNumDevs();

	while ( Counter < Count ) {
		midiOutGetDevCaps( Counter++, &OutCaps, sizeof( OutCaps ) );

		Description.Init();

		Convert_( OutCaps.szPname, Description );

		Descriptions.Append( Description );
	}
ERRErr
ERREnd
ERREpilog
	return Count;
}


using namespace mscmdd;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class mscmddpersonnalization
: public mscmddtutor
{
public:
	mscmddpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~mscmddpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static mscmddpersonnalization Tutor;

ttr_tutor &MSCMDDTutor = Tutor;
