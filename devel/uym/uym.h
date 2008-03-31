/*
	Header for the 'uym' library by Claude SIMON (csimon@epeios.org)
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
#include "tol.h"
#include "mdr.h"
#include "cvm.h"
#ifndef FLM__COMPILATION	// To avoid resursive inclusion.
#	include "flm.h"
#endif

//d Value that a position can not have.
#define UYM_UNREACHABLE_POSITION	((mdr::row_t__)-1)

//d Maximal size of a memory.
#define UYM_MAX_SIZE			UYM_UNREACHABLE_POSITION

#ifdef UYM__DEFAULT_MEMORY_DRIVER
#	define UYM_DEFAULT_MEMORY_DRIVER UYM__DEFAULT_MEMORY_DRIVER
#else
#	define UYM_DEFAULT_MEMORY_DRIVER cvm::E_CONVENTIONAL_MEMORY_DRIVER__
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
	// Pilote m�moire.
	class _memory_driver
	{
	private:
		// Le pilote.
		mdr::E_MEMORY_DRIVER__ *Pilote_;
		// Indique si le pilote a �t� d�fini de mani�re interne ou non.
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
		void plug( mdr::E_MEMORY_DRIVER__ &Pilote )
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
				if ( ( Pilote_ = new UYM_DEFAULT_MEMORY_DRIVER ) == NULL )
					ERRa();
				else
				{
					Interne_ = true;
					((UYM_DEFAULT_MEMORY_DRIVER *)Pilote_)->Init();
				}
			}
		}
		mdr::E_MEMORY_DRIVER__ *Driver( bso::bool__ Ignore = false ) const
		{
	#ifdef UYM_DBG
			if ( !Ignore && !Pilote_ )
				ERRu();
	#endif
			return Pilote_;
		}
		// Operateur d'indirection.
		mdr::E_MEMORY_DRIVER__ *operator ->( void ) const
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
		// Le pilote m�moire.
		_memory_driver Pilote_;
	#ifdef UYM_DBG
		void _Test( void ) const
		{
			if ( !Pilote_ )
				ERRu();
		}
	#endif
		void _Recall(
			mdr::row_t__ Position,
			mdr::size__ Nombre, 
			mdr::datum__ *Tampon ) const
		{
	#ifdef UYM_DBG
			_Test();
	#endif
			Pilote_->Recall( Position, Nombre, Tampon );
		}
		void _Store(
			const mdr::datum__ *Tampon,
			mdr::size__ Nombre,
			mdr::row_t__ Position )
		{
	#ifdef UYM_DBG
			_Test();
	#endif
			Pilote_->Store( Tampon, Nombre, Position );
		}
		void _Allocate( mdr::size__ Size )
		{
	#ifdef UYM_DBG
			_Test();
	#endif
			Pilote_->Allocate( Size );
		}
		void _Flush( void ) const
		{
			if ( Pilote_ )
				Pilote_->Flush();
		}
	public:
		struct s {
			// A des fins de standardisation.
		};
		void reset( bool P )
		{
			Pilote_.reset( P );
		}
		untyped_memory_( s &S )
		{
			reset( false );
		}
		void plug( mdr::E_MEMORY_DRIVER__ &Driver  )
		{
			Pilote_.plug( Driver );
		}
		untyped_memory_ &operator =( const untyped_memory_ & ) const
		{
			ERRu();	// Parce que cette op�ration n'a pas de sens.

			return *(untyped_memory_ *)NULL;
		}
		//f Initialization.
		void Init( void )
		{
			Pilote_.Init();
		}
		void WriteToFlow(
			mdr::row_t__ Position,
			mdr::size__ Size,
			flw::oflow__ &OFlow ) const;
		void ReadFromFlow(
			flw::iflow__  &IFlow,
			mdr::row_t__ Position,
			mdr::size__ Size );
		//f Allocates 'Capacity' bytes.
		void Allocate( mdr::size__ Size )
		{
			_Allocate( Size );
		}
		/*f Recall 'Amount' bytes at 'Position' and put them in 'Buffer'.
		Ignore is only for 'UYM_DBG' mode and for the 'MMG' library.
		When 'true', it didn't make the test about the size. */
		void Recall(
			mdr::row_t__ Position,
			mdr::size__ Amount,
			mdr::datum__ *Buffer ) const
		{
			_Recall( Position, Amount, Buffer );
		}
		//f Store 'Amount' bytes from 'Buffer' at 'Offset'.
		void Store(
			const mdr::datum__ *Buffer,
			mdr::size__ Amount,
			mdr::row_t__ Position )
		{
			_Store( Buffer, Amount, Position );
		}
		//f Put byte at 'Position' in 'Datum'.
		void Recall(
			mdr::row_t__ Position,
			mdr::datum__ &Datum ) const
		{
			Recall( Position, 1, &Datum );
		}
		//f Return byte at 'Position'.
		mdr::datum__ Get( mdr::row_t__ Position ) const
		{
			mdr::datum__ D;

			Recall( Position, D );

			return D;
		}
		//f Write 'Byte' at 'Position'.
		void Store(
			mdr::datum__ Datum,
			mdr::row_t__ Position )
		{
			_Store( &Datum, 1, Position );
		}
		void Put(
			mdr::datum__ Datum,
			mdr::row_t__ Position )
		{
			Store( Datum, Position );
		}
		//f Store 'Amount' bytes at 'Position' in 'Begin' at 'Offset'.
		void Store_(
			const untyped_memory_ &Source,
			mdr::size__ Amount,
			mdr::row_t__ Position,
			mdr::row_t__ Offset = 0 );
		//f Store 'Count' 'Object's of size 'Size' at 'Position'.
		void Store(
			const mdr::datum__ *Object,
			mdr::size__ Size,
			mdr::row_t__ Position,
			mdr::size__ Count );
		//f Search 'Object' of size 'Size' between 'Begin' and 'End' (excluded) and return its position or 'NONE' if non-existant.
		mdr::row_t__ Search(
			const mdr::datum__ *Objet,
			mdr::size__ Size,
			mdr::row_t__ Begin,
			mdr::row_t__ End ) const;
		//f Return the used memory driver. 'Ignore' is only for 'UYM_DBG' mode and for the 'MMG' library.
		mdr::E_MEMORY_DRIVER__ *Driver( bso::bool__ Ignore = false )
		{
			return Pilote_.Driver( Ignore );
		}
		//f Flushes all the caches.
		void Flush( void ) const
		{
			_Flush();
		}
	};

	typedef flm::E_FILE_MEMORY_DRIVER___ _file_memory_driver___;

	class untyped_memory_file_manager___
	: public _file_memory_driver___
	{
	public:
		void Init( 
			const char *FileName,
			mdr::mode__ Mode,
			bso::bool__ Persistent,
			flm::id__ ID )
		{
			_file_memory_driver___::Init( ID, Persistent, FileName, Mode, flm::cFirstUse );

			if ( Persistent )
				_file_memory_driver___::Persistent();
		}
	};

	inline bso::bool__ Connect(
		untyped_memory_ &Memory,
		untyped_memory_file_manager___ &FileManager )
	{
		bso::bool__ Exists = FileManager.Exists();

		Memory.plug( FileManager );

		if ( Exists )
			Memory.Allocate( FileManager.FileSize() );

		return Exists;
	}

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
#ifndef NONE
	#define NONE	UYM_UNREACHABLE_POSITION
#endif

	void _Copy(
		const class untyped_memory_ &Source,
		mdr::row_t__ PosSource,
		class untyped_memory_ &Dest,
		mdr::row_t__ PosDest,
		mdr::size__ Quantity,
		mdr::datum__ *Buffer,
		mdr::size__ BufferSize );

	//f Return 'E1' - 'E2' which begin at 'BeginS1' and 'BeginS2' and have a length of 'Quantity'.
	bso::sign__ Compare(
		const untyped_memory_ &M1,
		const untyped_memory_ &M2,
		mdr::row_t__ BeginM1,
		mdr::row_t__ BeginM2,
		mdr::size__ Quantity );

	void _Fill(
		const mdr::datum__ *Object,
		mdr::size__ Size,
		mdr::size__ Count,
		mdr::row_t__ Position,
		mdr::datum__ *Data );
#if 0
	row__ _Position(
		const datum__ *Objet,
		size__ Size,
		row__ Begin,
		row__ End,
		const datum__ *Data );
#endif

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
		void Recall(
			mdr::row_t__ Position,
			mdr::size__ Amount,
			mdr::datum__ *Buffer ) const
		{
			memcpy( Buffer, m::Data_ + Position, Amount ); 
		}
		//f Write to 'Position' 'Amount' bytes from 'Buffer'.
		void Store(
			const mdr::datum__ *Buffer,
			mdr::size__ Amount,
			mdr::row_t__ Position )
		{
			memcpy( m::Data_ + Position, Buffer, Amount ); 
		}
		//f Return byte at 'Position'.
		mdr::datum__ Get( mdr::row_t__ Position ) const
		{
			return *m::Data_[Position];
		}
		//f Write 'Byte' at 'Position'.
		void Store(
			mdr::datum__ Datum,
			mdr::row_t__ Position )
		{
			*m::Data_[Position] = Datum;
		}
		void Put(
			mdr::datum__ Datum,
			mdr::row_t__ Position )
		{
			Store( Datum, Position );
		}
		/*f Write to 'Offset' 'Quantity' bytes at 'Position' from 'Source'. */
		void Store(
			const _memory__ &Source,
			mdr::size__ Quantity,
			mdr::row_t__ Position = 0,
			mdr::row_t__ Offset = 0 )
		{
			memmove( m::Data_ + Offset, Source.m::Data_ + Position, Quantity ); 
		}
		/*f Store to 'Offset' 'Quantity' bytes at 'Position' from 'Source'. */
		void Store(
			const untyped_memory_ &Source,
			mdr::size__ Quantity,
			mdr::row_t__ Position = 0,
			mdr::row_t__ Offset = 0 )
		{
			Source.Recall( Position, Quantity, *m::Data_ + Offset );
		}
		//f Fill at 'Position' with 'Object' of size 'Size' 'Count' times.
		void Store(
			const mdr::datum__ *Object,
			mdr::size__ Size,
			mdr::row_t__ Position,
			mdr::size__ Count )
		{
			_Store( Object, Size, Count, Position, m::Data_ );
		}
		//f Return the position from 'Object' of size 'Size' between 'Begin' and 'End' (excluded) oR 'NONE' if non-existant.
		mdr::row_t__ Search(
			const mdr::datum__ *Object,
			mdr::size__ Size,
			mdr::row_t__ Begin,
			mdr::row_t__ End ) const
		{
			return _Search( Object, Size, Begin, End, m::Data_ );
		}
		//f Return the used buffer.
		const mdr::datum__ *Buffer( void ) const
		{
			return m::Data_;
		}
	};

	//c A fixed-size memory of size 'size'.
	template <int size> class _untyped_memory__
	{
	protected:
		mdr::datum__ Data_[size];
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
		void Allocate( mdr::size__ Size )
		{
			if ( Size >= size )
				ERRl();
		}
	};

	//c A untyped memory using conventional memory.
	class _untyped_memory___
	{
	protected:
		tol::E_FPOINTER___( mdr::datum__ ) Data_;
	public:
		struct s {};	// To simplify use in library 'BCH'
		void reset( bso::bool__ P = true )
		{
			Data_.reset( P );
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
		void Allocate( mdr::size__ Size )
		{
			Data_ = realloc( Data_, Size );
		}
		//f Initialization.
		void Init( void )
		{
			Data_.Init();
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
}
/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
