/*
	'ndbdct' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'ndbdct' header file ('ndbdct.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#define NDBDCT__COMPILATION

#include "ndbdct.h"

class ndbdcttutor
: public ttr_tutor
{
public:
	ndbdcttutor( void )
	: ttr_tutor( NDBDCT_NAME )
	{
#ifdef NDBDCT_DBG
		Version = NDBDCT_VERSION "\b\bD $";
#else
		Version = NDBDCT_VERSION;
#endif
		Owner = NDBDCT_OWNER;
		Date = "$Date$";
	}
	virtual ~ndbdcttutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "flf.h"

using namespace ndbdct;

#define AVAILABLES_FILE_NAME_EXTENSION	".eda"
#define CONTENT_FILE_NAME_EXTENSION		".edc"
#define ENTRIES_FILE_NAME_EXTENSION		".ede"
#define LIST_FILE_NAME_EXTENSION		".edl"

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
	const char *Extension,
	time_t UnderlyingFilesLastModificationTime )
{
ERRProlog
	str::string FileName;
	STR_BUFFER___ FileNameBuffer;
ERRBegin
	FileName.Init( RootFileName );
	FileName.Append( Extension );
	Save_( Bunch, FileName.Convert( FileNameBuffer ) );

	while ( UnderlyingFilesLastModificationTime >= fil::GetFileLastModificationTime( FileNameBuffer ) ) {
		tol::Clock( true );
		fil::TouchFile( FileNameBuffer );
	}
ERRErr
ERREnd
ERREpilog
}

void ndbdct::dynamic_content_spreaded_file_manager___::_SaveLocationsAndAvailables( void ) const
{
	Save_( _Content->Availables, _RootFileName, AVAILABLES_FILE_NAME_EXTENSION, _GetUnderlyingFilesLastModificationTime() );
}

void ndbdct::dynamic_content_spreaded_file_manager___::Init(
	dynamic_content_ &Content,
	const str::string_ &RootFileName,
	mdr::mode__ Mode,
	flm::id__ ID )
{
ERRProlog
	str::string ContentFileName;
	STR_BUFFER___ ContentFileNameBuffer;
	str::string EntriesBunchFileName;
	STR_BUFFER___ EntriesBunchFileNameBuffer;
	str::string EntriesListFileName;
	STR_BUFFER___ EntriesListFileNameBuffer;
ERRBegin
	reset();

	ContentFileName.Init( RootFileName );
	ContentFileName.Append( CONTENT_FILE_NAME_EXTENSION );
	_StorageFileManager.Init( ContentFileName.Convert( ContentFileNameBuffer ), Mode, true, ID );

	EntriesBunchFileName.Init( RootFileName );
	EntriesBunchFileName.Append( ENTRIES_FILE_NAME_EXTENSION );

	EntriesListFileName.Init( RootFileName );
	EntriesListFileName.Append( LIST_FILE_NAME_EXTENSION );

	_EntriesFileManager.Init( EntriesBunchFileName.Convert( EntriesBunchFileNameBuffer) , EntriesListFileName.Convert( EntriesListFileNameBuffer ), Mode, true, ID );

	_RootFileName.Init( RootFileName );
	_Mode = Mode;

	_Content = &Content;

	_ConnectToFiles();
ERRErr
ERREnd
ERREpilog
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
		if ( fil::GetFileLastModificationTime( RootFileName ) < TimeStamp )
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
	STR_BUFFER___ FileNameBuffer;
ERRBegin
	FileName.Init( RootFileName );
	FileName.Append( Extension );
	Success = Load_( FileName.Convert( FileNameBuffer ), Bunch, TestValue, TimeStamp );
ERRErr
ERREnd
ERREpilog
	return Success;
}


bso::bool__ ndbdct::dynamic_content_spreaded_file_manager___::_ConnectToFiles( void )
{
	bso::bool__ Exists = false;
ERRProlog
	available__ TestAvailable;
ERRBegin
	Exists = tym::Connect( _Content->Storage.Memory, _StorageFileManager );

	if ( lstbch::Connect( _Content->Entries, _EntriesFileManager ) != Exists )
		ERRu();

	if ( Exists )
		_Content->S_.Unallocated = _StorageFileManager.FileSize();
	else
		_Content->S_.Unallocated = 0;

	if ( Exists ) {
		if ( !Load_<available__>( _RootFileName, _Content->Availables, TestAvailable, AVAILABLES_FILE_NAME_EXTENSION, _GetUnderlyingFilesLastModificationTime() ) )
			_Content->RebuildAvailables();
	}
ERRErr
ERREnd
ERREpilog
	return Exists;
}

void ndbdct::dynamic_content_spreaded_file_manager___::_ErasePhysically( void )
{
ERRProlog
ERRBegin
	_StorageFileManager.Drop();
	_EntriesFileManager.Drop();

	ndbbsc::DropFile( _RootFileName, AVAILABLES_FILE_NAME_EXTENSION );
ERRErr
ERREnd
ERREpilog
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class ndbdctpersonnalization
: public ndbdcttutor
{
public:
	ndbdctpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~ndbdctpersonnalization( void )
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

static ndbdctpersonnalization Tutor;

ttr_tutor &NDBDCTTutor = Tutor;
