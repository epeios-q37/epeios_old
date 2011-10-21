/*
	'sclmisc' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'sclmisc' header file ('sclmisc.h').
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

#define SCLMISC__COMPILATION

#include "sclmisc.h"

class sclmisctutor
: public ttr_tutor
{
public:
	sclmisctutor( void )
	: ttr_tutor( SCLMISC_NAME )
	{
#ifdef SCLMISC_DBG
		Version = SCLMISC_VERSION "\b\bD $";
#else
		Version = SCLMISC_VERSION;
#endif
		Owner = SCLMISC_OWNER;
		Date = "$Date$";
	}
	virtual ~sclmisctutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "fil.h"
#include "str.h"
#include "cio.h"

#include "scllocale.h"
#include "sclrgstry.h"

using namespace sclmisc;

void sclmisc::Initialize( const char *TargetName )
{
ERRProlog
	str::string LocaleRootPath, RegistryRootPath;
	STR_BUFFER___ Buffer;
ERRBegin
	LocaleRootPath.Init( "Locales/Locale[target=\"" );
	LocaleRootPath.Append( TargetName );
	LocaleRootPath.Append( "\"]" );

	RegistryRootPath.Init( "Configurations/Configuration[target=\"" );
	RegistryRootPath.Append( TargetName );
	RegistryRootPath.Append( "\"]" );

	scllocale::LoadTemporaryLocale( TargetName, LocaleRootPath.Convert( Buffer ) );

	sclrgstry::LoadRegistry( TargetName, RegistryRootPath.Convert( Buffer ) );

	scllocale::LoadLocale( TargetName, LocaleRootPath.Convert( Buffer ) );
ERRErr
ERREnd
ERREpilog
}

void sclmisc::Terminate( void )
{
	cio::COut << txf::commit;
	cio::CErr << txf::commit;
}


void sclmisc::CreateBackupFile( const char *FileName )
{
ERRProlog
	fil::backup_status__ Status = fil::bs_Undefined;
	str::string Translation;
ERRBegin
	if ( ( Status = fil::CreateBackupFile( FileName, fil::bmDuplicate, err::hUserDefined ) )!= fil::bsOK ) {
		Translation.Init();
		fil::GetTranslation( Status, FileName, scllocale::GetRack(), Translation );
		cio::CErr << Translation << txf::nl;
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
}

void sclmisc::RecoverBackupFile( const char *FileName )
{
ERRProlog
	fil::recover_status__ Status = fil::rs_Undefined;
	str::string Translation;
ERRBegin
	if ( ( Status = fil::RecoverBackupFile( FileName, err::hUserDefined ) )!= fil::rsOK ) {
		Translation.Init();
		fil::GetTranslation( Status, FileName, scllocale::GetRack(), Translation );
		cio::CErr << Translation << txf::nl;
		ERRExit( EXIT_FAILURE );
	}
ERRErr
ERREnd
ERREpilog
}



/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class sclmiscpersonnalization
: public sclmisctutor
{
public:
	sclmiscpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~sclmiscpersonnalization( void )
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

static sclmiscpersonnalization Tutor;

ttr_tutor &SCLMISCTutor = Tutor;
