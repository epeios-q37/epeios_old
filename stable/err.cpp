/*
  'err' library by Claude L. Simon (epeios@epeios.org)
  Requires the 'err' header file ('err.h').
  Copyright (C) 2000 Claude L. SIMON (epeios@epeios.org).

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
  along with this program; if not, go to http://www.fsf.org or write to the:
  
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
		Version = ERR_VERSION " (DBG)";
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

#include <new.h>

err_ ERR;

#include "stf.h"

#ifdef ERR__THREAD_SAFE
#include "mtx.h"
#include "mtk.h"
static mtx::mutex_handler__ MutexHandler_ = MTX_INVALID_HANDLER;
static mtk::thread_id__ ThreadID_;
#endif

/* fin du pré-sous-module */
int err_::Line = 0;
const char *err_::File = NULL;
#ifdef ERR_JMPUSE
jmp_buf *err_::Jump = NULL;
#endif
err::type err_::Major = err::ok;
int err_::Minor = 0;
err::type ERRFilter = err::ok;

#ifdef ERR__THREAD_SAFE
bool ERRConcerned( void )
{
	return ( ThreadID_ == MTKGetTID() );
}
void ERRUnlock( void )
{
#if 0
#ifdef ERR_DBG	// Doesn't work (is ignored because there was necessary an error before ...
	if ( !ERR.Error || !ERRConcerned() )
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
const char *ERRMessage(
	const char *Fichier,
	int Ligne,
	err::type Majeur,
	int Mineur )
{
	static char Message[150];

	strcpy( Message, "{ " );

	strcat( Message, TOLDate() );

	strcat( Message, " " );

	strcat( Message, TOLTime() );

	strcat( Message, " Error " );

	switch( Majeur ) {
	case err::alc:
		strcat( Message, "ALC" );
		break;
	case err::dvc:
		strcat( Message, "DVC" );
		break;
	case err::sys:
		strcat( Message, "SYS" );
		break;
	case err::usr:
		strcat( Message, "USR" );
		break;
	case err::brk:
		strcat( Message, "BRK" );
		break;
	case err::thw:
		strcat( Message, "THW" );
		break;
	case err::ccp:
		strcat( Message, "CCP" );
		break;
	case err::frm:
		strcat( Message, "FRM" );
		break;
	case err::phb:
		strcat( Message, "PHB" );
		break;
	case err::lmt:
		strcat( Message, "LMT" );
		break;
	case err::mem:
		strcat( Message, "MEM" );
		break;
	default:
		strcat( Message, "unknow" );
		break;
	}

	sprintf( strchr( Message, 0 ), "(%i)", Mineur );
	strcat( Message, "; F: " );
	strcat( Message, fnm::GetFileName( Fichier ) );
	strcat( Message, ", L: " );
	sprintf( strchr( Message, 0 ), "%i", Ligne );
	strcat( Message, " }\0" );

	return Message;
}


// Handler par défaut.
void err_::Handler(
	const char *Fichier,
	int Ligne,
	err::type Majeur,
	int Mineur )
{
#ifdef ERR__THREAD_SAFE
	if ( ( this->Error == false ) || !ERRConcerned() )
	{
		mtx::mutex___ M;

		M.Init( MutexHandler_ );

		M.Lock();

		ThreadID_ = MTKGetTID();
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

	ERRR();
}
	/* handler de traitement d'erreur; 'Fichier' contient le nom du fichier,
	'Ligne' le numéro de ligne, 'Type', le type de l'erreur */


void ERRFinal( void )
{
	const char *Message = ERRMessage( ERR.File, ERR.Line, ERR.Major, ERR.Minor );

	fout << txf::sync;
	ferr << txf::nl << txf::tab << Message << txf::nl /*<< '\a'*/;

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
