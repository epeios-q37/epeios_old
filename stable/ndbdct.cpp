/*
	'ndbdct' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'ndbdct' header file ('ndbdct.h').
	Copyright (C) 2004 Claude SIMON.

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
	const char *BaseFileName )
{
ERRProlog
	flf::file_oflow___ Flow;
ERRBegin
	Flow.Init( BaseFileName );

	Save_( Bunch, Flow );
ERRErr
ERREnd
ERREpilog
}

template <typename item> static void Save_(
	const stk::E_BSTACK_( item ) &Bunch,
	const str::string_ &BaseFileName,
	const char *Extension,
	time_t UnderlyingFilesLastModificationTime )
{
ERRProlog
	str::string FileName;
	STR_BUFFER___ FileNameBuffer;
ERRBegin
	FileName.Init( BaseFileName );
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

void ndbdct::dynamic_content_atomized_file_manager___::_SaveLocationsAndAvailables( void ) const
{
	Save_( _Content->Availables, _BaseFileName, AVAILABLES_FILE_NAME_EXTENSION, _GetUnderlyingFilesLastModificationTime() );
}

void ndbdct::dynamic_content_atomized_file_manager___::Init(
	dynamic_content_ &Content,
	const str::string_ &BaseFileName,
	fil::mode__ Mode,
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

	ContentFileName.Init( BaseFileName );
	ContentFileName.Append( CONTENT_FILE_NAME_EXTENSION );
	_StorageFileManager.Init( ContentFileName.Convert( ContentFileNameBuffer ), Mode, true, ID );

	EntriesBunchFileName.Init( BaseFileName );
	EntriesBunchFileName.Append( ENTRIES_FILE_NAME_EXTENSION );

	EntriesListFileName.Init( BaseFileName );
	EntriesListFileName.Append( LIST_FILE_NAME_EXTENSION );

	_EntriesFileManager.Init( EntriesBunchFileName.Convert( EntriesBunchFileNameBuffer) , EntriesListFileName.Convert( EntriesListFileNameBuffer ), Mode, true, ID );

	_BaseFileName.Init( BaseFileName );
	_Mode = Mode;

	_Content = &Content;

//	_ConnectToFiles();
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
	const char *FileName,
	stk::E_BSTACK_( item ) &Bunch,
	item TestValue,
	time_t TimeStamp )
{
	bso::bool__ Success = false;
ERRProlog
	flf::file_iflow___ Flow;
	static flw::datum__ Buffer[sizeof( item )];
ERRBegin
	if ( Flow.Init( FileName, err::hUserDefined ) == fil::sSuccess ) {
		if ( fil::GetFileLastModificationTime( FileName ) < TimeStamp )
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

const char *BuildFileName_(
	const str::string_ &BaseFileName,
	const char *Extension,
	STR_BUFFER___ &Buffer )
{
ERRProlog
	str::string FileName;
ERRBegin
	FileName.Init( BaseFileName );
	FileName.Append( Extension );
	FileName.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}


template <typename item> static bso::bool__ Load_(
	const str::string_ &BaseFileName,
	stk::E_BSTACK_( item ) &Bunch,
	item TestValue,
	const char *Extension,
	time_t TimeStamp )
{
	bso::bool__ Success = false;
ERRProlog
	STR_BUFFER___ Buffer;
ERRBegin
	Success = Load_( BuildFileName_( BaseFileName, Extension, Buffer ), Bunch, TestValue, TimeStamp );
ERRErr
ERREnd
ERREpilog
	return Success;
}

bso::bool__ Test_(
	const str::string_ &BaseFileName,
	const char *Extension,
	time_t TimeStamp )
{
	bso::bool__ Success = false;
ERRProlog
	STR_BUFFER___ Buffer;
	const char *FileName = NULL;
ERRBegin
	FileName = BuildFileName_( BaseFileName, Extension, Buffer );

	if ( !fil::FileExists( FileName ) )
		ERRReturn;

	if ( fil::GetFileLastModificationTime( FileName ) < TimeStamp )
		ERRReturn;

	Success = true;
ERRErr
ERREnd
ERREpilog
	return Success;
}



uym::status__ ndbdct::dynamic_content_atomized_file_manager___::ConnectToFiles( uym::purpose__ Purpose )
{
	uym::status__ Status = uym::s_Undefined;
ERRProlog
	available__ TestAvailable;
ERRBegin
	Status = tym::Connect( _Content->Storage.Memory, _StorageFileManager, Purpose );

	if ( lstbch::Connect( _Content->Entries, _EntriesFileManager, Purpose ) != Status ) {
		Status = uym::sInconsistent;
		ERRReturn;
	}

	if ( Purpose == uym::pProceed )
		if ( Status == uym::sExists )
			_Content->S_.Unallocated = _StorageFileManager.FileSize();
		else
			_Content->S_.Unallocated = 0;

	if ( Status == uym::sExists )
		if ( Purpose == uym::pProceed ) {
			if ( !Load_<available__>( _BaseFileName, _Content->Availables, TestAvailable, AVAILABLES_FILE_NAME_EXTENSION, _GetUnderlyingFilesLastModificationTime() ) )
				Status = uym::sInconsistent;
		} else
			if ( !Test_( _BaseFileName, AVAILABLES_FILE_NAME_EXTENSION, _GetUnderlyingFilesLastModificationTime() ) )
					Status = uym::sInconsistent;
ERRErr
ERREnd
ERREpilog
	return Status;
}

void ndbdct::dynamic_content_atomized_file_manager___::_ErasePhysically( void )
{
ERRProlog
ERRBegin
	_StorageFileManager.Drop();
	_EntriesFileManager.Drop();

	ndbbsc::DropFile( _BaseFileName, AVAILABLES_FILE_NAME_EXTENSION );
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
