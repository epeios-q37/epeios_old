/*
  Header for the 'err' library by Claude L. Simon (csimon@webmails.com)
  Copyright (C) 2000,2001 Claude L. SIMON (csimon@webmails.com) 

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

#ifndef ERR__INC
#define ERR__INC

#define ERR_NAME		"ERR"

#define	ERR_VERSION	"$Revision$"	

#define ERR_OWNER		"the Epeios project (http://epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &ERRTutor;

#if defined( XXX_DBG ) && !defined( ERR_NODBG )
#define ERR_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (csimon@webmails.com)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include <stdio.h>
#include <stdlib.h>

#ifdef ERR_JMPUSE
#include <setjmp.h>
#endif

#include "cpe.h"

#ifdef CPE__MT
#define ERR__THREAD_SAFE
#endif

struct err {
	enum handle {
		hUsual,
		hSkip };
	enum type {
		ok = 0,
		// no error
		alc = 1,
		// allocation error (only RAM)
		dvc = 2,
		// device error
		sys = 4,
		// system error
		usr = 8,
		// user error
		brk = 16,
		// broker error
		thw = 32,
		// throw error; launch the error behavior (user press the ctrl-break key, for example).
		ccp = 64,
		// conception error: anything unexpected occurs.
		frm = 128,
		// format error.
		phb = 256,
		// prohibibition error; a prohibited fonction was called.
		lmt = 512,
		// limit error; attempt to exceed the limit
		mem = 1024
		// memory error (all memory type).
	};
	enum a
	{
		aGeneric
	};
		// allocation error
	enum d
	{
		dGeneric
	};
		// device error
	enum s
	{
		sGeneric
	};
		// system error
	enum u
	{
		uGeneric
	};
		// user error
	enum b
	{
		bGeneric
	};
		// broker error
	enum t
	{
		tGeneric
	};
		// throw error
	enum c
	{
		cGeneric
	};
		// conception error
	enum f
	{
		fGeneric
	};
		// format error
	enum p
	{
		pGeneric
	};
		// prohibition error
	enum l
	{
		lGeneric
	};
		// limit overflow error
	enum m
	{
		mGeneric
	};
		// memory error
};

#ifdef ERR__THREAD_SAFE
// If an error occurs, test if the current thread is concerned.
bool ERRConcerned( void );
void ERRUnlock( void );
#endif

struct err_ {
	// line where the error occurs
	static int Line;
	// file where the error occurs
	static const char *File;
#ifdef ERR_JMPUSE
	// where to jump
	static jmp_buf *Jump;
#endif
	// Report that an error occurs
	int Error: 1;
	// Major code of error.
	static err::type Major;
	// Minor code of error
	static int Minor;
	// General handler.
	void Handler(
		const char *File = NULL,
		int Line = 0,
		err::type Major = Major,
		int Minor = Minor);
};

void ERRFinal( void );


#ifndef ERR__COMPILATION
extern err_ ERR;
#endif

#define ERRCommon( M, m )	ERR.Handler( __FILE__, __LINE__, M, err::m )

#define	ERRA( m )		ERRCommon( err::alc, m )
//m Throw an allocation error (only RAM).
#define	ERRa()			ERRA( aGeneric )

#define	ERRD( m )		ERRCommon( err::dvc, m )
//m Throw a device error.
#define ERRd()			ERRD( dGeneric )

#define	ERRS( m )		ERRCommon( err::sys, m )
//m Throw a system error.
#define ERRs()			ERRS( sGeneric )

#define ERRU( m )		ERRCommon( err::usr, m )
//m Throw an user error.
#define ERRu()			ERRU( uGeneric )

#define ERRB( m )		ERRCommon( err::brk, m )
//m Throw an interface error.
#define ERRb()			ERRB( bGeneric )

#define ERRT( m )		ERRCommon( err::thw, m )
//m Throw an error without an error.
#define ERRt()			ERRT( tGeneric )

#define ERRC( m )		ERRCommon( err::ccp, m )
//m Throw en conception error.
#define ERRc()			ERRC( cGeneric )

#define ERRF( m )		ERRCommon( err::frm, m )
//m Throw a format error.
#define ERRf()			ERRF( fGeneric )

#define ERRP( m )		ERRCommon( err::phb, m )
//m Throw a prohibited function call error.
#define ERRp()			ERRP( pGeneric )

#define ERRL( m )		ERRCommon( err::lmt, m )
//m Throw a limit exceed ferror.
#define ERRl()			ERRL( lGeneric )

#define ERRM( m )		ERRCommon( err::mem, m )
//m Throw a memory error.
#define ERRm()			ERRM( mGeneric )

#ifdef ERR_JMPUSE
//m Throw the handler.
#define ERRR()		{longjmp( *ERR.Jump, 1 );}
#else
#define ERRR()		throw( ERR )
#endif

#ifdef ERR__THREAD_SAFE
//m Discard error; restore the default behavior of the programm
#define ERRRst()	{ ERRUnlock(); }
#else
#define ERRRst()	{ ERR.Error = false; }
#endif

//d Major code.
#define ERRMajor		ERR.Major

//d Minor code.
#define ERRMinor		ERR.Minor

//d File in which the error was thrown.
#define ERRFile			ERR.File

//d Line where the error was thrown.
#define ERRLine			ERR.Line

#ifdef ERR_JMPUSE
#define ERRGetJ()		ERRFGetJ( ERR )
#define ERRPutJ( J )	ERRFSetJ( ERR, J )
#endif

#ifdef ERR_JMPUSE

//d Put the declaration after this.
#define ERRProlog	{ jmp_buf ERRJmp, *ERROJmp = ERRGetJ();\
					ERRPutJ( &ERRJmp );

//d Put the instructions to survey after this.
#define ERRBegin	if ( !setjmp( ERRJmp ) ) {

//d Put the instruction to launch if an error occurs.
#define ERRErr		} else { ERRPutJ( ERROJmp );

//d Put the instruction to launch, error or not.
#define ERREnd		}

#define ERRCommonEpilog	ERRPutJ( ERROJmp ); }

#else

#define ERRProlog	{
// précède les déclarations
#define ERRBegin	try {
// précède les instructions proprement dites
#define ERRErr		} catch ( err_ ) {
// précède les instructions à effectuer lors d'une erreur
#define ERREnd		}
// précède les instructions à exécuter, erreur ou pas
#define ERRCommonEpilog	}
// boucle la partie de traitement d'erreur

#endif

#ifdef ERR__THREAD_SAFE
#define ERRTestEpilog	ERR.Error && ERRConcerned()
#else
#define ERRTestEpilog	ERR.Error
#endif

//d End of the error bloc.
#define ERREpilog	ERRCommonEpilog if ( ERRTestEpilog ) ERRR();
#define ERRFEpilog	ERRCommonEpilog if ( ERRTestEpilog ) ERRFinal();
#define ERRFProlog	ERRProlog
#define ERRFBegin	ERRBegin
#define ERRFErr		ERRErr
#define ERRFEnd		ERREnd

#ifdef ERR_JMPUSE
inline jmp_buf *ERRFGetJ( struct err_ &ERR_ )
{
	return ERR_.Jump;
}

inline void ERRFSetJ(
	struct err_ &ERR_,
	jmp_buf *Jump )
{
	ERR_.Jump = Jump;
}
#endif

//f Return the error message which goes along the given parameters
const char *ERRMessage(
	const char *File,
	int Line,
	err::type Major,
	int Minor );

#ifndef ERR__COMPILATION
inline const char *ERRMessage( void )
{
	return ERRMessage( ERRFile, ERRLine, ERRMajor, ERRMinor );
}
#endif

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
