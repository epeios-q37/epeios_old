/*
	'dbssct' library by Claude SIMON (csimon@epeios.org)
	Requires the 'dbssct' header file ('dbssct.h').
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

#define DBSSCT__COMPILATION

#include "dbssct.h"

class dbsscttutor
: public ttr_tutor
{
public:
	dbsscttutor( void )
	: ttr_tutor( DBSSCT_NAME )
	{
#ifdef DBSSCT_DBG
		Version = DBSSCT_VERSION "\b\bD $";
#else
		Version = DBSSCT_VERSION;
#endif
		Owner = DBSSCT_OWNER;
		Date = "$Date$";
	}
	virtual ~dbsscttutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "dtfptb.h"

using namespace dbssct;

#include "flf.h"

#define LOCATIONS_FILE_NAME_EXTENSION	".edl"
#define CONTENT_FILE_NAME_EXTENSION		".edc"

template <typename container> static bso::bool__ Set_(
	flm::E_FILE_MEMORY_DRIVER___ &MemoryDriver,
	const char *FileName,
	mdr::mode__ Mode,
	container &C )
{
	bso::bool__ Exists = false;

	Exists = tol::FileExists( FileName );

	MemoryDriver.Init( FileName, Mode );
	MemoryDriver.Persistant();
	C.plug( MemoryDriver );

	if ( Exists )
		C.Allocate( tol::GetFileSize( FileName ) / C.GetItemSize() );

	return Exists;
}

static inline void Save_(
	epeios::row__ Row,
	flw::oflow__ &Flow )
{
	dtfptb::PutULong( *Row, Flow );
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

void dbssct::file_static_content_::_SaveLocations( void ) const
{
	Save_( _list_::Locations.Released, RootFileName, LOCATIONS_FILE_NAME_EXTENSION );
}

static inline void Load_(
	flw::iflow__ &Flow,
	epeios::row__ &Row )
{
	Row = dtfptb::GetULong( Flow );
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

template <typename item> static bso::bool__ Load_(
	const char *RootFileName,
	stk::E_BSTACK_( item ) &Bunch,
	item TestValue,
	time_t TimeStamp )
{
	bso::bool__ Success = false;
ERRProlog
	flf::file_iflow___ Flow;
	static flw::datum__ Buffer[sizeof( item )];
ERRBegin
	if ( Flow.Init( RootFileName, err::hSkip ) == fil::sSuccess ) {
		if ( tol::GetFileLastModificationTime( RootFileName ) < TimeStamp )
			ERRReturn;

		memcpy( Buffer, &TestValue, sizeof( item ) );

		Flow.EOFD( (void *)Buffer, sizeof( item ) );

		Load_( Flow, Bunch, TestValue );

		Flow.reset();

		Success = true;
	}
ERRErr
ERREnd
ERREpilog
	return Success;
}

template <typename item> static bso::bool__ Load_(
	const str::string_ &RootFileName,
	stk::E_BSTACK_( item ) &Bunch,
	item TestValue,
	const char *Extension,
	time_t TimeStamp )
{
	bso::bool__ Success = false;
ERRProlog
	str::string FileName;
	tol::E_FPOINTER___( bso::char__ ) FileNameBuffer;
ERRBegin
	FileName.Init( RootFileName );
	FileName.Append( Extension );
	Success = Load_( FileNameBuffer = FileName.Convert(), Bunch, TestValue, TimeStamp );
ERRErr
ERREnd
ERREpilog
	return Success;
}

static time_t GetModificationTimeStamp_( const str::string_ &FileName )
{
	time_t TimeStamp;
ERRProlog
	tol::E_FPOINTER___( bso::char__ ) FileNameBuffer;
ERRBegin
	FileNameBuffer = FileName.Convert();

	TimeStamp = tol::GetFileLastModificationTime( FileNameBuffer );
ERRErr
ERREnd
ERREpilog
	return TimeStamp;
}

bso::bool__ dbssct::file_static_content_::_ConnectToFiles( void )
{
	bso::bool__ Exists = false;
ERRProlog
	str::string ContentFileName;
	tol::E_FPOINTER___( bso::char__ ) ContentFileNameBuffer;
	str::string ListFileName;
	tol::E_FPOINTER___( bso::char__ ) ListFileNameBuffer;
ERRBegin
	ContentFileName.Init( RootFileName );
	ContentFileName.Append( CONTENT_FILE_NAME_EXTENSION );
	ContentFileNameBuffer = ContentFileName.Convert();
	Exists = Set_( S_.MemoryDriver.Storage, ContentFileNameBuffer, S_.Mode, static_content_::Storage );

	if ( Exists ) {
		time_t TimeStamp;

		TimeStamp = GetModificationTimeStamp_( ContentFileName );

		if ( !Load_<epeios::row__>( RootFileName, _list_::Locations.Released, NONE, LOCATIONS_FILE_NAME_EXTENSION, TimeStamp ) )
			RebuildLocations();
	}
ERRErr
ERREnd
ERREpilog
	return Exists;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class dbssctpersonnalization
: public dbsscttutor
{
public:
	dbssctpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~dbssctpersonnalization( void )
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

static dbssctpersonnalization Tutor;

ttr_tutor &DBSSCTTutor = Tutor;
