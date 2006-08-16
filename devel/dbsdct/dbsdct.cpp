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

#include "flf.h"

#define LOCATIONS_EXTENSION		".edl"
#define AVAILABLES_EXTENSION	".eda"

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

static inline void Save_(
	epeios::row__ Row,
	flw::oflow__ &Flow )
{
	dtfptb::PutULong( *Row, Flow );
}

static inline void Save_(
	const available__ &Available,
	flw::oflow__ &Flow )
{
	dtfptb::PutULong( *Available.Row, Flow );
	dtfptb::PutULong( Available.RawSize, Flow );
}

template <typename item> static void Save_(
	const stk::E_BSTACK_( item ) &Bunch,
	flw::oflow__ &Flow )
{
	stk::row__ Row = Bunch.First();

	while ( Row != NONE ) {
		Save_( Bunch( Row ), Flow );

		Row = Bunch.Next( Row );
	}
}

template <typename item> static void Save_(
	const stk::E_BSTACK_( item ) &Bunch,
	const char *RootFileName )
{
ERRProlog
	flf::file_oflow___ Flow;
ERRBegin
	Flow.Init( RootFileName );

	Save_( Bunch, Flow );
ERRErr
ERREnd
ERREpilog
}

template <typename item> static void Save_(
	const stk::E_BSTACK_( item ) &Bunch,
	const str::string_ &RootFileName,
	const char *Extension )
{
ERRProlog
	str::string FileName;
	tol::E_FPOINTER___( bso::char__ ) FileNameBuffer;
ERRBegin
	FileName.Init( RootFileName );
	FileName.Append( Extension );
	Save_( Bunch, FileNameBuffer = FileName.Convert() );
ERRErr
ERREnd
ERREpilog
}

void dbsdct::file_content_::_SaveLocationsAndAvailables( void ) const
{
	Save_( Entries.List().Locations.Released, RootFileName, LOCATIONS_EXTENSION );
	Save_( Availables, RootFileName, AVAILABLES_EXTENSION );
}

static inline void Load_(
	flw::iflow__ &Flow,
	epeios::row__ &Row )
{
	Row = dtfptb::GetULong( Flow );
}
	
static inline void Load_(
	flw::iflow__ &Flow,
	available__ &Available )
{
	Available.Row = dtfptb::GetULong( Flow );
	Available.RawSize = dtfptb::GetULong( Flow );
}
	

template <typename item> static void Load_(
	flw::iflow__ &Flow,
	stk::E_BSTACK_( item ) &Bunch,
	item TestValue )
{
	item Item;

	Load_( Flow, Item );

	while ( Item != TestValue ) {
		Bunch.Append( Item );
		Load_( Flow, Item );
	}
}

template <typename item> static void Load_(
	const char *RootFileName,
	stk::E_BSTACK_( item ) &Bunch,
	item TestValue )
{
ERRProlog
	flf::file_iflow___ Flow;
static flw::datum__ Buffer[sizeof( item )];
ERRBegin
	Flow.Init( RootFileName );

	memcpy( Buffer, &TestValue, sizeof( item ) );

	Flow.EOFD( (void *)Buffer, sizeof( item ) );

	Load_( Flow, Bunch, TestValue );
ERRErr
ERREnd
ERREpilog
}

template <typename item> static void Load_(
	const str::string_ &RootFileName,
	stk::E_BSTACK_( item ) &Bunch,
	item TestValue,
	const char *Extension )
{
ERRProlog
	str::string FileName;
	tol::E_FPOINTER___( bso::char__ ) FileNameBuffer;
ERRBegin
	FileName.Init( RootFileName );
	FileName.Append( Extension );
	Load_( FileNameBuffer = FileName.Convert(), Bunch, TestValue );
ERRErr
ERREnd
ERREpilog
}

bso::bool__ dbsdct::file_content_::Init( const str::string_ &RootFileName )
{
	bso::bool__ Exists = false;
ERRProlog
	str::string FileName;
	available__ TestAvailable;
ERRBegin
	FileName.Init( RootFileName );
	FileName.Append( ".edc" );
	Exists = Set_( _S.MemoryDriver.Storage, FileName, content_::Storage );
	content_::Storage.Memory.Allocate( _S.MemoryDriver.Storage.Size() );

	FileName.Init( RootFileName );
	FileName.Append( ".edb" );
	if ( Set_( _S.MemoryDriver.Entries, FileName, Entries.Bunch() ) != Exists )
		ERRu();

	this->RootFileName.Init( RootFileName );

	content_::Init();
	content_::_S.Unallocated = _S.MemoryDriver.Storage.Size();

	Entries.Bunch().Allocate( _S.MemoryDriver.Entries.Size() / sizeof( entry__ ), aem::mFit );
	Entries.Bunch().SetStepValue( 0 );	// Pas de préallocation ('Extent' == 'Size' ).

	Entries.List().Locations.Init( _S.MemoryDriver.Entries.Size() / sizeof( entry__ ) );

	if ( Exists ) {
		Load_<epeios::row__>( RootFileName, Entries.List().Locations.Released, NONE, LOCATIONS_EXTENSION );
		Load_<available__>( RootFileName, Availables, TestAvailable, AVAILABLES_EXTENSION );
	}
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
