/*
	'err' library by Claude SIMON (csimon@epeios.org)
	Requires the 'err' header file ('err.h').
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

#define ERR__COMPILATION

#include "err.h"

class errtutor
: public ttr_tutor
{
public:
	errtutor( void )
	: ttr_tutor( ERR_NAME )
	{
#ifdef ERR_DBG
		Version = ERR_VERSION "\b\bD $";
#else
		Version = ERR_VERSION;
#endif
		Owner = ERR_OWNER;
		Date = "$Date$";
	}
	virtual ~errtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/
#include "fnm.h"

using namespace err;

namespace err {
	err_ ERR;
}

#ifdef CPE__CONSOLE
#	include "cio.h"
#endif

#ifdef CPE__GUI
#	include "wx/defs.h"
#	include "wx/msgdlg.h"
#endif

#ifdef ERR__THREAD_SAFE
#	include "mtx.h"
#	include "mtk.h"
static mtx::mutex_handler__ MutexHandler_ = MTX_INVALID_HANDLER;
static mtk::thread_id__ ThreadID_;
#endif

#include "tol.h"

/* fin du pré-sous-module */
int err_::ExitValue = EXIT_SUCCESS;
int err_::Line = 0;
const char *err_::File = NULL;
#ifdef ERR_JMPUSE
jmp_buf *err_::Jump = NULL;
#endif
err::type err_::Major = err::ok;
int err_::Minor = 0;
err::type ERRFilter = err::ok;

#ifdef ERR__THREAD_SAFE
bool err::Concerned( void )
{
	return ( ThreadID_ == mtk::GetTID() );
}

void err::Unlock( void )
{
#if 0
#ifdef ERR_DBG	// Doesn't work (is ignored because there was necessary an error before ...
	if ( !ERR.Error || !err::Concerned() )
		ERRu();
#endif
#endif
	mtx::mutex___ M;

	M.Init( MutexHandler_ );

	ERR.Error = false;

	M.Unlock();
}
#endif

// Retourne une chaîne ASCII contenant une brève description de ce qui est passé en paramètre.
const char *err::Message(
	const char *Fichier,
	int Ligne,
	err::type Majeur,
	int Mineur,
	buffer__ &Buffer )
{
	tol::buffer__ TOLBuffer;

	strcpy( Buffer, tol::Date( TOLBuffer ) );

	strcat( Buffer, " " );

	strcat( Buffer, tol::Time( TOLBuffer ) );

	strcat( Buffer, " Error " );

	switch( Majeur ) {
	case err::alc:
		strcat( Buffer, "ALC" );
		break;
	case err::dvc:
		strcat( Buffer, "DVC" );
		break;
	case err::sys:
		strcat( Buffer, "SYS" );
		break;
	case err::usr:
		strcat( Buffer, "USR" );
		break;
	case err::bkd:
		strcat( Buffer, "BKD" );
		break;
	case err::itn:
		strcat( Buffer, "ITN" );
		break;
	case err::ccp:
		strcat( Buffer, "CCP" );
		break;
	case err::frm:
		strcat( Buffer, "FRM" );
		break;
	case err::phb:
		strcat( Buffer, "PHB" );
		break;
	case err::lmt:
		strcat( Buffer, "LMT" );
		break;
	case err::mem:
		strcat( Buffer, "MEM" );
		break;
	case err::ext:
		strcat( Buffer, "EXT" );
		break;
	default:
		strcat( Buffer, "unknow" );
		break;
	}

	sprintf( strchr( Buffer, 0 ), "(%i)", Mineur );
	strcat( Buffer, "; F: " );
	strcat( Buffer, fnm::GetFileName( Fichier ) );
	strcat( Buffer, ", L: " );
	sprintf( strchr( Buffer, 0 ), "%i", Ligne );

	return Buffer;
}


// Handler par défaut.
void err_::Handler(
	const char *Fichier,
	int Ligne,
	err::type Majeur,
	int Mineur )
{
#ifdef ERR__THREAD_SAFE
	if ( ( this->Error == false ) || !err::Concerned() )
	{
		mtx::mutex___ M;

		M.Init( MutexHandler_ );

		M.Lock();

		ThreadID_ = mtk::GetTID();
	}
#endif
	if ( Fichier && !this->Error )
	{
		this->Major = Majeur;
		this->Minor = Mineur;
		this->File = Fichier;
		this->Line = Ligne;
	}

	this->Error = true;

/* Following lines are here to allowing the insertion of a breakpoint which
is not concerned by the 'ITN' error. Concenrs the whole software, and
not the 'ERR' library, thus the using of 'XXX_DBG' and not 'ERR_DBG'. */
#ifdef XXX_DBG
	if ( Majeur != err::itn )
		this->Major = Majeur;	// Silly, but is only goal is to allox the insertion of a breakpoint.
	else
		this->Major = err::itn;	// Silly too, because same goal.
#endif

	ERRR();
}
	/* handler de traitement d'erreur; 'Fichier' contient le nom du fichier,
	'Ligne' le numéro de ligne, 'Type', le type de l'erreur */


void err::Final( void )
{

	if ( ERR.Major != err::itn ) {
		buffer__ Buffer;

		const char *Message = err::Message( ERR.File, ERR.Line, ERR.Major, ERR.Minor, Buffer );

		if ( ERR.ExitValue == EXIT_SUCCESS )
			ERR.ExitValue = EXIT_FAILURE;

		ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.

#ifdef CPE__CONSOLE
		cio::cout << txf::sync;
		cio::cerr << txf::nl << txf::tab << "{ " << Message << " } " << txf::nl << txf::sync /*<< '\a'*/;
#endif

#ifdef CPE__GUI
		wxMessageBox( Message, "Epeios error manager message", wxICON_ERROR );
#endif
	} else
		ERRRst();
}


/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */
class errpersonnalization
: public errtutor
{
public:
	errpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
#ifdef ERR__THREAD_SAFE
		MutexHandler_ = mtx::Create();
#endif
		ERR.Error = false;
		ERR.Handling = false;
	}
	~errpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
#ifdef ERR__THREAD_SAFE
		if ( MutexHandler_ != MTX_INVALID_HANDLER )
			mtx::Delete( MutexHandler_ );
#endif
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static errpersonnalization Tutor;

ttr_tutor &ERRTutor = Tutor;
