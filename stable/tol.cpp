/*
  'tol' library by Claude L. Simon (simon@epeios.org)
  Requires the 'tol' header file ('tol.h').
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org).

  This file is part of the Epeios (http://www.epeios.org/) project.
  

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

#define TOL__COMPILATION

#include "tol.h"

class toltutor
: public ttr_tutor
{
public:
	toltutor( void )
	: ttr_tutor( TOL_NAME )
	{
#ifdef TOL_DBG
		Version = TOL_VERSION "\b\bD $";
#else
		Version = TOL_VERSION;
#endif
		Owner = TOL_OWNER;
		Date = "$Date$";
	}
	virtual ~toltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#ifdef CPE__MS
#	include <stdlib.h>
#	include "windows.h"
#elif defined( CPE__UNIX ) || defined( CPE__BEOS )
#	include <unistd.h>
#	include <sched.h>
#else
#	error "Unknown compilation enviroment"
#endif

#include "stf.h"

#ifdef CPE__MT
#include "mtk.h"
#endif

bool tol::FileExists( const char *Nom )
{
	ifstream Stream( Nom, ifstream::binary | ifstream::nocreate );

	return !Stream.fail();
}

tol::rbf tol::CreateBackupFile(
	const char *NomFichier,
	tol::hbf Handle,
	const char *Extension,
	err::handle Error )
{
	tol::rbf Etat = tol::rbfOK;
ERRProlog
	char *NomFichierSecurite = NULL;
ERRBegin
	if ( tol::FileExists( NomFichier ) )
	{
		if ( ( NomFichierSecurite = (char *)malloc( strlen( NomFichier ) + strlen( Extension ) + 1 ) ) == NULL )
			ERRa();

		sprintf( NomFichierSecurite, "%s%s", NomFichier, Extension );

		if ( tol::FileExists( NomFichierSecurite ) )
			if ( remove( NomFichierSecurite ) )
				Etat = tol::rbfSuppression;

		if ( Etat == tol::rbfOK )
			if ( Handle == tol::hbfDuplicate )
			{
				ofstream Out( NomFichierSecurite );
				ifstream In( NomFichier );
				int C;

				while( Out && In && ( ( C = In.get() ) != EOF ) )
					Out.put( (char)C );

				Out.close();
				In.close();

				if ( C != EOF )
				{
					Etat = tol::rbfDuplication;
					remove( NomFichierSecurite );
				}
			}
			else if ( Handle == tol::hbfRename )
			{
				if ( rename( NomFichier, NomFichierSecurite ) )
					Etat = tol::rbfRenaming;
			}
			else
				ERRu();
	}

	if ( Error == err::hUsual )
	{
		switch( Etat ) {
		case tol::rbfSuppression:
			ferr << "Backup file creation: error at deletion of '" << NomFichierSecurite << "' file." << txf::nl;
			ERRt();
			break;
		case tol::rbfDuplication:
			ferr << "Backup file creation: error while duplicating '" << NomFichier << "'." << txf::nl;
			ERRt();
			break;
		case tol::rbfRenaming:
			ferr << "Backup file creation: error while renaming '" << NomFichier << "' in '" << NomFichierSecurite << "'." << txf::nl;
			ERRt();
			break;
		case tol::rbfOK:
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



tol::rbf tol::RecoverBackupFile(
	const char *NomFichier,
	const char *Extension,
	err::handle Error )
{
	tol::rbf Etat = tol::rbfOK;
ERRProlog
	char *NomFichierSecurite = NULL;
ERRBegin
	if ( tol::FileExists( NomFichier ) )
		if ( remove( NomFichier ) )
			Etat = tol::rbfSuppression;

	if ( Etat == tol::rbfOK )
		if ( ( NomFichierSecurite = (char *)malloc( strlen( NomFichier ) + strlen( Extension ) + 1 ) ) == NULL )
			Etat = tol::rbfAllocation;

	if ( Etat == tol::rbfOK )
	{
		sprintf( NomFichierSecurite, "%s%s", NomFichier, Extension );

		if ( tol::FileExists( NomFichierSecurite ) )
			if ( rename( NomFichierSecurite, NomFichier ) )
				Etat = tol::rbfRenaming;
	}

	if ( Error == err::hUsual )
	{
		switch( Etat ) {
		case tol::rbfAllocation:
			ferr << "Backup file recovering: error during memory allocation (is about file '" << NomFichier << "')." << txf::nl;
			break;
		case tol::rbfSuppression:
			ferr << "Backup file recovering: error at suppression of file'" << NomFichier << "'." << txf::nl;
			break;
		case tol::rbfRenaming:
			ferr << "Backup file recovering: error while renaming '" << NomFichierSecurite << "' in '" << NomFichier << "'." << txf::nl;
			break;
		case tol::rbfOK:
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

const char *tol::Date( void )
{
   struct tm *time_now;
   time_t secs_now;
   static char str[20];

   time(&secs_now);
   time_now = localtime(&secs_now);

   if ( !strftime(str, sizeof( str ), "%d/%m/%Y", time_now) )
	ERRl();

   return str;
}

const char *tol::Time( void )
{
   struct tm *time_now;
   time_t secs_now;
   static char str[20];

   time(&secs_now);
   time_now = localtime(&secs_now);

   if ( !strftime(str, sizeof( str ), "%H:%M:%S", time_now) )
	ERRl();

   return str;
}

const char *tol::DateAndTime( void )
{
   struct tm *time_now;
   time_t secs_now;
   static char str[20];

   time(&secs_now);
   time_now = localtime(&secs_now);

   if ( !strftime(str, sizeof( str ), "%d/%m/%Y %H:%M:%S", time_now) )
	ERRl();

   return str;
}

void tol::Wait( unsigned int Secondes )
{
#ifdef CPE__MS
	Sleep( Secondes * 1000 );
#elif defined( CPE__UNIX ) || defined( CPE__BEOS )
	while( Secondes )		
		Secondes = sleep( Secondes );
	/* Because the 'sleep()' may be interrupted by an exiting child
	in a multitasking program. (Is that a good idea ???)*/
#else
	#error "Unknown compilation enviroment"
#endif
}


#ifdef CPE__MT
namespace {
	void WaitAndExit( void *UP )
	{
		tol::Wait( (unsigned int)UP );
		exit( EXIT_SUCCESS );
	}
}

void tol::Yield( void )
{
#ifdef CPE__MS
	Sleep( 0 );
#elif defined( CPE__UNIX )
	if( sched_yield() != 0 )
		ERRs();
#else
	#error "Unknown compilation enviroment"
#endif
}

void tol::ForceExit( unsigned int Seconds )
{
	mtk::Launch( WaitAndExit, (void *)Seconds );
}
#endif

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class tolpersonnalization
: public toltutor
{
public:
	tolpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~tolpersonnalization( void )
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

static tolpersonnalization Tutor;

ttr_tutor &TOLTutor = Tutor;
