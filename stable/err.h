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
# define ERR__INC

# define ERR_NAME		"ERR"

# define	ERR_VERSION	"$Revision$"

# define ERR_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

# include "ttr.h"

extern class ttr_tutor &ERRTutor;

# if defined( XXX_DBG ) && !defined( ERR_NODBG )
#  define ERR_DBG
# endif

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

# include <stdio.h>
#include <stdlib.h>

# ifdef ERR_JMPUSE
#  define ERR__JMPUSE
# endif

# ifdef ERR__JMPUSE
#  include <setjmp.h>
# endif



# include "cpe.h"

# ifdef CPE__MT
#  define ERR__THREAD_SAFE
# endif


namespace err {
	typedef char buffer__[150];

	enum handling__ {
		hThrowException,	// Une erreur provoque une exception.
		hUserDefined,		// Le traitement de l'erreur est à la charge de l'utilisateur.
		h_amount,
		h_Undefined,
		h_Default = hThrowException	// Comportement par défaut.
	};

	enum type {
		tAllocation,	// (ERRAlc) Erreur lors de l'allocation de RAM.
		tDevice,		// (ERRDvc) Erreur signalant un problème avec un périphérique.
		tSystem,		// (ERRSys) Erreur du système.
		tUser,			// (ERRUsr) Erreur de l'utilisateur.
		tBackend,		// (ERRBkd) Erreur 'backend'.
		tVacant,		// (ERRVct) Appel à une fonctionnalité absente (à priori non encore implémentée).	
		tLimitation,	// (ERRLmt) Erreur signalant un dépassement d'une limite inhérente à un fonctionnalité.
		tStorage,		// (ERRStg) Erreur lors du traitement d'une fonction de stockage (tout type).
		tConception,	// (ERRCcp) Erreur de conception ; une situation algorithmiquement impossible est survenue.
		tForbidden,		// (ERRFbd) Erreur du à l'appel d'une fonctionnalité non autorisée.
		tExternal,		// (ERRExt) Un erreur du à un acteur extérieur s'est produite.
		tFlow,			// (ERRFlw) Une errreur a été détectée lors du traitement d'un flux (pendant du 'tStorage').
		t_amount,
		t_None,			// Signale l'absence d'erreur.
		t_Free,			// (ERRFree) Pas une erreur au sens propre. Permet de profiter du mécanisme de gestion d'erreur.
		t_Exit,			// Facilite la gestion d'un 'ERRExit(...)'
		t_Return,		// Facilite la gestion d'un 'ERRReturn'.
		t_Abort,		// Facilite la gestion d'un 'ERRAbort()'.
		t_Undefined
	};


	struct err_ {
		// The exit value of the software.
		static int ExitValue;
		// line where the error occurs
		static int Line;
		// file where the error occurs
		static const char *File;
# ifdef ERR__JMPUSE
		// where to jump
		static jmp_buf *Jump;
# endif

# if 0	// Utilisation de 'Majot' à la place.
		int
			Error: 1;	// Erreur en cours de traitement (Entre un 'ERRErr' et un 'ERREnd'.
# endif
		// Type of error.
		static err::type Type;
		void Handler(
			const char *File = NULL,
			int Line = 0,
			err::type Type = Type );
	};

	void Final( void );


//#ifndef ERR__COMPILATION
	extern err_ ERR;
//#endif

# ifdef ERR__THREAD_SAFE
	// If an error occurs, test if the current thread is concerned.
	bool Concerned( void );
	void Unlock( void );
# else
	inline bool Concerned( void )
	{
		return true;
	}
	inline void Unlock( void )
	{
		err::ERR.Type = err::t_None;
	}
# endif

# define ERRCommon( T )	err::ERR.Handler( __FILE__, __LINE__, T )

//m Throw an allocation error (only RAM).
# define ERRAlc()	ERRCommon( err::tAllocation )

//m Throw a device error.
# define ERRDvc()	ERRCommon( err::tDevice )

//m Throw a system error.
# define ERRSys()	ERRCommon( err::tSystem )

//m Throw an user error.
# define ERRUsr()	ERRCommon( err::tUser )

//m Throw a backend error.
# define ERRBkd()	ERRCommon( err::tBackend )

//m Throw en conception error.
# define ERRCcp()	ERRCommon( err::tConception )

//m Throw a format error.
# define ERRFlw()	ERRCommon( err::tFlow )	//	A renommer ultèrieurement en 'ERRF', en attendant d'avoir détecté les 'ERRf' qui était des erreurs de formats.

//m Throw a prohibited function call error.
# define ERRFbd()	ERRCommon( err::tForbidden )

//m Throw a limit exceed ferror.
# define ERRLmt()	ERRCommon( err::tLimitation )

//m Throw a memory error.
# define ERRStg()	ERRCommon( err::tStorage )

//m Throw a memory error.
# define ERRExt()	ERRCommon( err::tExternal )

# define ERRVct()	ERRCommon( err::tVacant )

# define ERRRst()	{ err::Unlock(); }


# ifdef ERR__JMPUSE
//m Throw the handler.
#  define ERRT()		{longjmp( *err::ERR.Jump, 1 );}
# else
#  define ERRT()		throw( err::ERR )
# endif

//d Error type.
# define ERRType	err::ERR.Type

//d File in which the error was thrown.
# define ERRFile			err::ERR.File

//d Line where the error was thrown.
# define ERRLine			err::ERR.Line

# ifdef ERR__JMPUSE
#  define ERRGetJ()		err::FGetJ( err::ERR )
#  define ERRPutJ( J )	err::FSetJ( err::ERR, J )
# endif

# ifdef ERR__JMPUSE

//d Put the declaration after this.
#  define ERRProlog	bso::bool__ ERRNoError = true; { jmp_buf ERRJmp, *ERROJmp = ERRGetJ();\
	ERRPutJ( &ERRJmp );

//d Put the instructions to survey after this.
#  define ERRBegin	if ( !setjmp( ERRJmp ) ) {

//d Put the instruction to launch if an error occurs.
#  define ERRErr		} else { ERRPutJ( ERROJmp ); ERRNoError = false;

//d Put the instruction to launch, error or not.
#  define ERREnd		}

#  define ERRCommonEpilog	ERRPutJ( ERROJmp ); }

# else

#  define ERRProlog	bso::bool__ ERRNoError = true; {
// précède les déclarations
#  define ERRBegin	try {
// précède les instructions proprement dites
#  define ERRErr		} catch ( err::err_ ) { ERRNoError = false;
// précède les instructions à effectuer lors d'une erreur
#  define ERREnd		}
// précède les instructions à exécuter, erreur ou pas
#  define ERRCommonEpilog	}
// boucle la partie de traitement d'erreur

# endif

# define ERRTestEpilog	if ( ERRHit() && !ERRNoError && err::Concerned() )\
							if ( ERRType == err::t_Return )\
								ERRRst()

//d End of the error bloc.
# define ERREpilog	ERRCommonEpilog ERRTestEpilog else ERRT();
# define ERRFEpilog	ERRCommonEpilog ERRTestEpilog else err::Final();
# define ERRFProlog	ERRProlog
# define ERRFBegin	ERRBegin
# define ERRFErr		ERRErr
# define ERRFEnd		ERREnd

# ifdef ERR__JMPUSE
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
# endif

	//f Return the error message which goes along the given parameters
	const char *Message(
		const char *File,
		int Line,
		err::type Type,
		buffer__ &Buffer );

# ifndef ERR__COMPILATION
	inline const char *Message( buffer__ &Buffer )
	{
		return err::Message( ERRFile, ERRLine, ERRType, Buffer );
	}
# endif

# define ERRFailure()	( ERRType < err::t_amount )

# define ERRHit()	( ERRType != err::t_None )


// quitte le logiciel an retournant la valeur 'v'.
# define ERRExit( v )	{ err::ERR.ExitValue = v; ERRCommon( err::t_Exit ); }

// Similaire à un simple 'return', mais dans une section surveillé ('ERRBegin'...'ERRErr'; un simple 'return' poserait problème dans une telle section).
# define ERRReturn		ERRCommon( err::t_Return )

// Interruption de l'action en cours. Utilisé avec un gestionnaire d'interface évennementielle, pour revenir rapîdement à la boucle d'attente.
# define ERRAbort()		ERRCommon( err::t_Abort )

// Pour profiter du mécanisme de gestion d'erreur, sans qu'il n'y ai réellement une erreur dans le sens de cette bibliothèque.
# define ERRFree()		ERRCommon( err::t_Free )

# define ERRExitValue	err::ERR.ExitValue
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
