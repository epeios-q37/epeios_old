/*
  Header for the 'tym' library by Claude L. Simon (simon@epeios.org)
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org) 

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

#ifndef TYM__INC
#define TYM__INC

#define TYM_NAME		"TYM"

#define	TYM_VERSION	"$Revision$"	

#define TYM_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &TYMTutor;

#if defined( XXX_DBG ) && !defined( TYM_NODBG )
#define TYM_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (simon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "uym.h"
#include "mmm.h"

namespace tym {
	using namespace uym;

	#define TYM_MAX_SIZE	UYM_MAX_SIZE

	//c Typed memory core. Don't use; for internal use only.
	template <typename t, typename b> class memory_core_
	: public b
	{
	private:
		/* lit 'Quantite' objets à partir de 'Position' et les place dans 'Destination'
		à la position 'Decalage' */
		void Lire_(
			tym::row__ Position,
			tym::size__ Quantite,
			memory_core_ &Destination,
			tym::row__ Decalage ) const
		{
			b::Read( ( Position * sizeof( t ) ), ( Quantite * sizeof( t ) ), Destination, ( Decalage * sizeof( t ) ) );
		}
		// place dans 'Tampon' 'Nomnbre' objets à la position 'Position'
		void Lire_(
			tym::row__ Position,
			tym::bsize__ Nombre,
			t *Tampon ) const
		{
			b::Read( Position * sizeof( t ), Nombre * sizeof( t ), (tym::data__ *)Tampon );
		}
		// écrit 'Taille' objets de 'Tampon' à la position 'Position'
		void Ecrire_(
			const t *Tampon,
			tym::bsize__ Nombre,
			tym::row__ Position )
		{
			b::Write( (tym::data__ *)Tampon, Nombre * sizeof( t ), Position * sizeof( t ) );
		}
		/* écrit 'Nombre' objets de 'Source' à partir de 'Position'
		à la position 'Decalage' */
		void Ecrire_(
			const memory_core_ &Source,
			tym::size__ Quantite,
			tym::row__ Position,
			tym::row__ Decalage )
		{
			b::Write( Source, ( Quantite * sizeof( t ) ),( Position * sizeof( t ) ),  ( Decalage * sizeof( t ) ) );
		}
		// allocation de 'Capacite' objets
		void Allocate_( tym::size__ Size )
		{
			b::Allocate( ( Size * sizeof( t ) ) );
		}
	public:
		struct s
		: public b::s
		{};
		memory_core_( s &S )
		: b( S )
		{}
		//f Initialization.
		void Init( void )
		{
			b::Init();
		}
		//f Put 'Amount' objects at 'Position' to 'Buffer'.
		void Read(
			tym::row__ Position,
			tym::bsize__ Amount,
			t *Buffer ) const
		{
			Lire_( Position, Amount, Buffer );
		}
		//f Put in 'Value' the object at 'Position'.
		void Read(
			tym::row__ Position,
			t &Value ) const
		{
			Lire_( Position, 1, &Value );
		}
		//f Put to 'Destination' at 'Offset' 'Quantity' objects at 'Position'.
		void Read(
			tym::row__ Position,
			tym::size__ Quantity,
			memory_core_<t,b> &Destination,
			tym::row__ Offset = 0 ) const
		{
			Lire_( Position, Quantity, Destination, Offset );
		}
		//f Return the object at 'Position'.
		t Read( tym::row__ Position ) const
		{
			t V;

			Lire_( Position, 1, &V );

			return V;
		}
		//f Write 'Amount' object from 'Buffer' at 'Position'.
		void Write(
			const t *Buffer,
			tym::bsize__ Amount,
			tym::row__ Position )
		{
			Ecrire_( Buffer, Amount, Position );
		}
		//f Write 'Value' at 'Position'.
		void Write(
			const t &Valeur,
			tym::row__ Position )
		{
			Ecrire_( &Valeur, 1, Position );
		}
		/*f Write 'Quantity' objects at 'Position' from 'Source' at 'Offset'. */
		void Write(
			const memory_core_<t,b> &Source,
			tym::size__ Quantity,
			tym::row__ Position = 0,
			tym::row__ Offset = 0 )
		{
			Ecrire_( Source, Quantity, Position, Offset );
		}
		//f Fill at 'Position' with 'Object' 'Count' times.
		void Fill(
			const t &Object,
			tym::size__ Count,
			tym::row__ Position = 0 )
		{
			b::Fill( (tym::data__ *)&Object, sizeof( t ), Count, Position * sizeof( t ) );
		}
		//f Return the position from 'Object' between 'Begin' and 'End' (excluded) oR 'NONE' if non-existant.
		tym::row__ Position(
			const t &Object,
			tym::row__ Begin,
			tym::row__ End )
		{
			tym::row__ Position;

			if ( ( Position = b::Position( (tym::data__ *)&Object, sizeof( t ), Begin * sizeof( t ), End * sizeof( t ) ) ) != NONE )
				Position /= sizeof( t );

			return Position;
		}
		//f Allocate 'Size' objects.
		void Allocate( tym::size__ Size )
		{
			Allocate_( Size );
		}
		//f Return the object at 'Position'..
		t operator ()( tym::row__ Position ) const
		{
			return Read( Position );
		}
	};

	/*c Memory of statical object of type 't'. */
	template <typename t> class memory_
	: public memory_core_< t, tym::untyped_memory_ >
	/* NOTA: See 'memory_core about' '::s'. */
	{
	private:
		// l'éventuel pilote de la multimemoire
		mmm::multimemory_driver_ PiloteMultimemoire_;
	public:
		struct s
		: public tym::memory_core_< t, tym::untyped_memory_ >::s
		{
			mmm::multimemory_driver_::s PiloteMultimemoire_;
		};
		memory_( s &S )
		: memory_core_< t, tym::untyped_memory_ >( S ),
		  PiloteMultimemoire_( S.PiloteMultimemoire_ )
		{}
		void reset( bool P = true )
		{
			memory_core_< t, tym::untyped_memory_ >::reset( P );
			PiloteMultimemoire_.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			PiloteMultimemoire_.Init( M );
			memory_core_< t, tym::untyped_memory_ >::plug( PiloteMultimemoire_ );
		}
		void plug( mdr::E_MEMORY_DRIVER_ &Pilote )
		{
			PiloteMultimemoire_.reset();
			memory_core_< t, tym::untyped_memory_ >::plug( Pilote );
		}
		void write(
			tym::row__ Position,
			tym::size__ Quantity,
			flw::oflow___ &OFlow ) const
		{
			memory_core_<t, tym::untyped_memory_ >::write( Position * sizeof( t ), Quantity * sizeof( t ) , OFlow );
		}
		void read(
			flw::iflow___  &IFlow,
			tym::row__ Position,
			tym::size__ Quantite )
		{
			memory_core_<t, tym::untyped_memory_ >::read( IFlow, Position * sizeof( t ), Quantite * sizeof( t ) );
		}
	};

	/*
	template <class t> class memory
	: public memory_<t>
	{};
	*/

	AUTO1( memory )

	//m 'memory' would be often used, then create a special name.
	#define E_MEMORY( t )	memory< t >
	#define E_MEMORY_( t )	memory_< t >

	//f Return 'E1' - 'E2' which begin at 'BeginS1' and 'BeginS2' and have a length of 'Quantity'.
	template <class t> inline bso__sbyte Compare(
		const E_MEMORY_( t ) &S1,
		const E_MEMORY_( t ) &S2,
		tym::row__ BeginS1,
		tym::row__ BeginS2,
		tym::size__ Quantity )
	{
		return uym::Compare( S1, S2, BeginS1 * sizeof( t ), BeginS2 * sizeof( t ), Quantity * sizeof( t ) );
	}

	/*c A static set of object of 'amount' objects of type 't' of size 'size'.
	The size parameter was added due to a bug of Borland C++, which doesn't like a 'sizeof'
	as template parameter. Use 'E_MEMORY__', it's easier. */
	template <class t, int amount, int size> class memory__
	: public memory_core_< t, uym::untyped_memory__< amount * size > >
	{
	private:
		memory_core_<t, uym::untyped_memory__< amount * size > >::s Static_;
	public:
		memory__( memory_core_<t, uym::untyped_memory__<  amount * size > >::s &S = *(memory_core_<t, uym::untyped_memory__<  amount * size > >::s *) NULL )	// To simplify use in 'SET'.
		: memory_core_<t, uym::untyped_memory__< amount * size > >( Static_ )
		{}
	};

	//d A static set of 'amount' object of type 'Type'.
	#define E_MEMORY__( type, amount ) memory__< type, amount, sizeof( type ) > 
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
