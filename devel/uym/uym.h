/*
  Header for the 'uym' library by Claude L. Simon (simon@epeios.org)
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

#ifndef UYM__INC
#define UYM__INC

#define UYM_NAME		"UYM"

#define	UYM_VERSION	"$Revision$"	

#define UYM_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &UYMTutor;

#if defined( XXX_DBG ) && !defined( UYM_NODBG )
#define UYM_DBG 
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

//D UntYped Memory.

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
	class memory_driver
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
				}

			}

			Pilote_ = NULL;
			Interne_ = false;
		}
		memory_driver( void )
		{
			reset( false );
		}
		virtual ~memory_driver( void )
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
		operator int( void ) const
		{
			return Pilote_ != NULL;
		}
	};

	//c Untyped memory core. Don't use; for internal use only.
	class untyped_memory_
	{
	private:
		// Le pilote mémoire.
		memory_driver Pilote_;
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
		//f Wrtie to 'Position' 'Amount' bytes from 'Buffer'
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
			row__ End );
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

	//c Untyped memory core. Don't use; for internal use only.
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
	bso::sbyte__ Compare(
		const untyped_memory_ &M1,
		const untyped_memory_ &M2,
		row__ BeginM1,
		row__ BeginM2,
		size__ Quantity );


	//c Fixed-length memory core. Don't use; for internal use only.
	class memory_core__
	{
	private:
		data__ *Data_;
	public:
		memory_core__( data__ *Data )
		{
			Data_ = Data;
		}
		//f Initialization.
		void Init( void ){}
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
			return Data_[Position];
		}
		//f Write 'Byte' at 'Position'.
		void Write(
			data__ Byte,
			row__ Position )
		{
			Data_[Position] = Byte;
		}
		/*f Write at 'Offset' in 'Destination' 'Quantity' bytes at 'Position'. */
		void Read(
			row__ Position,
			size__ Quantity,
			memory_core__ &Destination,
			row__ Offset = 0 ) const
		{
			memmove( Destination.Data_ + Position, Data_ + Position, Quantity );
		}
		/*f Write to 'Offset' 'Quantity' bytes at 'Position' from 'Source'. */
		void Write(
			const memory_core__ &Source,
			size__ Quantity,
			row__ Position = 0,
			row__ Offset = 0 )
		{
			memmove( Data_ + Offset, Source.Data_ + Position, Quantity ); 
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
			row__ End );
	};


	//c A memory with a size of 'size'.
	template <int size> class untyped_memory__
	: public memory_core__
	{
	private:
		uym::data__ Data_[size];
	public:
		struct s {};	// To simplify use in library 'SET'
		untyped_memory__( s &S = *(s *)NULL )
		: memory_core__( Data_ ) {}
		// Simplifies the 'SET' library.
		void Allocate( uym::size__ Size )
		{
			if ( Size > size )
				ERRl();
		}
	};
}


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
