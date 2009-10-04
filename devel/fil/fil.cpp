/*
	'fil' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'fil' header file ('fil.h').
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
		Flags |= _O_TRUNC | _O_CREAT |_O_RDWR;
		PMode |= _S_IWRITE;
		break;
	case mAppend:
		Flags |= _O_CREAT | _O_APPEND | _O_RDWR;
		PMode |= _S_IWRITE | _S_IREAD;
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
		Flags |= O_TRUNC | O_CREAT | O_RDWR;
		break;
	case mAppend:
		Flags |= O_APPEND | O_RDWR;
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
	err::handle ErrHandle )
{
	bso::bool__ Success = false;
ERRProlog
	iop::descriptor__ Descriptor = IOP_UNDEFINED_DESCRIPTOR;
ERRBegin
	Descriptor = fil::Open( FileName, fil::mReadWrite );

	Success = ( Descriptor != IOP_UNDEFINED_DESCRIPTOR );

	if ( !Success && ( ErrHandle == err::hUsual ) )
		ERRf();
ERRErr
ERREnd
	if ( Descriptor != IOP_UNDEFINED_DESCRIPTOR )
		fil::Close( Descriptor );
ERREpilog
	return Success;
}


rbf fil::CreateBackupFile(
	const char *NomFichier,
	hbf Handle,
	txf::text_oflow__ &Flow,
	const char *Extension,
	err::handle Error )
{
	rbf Etat = rbfOK;
ERRProlog
	char *NomFichierSecurite = NULL;
ERRBegin
	if ( FileExists( NomFichier ) )
	{
		if ( ( NomFichierSecurite = (char *)malloc( strlen( NomFichier ) + strlen( Extension ) + 1 ) ) == NULL )
			ERRa();

		sprintf( NomFichierSecurite, "%s%s", NomFichier, Extension );

		if ( FileExists( NomFichierSecurite ) )
			if ( remove( NomFichierSecurite ) )
				Etat = rbfSuppression;

		if ( Etat == rbfOK )
			if ( Handle == hbfDuplicate )
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
					Etat = rbfDuplication;
					remove( NomFichierSecurite );
				}
			}
			else if ( Handle == hbfRename )
			{
				if ( rename( NomFichier, NomFichierSecurite ) )
					Etat = rbfRenaming;
			}
			else
				ERRu();
	}

	if ( Error == err::hUsual )
	{
		switch( Etat ) {
		case rbfSuppression:
			Flow << "Backup file creation: error at deletion of '" << NomFichierSecurite << "' file." << txf::nl;
			ERRu();
			break;
		case rbfDuplication:
			Flow << "Backup file creation: error while duplicating '" << NomFichier << "'." << txf::nl;
			ERRu();
			break;
		case rbfRenaming:
			Flow << "Backup file creation: error while renaming '" << NomFichier << "' in '" << NomFichierSecurite << "'." << txf::nl;
			ERRu();
			break;
		case rbfOK:
			break;
		default:
			ERRc();
			break;
		}
	}

ERRErr
ERREnd
	if ( NomFichierSecurite )
		free( NomFichierSecurite );
ERREpilog
	return Etat;
}



rbf fil::RecoverBackupFile(
	const char *NomFichier,
	txf::text_oflow__ &Flow,
	const char *Extension,
	err::handle Error )
{
	rbf Etat = rbfOK;
ERRProlog
	char *NomFichierSecurite = NULL;
ERRBegin
	if ( FileExists( NomFichier ) )
		if ( remove( NomFichier ) )
			Etat = rbfSuppression;

	if ( Etat == rbfOK )
		if ( ( NomFichierSecurite = (char *)malloc( strlen( NomFichier ) + strlen( Extension ) + 1 ) ) == NULL )
			Etat = rbfAllocation;

	if ( Etat == rbfOK )
	{
		sprintf( NomFichierSecurite, "%s%s", NomFichier, Extension );

		if ( FileExists( NomFichierSecurite ) )
			if ( rename( NomFichierSecurite, NomFichier ) )
				Etat = rbfRenaming;
	}

	if ( Error == err::hUsual )
	{
		switch( Etat ) {
		case rbfAllocation:
			Flow << "Backup file recovering: error during memory allocation (is about file '" << NomFichier << "')." << txf::nl;
			break;
		case rbfSuppression:
			Flow << "Backup file recovering: error at suppression of file'" << NomFichier << "'." << txf::nl;
			break;
		case rbfRenaming:
			Flow << "Backup file recovering: error while renaming '" << NomFichierSecurite << "' in '" << NomFichier << "'." << txf::nl;
			break;
		case rbfOK:
			break;
		default:
			ERRc();
			break;
		}
	}
ERRErr
ERREnd
	if ( NomFichierSecurite )
		free( NomFichierSecurite );
ERREpilog
	return Etat;
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
