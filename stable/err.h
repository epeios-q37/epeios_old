/*
	Header for the 'err' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

#ifndef ERR__INC
#define ERR__INC

#define ERR_NAME		"ERR"

#define	ERR_VERSION	"$Revision$"

#define ERR_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &ERRTutor;

#if defined( XXX_DBG ) && !defined( ERR_NODBG )
#define ERR_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (http://zeusw.org/intl/contact.html)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D ERRor handling 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include <stdio.h>
#include <stdlib.h>

#ifdef ERR_JMPUSE
#include <setjmp.h>
#endif

#include "cpe.h"

#ifdef CPE__T_MT
#define ERR__THREAD_SAFE
#endif


namespace err {

	typedef char buffer__[150];

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
		bkd = 16,
		// backend error
		itn = 32,
		// intentional error; launch the error behavior (user press the ctrl-break key, for example).
		ccp = 64,
		// conception error: anything unexpected occurs.
		frm = 128,
		// format error.
		phb = 256,
		// prohibibition error; a prohibited fonction was called.
		lmt = 512,
		// limitation error; a software limitation xas reached.
		mem = 1024,
		// memory error (all memory type).
		ext = 2048,
		// external error; third part error.
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
		sGeneric,
		sPThreadCreateEAGAIN,
		sPThreadCreateEOther
	};
		// system error
	enum u
	{
		uGeneric,
		uTest,	// for testing purpose.
		uObsolete,	// Calling of obsolete code.
	};
		// user error
	enum b
	{
		bGeneric,
		bTest,	// for testing purpose.
	};
		// backend error
	enum i
	{
		iGeneric,
		iError,		// To signal an error (generally a user error, an error in command line, for example).
		iBeam,		// To rapidly go back to a certain point.
		iReturn		// To quickly exit, without an error.
	};
		// throw error
	enum c
	{
		cGeneric,
		cIncoherency,	// Pour signaler une incoh�rence.
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
		lGeneric,
		lNotImplemented,	// Non impl�ment�.
	};
		// limit overflow error
	enum m
	{
		mGeneric
	};
		// memory error
	enum x
	{
		xGeneric
	};
		// external error

	struct err_ {
		// The exit value of the software.
		static int ExitValue;
		// line where the error occurs
		static int Line;
		// file where the error occurs
		static const char *File;
	#ifdef ERR_JMPUSE
		// where to jump
		static jmp_buf *Jump;
	#endif
		
		int
			Error: 1;	// Erreur en cours de traitement (Entre un 'ERRErr' et un 'ERREnd'.
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

	void Final( void );


//#ifndef ERR__COMPILATION
	extern err_ ERR;
//#endif

#ifdef ERR__THREAD_SAFE
	// If an error occurs, test if the current thread is concerned.
	bool Concerned( void );
	void Unlock( void );
#else
	inline bool Concerned( void )
	{
		return true;
	}
	inline void Unlock( void )
	{
		err::ERR.Error = false;
	}
#endif


#define ERRCommon( M, m )	err::ERR.Handler( __FILE__, __LINE__, M, err::m )

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

#define ERRB( m )		ERRCommon( err::bkd, m )
//m Throw a backend error.
#define ERRb()			ERRB( bGeneric )

#define ERRI( m )		ERRCommon( err::itn, m )
//m Throw an intentional, error without an error.
#define ERRi()			ERRI( iGeneric )

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

#define ERRX( m )		ERRCommon( err::ext, m )
//m Throw a memory error.
#define ERRx()			ERRX( xGeneric )

#ifdef ERR_JMPUSE
//m Throw the handler.
#define ERRR()		{longjmp( *err::ERR.Jump, 1 );}
#else
#define ERRR()		throw( err::ERR )
#endif

#define ERRRst()	{ err::Unlock(); }

//d Major code.
#define ERRMajor		err::ERR.Major

//d Minor code.
#define ERRMinor		err::ERR.Minor

//d File in which the error was thrown.
#define ERRFile			err::ERR.File

//d Line where the error was thrown.
#define ERRLine			err::ERR.Line

#ifdef ERR_JMPUSE
#define ERRGetJ()		err::FGetJ( err::ERR )
#define ERRPutJ( J )	err::FSetJ( err::ERR, J )
#endif

#ifdef ERR_JMPUSE

//d Put the declaration after this.
#define ERRProlog	bso::bool__ ERRNoError = true; { jmp_buf ERRJmp, *ERROJmp = ERRGetJ();\
	ERRPutJ( &ERRJmp );

//d Put the instructions to survey after this.
#define ERRBegin	if ( !setjmp( ERRJmp ) ) {

//d Put the instruction to launch if an error occurs.
#define ERRErr		} else { ERRPutJ( ERROJmp ); ERRNoError = false;

//d Put the instruction to launch, error or not.
#define ERREnd		}

#define ERRCommonEpilog	ERRPutJ( ERROJmp ); }

#else

#define ERRProlog	bso::bool__ ERRNoError = true; {
// pr�c�de les d�clarations
#define ERRBegin	try {
// pr�c�de les instructions proprement dites
#define ERRErr		} catch ( err::err_ ) { ERRNoError = false;
// pr�c�de les instructions � effectuer lors d'une erreur
#define ERREnd		}
// pr�c�de les instructions � ex�cuter, erreur ou pas
#define ERRCommonEpilog	}
// boucle la partie de traitement d'erreur

#endif

#define ERRTestEpilog	if ( err::ERR.Error && !ERRNoError && err::Concerned() )\
							if ( ( ERRMajor == err::itn ) && ( ERRMinor == err::iReturn ) )\
								ERRRst()

//d End of the error bloc.
#define ERREpilog	ERRCommonEpilog ERRTestEpilog else ERRR();
#define ERRFEpilog	ERRCommonEpilog ERRTestEpilog else err::Final();
#define ERRFProlog	ERRProlog
#define ERRFBegin	ERRBegin
#define ERRFErr		ERRErr
#define ERRFEnd		ERREnd

#ifdef ERR_JMPUSE
	inline jmp_buf *FGetJ( struct err_ &ERR_ )
	{
		return ERR_.Jump;
	}

	inline void FSetJ(
		struct err_ &ERR_,
		jmp_buf *Jump )
	{
		ERR_.Jump = Jump;
	}
#endif

	//f Return the error message which goes along the given parameters
	const char *Message(
		const char *File,
		int Line,
		err::type Major,
		int Minor,
		buffer__ &Buffer );

#ifndef ERR__COMPILATION
	inline const char *Message( buffer__ &Buffer )
	{
		return err::Message( ERRFile, ERRLine, ERRMajor, ERRMinor, Buffer );
	}
#endif


// Exits the software returning 'v'.
#define ERRExit( v )	{ err::ERR.ExitValue = v; ERRI( iError ); }

// Jump to 'ERRErr' and reset the reset the err::itn/iNoError' error.
#define	ERRReturn		ERRI( iReturn )

#define ERRExitValue	err::ERR.ExitValue
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
