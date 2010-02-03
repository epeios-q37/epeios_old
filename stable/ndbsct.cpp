/*
	'ndbsct' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'ndbsct' header file ('ndbsct.h').
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

#define NDBSCT__COMPILATION

#include "ndbsct.h"

class ndbscttutor
: public ttr_tutor
{
public:
	ndbscttutor( void )
	: ttr_tutor( NDBSCT_NAME )
	{
#ifdef NDBSCT_DBG
		Version = NDBSCT_VERSION "\b\bD $";
#else
		Version = NDBSCT_VERSION;
#endif
		Owner = NDBSCT_OWNER;
		Date = "$Date$";
	}
	virtual ~ndbscttutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace ndbsct;

#define LIST_FILE_NAME_EXTENSION	".edl"
#define CONTENT_FILE_NAME_EXTENSION		".edc"

static void Save_(
	const _list_ &List,
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
	lst::WriteToFile( List, FileName.Convert( FileNameBuffer ), UnderlyingFilesLastModificationTime );

/*
	while ( UnderlyingFilesLastModificationTime >= tol::GetFileLastModificationTime( FileNameBuffer ) ) {
		tol::Clock( true );
		tol::Touch( FileNameBuffer );
	}
*/
ERRErr
ERREnd
ERREpilog
}

void ndbsct::static_content_atomized_file_manager___::_SaveLocations( void ) const
{
	Save_( *_Content, _BaseFileName, LIST_FILE_NAME_EXTENSION, _GetUnderlyingFilesLastModificationTime() );
}

void ndbsct::static_content_atomized_file_manager___::Init(
	static_content_ &Content,
	const str::string_ &BaseFileName,
	mdr::mode__ Mode,
	flm::id__ ID )
{
ERRProlog
	str::string ContentFileName;
	tol::E_FPOINTER___( bso::char__ ) ContentFileNameBuffer;
ERRBegin
	reset();

	_BaseFileName.Init( BaseFileName );
	_Mode = Mode;

	ContentFileName.Init( BaseFileName );
	ContentFileName.Append( CONTENT_FILE_NAME_EXTENSION );

	_FileManager.Init( ContentFileName.Convert( ContentFileNameBuffer ), Mode, true, ID );

	_Content = &Content;

//	_ConnectToFiles();
ERRErr
ERREnd
ERREpilog
}

// Permet de stocker les données entièrement en mémoire. NON UTILISABLE_EN_EXPOITATION !
//#define IN_MEMORY

bso::bool__ ndbsct::static_content_atomized_file_manager___::ConnectToFiles( void )
{
	bso::bool__ Exists = false;
ERRProlog
	str::string ContentFileName;
	STR_BUFFER___ ContentFileNameBuffer;
	str::string ListFileName;
	STR_BUFFER___ ListFileNameBuffer;
#ifdef IN_MEMORY
	tym::E_MEMORY( atom__) Storage;
#endif
ERRBegin
	ContentFileName.Init( _BaseFileName );
	ContentFileName.Append( CONTENT_FILE_NAME_EXTENSION );
#ifdef IN_MEMORY
	Storage.Init();
	Exists = Set_( S_.MemoryDriver.Storage, ContentFileNameBuffer, S_.Mode, Storage );

	static_content_::Storage.Allocate( tol::GetFileSize( ContentFileNameBuffer ) );
	static_content_::Storage.Store( Storage, tol::GetFileSize( ContentFileNameBuffer ) );
#else
//	Exists = Set_( S_.MemoryDriver.Storage, ContentFileNameBuffer, S_.Mode, static_content_::Storage );
	Exists = tym::Connect( _Content->Storage, _FileManager );
#endif

	if ( Exists ) {
		ListFileName.Init( _BaseFileName );
		ListFileName.Append( LIST_FILE_NAME_EXTENSION );

		if ( !lst::ReadFromFile( ListFileName.Convert( ListFileNameBuffer ), fil::GetFileSize( ContentFileName.Convert( ContentFileNameBuffer ) ) / _Content->S_.Size, *_Content, _GetUnderlyingFilesLastModificationTime() ) )
			_Content->RebuildLocations();
	}
ERRErr
ERREnd
ERREpilog
	return Exists;
}

void ndbsct::static_content_atomized_file_manager___::_ErasePhysically( void )
{
ERRProlog
ERRBegin
	_FileManager.Drop();

	ndbbsc::DropFile( _BaseFileName, LIST_FILE_NAME_EXTENSION );
ERRErr
ERREnd
ERREpilog
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class ndbsctpersonnalization
: public ndbscttutor
{
public:
	ndbsctpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~ndbsctpersonnalization( void )
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

static ndbsctpersonnalization Tutor;

ttr_tutor &NDBSCTTutor = Tutor;
