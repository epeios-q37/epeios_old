/*
	'fil' library by Claude SIMON (csimon@epeios.org)
	Requires the 'fil' header file ('fil.h').
	Copyright (C) 2000-2001, 2004 Claude SIMON (csimon@epeios.org).

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
#include "iof.h"

using namespace fil;

#ifdef IOF__USE_STANDARD_IO
static inline iof::descriptor__ _Open(
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
		strcat( Flags, "r+" );
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

static void _Close( iof::descriptor__ D )
{
	if ( fclose( D ) != 0 )
		ERRd();
}

#elif defined( IOF__USE_LOWLEVEL_IO )
#	ifdef CPE__MS
#include "sys/stat.h"

static inline iof::descriptor__ _Open(
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
		break;
	case mReadWrite:
		Flags |= _O_CREAT | _O_RDWR;
		PMode |= _S_IWRITE;
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

static void _Close( iof::descriptor__ D )
{
	if ( _close( D ) != 0 )
		ERRd();
}

#	elif defined( CPE__UNIX )
static inline iof::descriptor__ _Open(
	const char *Nom,
	mode__ Mode )
{
#ifdef CPE__CYGWIN
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
		Flags |= O_RDWR;
		break;
	case mReadOnly:
		Flags |= O_RDONLY;
		break;
	default:
		ERRu();
		break;
	}

	return open( Nom, Flags );
}

static void _Close( iof::descriptor__ D )
{
	if ( close( D ) != 0 )
		ERRd();
}


#	elif defined( CPE__MAC )
#		error "MAC not implemented yet !"
#	else
#		error "Unknow complation enviroment !"
#	endif
#else
#	error "Unknow IO enviroment !"
#endif

iof::descriptor__ fil::_Open(
	const char *Nom,
	mode__ Mode )
{
	return ::_Open( Nom, Mode );
}

void fil::_Close( iof::descriptor__ D )
{
	::_Close( D );
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
