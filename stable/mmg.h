/*
	Header for the 'mmg' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2000-2004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#ifndef MMG__INC
#define MMG__INC

#define MMG_NAME		"MMG"

#define	MMG_VERSION	"$Revision$"

#define MMG_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &MMGTutor;

#if defined( XXX_DBG ) && !defined( MMG_NODBG )
#define MMG_DBG
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
//D Memory MerGer 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

//D Memory MerGer.

#include "err.h"
#include "mmm.h"
#include "flm.h"

namespace mmg
{
	//e Rule about how the object is handle.
	enum rule
	{
		//i L'object est created.
		rCreation,
		//i L'objet is recovered.
		rRecovery
	};
	//e state of the object
	enum state
	{
		//i Immortal.
		sImmortal,
		//i Mortal.
		sMortal,
	};

	struct merger_memory_driver_s__
	{
		uym::untyped_memory_::s Memory;
	};


	// Pilote mémoire à usage interne.
	template <typename st> class merger_memory_driver_
	: public mdr::E_MEMORY_DRIVER__
	{
	private:
		// Pointeur sur la partie statique de l'objet à sauver.
		st *Static_;
	protected:
		// Alloue 'Capacite' octets.
		virtual void MDRAllocate( mdr::size__ Capacity )
		{
			Memory.Allocate( Capacity + sizeof( st ) );
		}
		virtual mdr::size__ MDRUnderlyingSize( void )
		{
			mdr::size__ Size = Memory.Size();

			if ( Size >= sizeof( st ) )
				Size -= sizeof( st );
			else if ( Size != 0 )
				ERRc();

			return Size;
		}
		virtual void MDRRecall(
			mdr::row_t__ Position,
			mdr::size__ Amount,
			mdr::datum__ *Buffer )
		{
			Memory.Recall( Position + sizeof( st ), Amount, Buffer );
		}
		virtual void MDRStore(
			const mdr::datum__ *Buffer,
			mdr::size__ Amount,
			mdr::row_t__ Position )
		{
			StockerStatique();
			Memory.Store( Buffer, Amount, Position + sizeof( st ) );
		}
	public:
		typedef merger_memory_driver_s__ s;
		s &S_;
		uym::untyped_memory_ Memory;
		void reset( bool P = true )
		{
			E_MEMORY_DRIVER__::reset( P );
			Memory.reset( P );
			Static_ = NULL;
		}
		merger_memory_driver_( s &S )
		: S_( S ),
		  Memory( S.Memory )
		{
			reset( false );
		}
		~merger_memory_driver_( void )
		{
			reset();
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Memory.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MMM )
		{
			Memory.plug( MMM );
		}
		void Init(
			st *Static,
			rule Regle )
		{
			Static_ = Static;
			mdr::E_MEMORY_DRIVER__::Init();
			Memory.Init();

			if ( Regle == mmg::rCreation )
			{
				Memory.Allocate( sizeof( st ) );
				StockerStatique();
			}
			else if ( Regle == mmg::rRecovery ) {
				RecupererStatique();
			} else
				ERRc();
		}
		void StockerStatique( void )
		{
			if ( Static_ /* && ( Mode_== mdr::mReadWrite )*/ )
				Memory.Store( (mdr::datum__ *)Static_, sizeof( st ), 0 );
		}
		void EcrireDansFlot( flw::oflow__ &Flot ) const
		{
			Flot.Put( Static_, sizeof( st ) );
		}
		void LireDeFlot( flw::iflow__ &Flot )
		{
			Flot.Get( sizeof( st ), Static_ );
		}
		void RecupererStatique( void )
		{
			Memory.Recall( 0, sizeof( st ), (mdr::datum__ *)Static_ );
		}
		void Immortalize( void )
		{
			reset( false );
			plug( *(mdr::E_MEMORY_DRIVER__ *)NULL );
		}

	};

	// Pour VC++, qui ne comprend rien sinon.
	template <class st> struct mmg_s
	{
		mmm::multimemory_::s Memoire;
		// A vrai si le contenu survit à l'objet.
		st Object;
		merger_memory_driver_s__ Driver_;
		mmg::state State;
	};

	/*c Merge all the memory of an object of type 't', including the statical
	part, in one memory. Use 'MMG_MEMORY_MERGER_( t )' rather then directly this class. */
	template <class t, class st> class memory_merger_
	{
	private:
		t Object;
	public:
		// La mémoire qui contient la partie dynamique de l'objet.
		mmm::multimemory_ Memoire;
		// La partie statique de l'objet à persister.
	/*	struct s
		: public st
		{
			mmm_multimemoire_::s Memoire;
			// A vrai si le contenu survit à l'objet.
			fdm__bool Persistant;
			friend mmg_centralisateur_memoire_;
		} *S_;
	*/
		struct s
		: public mmg::mmg_s < st > {} &S_;
	private:
		// Le pilote gèrant les particularités de cet objet
		mmg::merger_memory_driver_ < st > Driver_;
	public:
		memory_merger_( s &S )
		: S_( S ),
		  Object( S.Object ),
		  Memoire( S.Memoire ),
		  Driver_( S.Driver_ )
		{}
		void reset( bool P = true )
		{
			if ( P ) {
				if ( S_.State == mmg::sImmortal )
					Immortalize();
			}

			Object.reset( P );
			Memoire.reset( P );
			Driver_.reset( P );

			S_.State = mmg::sMortal;

		}
		//f Utilisation de 'Pilote' comme pilote mémoire.
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Driver_.plug( MD );
			Memoire.plug( Driver_ );	// Notamment pour initialiser correctement le 'size'.
		}
		void plug( mmm::multimemory_ &MM )
		{
			Memoire.Flush();

			Driver_.plug( MM );
			Memoire.plug( Driver_ );
		}
		//f Initialization with rule 'Rule' and mode 'Mode'.
		void Init( mmg::rule Rule )
		{
			if ( S_.State == mmg::sImmortal )
				Immortalize();

			S_.State = mmg::sMortal;

			Object.plug( Memoire );

			Driver_.Init( &S_.Object, Rule );

			Memoire.plug( Driver_ );

			if ( Rule == mmg::rCreation )
				Memoire.Init();
		}
		//f The memory is unpluged and can be used by an other object.
		void Immortalize( void )
		{
			Driver_.Immortalize();
			Object.reset( false );
			Memoire.reset( false );
			Driver_.reset( false );
		}
		memory_merger_ &operator =( const memory_merger_ & )
		{
			ERRu();

			return *this;	// Only to avoid a warning.
		}
		//f The object would be immortalized or destructed depend of the value of 'State'.
		void State( mmg::state State )
		{
			S_.State = State;
		}
		//f Return the object which is handled by.
		t &operator ()( void )
		{
			return Object;
		}
		//f Return the object which is handled by.
		t *operator ->( void )
		{
			return &Object;
		}
	};

	E_AUTO2( memory_merger )

	//m Merge all the memory, including the statical part, of a 't' type object in one memory.
	#define E_MEMORY_MERGER_( t )	memory_merger_<t, t::s>

	#define E_MEMORY_MERGER( t )	memory_merger<t, t::s>

	/*c File memory merger. Same as mmg_memory_merger_<t, t::s>, but use a file as memory.
	Use 'MMG_FILE_MEMORY_MERGER( t )' rather then directly this class. */
	template <class t, class st> class file_merger___
	: public memory_merger<t, st>
	{
	private:
		flm::file_memory_driver___ PiloteFichier_;
		flm::id__ _ID;
	public:
		void reset( bool P = true )
		{
			memory_merger<t, st>::reset( P );

			PiloteFichier_.reset( P );

			if ( P )
				if ( _ID != FLM_UNDEFINED_ID )
					flm::ReleaseId( _ID );

			_ID = FLM_UNDEFINED_ID;
		}
		file_merger___( void )
		: PiloteFichier_()
		{
			reset( false );
		}
		~file_merger___( void )
		{
			reset();
		}
		/*f Initialization with file named 'FileName'. The object is placed
		in 'ObjectMode', and the file in 'FileMode'. Return a value which depends
		on whether exists or not. If the file doesn't already exist,
		both 'ObjectMode' and 'FileMode' are forced for to 'mdr::mReadWrite'. */
		mmg::rule Init(
			const char *FileName,
			fil::mode__ FileMode )
		{	
			reset();

			bso::bool__ Test;

			Test = fil::FileExists( FileName );

			_ID = flm::GetId();

			PiloteFichier_.Init( _ID, FileName, FileMode );
			PiloteFichier_.Persistent();
			PiloteFichier_.Manual();

			memory_merger<t, st>::plug( PiloteFichier_ );

			if ( Test )
			{
				memory_merger<t, st>::Init( mmg::rRecovery );
				PiloteFichier_.Mode( FileMode );
			}
			else
			{
				memory_merger<t, st>::Init( mmg::rCreation );
				PiloteFichier_.Mode( fil::mReadWrite );
			}

			memory_merger<t, st>::State( mmg::sImmortal );

			return ( Test ? mmg::rRecovery : mmg::rCreation );
		}
		//f The object is placed in 'ObjectMode', the file in 'FileMode'.
		void Mode( fil::mode__ Mode  )
		{
			PiloteFichier_.Mode( Mode );
		}
		void plug( mdr::E_MEMORY_DRIVER__ & )
		{
			ERRu();
		}
		void State( mmg::state )
		{
			ERRu();
		}
	};

	#define E_FILE_MERGER___( t )		file_merger___<t, t::s>
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
