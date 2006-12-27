/*
	'fnm' library by Claude SIMON (csimon@epeios.org)
	Requires the 'fnm' header file ('fnm.h').
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

#define FNM__COMPILATION

#include "fnm.h"

class fnmtutor
: public ttr_tutor
{
public:
	fnmtutor( void )
	: ttr_tutor( FNM_NAME )
	{
#ifdef FNM_DBG
		Version = FNM_VERSION "\b\bD $";
#else
		Version = FNM_VERSION;
#endif
		Owner = FNM_OWNER;
		Date = "$Date$";
	}
	virtual ~fnmtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "tol.h"

using namespace fnm;

const char *fnm::Description( fnm::type Type )
{
	switch ( Type ) {
	case fnm::tEmpty:
		return "Empty";
	case fnm::tLocalised:
		return "Localised";
	case fnm::tSuffixed:
		return "Suffixed";
	case fnm::tFree:
		return "Free";
	case fnm::tPath:
		return "Path";
	case fnm::tUnknow:
		return "Unknow";
	default:
		return "Undefined";
	}
}

fnm::type fnm::Type( const char *Nom )
{
	const char *Repere;

	if ( ( Nom == NULL ) || ( *Nom == '\0' ) )
		return fnm::tEmpty;

	if ( ( Nom[strlen( Nom )-1] == ':' )
			|| ( Nom[strlen( Nom )-1] == '/' )
			|| ( Nom[strlen( Nom )-1] == '\\' ) )
		return fnm::tPath;

	Repere = strrchr( Nom, ':' );

	if ( Repere < strrchr( Nom, '/' ) )
		Repere = strrchr( Nom, '/' );

	if ( Repere < strrchr( Nom, '\\' ) )
		Repere = strrchr( Nom, '\\' );

	if ( Repere )
		return fnm::tLocalised;

	if ( Repere < strrchr( Nom,'.' ) )
		return fnm::tSuffixed;
	else
		return fnm::tFree;
}

char *fnm::MakeFileName(
	const char *Rep,
	const char *Nom,
	const char *Ext )
{
	char *Resultat = NULL;
ERRProlog
	size_t TailleRep = 0, TailleNom = 0, TailleExt = 0;
ERRBegin
	if ( Rep )
		TailleRep = strlen( Rep );

	if ( Nom )
		TailleNom = strlen( Nom );

	if ( Ext )
		TailleExt = strlen( Ext );

	if ( TailleNom )
	{
		if ( ( Resultat = (char *)calloc( TailleRep + TailleNom + TailleExt + 2, sizeof( char ) ) ) == NULL )
			ERRa();
	}
	else
		return NULL;

	if ( TailleRep )
	{
		if ( ( Rep[TailleRep-1] != ':' ) && ( Rep[TailleRep-1] != '/' ) && ( Rep[TailleRep-1] != '\\' ))
			sprintf( Resultat, "%s%c", Rep, FNM_DIRECTORY_SEPARATOR_CHARACTER );
		else
			sprintf( Resultat, "%s", Rep );
	}

	switch ( Type( Nom ) ) {
	case fnm::tEmpty:
		free( Resultat );
		Resultat = NULL;
		break;
	case fnm::tSuffixed:
		strcat( Resultat, Nom );
		break;
	case fnm::tLocalised:
		strcpy( Resultat, Nom );
		break;
	case fnm::tFree:
		strcat( Resultat, Nom );

		if ( TailleExt )
			strcat( Resultat, Ext );

		break;
	case fnm::tPath:
		strcpy( Resultat, Nom );
		break;
	case fnm::tUnknow:
		free( Resultat );
		Resultat = NULL;
		break;
	default:
		ERRc();
		break;
	}
ERRErr
	if ( Resultat )
		free( Resultat );
ERREnd
ERREpilog
	return Resultat;
}

#ifndef CPE__T_MT
const char *fnm::file_name_manager::MakeFileName_(
	const char *Nom,
	int Occurence )
{
	int i;
	static char Resultat[9];

	strcpy( Resultat, "00000000" );

	Resultat[7] = (char)( '0' + Occurence );

	i = strlen( Nom );

	while( i-- )
	{
		int j = i%7;

		Resultat[j] += Nom[i];

		Resultat[j] &= 0x7f;

		Resultat[j] %= (char)36;

		if ( Resultat[j] > 9 )
			Resultat[j] += (char)7;

		Resultat[j] += '0';
	}

	return Resultat;
}

char *fnm::file_name_manager::SearchFileName(
	const char *Repertoire,
	const char *Parametres,
	const char *Extension )
{
	char *Nom = NULL;
ERRProlog
	int Occurence;
ERRBegin
	for ( Occurence = 0; Occurence <= 36; Occurence++ )
	{
		Nom = MakeFileName(
			Repertoire,
			MakeFileName_( Parametres, Occurence ),
			Extension );

		if ( !tol::FileExists( Nom ) || FNMMatch( Nom ) )
			break;

		free( (void *)Nom );
		Nom = NULL;
	}
ERRErr
	if ( Nom )
		free( Nom );

	Nom = NULL;
ERREnd
ERREpilog
	return Nom;
}

const char *fnm::GetFileNameRoot( const char *Nom )
{
	const char *Repere;
	static char Resultat[20];

	Repere = GetFileName( Nom );

	if ( strlen( Repere ) >= sizeof( Resultat ) )
		ERRl();

	strcpy( Resultat, Repere );

	if ( strchr( Resultat, '.' ) )
		*strchr( Resultat, '.' ) = 0;

	return Resultat;
}
#endif

char *fnm::CorrectLocation( const char *Location )
{
	char *Buffer = (char *)malloc( strlen( Location ) + 1 );
	char *P;

	if ( Buffer == NULL )
		ERRa();

	strcpy( Buffer, Location );

	P = strpbrk( Buffer, "\\/" );

	while( P != NULL ) {
		*P = FNM_DIRECTORY_SEPARATOR_CHARACTER;
		P = strpbrk( P+1, "\\/" );
	}

	return Buffer;
}

char *fnm::GetLocation( const char *Name )
{
	char *P = NULL;

	size_t L = GetFileName( Name ) - Name;

	if ( L != 0 ) {
		if ( ( P = (char *)malloc( L + 1 ) ) == NULL )
			ERRa();

		memcpy( P, Name, L );

		P[L] = 0;
	} else {
		if ( ( P = (char *)malloc( 1 ) ) == NULL )
			ERRa();

		P[0] = 0;
	}

	return P;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class fnmpersonnalization
: public fnmtutor
{
public:
	fnmpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~fnmpersonnalization( void )
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

static fnmpersonnalization Tutor;

ttr_tutor &FNMTutor = Tutor;
