/*
	Header for the 'uym' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2003 Claude SIMON (csimon@epeios.org).

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

#ifndef UYM__INC
#define UYM__INC

#define UYM_NAME		"UYM"

#define	UYM_VERSION	"$Revision$"

#define UYM_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &UYMTutor;

#if defined( XXX_DBG ) && !defined( UYM_NODBG )
#define UYM_DBG
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
//D UnTyped Memory 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "mdr.h"
#include "cvm.h"
#include "fam.h"

//d Value that a position can not have.
#define UYM_UNREACHABLE_POSITION	((uym::row__)-1)

//d Maximal size of a memory.
#define UYM_MAX_SIZE			UYM_UNREACHABLE_POSITION

#ifndef UYM_MAX_BUFFER_SIZE
//d Max. size max. of a copy buffer.
	#define UYM_MAX_BUFFER_SIZE	10000
#endif

#ifndef UYM_MIN_BUFFER_SIZE
//d Min. size of a copy buffer.
	#define UYM_MIN_BUFFER_SIZE	10
#endif

#ifndef UYM_DEFAULT_E_MEMORY_DRIVER
#define UYM_DEFAULT_E_MEMORY_DRIVER cvm::conventional_memory_driver
#endif
/*

#ifdef DED_MS
	#ifdef __MSDOS__
		#include "mfg.h"
		#define UYM_PILOTE_DEFAUT mfg_pilote_memoire_fragmentee
	#else
		#include "mmb.h"
		#define UYM_PILOTE_DEFAUT mmb_pilote_memoire_monobloc
	#endif
#else
	#include "mmb.h"
	#define UYM_PILOTE_DEFAUT mmb_pilote_memoire_monobloc
#endif
*/

namespace uym {
	using namespace mdr;

	// Pilote mémoire.
	class _memory_driver
	{
	private:
		// Le pilote.
		E_MEMORY_DRIVER_ *Pilote_;
		// Indique si le pilote a été défini de manière interne ou non.
		bso::bool__ Interne_;
	public:
		void reset( bool P = true )
		{
			if ( P )
			{
				if ( Interne_ )
				{
					delete Pilote_;
					Interne_ = false;
					Pilote_ = NULL;
				}

			} else {
				Pilote_ = NULL;
				Interne_ = false;
			}
		}
		_memory_driver( void )
		{
			reset( false );
		}
		virtual ~_memory_driver( void )
		{
			reset( true );
		}
		void plug( E_MEMORY_DRIVER_ &Pilote )
		{
			if ( &Pilote != NULL )
			{
				if ( Interne_ )
					delete Pilote_;

				Pilote_ = &Pilote;
				Interne_ = false;
			}
			else if ( !Interne_ )
				Pilote_ = NULL;
		}
		// Initialization.
		void Init( void )
		{
			if ( Pilote_ == NULL )
			{
				if ( ( Pilote_ = new UYM_DEFAULT_E_MEMORY_DRIVER ) == NULL )
					ERRa();
				else
				{
					Interne_ = true;
					((UYM_DEFAULT_E_MEMORY_DRIVER *)Pilote_)->Init();
				}
			}
		}
		E_MEMORY_DRIVER_ *Driver( bso::bool__ Ignore = false ) const
		{
	#ifdef UYM_DBG
			if ( !Ignore && !Pilote_ )
				ERRu();
	#endif
			return Pilote_;
		}
		// Operateur d'indirection.
		E_MEMORY_DRIVER_ *operator ->( void ) const
		{
			return Driver();
		}
		operator bool( void ) const
		{
			return Pilote_ != NULL;
		}
	};

	//c Untyped memory.
	class untyped_memory_
	{
	private:
		// Le pilote mémoire.
		_memory_driver Pilote_;
	#ifdef UYM_DBG
		void Test_(
			row__ Position,
			bsize__ Nombre ) const
		{
			if ( !Pilote_ )
				ERRu();

			if ( ( Position + Nombre ) > S_.Size )
				ERRu();
		}
	#endif
		void Lire_(
			row__ Position,
			bsize__ Nombre, 
			data__ *Tampon,
			bso::bool__ Ignore = false ) const
		{
	#ifdef UYM_DBG
			if ( !Ignore )
				Test_( Position, Nombre );
	#endif
			Pilote_->Read( Position, Nombre, Tampon );
		}
		// lit à partir de 'Position' et place dans 'Tampon' 'Nombre' octets;
		void Ecrire_(
			const data__ *Tampon,
			bsize__ Nombre,
			row__ Position )
		{
	#ifdef UYM_DBG
			Test_( Position, Nombre );
	#endif
			Pilote_->Write( Tampon, Nombre, Position );
		}
		// écrit 'Nombre' octets à la position 'Position'
		void Allouer_( size__ Size )
		{
	#ifdef UYM_DBG
			S_.Size = Size;

			if ( !Pilote_ )
				ERRu();
	#endif
			Pilote_->Allocate( Size );
		}
		// alloue 'Capacite' octets
		void Synchroniser_( void ) const
		{
			if ( Pilote_ )
				Pilote_->Synchronize();
		}
	public:
		struct s {
	#ifdef UYM_DBG
			size__ Size;
		} &S_;
	#else
		};
	#endif
		void reset( bool P )
		{
			Pilote_.reset( P );
	#ifdef UYM_DBG
			S_.Size = 0;
	#endif
		}
		untyped_memory_( s &S )
	#ifdef UYM_DBG
		: S_( S )
	#endif
		{
			reset( false );
		}
		void plug( E_MEMORY_DRIVER_ &Driver = *(E_MEMORY_DRIVER_ *)NULL )
		{
			Pilote_.plug( Driver );
		}
		//f Initialization.
		void Init( void )
		{
	#ifdef UYM_DBG
			S_.Size = 0;
	#endif
			Pilote_.Init();
		}
		void write(
			row__ Position,
			size__ Size,
			flw::oflow___ &OFlow ) const;
		void read(
			flw::iflow___  &IFlow,
			row__ Position,
			size__ Size );
		//f Allocates 'Capacity' bytes.
		void Allocate( size__ Size )
		{
			Allouer_( Size );
		}
		/*f Put in 'Buffer' 'Amount' bytes at 'Position'.
		Ignore is only for 'UYM_DBG' mode and for the 'MMG' library.
		When 'true', it didn't make the test about the size. */
		void Read(
			row__ Position,
			bsize__ Amount,
			data__ *Buffer,
			bso::bool__ Ignore = false ) const
		{
			Lire_( Position, Amount, Buffer, Ignore );
		}
		//f Write to 'Position' 'Amount' bytes from 'Buffer'
		void Write(
			const data__ *Buffer,
			bsize__ Amount,
			row__ Position )
		{
			Ecrire_( Buffer, Amount, Position );
		}
		//f Return byte at 'Position'.
		data__ Read( row__ Position ) const
		{
			data__ D;

			Lire_( Position, 1, &D );

			return D;
		}
		//f Write 'Byte' at 'Position'.
		void Write(
			data__ Byte,
			row__ Position )
		{
			Ecrire_( &Byte, 1, Position );
		}
		/*f Write at 'Offset' in 'Destination' 'Quantity' bytes at 'Position'. */
		void Read(
			row__ Position,
			size__ Quantity,
			untyped_memory_ &Destination,
			row__ Offset = 0 ) const
		{
			Destination.Write( *this, Quantity, Position, Offset );
		}
		/*f Write to 'Offset' 'Quantity' bytes at 'Position' from 'Source'. */
		void Write(
			const untyped_memory_ &Source,
			size__ Size,
			row__ Position = 0,
			row__ Offset = 0 );
		/*f Write to 'Offset' 'Quantity' bytes at 'Position' from 'Source'. */
#if 0
		void Write(
			const class _memory__ &Source,
			size__ Size,
			row__ Position = 0,
			row__ Offset = 0 );
#endif
		//f Fill at 'Position' with 'Object' of size 'Size' 'Count' times.
		void Fill(
			const data__ *Object,
			bsize__ Size,
			size__ Count,
			row__ Position = 0 );
		//f Return the position from 'Object' of size 'Size' between 'Begin' and 'End' (excluded) or 'NONE' if non-existant.
		row__ Position(
			const data__ *Objet,
			bsize__ Size,
			row__ Begin,
			row__ End ) const;
		//f Return the used memory driver. 'Ignore' is only for 'UYM_DBG' mode and for the 'MMG' library.
		E_MEMORY_DRIVER_ *Driver( bso::bool__ Ignore = false )
		{
			return Pilote_.Driver( Ignore );
		}
		//f Flushes all the caches.
		void Synchronize( void ) const
		{
			Synchroniser_();
		}
	};

	//c Untyped memory. 
	class untyped_memory
	: public untyped_memory_
	{
	private:
		untyped_memory_::s S_;
	public:
		untyped_memory( void )
		: untyped_memory_( S_ )
		{}
	};


	//d A position take this value if an object cannot be find.
	#define NONE	UYM_UNREACHABLE_POSITION

	void Copy_(
		const class untyped_memory_ &Source,
		row__ PosSource,
		class untyped_memory_ &Dest,
		row__ PosDest,
		size__ Quantity,
		data__ *Buffer,
		bsize__ BufferSize );

	//f Return 'E1' - 'E2' which begin at 'BeginS1' and 'BeginS2' and have a length of 'Quantity'.
	bso::sign__ Compare(
		const untyped_memory_ &M1,
		const untyped_memory_ &M2,
		row__ BeginM1,
		row__ BeginM2,
		size__ Quantity );

#if 0
	//c The core of a memory. Don't use; for internal use only.
	class _memory__
	{
	private:
		data__ **Data_;
	public:
		void reset( bso::bool__ = true )
		{
			Data_ = NULL;
		}
		_memory__( data__ *&Data )
		{
			reset( false );

			Data_ = &Data;
		}
		~_memory__( void )
		{
			reset( true );
		}
		//f Initialization.
		void Init( void )
		{}
		//f Put in 'Buffer' 'Amount' bytes at 'Position'.
		void Read(
			row__ Position,
			bsize__ Amount,
			data__ *Buffer ) const
		{
			memcpy( Buffer, Data_ + Position, Amount ); 
		}
		//f Write to 'Position' 'Amount' bytes from 'Buffer'.
		void Write(
			const data__ *Buffer,
			bsize__ Amount,
			row__ Position )
		{
			memcpy( Data_ + Position, Buffer, Amount ); 
		}
		//f Return byte at 'Position'.
		data__ Read( row__ Position ) const
		{
			return *Data_[Position];
		}
		//f Write 'Byte' at 'Position'.
		void Write(
			data__ Byte,
			row__ Position )
		{
			*Data_[Position] = Byte;
		}
		/*f Write at 'Offset' in 'Destination' 'Quantity' bytes at 'Position'. */
		void Read(
			row__ Position,
			size__ Quantity,
			_memory__ &Destination,
			row__ Offset = 0 ) const
		{
			memmove( Destination.Data_ + Position, Data_ + Position, Quantity );
		}
		/*f Write to 'Offset' 'Quantity' bytes at 'Position' from 'Source'. */
		void Write(
			const _memory__ &Source,
			size__ Quantity,
			row__ Position = 0,
			row__ Offset = 0 )
		{
			memmove( Data_ + Offset, Source.Data_ + Position, Quantity ); 
		}
		/*f Write to 'Offset' 'Quantity' bytes at 'Position' from 'Source'. */
		void Write(
			const untyped_memory_ &Source,
			size__ Quantity,
			row__ Position = 0,
			row__ Offset = 0 )
		{
			Source.Read( Position, Quantity, *Data_ + Offset );
		}
		//f Fill at 'Position' with 'Object' of size 'Size' 'Count' times.
		void Fill(
			const data__ *Object,
			bsize__ Size,
			size__ Count,
			row__ Position = 0 );
		//f Return the position from 'Object' of size 'Size' between 'Begin' and 'End' (excluded) oR 'NONE' if non-existant.
		row__ Position(
			const data__ *Objet,
			bsize__ Size,
			row__ Begin,
			row__ End ) const;
		//f Return the used buffer.
		const data__ *Buffer( void ) const
		{
			return *Data_;
		}
	};

	inline void untyped_memory_::Write(
		const _memory__ &Source,
		size__ Size,
		row__ Position,
		row__ Offset )
	{
		Write( Source.Buffer() + Position, Size, Offset );
	}


	//c A fixed-size memory of size 'size'.
	template <int size> class untyped_memory__
	: public _memory__
	{
	private:
		uym::data__ Data_[size];
	public:
		struct s {};	// To simplify use in library 'SET'
		untyped_memory__( s &S = *(s *)NULL )
			: _memory__( &Data_[0] ) {}
		// Simplifies the 'SET' library.
		void Allocate( uym::size__ Size )
		{
			if ( Size >= size )
				ERRl();
		}
		void Init( void )
		{
			_memory__::Init();
		}
	};

	typedef _memory__	_memory___;

	//c A untyped memory using conventional memory.
	class untyped_memory___
	: public _memory___
	{
	private:
		uym::data__ *Data_;
	public:
		struct s {};	// To simplify use in library 'SET'
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				tol::Free( Data_ );
			}

			Data_ = NULL;
		}
		untyped_memory___( s &S = *(s *)NULL )
		: _memory___( Data_ )
		{
			reset( false );
		}
		~untyped_memory___( void )
		{
			reset( true );
		}
		//f Allocation of size 'Size'.
		void Allocate( uym::size__ Size )
		{
			realloc( Data_, Size );
		}
		//f Initialization.
		void Init( void )
		{
			tol::Free( Data_ );

			_memory___::Init();
		}
	};
#else

	void _Fill(
		const data__ *Object,
		bsize__ Size,
		size__ Count,
		row__ Position,
		data__ *Data );

	row__ _Position(
		const data__ *Objet,
		bsize__ Size,
		row__ Begin,
		row__ End,
		const data__ *Data );

	template <typename m> class _memory__
	: public m
	{
	public:
		struct s
		: public m::s
		{};
		void reset( bso::bool__ P = true )
		{
			m::reset( P );
		}
		_memory__( s &S = *(s *)NULL )
		: m( S )
		{
			reset( false );
		}
		~_memory__( void )
		{
			reset( true );
		}
		//f Initialization.
		void Init( void )
		{}
		//f Put in 'Buffer' 'Amount' bytes at 'Position'.
		void Read(
			uym::row__ Position,
			uym::bsize__ Amount,
			uym::data__ *Buffer ) const
		{
			memcpy( Buffer, Data_ + Position, Amount ); 
		}
		//f Write to 'Position' 'Amount' bytes from 'Buffer'.
		void Write(
			const uym::data__ *Buffer,
			uym::bsize__ Amount,
			uym::row__ Position )
		{
			memcpy( Data_ + Position, Buffer, Amount ); 
		}
		//f Return byte at 'Position'.
		uym::data__ Read( uym::row__ Position ) const
		{
			return *Data_[Position];
		}
		//f Write 'Byte' at 'Position'.
		void Write(
			uym::data__ Byte,
			uym::row__ Position )
		{
			*Data_[Position] = Byte;
		}
		/*f Write at 'Offset' in 'Destination' 'Quantity' bytes at 'Position'. */
		void Read(
			uym::row__ Position,
			uym::size__ Quantity,
			_memory__ &Destination,
			uym::row__ Offset = 0 ) const
		{
			memmove( Destination.Data_ + Position, Data_ + Position, Quantity );
		}
		/*f Write to 'Offset' 'Quantity' bytes at 'Position' from 'Source'. */
		void Write(
			const _memory__ &Source,
			uym::size__ Quantity,
			uym::row__ Position = 0,
			uym::row__ Offset = 0 )
		{
			memmove( Data_ + Offset, Source.Data_ + Position, Quantity ); 
		}
		/*f Write to 'Offset' 'Quantity' bytes at 'Position' from 'Source'. */
		void Write(
			const untyped_memory_ &Source,
			uym::size__ Quantity,
			uym::row__ Position = 0,
			uym::row__ Offset = 0 )
		{
			Source.Read( Position, Quantity, *Data_ + Offset );
		}
		//f Fill at 'Position' with 'Object' of size 'Size' 'Count' times.
		void Fill(
			const uym::data__ *Object,
			uym::bsize__ Size,
			uym::size__ Count,
			uym::row__ Position = 0 )
		{
			_Fill( Object, Size, Count, Position, Data_ );
		}
		//f Return the position from 'Object' of size 'Size' between 'Begin' and 'End' (excluded) oR 'NONE' if non-existant.
		uym::row__ Position(
			const uym::data__ *Objet,
			uym::bsize__ Size,
			uym::row__ Begin,
			uym::row__ End ) const
		{
			return _Position( Object, Size, Begin, End, Data_ );
		}
		//f Return the used buffer.
		const uym::data__ *Buffer( void ) const
		{
			return *Data_;
		}
	};

	//c A fixed-size memory of size 'size'.
	template <int size> class _untyped_memory__
	{
	protected:
		uym::data__ Data_[size];
	public:
		void reset( bso::bool__ = true )
		{}
		struct s {};	// To simplify use in library 'BCH'
		_untyped_memory__( s &S = *(s *)NULL )
		{
			reset( false );
		}
		~_untyped_memory__( void )
		{
			reset();
		}
		// Simplifies the 'SET' library.
		void Allocate( uym::size__ Size )
		{
			if ( Size >= size )
				ERRl();
		}
	};

	//c A untyped memory using conventional memory.
	class _untyped_memory___
	{
	protected:
		uym::data__ *Data_;
	public:
		struct s {};	// To simplify use in library 'BCH'
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				tol::Free( Data_ );
			}

			Data_ = NULL;
		}
		_untyped_memory___( s &S = *(s *)NULL )	// To simplify use in library 'BCH'
		{
			reset( false );
		}
		~_untyped_memory___( void )
		{
			reset( true );
		}
		//f Allocation of size 'Size'.
		void Allocate( uym::size__ Size )
		{
			realloc( Data_, Size );
		}
		//f Initialization.
		void Init( void )
		{
			tol::Free( Data_ );
		}
	};

	template <int size> class untyped_memory__
	: public _memory__< _untyped_memory__< size > >
	{
	public:
		struct s
		: _memory__< _untyped_memory__< size > >::s
		{};
		untyped_memory__( s &S = *(s *)NULL )	// To simplify use in library 'BCH'
		: _memory__< _untyped_memory__< size > >( S )
		{}
	};

	typedef _memory__< _untyped_memory___>	untyped_memory___;
#endif
}
/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
