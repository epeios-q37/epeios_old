/*
	Header for the 'txf' library by Claude SIMON (csimon@epeios.org)
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

#ifndef TXF__INC
#define TXF__INC

#define TXF_NAME		"TXF"

#define	TXF_VERSION	"$Revision$"

#define TXF_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &TXFTutor;

#if defined( XXX_DBG ) && !defined( TXF_NODBG )
#define TXF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon@epeios.org)
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

namespace txf {
	using flw::amount__;
	using flw::datum__;

	//c Input text flow.
	class text_iflow___
	{
	private:
		// Flow from which data are red.
		flw::iflow___ *Flow_;
		datum__ Lire_( void )
		{
			return Flow_->Get();
		}
		amount__ Lire_(
			amount__ Nombre,
			datum__ *Tampon )
		{
			return Flow_->GetUpTo( Nombre, Tampon );
		}
	public:
		void reset( bool = true )
		{
			Flow_ = NULL;
		}
		//f Initialization with the input flow 'Flow'.
		void Init( flw::iflow___ &Flow )
		{
			reset();

			Flow_ = &Flow;
		}
		text_iflow___ &operator >>( char &C )
		{
			C = (char)Lire_();

			return *this;
		}
		text_iflow___ &operator >>( unsigned long &E )
		{
			datum__ C[9];
			amount__ Pos;

			if ( ( Pos = Lire_( 9, C ) ) > 8 )
				ERRf();

			if ( !isxdigit( C[0] ) )
				ERRf();

			C[Pos] = 0;

			sscanf( (const char *)C, "%lx", &E );

			return *this;
		}
		text_iflow___ &operator >>( unsigned short &E )
		{
			unsigned long L;

			operator >>( L );

			if ( L > USHRT_MAX )
				ERRf();

			E = (unsigned short)L;

			return *this;
		}
		text_iflow___ &operator >>( signed long &E )
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
		text_iflow___ &operator >>( signed short &E )
		{
			signed long L;

			operator >>( L );

			if ( ( L > SHRT_MAX ) || ( L < SHRT_MIN ) )
				ERRf();

			E = (signed short)L;

			return *this;
		}
		//f Read 'Amount' characters and put them in 'Buffer'.
		amount__ Get(
			amount__ Amount,
			datum__ *Buffer )
		{
			return Lire_( Amount, Buffer );
		}
		//f Return the next character.
		datum__ Get( void )
		{
			return Lire_();
		}
	/*	//f Return the used flow.
		flw__iflow_ &IFlow( void )
		{
			return *Flow_;
		}
	*/	text_iflow___( void )
		{
			reset( false );
		}
		~text_iflow___( void )
		{
			reset( true );
		}
	};


	//c Output text flow.
	class text_oflow___
	{
	private:
		// Flow to write into.
		flw::oflow___ *Flow_;
		void Ecrire_( datum__ C )
		{
			Flow_->Put( C );
		}
		void Ecrire_(
			const datum__ *Tampon,
			amount__ Nombre )
		{
			Flow_->Put( Tampon, Nombre );
		}
		void Synchroniser_( void )
		{
			Flow_->Synchronize();
		}
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( Flow_ != NULL )
					Synchroniser_();
			}

			Flow_ = NULL;
		}
		//f Initializing with 'Flow' as output flow.
		void Init( flw::oflow___ &Flow )
		{
			reset();

			Flow_ = &Flow;
		}
		text_oflow___ &operator <<( text_oflow___ &(* Function )( text_oflow___ &Flot ) )
		{
			return Function( *this );
		}
		text_oflow___ &operator <<( char C )
		{
			Ecrire_( (datum__)C );

			return *this;
		}
		text_oflow___ &operator <<( const char *C )
		{
			size_t Length = strlen( C );

			if ( Length > FLW_AMOUNT_MAX )
				ERRl();

			Ecrire_( (const datum__ *)C, (amount__)Length );

			return *this;
		}
		text_oflow___ &operator <<( unsigned long E )
		{
			char C[11];

			sprintf( C, "%lu", E );

			return operator <<( C );
		}
		text_oflow___ &operator <<( long double E )
		{
			char C[40];

			sprintf( C, "%LG", E );

			return operator <<( C );
		}
		text_oflow___ &operator <<( unsigned short E )
		{
			return operator <<( (unsigned long) E );
		}
		text_oflow___ &operator <<( signed long E )
		{
			char C[12];

			sprintf( C, "%li", E );

			return operator <<( C );
		}
		text_oflow___ &operator <<( signed short E )
		{
			return operator <<( (signed long) E );
		}
		//f Write 'C';
		void Put( datum__ C )
		{
			Ecrire_( C );
		}
		//f Wrtie 'Amount' data from 'Buffer'.
		void Put(
			const datum__ *Buffer,
			amount__ Amount )
		{
			Ecrire_( Buffer, Amount );
		}
		//f Synchronization.
		void Synchronize( void )
		{
			Synchroniser_();
		}
/*		//f Return the used flow.
		flw__oflow_ &OFlow( void )
		{
			return *Flow_;
		}
*/		text_oflow___( void )
		{
			reset( false );
		}
		~text_oflow___( void )
		{
			reset( true );
		}
	};

	/* Macro pour la définition d'une fonction de sortie de nom 'Nom'.
	( Déclaration d'une Fonction pour le txft de Sortie). */
	#define TXF_DFS( Nom ) inline text_oflow___ & Nom ( text_oflow___ &Flow )

	// Synchronisation (vide les caches).
	TXF_DFS( sync )
	{
		Flow.Synchronize();
		return Flow;
	}

	// Place une fin de ligne sur le txft.
	TXF_DFS( nl )
	{
	#ifdef CPE__MS
		return Flow << "\r\n" << sync;
	#elif defined( CPE__UNIX ) || defined( CPE__BEOS ) || defined( CPE__MAC ) 
		return Flow << '\n' << sync;
	#else
		#error "Unknow compiler enviroment."
	#endif
	}

	// Place une tabulation sur le txft.
	TXF_DFS( tab )
	{
		return Flow << '\t';
	}

	// Revient en début de ligne.
	TXF_DFS( rfl )
	{
		return Flow << '\r' << sync;
	}
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
