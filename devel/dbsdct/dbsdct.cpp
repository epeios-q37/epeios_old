/*
	'dbsdct' library by Claude SIMON (csimon@epeios.org)
	Requires the 'dbsdct' header file ('dbsdct.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
	This file is part of the Epeios (http://epeios.org/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
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

#define DBSDCT__COMPILATION

#include "dbsdct.h"

class dbsdcttutor
: public ttr_tutor
{
public:
	dbsdcttutor( void )
	: ttr_tutor( DBSDCT_NAME )
	{
#ifdef DBSDCT_DBG
		Version = DBSDCT_VERSION "\b\bD $";
#else
		Version = DBSDCT_VERSION;
#endif
		Owner = DBSDCT_OWNER;
		Date = "$Date$";
	}
	virtual ~dbsdcttutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace dbsdct;

template <typename container> static bso::bool__ Set_(
	flm::E_FILE_MEMORY_DRIVER___ &MemoryDriver,
	const str::string_ &FileName,
	container &C )
{
	bso::bool__ Exists = false;
ERRProlog
	tol::E_FPOINTER___( bso::char__ ) FileNameBuffer;
ERRBegin
	FileNameBuffer = FileName.Convert();

	Exists = tol::FileExists( FileNameBuffer );

	MemoryDriver.Init( FileNameBuffer );
	MemoryDriver.Persistant();
	C.plug( MemoryDriver );
ERRErr
ERREnd
ERREpilog
	return Exists;
}


bso::bool__ dbsdct::file_content_::Init( const str::string_ &RootFileName )
{
	bso::bool__ Exists = false;
ERRProlog
	str::string FileName;
ERRBegin
	FileName.Init( RootFileName );
	FileName.Append( ".ctt" );
	Exists = Set_( _S.MemoryDriver.Storage, FileName, content_::Storage );
	content_::Storage.Memory.Allocate( _S.MemoryDriver.Storage.Size() );

	FileName.Init( RootFileName );
	FileName.Append( ".etr" );

	if ( Set_( _S.MemoryDriver.Entries, FileName, Entries.Bunch() ) != Exists )
		ERRu();

	this->RootFileName.Init( RootFileName );

	content_::Init();
	content_::_S.Unallocated = _S.MemoryDriver.Storage.Size();
	Entries.Bunch().Allocate( _S.MemoryDriver.Entries.Size() / sizeof( entry__ ), aem::mFit );
	Entries.Bunch().SetStepValue( 0 );	// Pas de préallocation ('Extent' == 'Size' ).
ERRErr
ERREnd
ERREpilog
	return Exists;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dbsdctpersonnalization
: public dbsdcttutor
{
public:
	dbsdctpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dbsdctpersonnalization( void )
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

static dbsdctpersonnalization Tutor;

ttr_tutor &DBSDCTTutor = Tutor;
