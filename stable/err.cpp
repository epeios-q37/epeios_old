/*
	'err' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Requires the 'err' header file ('err.h').
	Copyright (C) 2000-2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

err_ err::ERR;

#include "cio.h"

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
#ifdef ERR__JMPUSE
jmp_buf *err_::Jump = NULL;
#endif
err::type err_::Type = err::t_None;

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

	ERR.Type = err::t_None;

	M.Unlock();
}
#endif

// Retourne une chaîne ASCII contenant une brève description de ce qui est passé en paramètre.
const char *err::Message(
	const char *Fichier,
	int Ligne,
	err::type Type,
	buffer__ &Buffer )
{
	tol::buffer__ TOLBuffer;

	strcpy( Buffer, tol::Date( TOLBuffer ) );

	strcat( Buffer, " " );

	strcat( Buffer, tol::Time( TOLBuffer ) );

	strcat( Buffer, " " );

	switch( Type ) {
	case err::tAllocation:
		strcat( Buffer, "ALC" );
		break;
	case err::tDevice:
		strcat( Buffer, "DVC" );
		break;
	case err::tSystem:
		strcat( Buffer, "SYS" );
		break;
	case err::tUser:
		strcat( Buffer, "USR" );
		break;
	case err::tBackend:
		strcat( Buffer, "BKD" );
		break;
	case err::tConception:
		strcat( Buffer, "CCP" );
		break;
	case err::tFlow:
		strcat( Buffer, "FLW" );
		break;
	case err::tProhibition:
		strcat( Buffer, "PRH" );
		break;
	case err::tLimitation:
		strcat( Buffer, "LMT" );
		break;
	case err::tMemory:
		strcat( Buffer, "MMR" );
		break;
	case err::tExternal:
		strcat( Buffer, "EXT" );
		break;
	case t_Free:
		strcat( Buffer, "Free" );
		break;
	case t_Exit:
		strcat( Buffer, "Exit" );
		break;
	case t_Return:
		strcat( Buffer, "Return" );
		break;
	case t_Abort:
		strcat( Buffer, "Abort" );
		break;
	default:
		strcat( Buffer, "Unknown" );
		break;
	}

	strcat( Buffer, " error : " );
	strcat( Buffer, fnm::GetFileName( Fichier ) );
	strcat( Buffer, "(" );
	sprintf( strchr( Buffer, 0 ), "%i", Ligne );
	strcat( Buffer, ")" );

	return Buffer;
}


// Handler par défaut.
void err_::Handler(
	const char *Fichier,
	int Ligne,
	err::type Type )
{
#ifdef ERR__THREAD_SAFE
	if ( ( !ERRError() ) || !err::Concerned() )
	{
		mtx::mutex___ M;

		M.Init( MutexHandler_ );

		M.Lock();

		ThreadID_ = mtk::GetTID();
	}
#endif
	if ( Fichier && !ERRError() )
	{
		this->Type = Type;
		this->File = Fichier;
		this->Line = Ligne;
	} else if ( !ERRError() )
		ERRc();

#if 0
	this->Error = true;
#endif

/* Following lines are here to allowing the insertion of a breakpoint which
is not concerned by the 'ITN' error. Concenrs the whole software, and
not the 'ERR' library, thus the using of 'XXX_DBG' and not 'ERR_DBG'. */
#ifdef XXX_DBG
	if ( Type < err::t_amount )
		this->Type = ( this->Type == Type ? Type : this->Type );	// Silly, but is only goal is to allow the insertion of a breakpoint.
	else
		this->Type = ( this->Type == Type ? Type : this->Type );	// Silly too, because same goal.
#endif

	ERRT();
}
	/* handler de traitement d'erreur; 'Fichier' contient le nom du fichier,
	'Ligne' le numéro de ligne, 'Type', le type de l'erreur */


void err::Final( void )
{

	if ( ERR.Type != err::t_Exit ) {
		buffer__ Buffer;

		const char *Message = err::Message( ERR.File, ERR.Line, ERR.Type, Buffer );

		if ( ERR.ExitValue == EXIT_SUCCESS )
			ERR.ExitValue = EXIT_FAILURE;

		ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.

ERRProlog
# ifdef CPE__MT
	cio::cout___ COut;
	cio::cerr___ CErr;
# else
	static txf::text_oflow__ &COut = cio::COut;
	static txf::text_oflow__ &CErr = cio::CErr;
# endif
ERRBegin
	if ( cio::IsInitialized() ) {
# ifdef CPE__MT
		COut.Init();
		CErr.Init();
# endif
		if ( cio::Target() == cio::tConsole ) {
			COut << txf::commit;
			CErr << txf::nl << txf::tab;
		}

		CErr << "{ " << Message << " }";

		if ( cio::Target() == cio::tConsole )
			CErr << txf::nl;

		CErr << txf::commit;
	}
ERRErr
ERREnd
ERREpilog
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
		MutexHandler_ = mtx::Create( mtx::mFree );
#endif
#if 0
		ERR.Error = false;
#else
		ERR.Type = err::t_None;
#endif
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
