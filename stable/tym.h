/*
	Header for the 'tym' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2004 Claude SIMON (csimon@epeios.org).

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

#ifndef TYM__INC
#define TYM__INC

#define TYM_NAME		"TYM"

#define	TYM_VERSION	"$Revision$"

#define TYM_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &TYMTutor;

#if defined( XXX_DBG ) && !defined( TYM_NODBG )
#define TYM_DBG
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
//D TYped Memory 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "uym.h"
#include "mmm0.h"
#include "epeios.h"
#include "flm.h"
/*
namespace mmm {
	class multimemory_driver__;
	class descriptor__;
}
*/
namespace tym {
	#define TYM_MAX_SIZE	UYM_MAX_SIZE

	//c Typed memory core. Don't use; for internal use only.
	template <typename t, typename b, typename r> class _memory_
	: public b
	{
	private:
		// place dans 'Tampon' 'Nomnbre' objets à la position 'Position'
		void _Recall(
			epeios::row_t__ Position,
			epeios::size__ Nombre,
			t *Tampon ) const
		{
			b::Recall( Position * sizeof( t ), Nombre * sizeof( t ), (epeios::datum__ *)Tampon );
		}
		// écrit 'Taille' objets de 'Tampon' à la position 'Position'
		void _Store(
			const t *Tampon,
			epeios::size__ Nombre,
			epeios::row_t__ Position )
		{
			b::Store( (epeios::datum__ *)Tampon, Nombre * sizeof( t ), Position * sizeof( t ) );
		}
		/* écrit 'Nombre' objets de 'Source' à partir de 'Position'
		à la position 'Decalage' */
		void _Store_(
			const _memory_ &Source,
			epeios::size__ Quantite,
			epeios::row_t__ Position,
			epeios::row_t__ Decalage )
		{
			b::Store_( Source, ( Quantite * sizeof( t ) ),( Position * sizeof( t ) ),  ( Decalage * sizeof( t ) ) );
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
		//f Put in 'Buffer' 'Amount' bytes at 'Position'.
		void Recall(
			r Position,
			epeios::size__ Amount,
			t *Buffer ) const
		{
			_Recall( *Position, Amount, Buffer );
		}
		//f Put in 'Buffer' 'Amount' bytes at 'Position'. Return 'Buffer'.
		t *Get(
			r Position,
			epeios::size__ Amount,
			t *Buffer ) const
		{
			_Recall( *Position, Amount, Buffer );

			return Buffer;
		}
		//f Put in 'Value' the object at 'Position'.
		void Recall(
			r Position,
			t &Value ) const
		{
			_Recall( *Position, 1, &Value );
		}
		//f Return the object at 'Position'.
		const t Get( r Position ) const
		{
			epeios::datum__ V[sizeof( t )];

			_Recall( *Position, 1, (t *)V );

			return *(t *)V;
		}
		//f Store 'Amount' object in 'Buffer' at 'Position'.
		void Store(
			const t *Buffer,
			epeios::size__ Amount,
			r Position )
		{
			_Store( Buffer, Amount, *Position );
		}
		//f Store 'Value' at 'Position'.
		void Store(
			const t &Valeur,
			r Position )
		{
			_Store( &Valeur, 1, *Position );
		}
		/*f Store 'Amount' objects at 'Position' in 'Source' at 'Offset'. */
		void Store_(
			const _memory_<t,b,r> &Source,
			epeios::size__ Amount,
			r Position = 0,
			r Offset = 0 )
		{
			_Store_( Source, Amount, *Position, *Offset );
		}
		//f Swap objects at 'Position1' and 'Position2'.
		void Swap(
			r Position1,
			r Position2 )
		{
			epeios::datum__ O[sizeof( t )];

			Recall( Position1, *(t *)O );
			Store( Get( Position2 ), Position1 );
			Store( *(t *) O, Position2 );
		}
		//f Store at 'Position' 'Amount' objects.
		void Store(
			const t &Object,
			r Position,
			epeios::size__ Amount )
		{
			b::Store( (mdr::datum__ *)&Object, sizeof( t ), *Position * sizeof( t ), Amount );
		}
		//f Return the position from 'Object' between 'Begin' and 'End' (both included) or 'NONE' if non-existant.
		r Search(
			const t &Object,
			r Begin,
			r End ) const
		{
			epeios::row_t__ Position;

			if ( ( Position = b::Search( (mdr::datum__ *)&Object, sizeof( t ), *Begin * sizeof( t ), *End * sizeof( t ) ) ) != NONE )
				Position /= sizeof( t );

			return Position;
		}
		//f Allocate 'Size' objects.
		void Allocate( epeios::size__ Size )
		{
			Allocate_( Size );
		}
		//f Return the object at 'Position'..
		const t operator ()( r Position ) const
		{
			return Get( Position );
		}
		static size_t GetItemSize( void )
		{
			return sizeof( t );
		}
		b &GetUnderlyingMemory( void )
		{
			return *this;
		}
	};

	/*c Memory of statical object of type 't'. */
	template <typename t, typename r> class memory_
		: public _memory_< t, uym::untyped_memory_, r >
	/* NOTA: See 'memory_core about' '::s'. */
	{
	private:
		// l'éventuel pilote de la multimemoire
		mmm::multimemory_driver__ PiloteMultimemoire_;
	public:
		struct s
		: public _memory_< t, uym::untyped_memory_, r >::s
		{
			mmm::descriptor__ MultimemoryDriverDescriptor;
			bso::ubyte__ MultimemoryDriverAddendum;
			mdr::size__ MultimemoryDriverExtent;
		} &S_;
		memory_( s &S )
		: S_( S ),
		  _memory_< t, uym::untyped_memory_, r >( S ),
		  PiloteMultimemoire_( S.MultimemoryDriverDescriptor, S.MultimemoryDriverAddendum, S.MultimemoryDriverExtent )
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
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			PiloteMultimemoire_.reset();
			_memory_< t, uym::untyped_memory_, r >::plug( MD );
		}
		void WriteToFlow(
			r Position,
			epeios::size__ Quantity,
			flw::oflow__ &OFlow ) const
		{
			_memory_<t, uym::untyped_memory_, r >::WriteToFlow( *Position * sizeof( t ), Quantity * sizeof( t ) , OFlow );
		}
		void ReadFromFlow(
			flw::iflow__  &IFlow,
			r Position,
			epeios::size__ Quantite )
		{
			_memory_<t, uym::untyped_memory_, r >::ReadFromFlow( IFlow, *Position * sizeof( t ), Quantite * sizeof( t ) );
		}
	};

	/*
	template <class t> class memory
	: public memory_<t>
	{};
	*/

	E_AUTO2( memory )

	typedef uym::untyped_memory_file_manager___ memory_file_manager___;

	template <typename memory> inline bso::bool__ Connect(
		memory &Memory,
		memory_file_manager___ &FileManager )
	{
		return uym::Connect( Memory.GetUnderlyingMemory(), FileManager );
	}

	//m 'memory' would be often used, then create a special name.
	#define E_MEMORYt( t, r )	memory< t, r >
	#define E_MEMORYt_( t, r )	memory_< t, r >

	#define E_MEMORY_( t )	E_MEMORYt_( t, epeios::row__ )
	#define E_MEMORY( t )	E_MEMORYt( t, epeios::row__ )

	//f Return 'E1' - 'E2' which begin at 'BeginS1' and 'BeginS2' and have a length of 'Quantity'.
	template <class t, typename r> inline bso::sign__ Compare(
		const E_MEMORYt_( t, r ) &S1,
		const E_MEMORYt_( t, r ) &S2,
		r BeginS1,
		r BeginS2,
		epeios::size__ Quantity )
	{
		return uym::Compare( S1, S2, *BeginS1 * sizeof( t ), *BeginS2 * sizeof( t ), Quantity * sizeof( t ) );
	}

#if 0
	//c The static part for the '_memory_' object. CodeWarrior needs this. Internal use.
	template <class t,int size, typename r> class _memory_static_part__
	: public _memory_<t, uym::untyped_memory__< size >, r >::s
	{};
	

	/*c A static set of object of 'amount' objects of type 't' of size 'size'.
	The size parameter was added due to a bug of Borland C++, which doesn't like a 'sizeof'
	as template parameter. Use 'E_MEMORY(t)__', it's easier. */
	template <class t, int amount, int size, typename r> class memory__
	: public _memory_< t, uym::untyped_memory__< amount * size >, r >
	{
	private:
		_memory_static_part__<t, amount * size, r > Static_;
	public:
		memory__( _memory_static_part__<t, amount * size , r > &S = *(_memory_static_part__<t, amount * size, r > *) NULL )	// To simplify use in 'BCH'.
		: _memory_<t, uym::untyped_memory__< amount * size >, r >( Static_ )
		{}
	
	};
#endif

	/*c A static set of object of 'amount' objects of type 't' of size 'size'.
	The size parameter was added due to a bug of Borland C++, which doesn't like a 'sizeof'
	as template parameter. Use 'E_MEMORY(t)__', it's easier. */
	template <class t, int amount, int size, typename r> class memory__
	: public _memory_< t, uym::untyped_memory__< amount * size >, r >
	{
/*	private:
		_memory_<t, uym::untyped_memory__< amount * size >, r >::s Static_;	// Is empty.
*/	public:
		memory__( typename _memory_<t, uym::untyped_memory__< amount * size >, r >::s &S = *( typename _memory_<t, uym::untyped_memory__< amount * size >, r >::s *) NULL )	// To simplify use in 'BCH'.
		: _memory_<t, uym::untyped_memory__< amount * size >, r >( S )
		{}
	
	};


	//d A static set of 'amount' object of type 'Type'.
	#define E_MEMORYt__( type, amount, r ) memory__< type, amount, sizeof( type ), r > 
	#define E_MEMORY__( type, amount ) memory__< type, amount, sizeof( type ), epeios::row__ > 

#if 0
	//c The static part for the '_memory_' object. CodeWarrior needs this. Internal use.
	template <class t, typename r> class _memory_static_part___
	: public _memory_<t, uym::untyped_memory___, r >::s
	{};
	
	/*c A bunch of object of 'amount' objects of type 't' of size 'size' stored in conventional memory.
	The size parameter was added due to a bug of Borland C++, which doesn't like a 'sizeof'
	as template parameter. Use 'E_MEMORY(t)__', it's easier. */
	template <class t, typename r> class memory___
	: public _memory_< t, uym::untyped_memory___, r >
	{
/*	private:
		_memory_static_part___<t, r > Static_; // Is empty.
*/	public:
		memory___( _memory_static_part___<t, r > &S = *(_memory_static_part___<t, r > *) NULL )	// To simplify use in 'BCH'.
		: _memory_<t, uym::untyped_memory___, r >( Static_ )
		{}
	};
#endif

		/*c A bunch of object of 'amount' objects of type 't' of size 'size' stored in conventional memory.
	The size parameter was added due to a bug of Borland C++, which doesn't like a 'sizeof'
	as template parameter. Use 'E_MEMORY(t)__', it's easier. */
	template <class t, typename r> class memory___
	: public _memory_< t, uym::untyped_memory___, r >
	{
	private:
		typename _memory_<t, uym::untyped_memory___, r >::s Static_;
	public:
		memory___( typename _memory_<t, uym::untyped_memory___, r >::s &S = *( typename _memory_<t, uym::untyped_memory___, r >::s *) NULL )	// To simplify use in 'BCH'.
		: _memory_<t, uym::untyped_memory___, r >( S )
		{}
		t *Buffer( void )
		{
			return _memory_< t, uym::untyped_memory___, r >::Buffer();
		}
	};


	//d A static set of 'amount' object of type 'Type'.
	#define E_MEMORYt___( type, r ) memory___< type, r > 
	#define E_MEMORY___( type ) memory___< type, epeios::row__ > 
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
