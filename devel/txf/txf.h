/*
	Header for the 'txf' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
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

#ifndef TXF__INC
#define TXF__INC

#define TXF_NAME		"TXF"

#define	TXF_VERSION	"$Revision$"

#define TXF_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &TXFTutor;

#if defined( XXX_DBG ) && !defined( TXF_NODBG )
#define TXF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon at zeusw dot org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D TeXt Flow 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include <ctype.h>
#include <limits.h>

#include "err.h"
#include "flw.h"
#include "tol.h"

#ifndef TXF_64_BITS_FORBIDDEN
#	if defined( CPE__64_BITS_TYPES_ALLOWED ) || defined(TXF_64_BITS_TYPES_ALLOWED )
#		define TXF__64_BITS_TYPES_ALLOWED
#	endif
#endif

namespace txf {
	using flw::size__;
	using flw::datum__;

	//c Input text flow.
	class text_iflow__
	{
	private:
		// Flow from which data are red.
		flw::iflow__ *_Flow;
		flw::iflow__ &_F( void ) const
		{
#ifdef TXF_DBG
			if ( _Flow == NULL )
				ERRu();
#endif
			return *_Flow;
		}
		bso::bool__ EndOfFlow( void )
		{
			return _F().EndOfFlow();
		}
		datum__ Lire_( void )
		{
			return _F().Get();
		}
		size__ Lire_(
			size__ Nombre,
			datum__ *Tampon )
		{
			return _F().ReadUpTo( Nombre, Tampon );
		}
	public:
		void reset( bso::bool__ = true )
		{
			_Flow = NULL;
		}
		E_CVDTOR( text_iflow__ );
		text_iflow__( flw::iflow__ &Flow )
		{
			reset( false );

			Init( Flow );
		}
		void Init( flw::iflow__ &Flow )
		{
			_Flow = &Flow;
		}
		text_iflow__ &operator >>( char &C )
		{
			C = (char)Lire_();

			return *this;
		}
#ifdef TXF__64_BITS_TYPES_ALLOWED
		text_iflow__ &operator >>( unsigned long long &E )
		{
			datum__ C[9];
			size__ Pos;

			if ( ( Pos = Lire_( 9, C ) ) > 8 )
				ERRf();

			if ( !isxdigit( C[0] ) )
				ERRf();

			C[Pos] = 0;

			sscanf( (const char *)C, "%llx", &E );

			return *this;
		}
#endif
		text_iflow__ &operator >>( unsigned long &E )
		{
			datum__ C[9];
			size__ Pos;

			if ( ( Pos = Lire_( 9, C ) ) > 8 )
				ERRf();

			if ( !isxdigit( C[0] ) )
				ERRf();

			C[Pos] = 0;

			sscanf( (const char *)C, "%lx", &E );

			return *this;
		}
		text_iflow__ &operator >>( unsigned short &E )
		{
			unsigned long L;

			operator >>( L );

			if ( L > USHRT_MAX )
				ERRf();

			E = (unsigned short)L;

			return *this;
		}
#ifdef TXF__64_BITS_TYPES_ALLOWED
		text_iflow__ &operator >>( signed long long &E )
		{
			unsigned long long L;
			datum__ C = Get();

			if ( ( C != '+' ) && ( C != '-' ) )
				ERRf();

			operator >>( L );

			if ( C == '+' )
				if ( L > LLONG_MAX )
					ERRf();
				else E = (signed long long)L;
			else
				if ( L > ((unsigned long long)-(LLONG_MIN + 1 ) + 1UL ) )
					ERRf();
				else
					E = -(signed long long)L;

			return *this;
		}
#endif
		text_iflow__ &operator >>( signed long &E )
		{
			unsigned long L;
			datum__ C = Get();

			if ( ( C != '+' ) && ( C != '-' ) )
				ERRf();

			operator >>( L );

			if ( C == '+' )
				if ( L > LONG_MAX )
					ERRf();
				else E = (signed long)L;
			else
				if ( L > ((unsigned long)-(LONG_MIN + 1 ) + 1UL ) )
					ERRf();
				else
					E = -(signed long)L;

			return *this;
		}
		text_iflow__ &operator >>( signed short &E )
		{
			signed long L;

			operator >>( L );

			if ( ( L > SHRT_MAX ) || ( L < SHRT_MIN ) )
				ERRf();

			E = (signed short)L;

			return *this;
		}
		//f Read 'Amount' characters and put them in 'Buffer'.
		size__ Get(
			size__ Amount,
			datum__ *Buffer )
		{
			return Lire_( Amount, Buffer );
		}
		//f Return the next character.
		datum__ Get( void )
		{
			return Lire_();
		}
		flw::iflow__ &Flow( void ) const
		{
			return _F();
		}
	};


	//c Output text flow.
	class text_oflow__
	{
	private:
		// Flow to write into.
		flw::oflow__ *_Flow;
		flw::oflow__ &_F( void ) const
		{
#ifdef TXF_DBG
			if ( _Flow == NULL )
				ERRu();
#endif
			return *_Flow;
		}
		void Ecrire_( datum__ C )
		{
			_F().Put( C );
		}
		void Ecrire_(
			const datum__ *Tampon,
			size__ Nombre )
		{
			_F().Write( Tampon, Nombre );
		}
		void Commit_( void )
		{
			_F().Commit();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( _Flow != NULL )
					Commit_();
			}

			_Flow = NULL;
		}
		E_CVDTOR( text_oflow__ )
		text_oflow__( flw::oflow__ &Flow )
		{
			reset( false );

			Init( Flow );
		}
		void Init( flw::oflow__ &Flow )
		{
			_Flow = &Flow;
		}
		bso::bool__ IsInitialized( void ) const
		{
			return _Flow != NULL;
		}
		text_oflow__ &operator <<( text_oflow__ &(* Function )( text_oflow__ &Flot ) )
		{
			return Function( *this );
		}
		text_oflow__ &operator <<( char C )
		{
			Ecrire_( (datum__)C );

			return *this;
		}
		text_oflow__ &operator <<( const char *C )
		{
			size_t Length = strlen( C );

			if ( Length > FLW_SIZE_MAX )
				ERRl();

			Ecrire_( (const datum__ *)C, (size__)Length );

			return *this;
		}
#ifdef TXF__64_BITS_TYPES_ALLOWED
		text_oflow__ &operator <<( unsigned long long E )
		{
			char C[21];

			sprintf( C, "%llu", E );

			return operator <<( C );
		}
#endif
		text_oflow__ &operator <<( unsigned long E )
		{
			char C[11];

			sprintf( C, "%lu", E );

			return operator <<( C );
		}
		text_oflow__ &operator <<( long double E )
		{
			char C[40];

			sprintf( C, "%LG", E );

			return operator <<( C );
		}
		text_oflow__ &operator <<( unsigned short E )
		{
			return operator <<( (unsigned long) E );
		}
#ifdef TXF__64_BITS_TYPES_ALLOWED
		text_oflow__ &operator <<( signed long long E )
		{
			char C[22];

			sprintf( C, "%lli", E );

			return operator <<( C );
		}
#endif
		text_oflow__ &operator <<( signed long E )
		{
			char C[12];

			sprintf( C, "%li", E );

			return operator <<( C );
		}
		text_oflow__ &operator <<( signed short E )
		{
			return operator <<( (signed long) E );
		}
		text_oflow__ &operator <<( const void *P )
		{
			char Buffer[sizeof( P ) * 2 + 2 + 1];	// Deux lettres pour chaque octet + le '0x' devant + le 'nul' final.

			sprintf( Buffer, "%p", P );

			return operator <<( Buffer );
		}
		//f Write 'C';
		void Put( datum__ C )
		{
			Ecrire_( C );
		}
		//f Wrtie 'Amount' data from 'Buffer'.
		void Put(
			const datum__ *Buffer,
			size__ Amount )
		{
			Ecrire_( Buffer, Amount );
		}
		//f Synchronization.
		void Commit( void )
		{
			if ( IsInitialized() ) 
				Commit_();
		}
		flw::oflow__ &Flow( void ) const
		{
			return _F();
		}
	};

	/* Macro pour la d�finition d'une fonction de sortie de nom 'Nom'.
	( D�claration d'une Fonction pour le txft de Sortie). */
	#define TXF_DFS( Nom ) inline text_oflow__ & Nom ( text_oflow__ &Flow )

	// Synchronisation (vide les caches).
	TXF_DFS( commit )
	{
		Flow.Commit();
		return Flow;
	}

	// Place une fin de ligne sur le txft.
	TXF_DFS( nl )
	{
#if defined( CPE__MS ) || defined( CPE__CYGWIN ) || defined( CPE__MINGW )
		return Flow << "\r\n";
#elif defined( CPE__LINUX ) || defined( CPE__MAC )
		return Flow << '\n';
#else
#	error "Unknow target !"
#endif
	}

	// Place une tabulation sur le txft.
	TXF_DFS( tab )
	{
		return Flow << '\t';
	}

	// 4 spaces (half tab).
	TXF_DFS( pad )
	{
		return Flow << "    ";
	}

	// Revient en d�but de ligne.
	TXF_DFS( rfl )
	{
		return Flow << '\r';
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
