/*
  Header for the 'flm' library by Claude L. Simon (simon@epeios.org)
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

#ifndef FLM__INC
#define FLM__INC

#define FLM_NAME		"FLM"

#define	FLM_VERSION	"$Revision$"	

#define FLM_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &FLMTutor;

#if defined( XXX_DBG ) && !defined( FLM_NODBG )
#define FLM_DBG 
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
//D FiLe Memory.

#include <fstream.h>
#include <iostream.h>

#include "err.h"
#include "flw.h"
#include "mdr.h"

namespace flm {
	using namespace mdr;

	typedef streampos capacite__;
	// type définissant une capacite mémoire

	typedef capacite__ position_;
	// type définissant une position dans la mémoire

	/*******************************************************************/
	/* CLASSE DE BASE DE GESTION D'UNE MEMOIRE STOCKEE DANS UN FICHIER */
	/*******************************************************************/

	struct flm
	{
		enum creation
		{
			cNow,
			cFirstUse
		};
	};

	class memoire_fichier_base
	{
		// le stream servant de mémoire
		fstream Stream_;
		// nom du fichier
		char *Nom_;
		// taille du fichier
		capacite__ TailleFichier_;
		// différents témoins
		struct {
			int
				// signale si le Stream est ouvert ou non
				Ouvert			:1,
				// signale que la fermeture du fichier se fait manuellement
				Manuel  		:1,
				// Signale que le fichier ne doit pas être détruit à la destruction de l'objet
				Persistant		:1,
				// Signale que le nom du fichier a été crée de manière interne
				Interne			:1;
				// Loi d'accés à la mémoire.
				mdr::mode Mode;
		} Temoin_;
	// Fonctions
		void Ouvrir_( void )
		{
			if ( !Temoin_.Ouvert )
			{
				if ( Temoin_.Mode == mdr::mReadOnly )
					Stream_.open( Nom_, ios::in | ios::binary );
				else
					Stream_.open( Nom_, ios::in | ios::out | ios::binary );

				if ( Stream_.fail() )
					ERRd();

				Temoin_.Ouvert = 1;
			}
		}
	protected:
		void Lire(
			streampos Position,
			int Nombre,
			void *Tampon )
		{
			Ouvrir_();

			if ( Stream_.seekg( Position ).fail() )
				ERRd();

			if ( Stream_.read( (char *)Tampon, Nombre ).fail() )
				ERRd();

			if ( !Temoin_.Manuel )
				Liberer();
		}
			/* lit à partir de 'Taille' et place dans 'Tampon' 'Taille' octets
			retourne le nombre d'octets effectivement lus */
		void Ecrire(
			const void *Tampon,
			int Nombre,
			streampos Position )
		{
			Ouvrir_();

			if ( Stream_.seekp( Position ).fail() )
				ERRd();

			if ( Stream_.write( (char *)Tampon, Nombre ).fail() )
				ERRd();

			if ( !Temoin_.Manuel )
				Liberer();
		}
			/* écrit 'Taille' octet à la position 'Position' dans 'Tampon';
			agrandit la mémoire si nécessaire */
		void Allouer( capacite__ Capacite )
		{
			if ( Capacite > TailleFichier_ )
			{
				Ouvrir_();

				if ( Stream_.seekp( ( Capacite - (capacite__)1 ) ).fail() || Stream_.put( (char)0 ).fail() )
				{
					if ( !Temoin_.Manuel )
						Liberer();

					ERRm();
				}
				else
					if ( !Temoin_.Manuel )
						Liberer();

				TailleFichier_ = Capacite;
			}
		}
		// alloue 'Capacite' octets
	public:
		void reset( bool P  = true )
		{
			if ( P )
			{
				Liberer();

				if ( Nom_ )
				{
					if ( !Temoin_.Persistant )
						remove( Nom_ );

					free( Nom_ );
				}
			}

			Nom_ = NULL;
			Temoin_.Ouvert = false;
			Temoin_.Manuel = true;
			Temoin_.Persistant = false;
			Temoin_.Interne = false;
			Temoin_.Mode = mdr::mReadOnly;
			TailleFichier_ = 0;
		}
		void plug( class mmm::multimemory_ & )
		{
			// Standardization.
		}
		memoire_fichier_base( void )
		{
			reset( false );
		}
		virtual ~memoire_fichier_base( void )
		{
			reset( true );
		}
		void Init(
			const char *NomFichier = NULL,
			mdr::mode Mode = mdr::mReadWrite,
			flm::creation Creation = flm::cFirstUse )
		{
			if ( NomFichier )
			{
				reset();

				if ( ( Nom_ = (char *)malloc( strlen( NomFichier ) + 1 ) ) == NULL )
					ERRa();

				strcpy( Nom_, NomFichier );

				Temoin_.Interne = false;
			}
			else if ( !Temoin_.Interne )
			{
				reset();

				if ( ( Nom_ = (char *)malloc( L_tmpnam ) ) == NULL )
					ERRa();

				tmpnam( Nom_ );

				Temoin_.Interne = true;
			}

			Temoin_.Mode = Mode;

			if ( Creation == flm::cNow  )
				if ( Mode == mdr::mReadWrite )
					Ouvrir_();
				else
					ERRu();
			else if ( Creation != flm::cFirstUse )
				ERRu();
		}
			// initialise l'objet avec le nom 'NomFichier'; si NULL, création d'un nom
		void Liberer( void )
		{
			if ( Temoin_.Ouvert )
				Stream_.close();

			Temoin_.Ouvert = 0;
		}
			// libère le File Descriptor
		void Manuel( void )
		{
			Temoin_.Manuel = 1;
		}
			// indique que la fermeture du fichier se fait manuellement
		void Automatique( void )
		{
			Temoin_.Manuel = 0;
		}
			// indique que la fermeture du fichier se fait automatiquement
		void Persistant( void )
		{
			Temoin_.Persistant = true;
		}
		// bascule en mode d'acces 'Acces'.
		void Mode( mdr::mode Mode )
		{
			if ( Temoin_.Mode != Mode )
			{
				Temoin_.Mode = Mode;
				Liberer();
			}
		}
		// Retourne le mode d'accés.
		mdr::mode Mode( void )
		{
			return Temoin_.Mode;
		}
	};

	//c The standard memory driver which handle a file as memory.
	class file_memory_driver
	: public E_MEMORY_DRIVER,
	  public memoire_fichier_base
	{
	protected:
		virtual void MDRRead(
			row__ Position,
			bsize__ Amount,
			data__ *Buffer )
		{
			memoire_fichier_base::Lire( Position, Amount, Buffer );
		}
		// lit à partir de 'Position' et place dans 'Tampon' 'Nombre' octets
		virtual void MDRWrite(
			const data__ *Buffer,
			bsize__ Amount,
			row__ Position )
		{
			memoire_fichier_base::Ecrire( Buffer, Amount, Position );
		}
		// écrit 'Nombre' octets à la position 'Position'
		virtual void MDRAllocate( size__ Size )
		{
			memoire_fichier_base::Allouer( Size );
		}
		// alloue 'Taille' octets
	public:
		file_memory_driver( void )
		{
			reset( false );
		}
		virtual ~file_memory_driver( void )
		{
			reset( true );
		}
		void plug( class mmm::multimemory_ &M )
		{
			memoire_fichier_base::plug( M );
			E_MEMORY_DRIVER::plug( M );
		}
		void reset( bool P = true )
		{
			memoire_fichier_base::reset( P );
			E_MEMORY_DRIVER::reset( P );
		}
		//f Return the mode.
		mdr::mode Mode( void )
		{
			return memoire_fichier_base::Mode();
		}
		//f 'Mode' becomes the mode.
		void Mode( mdr::mode Mode )
		{
			memoire_fichier_base::Mode( Mode );
		}
		//f Initialize using 'Filename' as file, open it in mode 'Mode'.
		void Init(
			const char *FileName = NULL,
			mdr::mode Mode = mdr::mReadWrite,
			flm::creation Creation = flm::cFirstUse )
		{
			memoire_fichier_base::Init( FileName, Mode, Creation );
			E_MEMORY_DRIVER::Init();
		}
	};

	typedef file_memory_driver file_memory_driver_;
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
