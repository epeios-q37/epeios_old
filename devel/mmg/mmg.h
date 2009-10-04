/*
	Header for the 'mmg' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (http://zeusw.org/intl/contact.html).
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

#ifndef MMG__INC
#define MMG__INC

#define MMG_NAME		"MMG"

#define	MMG_VERSION	"$Revision$"

#define MMG_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &MMGTutor;

#if defined( XXX_DBG ) && !defined( MMG_NODBG )
#define MMG_DBG
#endif

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

	// Pilote mémoire à usage interne.
	template <size_t Taille> class pilote_memoire
	: public mdr::E_MEMORY_DRIVER
	{
	private:
		// Pointeur sur la partie statique de l'objet à sauver.
		uym::datum__ *Statique_;
		// Loi d'accés.
		mdr::mode Mode_;
	protected:
		virtual void MDRRecall(
			mdr::row__ Position,
			mdr::bsize__ Amount,
			mdr::datum__ *Buffer )
		// écrit 'Nombre' octets de 'Tampon' à la position 'Position'.
		{
			Memoire.Recall( Position + Taille, Amount, Buffer );
		}
		virtual void MDRStore(
			const mdr::datum__ *Buffer,
			mdr::bsize__ Amount,
			mdr::row__ Position )
		{
			StockerStatique();
			Memoire.Store( Buffer, Amount, Position + Taille );
		}
		// Alloue 'Capacite' octets.
		virtual void MDRAllocate( mdr::size__ Capacity )
		{
			Memoire.Allocate( Capacity + Taille );
		}
		/* Synchronisation de la mémoire; met à jour la mémoire en vidant,
		notamment, les caches */
	public:
		uym::untyped_memory_ Memoire;
		void reset(
			bool P = true,
			bool Ecriture = true )
		{
			if ( P )
			{
				if ( Ecriture )
					StockerStatique();
			}

			E_MEMORY_DRIVER::reset( P );
			Statique_ = NULL;
		}
		pilote_memoire( uym::untyped_memory_::s &S )
		: Memoire( S )
		{
			reset( false );
		}
		~pilote_memoire( void )
		{
			reset( true );
		}
		void Init(
			uym::datum__ *Statique,
			rule Regle,
			mdr::mode Mode = mdr::mReadWrite )
		{
			reset();

			Statique_ = Statique;
			E_MEMORY_DRIVER::Init();
			Memoire.Init();

			if ( Regle == mmg::rCreation )
			{
				if ( Mode == mdr::mReadOnly )
					ERRu();

				Memoire.Allocate( Taille );
				StockerStatique();
			}
			else if ( Regle == mmg::rRecovery )
				RecupererStatique();
			else
				ERRc();

			Mode_ = Mode;
		}
		void StockerStatique( void )
		{
			if ( Statique_ && ( Mode_== mdr::mReadWrite ) )
				Memoire.Store( Statique_, Taille, 0 );
		}
		void EcrireDansFlot( flw::oflow__ &Flot ) const
		{
			Flot.Put( Statique_, Taille );
		}
		void LireDeFlot( flw::iflow__ &Flot )
		{
			Flot.Get( Taille, Statique_ );
		}
		void RecupererStatique( void )
		{
			Memoire.Recall( 0, Taille, Statique_, true );
		}
		void Mode( mdr::mode Mode )
		{
			Mode_ = Mode;
		}
		mdr::mode Mode( void )
		{
			return Mode_;
		}
	};

	// Pour VC++, qui ne comprend rien sinon.
	template <class st> struct mmg_s
	: public st
	{
		mmm::multimemory_::s Memoire;
		// A vrai si le contenu survit à l'objet.
		st Object;
		uym::untyped_memory_::s Pilote_;
		mmg::state State;
	};

	/*c Merge all the memory of an object of type 't', including the statical
	part, in one memory. Use 'MMG_MEMORY_MERGER_( t )' rather then directly this class. */
	template <class t, class st> class memory_merger_
	{
	private:
		// l'éventuel pilote de la multimemoire
		mmm::multimemory_driver PiloteMultimemoire_;
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
		mmg::pilote_memoire < sizeof( mmg_s < st > ) > Pilote_;
	public:
		memory_merger_( s &S )
		: S_( S ),
		  Object( S.Object ),
		  Memoire( S.Memoire ),
		  Pilote_( S.Pilote_ )
		{}
		void reset( bool P = true )
		{
			if ( P )
			{
				if ( S_.State == mmg::sImmortal )
					Immortalize();

				if ( Pilote_.Memoire.Driver( true ) )	// Test si immortalisation ...
				{
					Object.reset( true );
					Memoire.reset( true );
					Pilote_.reset( true );
				}
			}

			S_.State = mmg::sMortal;

			Pilote_.reset( false );
			Memoire.reset( false );

			Object.reset( false );
		}
		//f Initialization with rule 'Rule' and mode 'Mode'.
		void Init(
			mmg::rule Rule,
			mdr::mode Mode = mdr::mReadWrite)
		{
			reset();

			Object.plug( Memoire );

			Pilote_.Init( (uym::datum__ *)&S_, Rule, Mode );

			Memoire.plug( Pilote_ );

			if ( Rule == mmg::rCreation )
				Memoire.Init();
		}
		//f Utilisation de 'Pilote' comme pilote mémoire.
		void plug( mdr::E_MEMORY_DRIVER_ &MD )
		{
			Pilote_.Memoire.plug( MD );
		}
		//f The memory is unpluged and can be used by an other object.
		void Immortalize( void )
		{
			Pilote_.StockerStatique();
			Pilote_.Memoire.plug( *(mdr::E_MEMORY_DRIVER_ *)NULL );
		}
		void plug( mmm::multimemory_ &M )
		{
			Memoire.Synchronize();

			PiloteMultimemoire_.plug( M );
			Pilote_.Memoire.plug( PiloteMultimemoire_ );
		}
		//f Write to 'OFLow' as raw data.
		void Write( flw::oflow__ &OFlow ) const
		{
			Memoire.Flush();
			Pilote_.EcrireDansFlot( OFlow );
			Memoire.write( OFlow );
		}
		//f Read from 'IFLow' as raw data.
		void Read( flw::iflow__ &IFlow )
		{
			Pilote_.LireDeFlot( IFlow );
			Memoire.read( IFlow );
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
		//f The mode of the object decames 'Mode'.
		void Mode( mdr::mode Mode )
		{
			Pilote_.Mode( Mode );
		}
		//f Return the mode of the object.
		mdr::mode Mode( void ) const
		{
			return Pilote_.Mode();
		}
		//f Return the object which is handled by.
		t &operator*( void )
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
	template <class t, class st> class file_memory_merger
	: public memory_merger<t, st>
	{
	private:
		flm::file_memory_driver PiloteFichier_;
	public:
		void reset( bool P = true )
		{
			memory_merger<t, st>::reset( P );

			PiloteFichier_.reset( P );
		}
		/*f Initialization with file named 'FileName'. The object is placed
		in 'ObjectMode', and the file in 'FileMode'. Return a value which depends
		on whether exists or not. If the file doesn't already exist,
		both 'ObjectMode' and 'FileMode' are forced for to 'mdr::mReadWrite'. */
		mmg::rule Init(
			const char *FileName,
			mdr::mode ObjectMode,
			mdr::mode FileMode )
		{	
			bso::bool__ Test;

			Test = tol::FileExists( FileName );

			PiloteFichier_.Init( FileName, FileMode );
			PiloteFichier_.Persistant();
			PiloteFichier_.Manuel();

			memory_merger<t, st>::plug( PiloteFichier_ );

			if ( Test )
			{
				memory_merger<t, st>::Init( mmg::rRecovery, ObjectMode );
				PiloteFichier_.Mode( FileMode );
			}
			else
			{
				memory_merger<t, st>::Init( mmg::rCreation, mdr::mReadWrite );
				PiloteFichier_.Mode( mdr::mReadWrite );
			}

			memory_merger<t, st>::State( mmg::sImmortal );

			return ( Test ? mmg::rRecovery : mmg::rCreation );
		}
		/*f Initialization with the file named 'FileName'. If the file is create,
		'Mode' is forced to 'mdr::mReadWrite'.  Return a value which depends
		on whether exists or not. If the file doesn't already exist, 'Mode' is forced to
		'mdr::mReadWrite'. */
		mmg::rule Init(
			const char *NomFichier,
			mdr::mode Mode = mdr::mReadOnly )
		{
			return Init( NomFichier, Mode, Mode );
		}
		//f The object is placed in 'ObjectMode', the file in 'FileMode'.
		void Mode(
			mdr::mode ObjectMode,
			mdr::mode FileMode  )
		{
			memory_merger<t, st>::Mode( ObjectMode );
			PiloteFichier_.Mode( FileMode );
		}
		//f Fila and object are placed in 'Mode' mode.
		void Mode( mdr::mode Mode )
		{
			this->Mode( Mode, Mode );
		}
		// To change the default destruction order, which cuases error.
		~file_memory_merger( void )
		{
			memory_merger<t, st>::reset();
			PiloteFichier_.reset();
		}
		void plug( mdr::E_MEMORY_DRIVER & )
		{
			ERRu();
		}
		void State( mmg::state )
		{
			ERRu();
		}
	};

	//m Same as 'MMG_MEMORY_MERGER( t )', but use a file as memory.
	#define E_FILE_MEMORY_MERGER( t )		file_memory_merger<t, t::s>
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
