/*
	'fil' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'fil' header file ('fil.h').
	Copyright (C) 2000-2001, 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#define FIL__COMPILATION

#include "fil.h"

class filtutor
: public ttr_tutor
{
public:
	filtutor( void )
	: ttr_tutor( FIL_NAME )
	{
#ifdef FIL_DBG
		Version = FIL_VERSION "\b\bD $";
#else
		Version = FIL_VERSION;
#endif
		Owner = FIL_OWNER;
		Date = "$Date$";
	}
	virtual ~filtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "cpe.h"
#include <fstream>
#include <stdlib.h>

#include "str.h"
#include "lcl.h"

using namespace fil;

#ifdef IOP__USE_STANDARD_IO
static inline iop::descriptor__ Open_(
	const char *Nom,
	mode__ Mode )
{
	char Flags[4]="";

	switch ( Mode ) {
	case mRemove:
		strcat( Flags, "w+" );
		break;
	case mAppend:
		strcat( Flags, "a+" );
		break;
	case mReadWrite:
		if ( FileExists( Nom ) )
			strcat( Flags, "r+" );
		else
			strcat( Flags, "w+" );
		break;
	case mReadOnly:
		strcat( Flags, "r" );
		break;
	default:
		ERRu();
		break;
	}

	strcat( Flags, "b" );

	return fopen( Nom, Flags );
}

static void Close_( iop::descriptor__ D )
{
	if ( fclose( D ) != 0 )
		ERRd();
}

#elif defined( IOP__USE_LOWLEVEL_IO )
#	ifdef CPE__P_MS

static inline iop::descriptor__ Open_(
	const char *Nom,
	mode__ Mode )
{
	int Flags = _O_BINARY;
	int PMode = 0;

	switch ( Mode ) {
	case mRemove:
		Flags |= _O_TRUNC | _O_CREAT |_O_WRONLY;
		PMode |= _S_IWRITE;
		break;
	case mAppend:
		Flags |= _O_CREAT | _O_APPEND | _O_WRONLY;
		PMode |= _S_IWRITE /*| _S_IREAD*/;
		break;
	case mReadWrite:
		Flags |= _O_CREAT | _O_RDWR;
		PMode |= _S_IWRITE | _S_IREAD;
		break;
	case mReadOnly:
		Flags |= _O_RDONLY;
		break;
	default:
		ERRu();
		break;
	}

	return _open( Nom, Flags, PMode );
}

static void Close_( iop::descriptor__ D )
{
	if ( _close( D ) != 0 )
		ERRd();
}

#	elif defined( CPE__P_LINUX ) ||  defined( CPE__P_CYGWIN ) ||  defined( CPE__P_MAC )
static inline iop::descriptor__ Open_(
	const char *Nom,
	mode__ Mode )
{
#ifdef CPE__P_CYGWIN
	int Flags = O_BINARY;
#else
	int Flags = 0;
#endif

	switch ( Mode ) {
	case mRemove:
		Flags |= O_TRUNC | O_CREAT | O_WRONLY;
		break;
	case mAppend:
		Flags |= _APPEND | O_WRONLY;
		break;
	case mReadWrite:
		Flags |= O_CREAT | O_RDWR;
		break;
	case mReadOnly:
		Flags |= O_RDONLY;
		break;
	default:
		ERRu();
		break;
	}

	return open( Nom, Flags, 0666 );	/* rw-rw-rw- */
}

static void Close_( iop::descriptor__ D )
{
	if ( close( D ) != 0 )
		ERRd();
}

#	else
#		error "Unknow complation enviroment !"
#	endif
#else
#	error "Unknow IO enviroment !"
#endif

iop::descriptor__ fil::Open(
	const char *Nom,
	mode__ Mode )
{
	return ::Open_( Nom, Mode );
}

void fil::Close( iop::descriptor__ D )
{
	::Close_( D );
}

bso::bool__ fil::CreateFile(
	const char *FileName,
	err::handling__ ErrorHandling )
{
	bso::bool__ Success = false;
ERRProlog
	iop::descriptor__ Descriptor = IOP_UNDEFINED_DESCRIPTOR;
ERRBegin
	Descriptor = fil::Open( FileName, fil::mReadWrite );

	Success = ( Descriptor != IOP_UNDEFINED_DESCRIPTOR );

	if ( !Success && ( ErrorHandling == err::hThrowException ) )
		ERRf();
ERRErr
ERREnd
	if ( Descriptor != IOP_UNDEFINED_DESCRIPTOR )
		fil::Close( Descriptor );
ERREpilog
	return Success;
}

#define BUFFER___	tol::free_pointer___< char>

static const char* BuildBackupFileName_(
	const char *FileName,
	BUFFER___ &Buffer )
{

	if ( ( Buffer = (char *)malloc( strlen( FileName ) + sizeof( FIL__BACKUP_FILE_EXTENSION  ) + 1 ) ) == NULL )
		ERRa();

	sprintf( Buffer, "%s%s", FileName, FIL__BACKUP_FILE_EXTENSION );

	return Buffer;
}

#define CASE( label )	LCL_CASE( label, bs )

const char *fil::Label( backup_status__ Status )
{
	switch ( Status ) {
	CASE( UnableToRename )
	CASE( UnableToDuplicate )
	CASE( UnableToSuppress )
	default:
		ERRu();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

const str::string_ &fil::GetTranslation(
	backup_status__ Status,
	const char *FileName,
	const str::string_ &Language,
	const lcl::locale_ &Locale,
	str::string_ &Translation )
{
ERRProlog
	str::string Message;
	const char *BackupFileName = NULL;
	BUFFER___ Buffer;
ERRBegin
	Message.Init();

	Locale.GetTranslation( Label( Status ), Language, "EFIL_", Translation );

	switch ( Status ) {
	case bsUnableToRename:
	case bsUnableToDuplicate:
		lcl::ReplaceTag( Message, 1, str::string( FileName ) );
		break;
	case bsUnableToSuppress:
		lcl::ReplaceTag( Message, 1, str::string( BuildBackupFileName_( FileName, Buffer ) ) );
		break;
	default:
		ERRu();
		break;
	}
ERRErr
ERREnd
ERREpilog
	return Translation;
}

backup_status__ fil::CreateBackupFile(
	const char *NomFichier,
	backup_mode__ Mode,
	err::handling__ ErrorHandling )
{
	backup_status__ Status = bs_Undefined;
ERRProlog
	const char *NomFichierSecurite = NULL;
	BUFFER___ Buffer;
ERRBegin
	if ( FileExists( NomFichier ) )
	{
		NomFichierSecurite = BuildBackupFileName_( NomFichier, Buffer );

		if ( FileExists( NomFichierSecurite ) )
			if ( remove( NomFichierSecurite ) ) {
				Status = bsUnableToSuppress;
				ERRReturn;
			}

		if ( Mode == bmDuplicate )
		{
			std::ofstream Out( NomFichierSecurite );
			std::ifstream In( NomFichier );
			int C;

			while( Out && In && ( ( C = In.get() ) != EOF ) )
				Out.put( (char)C );

			Out.close();
			In.close();

			if ( C != EOF )
			{
				Status = bsUnableToDuplicate;
				remove( NomFichierSecurite );
				ERRReturn;
			}
		}
		else if ( Mode == bmRename )
		{
			if ( rename( NomFichier, NomFichierSecurite ) )
				Status = bsUnableToRename;
		}
		else
			ERRu();
	}

	Status = bsOK;

ERRErr
ERREnd
	if ( Status != bsOK )
		if ( ErrorHandling == err::hThrowException )
			ERRd();
ERREpilog
	return Status;
}

#undef CASE

#define CASE( label )	LCL_CASE( label, bs )

const char *fil::Label( recover_status__ Status )
{
	switch ( Status ) {
	CASE( UnableToRename )
	CASE( UnableToSuppress )
	default:
		ERRu();
		break;
	}

	return NULL;	// Pour éviter un 'warning'.
}

const str::string_ &fil::GetTranslation(
	recover_status__ Status,
	const char *FileName,
	const str::string_ &Language,
	const lcl::locale_ &Locale,
	str::string_ &Translation )
{
ERRProlog
	str::string Message;
	const char *BackupFileName = NULL;
	BUFFER___ Buffer;
ERRBegin
	Message.Init();

	Locale.GetTranslation( Label( Status ), Language, "EFIL_", Translation );

	switch ( Status ) {
	case rsUnableToRename:
		lcl::ReplaceTag( Message, 1, str::string( BuildBackupFileName_( FileName, Buffer ) ) );
		break;
	case rsUnableToSuppress:
		lcl::ReplaceTag( Message, 1, str::string( FileName ) );
		break;
	default:
		ERRu();
		break;
	}
ERRErr
ERREnd
ERREpilog
	return Translation;
}

recover_status__ fil::RecoverBackupFile(
	const char *NomFichier,
	err::handling__ ErrorHandling )
{
	recover_status__ Status = rs_Undefined;
ERRProlog
	const char *NomFichierSecurite = NULL;
	BUFFER___ Buffer;
ERRBegin
	if ( FileExists( NomFichier ) )
		if ( remove( NomFichier ) ) {
			Status = rsUnableToSuppress;
			ERRReturn;
		}

	NomFichierSecurite = BuildBackupFileName_( NomFichier, Buffer );

	if ( FileExists( NomFichierSecurite ) )
		if ( rename( NomFichierSecurite, NomFichier ) ) {
			Status = rsUnableToRename;
			ERRReturn;
		}

	Status = rsOK;

ERRErr
ERREnd
	if ( Status != bsOK )
		if ( ErrorHandling == err::hThrowException )
			ERRd();
ERREpilog
	return Status;
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class filpersonnalization
: public filtutor
{
public:
	filpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~filpersonnalization( void )
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

static filpersonnalization Tutor;

ttr_tutor &FILTutor = Tutor;
