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
#include "epeios.h"

namespace tym {
	using namespace epeios;

	#define TYM_MAX_SIZE	UYM_MAX_SIZE

	//c Typed memory core. Don't use; for internal use only.
	template <typename t, typename b, typename r> class _memory_
	: public b
	{
	private:
		/* lit 'Quantite' objets � partir de 'Position' et les place dans 'Destination'
		� la position 'Decalage' */
		void Lire_(
			epeios::row_t__ Position,
			epeios::size__ Quantite,
			_memory_ &Destination,
			epeios::row_t__ Decalage ) const
		{
			b::Read( ( Position * sizeof( t ) ), ( Quantite * sizeof( t ) ), Destination, ( Decalage * sizeof( t ) ) );
		}
		// place dans 'Tampon' 'Nomnbre' objets � la position 'Position'
		void Lire_(
			epeios::row_t__ Position,
			epeios::bsize__ Nombre,
			t *Tampon ) const
		{
			b::Read( Position * sizeof( t ), Nombre * sizeof( t ), (epeios::data__ *)Tampon );
		}
		// �crit 'Taille' objets de 'Tampon' � la position 'Position'
		void Ecrire_(
			const t *Tampon,
			epeios::bsize__ Nombre,
			epeios::row_t__ Position )
		{
			b::Write( (epeios::data__ *)Tampon, Nombre * sizeof( t ), Position * sizeof( t ) );
		}
		/* �crit 'Nombre' objets de 'Source' � partir de 'Position'
		� la position 'Decalage' */
		void Ecrire_(
			const _memory_ &Source,
			epeios::size__ Quantite,
			epeios::row_t__ Position,
			epeios::row_t__ Decalage )
		{
			b::Write( Source, ( Quantite * sizeof( t ) ),( Position * sizeof( t ) ),  ( Decalage * sizeof( t ) ) );
		}
		// allocation de 'Capacite' objets
		void Allocate_( epeios::size__ Size )
		{
			b::Allocate( ( Size * sizeof( t ) ) );
		}
	public:
		struct s
		: public b::s
		{};
		_memory_( s &S )
		: b( S )
		{}
		//f Initialization.
		void Init( void )
		{
			b::Init();
		}
		//f Put 'Amount' objects at 'Position' to 'Buffer'.
		void Read(
			r Position,
			epeios::bsize__ Amount,
			t *Buffer ) const
		{
			Lire_( *Position, Amount, Buffer );
		}
		//f Put in 'Value' the object at 'Position'.
		void Read(
			r Position,
			t &Value ) const
		{
			Lire_( *Position, 1, &Value );
		}
		//f Put to 'Destination' at 'Offset' 'Quantity' objects at 'Position'.
		void Read(
			r Position,
			epeios::size__ Quantity,
			_memory_<t,b,r> &Destination,
			r Offset = 0 ) const
		{
			Lire_( *Position, Quantity, Destination, *Offset );
		}
		//f Return the object at 'Position'.
		t Read( r Position ) const
		{
			t V;

			Lire_( *Position, 1, &V );

			return V;
		}
		//f Write 'Amount' object from 'Buffer' at 'Position'.
		void Write(
			const t *Buffer,
			epeios::bsize__ Amount,
			r Position )
		{
			Ecrire_( Buffer, Amount, *Position );
		}
		//f Write 'Value' at 'Position'.
		void Write(
			const t &Valeur,
			r Position )
		{
			Ecrire_( &Valeur, 1, *Position );
		}
		/*f Write 'Quantity' objects at 'Position' from 'Source' at 'Offset'. */
		void Write(
			const _memory_<t,b,r> &Source,
			epeios::size__ Quantity,
			r Position = 0,
			r Offset = 0 )
		{
			Ecrire_( Source, Quantity, *Position, *Offset );
		}
		//f Fill at 'Position' with 'Object' 'Count' times.
		void Fill(
			const t &Object,
			epeios::size__ Count,
			r Position = 0 )
		{
			b::Fill( (uym::data__ *)&Object, sizeof( t ), Count, *Position * sizeof( t ) );
		}
		//f Return the position from 'Object' between 'Begin' and 'End' (excluded) oR 'NONE' if non-existant.
		r Position(
			const t &Object,
			r Begin,
			r End )
		{
			row_t__ Position;

			if ( ( Position = b::Position( (data__ *)&Object, sizeof( t ), *Begin * sizeof( t ), *End * sizeof( t ) ) ) != NONE )
				Position /= sizeof( t );

			return Position;
		}
		//f Allocate 'Size' objects.
		void Allocate( epeios::size__ Size )
		{
			Allocate_( Size );
		}
		//f Return the object at 'Position'..
		t operator ()( r Position ) const
		{
			return Read( Position );
		}
	};

	/*c Memory of statical object of type 't'. */
	template <typename t, typename r> class memory_
		: public _memory_< t, uym::untyped_memory_, r >
	/* NOTA: See 'memory_core about' '::s'. */
	{
	private:
		// l'�ventuel pilote de la multimemoire
		mmm::multimemory_driver_ PiloteMultimemoire_;
	public:
		struct s
		: public _memory_< t, uym::untyped_memory_, r >::s
		{
			mmm::multimemory_driver_::s PiloteMultimemoire_;
		};
		memory_( s &S )
		: _memory_< t, uym::untyped_memory_, r >( S ),
		  PiloteMultimemoire_( S.PiloteMultimemoire_ )
		{}
		void reset( bool P = true )
		{
			_memory_< t, uym::untyped_memory_, r >::reset( P );
			PiloteMultimemoire_.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			PiloteMultimemoire_.Init( M );
			_memory_< t, uym::untyped_memory_, r >::plug( PiloteMultimemoire_ );
		}
		void plug( mdr::E_MEMORY_DRIVER_ &Pilote )
		{
			PiloteMultimemoire_.reset();
			_memory_< t, uym::untyped_memory_, r >::plug( Pilote );
		}
		void write(
			r Position,
			epeios::size__ Quantity,
			flw::oflow___ &OFlow ) const
		{
			_memory_<t, uym::untyped_memory_, r >::write( *Position * sizeof( t ), Quantity * sizeof( t ) , OFlow );
		}
		void read(
			flw::iflow___  &IFlow,
			r Position,
			epeios::size__ Quantite )
		{
			_memory_<t, uym::untyped_memory_, r >::read( IFlow, *Position * sizeof( t ), Quantite * sizeof( t ) );
		}
	};

	/*
	template <class t> class memory
	: public memory_<t>
	{};
	*/

	AUTO2( memory )

	//m 'memory' would be often used, then create a special name.
	#define E_MEMORYt( t, r )	memory< t, r >
	#define E_MEMORYt_( t, r )	memory_< t, r >

	#define E_MEMORY_( t )	E_MEMORYt_( t, epeios::row__ )
	#define E_MEMORY( t )	E_MEMORYt( t, epeios::row__ )

	//f Return 'E1' - 'E2' which begin at 'BeginS1' and 'BeginS2' and have a length of 'Quantity'.
	template <class t, typename r> inline bso::sbyte__ Compare(
		const E_MEMORYt_( t, r ) &S1,
		const E_MEMORYt_( t, r ) &S2,
		r BeginS1,
		r BeginS2,
		epeios::size__ Quantity )
	{
		return uym::Compare( S1, S2, *BeginS1 * sizeof( t ), *BeginS2 * sizeof( t ), Quantity * sizeof( t ) );
	}

	/*c A static set of object of 'amount' objects of type 't' of size 'size'.
	The size parameter was added due to a bug of Borland C++, which doesn't like a 'sizeof'
	as template parameter. Use 'E_MEMORY(t)__', it's easier. */
	template <class t, int amount, int size, typename r> class memory__
	: public _memory_< t, uym::untyped_memory__< amount * size >, r >
	{
	private:
		_memory_<t, uym::untyped_memory__< amount * size >, r >::s Static_;
	public:
		memory__( _memory_<t, uym::untyped_memory__<  amount * size >, r >::s &S = *(_memory_<t, uym::untyped_memory__<  amount * size >, r >::s *) NULL )	// To simplify use in 'SET'.
		: _memory_<t, uym::untyped_memory__< amount * size >, r >( Static_ )
		{}
	};

	//d A static set of 'amount' object of type 'Type'.
	#define E_MEMORYt__( type, amount, r ) memory__< type, amount, sizeof( type ), r > 
	#define E_MEMORY__( type, amount ) memory__< type, amount, sizeof( type ), epeios::row__ > 

	/*c A bunch of object of 'amount' objects of type 't' of size 'size' stored in conventional memory.
	The size parameter was added due to a bug of Borland C++, which doesn't like a 'sizeof'
	as template parameter. Use 'E_MEMORY(t)__', it's easier. */
	template <class t, int size, typename r> class memory___
	: public _memory_< t, uym::untyped_memory___, r >
	{
	private:
		_memory_<t, uym::untyped_memory___, r >::s Static_;
	public:
		memory___( _memory_<t, uym::untyped_memory___, r >::s &S = *(_memory_<t, uym::untyped_memory___, r >::s *) NULL )	// To simplify use in 'SET'.
		: _memory_<t, uym::untyped_memory___, r >( Static_ )
		{}
	};

	//d A static set of 'amount' object of type 'Type'.
	#define E_MEMORYt___( type, r ) memory___< type, sizeof( type ), r > 
	#define E_MEMORY___( type ) memory___< type, sizeof( type ), epeios::row__ > 
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
