/*
	'dbssct' library by Claude SIMON (csimon@epeios.org)
	Requires the 'dbssct' header file ('dbssct.h').
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

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

#define LIST_FILE_NAME_EXTENSION	".edl"
#define CONTENT_FILE_NAME_EXTENSION		".edc"

static void Save_(
	const _list_ &List,
	const str::string_ &RootFileName,
	const char *Extension,
	time_t UnderlyingFilesLastModificationTime )
{
ERRProlog
	str::string FileName;
	tol::E_FPOINTER___( bso::char__ ) FileNameBuffer;
ERRBegin
	FileName.Init( RootFileName );
	FileName.Append( Extension );
	lst::WriteToFile( List, FileNameBuffer = FileName.Convert() );

	while ( UnderlyingFilesLastModificationTime >= tol::GetFileLastModificationTime( FileNameBuffer ) ) {
		tol::Clock( true );
		tol::Touch( FileNameBuffer );
	}
ERRErr
ERREnd
ERREpilog
}

void dbssct::file_static_content_::_SaveLocations( void ) const
{
	Save_( *this, RootFileName, LIST_FILE_NAME_EXTENSION, _GetUnderlyingFilesLastModificationTime() );
}

void dbssct::file_static_content_::Init(
	epeios::size__ Size,
	const str::string_ &RootFileName,
	mdr::mode__ Mode,
	bso::bool__ Partial )
{
ERRProlog
	str::string ContentFileName;
	tol::E_FPOINTER___( bso::char__ ) ContentFileNameBuffer;
ERRBegin
	reset();

	this->RootFileName.Init( RootFileName );
	S_.Mode = Mode;

	static_content_::Init( Size, Partial );

	ContentFileName.Init( RootFileName );
	ContentFileName.Append( CONTENT_FILE_NAME_EXTENSION );
	ContentFileNameBuffer = ContentFileName.Convert();

	S_.FileManager.Init( ContentFileNameBuffer, Mode, true );
ERRErr
ERREnd
ERREpilog
}

// Permet de stocker les données entièrement en mémoire. NON UTILISABLE_EN_EXPOITATION !
//#define IN_MEMORY

bso::bool__ dbssct::file_static_content_::_ConnectToFiles( void )
{
	bso::bool__ Exists = false;
ERRProlog
	str::string ContentFileName;
	tol::E_FPOINTER___( bso::char__ ) ContentFileNameBuffer;
	str::string ListFileName;
	tol::E_FPOINTER___( bso::char__ ) ListFileNameBuffer;
#ifdef IN_MEMORY
	tym::E_MEMORY( atom__) Storage;
#endif
ERRBegin
	ContentFileName.Init( RootFileName );
	ContentFileName.Append( CONTENT_FILE_NAME_EXTENSION );
	ContentFileNameBuffer = ContentFileName.Convert();
#ifdef IN_MEMORY
	Storage.Init();
	Exists = Set_( S_.MemoryDriver.Storage, ContentFileNameBuffer, S_.Mode, Storage );

	static_content_::Storage.Allocate( tol::GetFileSize( ContentFileNameBuffer ) );
	static_content_::Storage.Store( Storage, tol::GetFileSize( ContentFileNameBuffer ) );
#else
//	Exists = Set_( S_.MemoryDriver.Storage, ContentFileNameBuffer, S_.Mode, static_content_::Storage );
	Exists = tym::Connect( this->Storage, S_.FileManager );
#endif

	if ( Exists ) {
		ListFileName.Init( RootFileName );
		ListFileName.Append( LIST_FILE_NAME_EXTENSION );
		ListFileNameBuffer = ContentFileName.Convert();

		if ( !lst::ReadFromFile( ListFileNameBuffer, tol::GetFileSize( ContentFileNameBuffer ) / S_.Size, *this, _GetUnderlyingFilesLastModificationTime() ) )
			RebuildLocations();
	}
ERRErr
ERREnd
ERREpilog
	return Exists;
}

void dbssct::file_static_content_::_Drop( void )
{
ERRProlog
ERRBegin
	S_.FileManager.Drop();

	dbsbsc::DropFile( RootFileName, LIST_FILE_NAME_EXTENSION );
ERRErr
ERREnd
ERREpilog
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
