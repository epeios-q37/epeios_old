/*
	Header for the 'flm' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2000-2003  Claude SIMON (csimon@epeios.org).

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

#ifndef FLM__INC
#define FLM__INC

#define FLM_NAME		"FLM"

#define	FLM_VERSION	"$Revision$"

#define FLM_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &FLMTutor;

#if defined( XXX_DBG ) && !defined( FLM_NODBG )
#define FLM_DBG
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
//D FiLe Memory 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/
//D FiLe Memory.

#include "cpe.h"

#ifdef FLM_UNIX_LIKE
#	define FLM__UNIX_LIKE
#elif defined( FLM_MS_LOWLEVEL_IO )
#	define FLM__MS_LOWLEVEL_IO
#elif defined( FLM_IOSTREAM )
#	define FLM__IOSTREAM
#else
#	ifdef CPE__UNIX
#		define FLM__UNIX_LIKE
#	elif defined( CPE__MS )
#		define FLM__MS_LOWLEVEL_IO
#	else
#		define FLM__IOSTREAM
#	endif
#endif

#ifdef FLM__UNIX_LIKE
#	include <unistd.h>
#	include <fcntl.h>
#elif defined( FLM__MS_LOWLEVEL_IO )
#	include <io.h>
#	include <fcntl.h>
#	include <sys/stat.h>
#elif defined( FLM__IOSTREAM )
#	include <fstream.h>
#	include <iostream.h>
#include <ostream.h>
#endif

#include "err.h"
#include "flw.h"
#include "mdr.h"

namespace flm {
	using namespace mdr;

#ifdef FLM__UNIX_LIKE
	typedef int	capacite__;
#elif defined( FLM__MS_LOWLEVEL_IO ) 
	typedef long	capacite__;
#elif defined( FLM__IOSTREAM )
	typedef fstream::off_type capacite__;
#endif

	typedef capacite__ position__;
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

	class memoire_fichier_base_
	{
	#if defined( FLM__UNIX_LIKE ) || defined( FLM__MS_LOWLEVEL_IO )
		int FD_;
	#elif defined( FLM__IOSTREAM )
		// le stream servant de mémoire
		fstream Stream_;
	#endif
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
#ifdef FLM__UNIX_LIKE
					FD_ = open( Nom_, O_RDONLY );
#elif defined( FLM__MS_LOWLEVEL_IO )
					FD_ = _open( Nom_, _O_RDONLY | _O_BINARY );
#elif defined( FLM__IOSTREAM )
#	ifdef CPE__NO_IOS_EXTENSION
					Stream_.open( Nom_, ios::in | ios::binary );
#	else
					Stream_.open( Nom_, ios::in | ios::binary | ios::nocreate );
#	endif
#endif
				else
#ifdef FLM__UNIX_LIKE
					FD_ = open( Nom_, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |S_IROTH | S_IWOTH );
#elif defined( FLM__MS_LOWLEVEL_IO )
					FD_ = _open( Nom_, _O_RDWR | _O_CREAT | _O_BINARY, _S_IREAD | _S_IWRITE );
#elif defined( FLM__IOSTREAM )
					Stream_.open( Nom_, ios::in | ios::out | ios::binary );
#endif

#if defined( FLM__UNIX_LIKE ) || defined( FLM__MS_LOWLEVEL_IO )
				if ( FD_ == -1 )
#else
				if ( Stream_.fail() )
#endif
					ERRd();

				Temoin_.Ouvert = 1;
			}
		}
	protected:
		void Lire(
			position__ Position,
			unsigned int Nombre,
			void *Tampon )
		{
			Ouvrir_();
#if defined( FLM__UNIX_LIKE ) || defined( FLM__MS_LOWLEVEL_IO )
#	ifdef FLM__UNIX_LIKE
			ssize_t Amount;
#	elif defined( FLM__MS_LOWLEVEL_IO )
			int Amount;
#	else
#		error "Bad preprocessing directive"
#	endif

#	ifdef FLM__UNIX_LIKE
			if ( lseek( FD_, Position, SEEK_SET ) != Position )
#	elif defined( FLM__MS_LOWLEVEL_IO )
			if ( _lseek( FD_, Position, SEEK_SET ) != Position )
#	else
#		error "Bad preprocessing directive"
#	endif
				ERRd();
				
			while( Nombre > 0 ) {
				
#	ifdef FLM__UNIX_LIKE
				if ( Nombre <= SSIZE_MAX )
					Amount = read( FD_, Tampon, Nombre );
				else
					Amount = read( FD_, Tampon, SSIZE_MAX );
#	elif defined( FLM__MS_LOWLEVEL_IO )
				Amount = _read( FD_, Tampon, Nombre );
#	else
#		error "Bad preprocessing directive"
#	endif
					
				if ( Amount <= 0 )
					ERRd();
					
				Nombre -= Amount;
				Tampon = (char *)Tampon + Amount;
			}
#elif defined( FLM__IOSTREAM )
			if ( Stream_.seekg( Position ).fail() )
				ERRd();

			if ( Stream_.read( (char *)Tampon, Nombre ).fail() )
				ERRd();
#endif

			if ( !Temoin_.Manuel )
				Liberer();
		}
			/* lit à partir de 'Taille' et place dans 'Tampon' 'Taille' octets
			retourne le nombre d'octets effectivement lus */
		void Ecrire(
			const void *Tampon,
			unsigned int Nombre,
			position__ Position )
		{
			Ouvrir_();
#if defined( FLM__UNIX_LIKE ) || defined( FLM__MS_LOWLEVEL_IO )
#	ifdef FLM__UNIX_LIKE
			ssize_t Amount;
#	elif defined( FLM__MS_LOWLEVEL_IO )
			int Amount;
#	else
#		error "Bad preprocessing directive"
#	endif

#	ifdef FLM__UNIX_LIKE
			if ( lseek( FD_, Position, SEEK_SET ) != Position )
#	elif defined( FLM__MS_LOWLEVEL_IO )
			if ( _lseek( FD_, Position, SEEK_SET ) != Position )
#	else
#		error "Bad preprocessing directive"
#	endif
				ERRd();

			while( Nombre > 0 ) {
			
#	ifdef FLM__UNIX_LIKE
				if ( Nombre <= SSIZE_MAX )
					Amount = write( FD_, Tampon, Nombre );
				else
					Amount = write( FD_, Tampon, SSIZE_MAX );
#	elif defined( FLM__MS_LOWLEVEL_IO )
				Amount = _write( FD_, Tampon, Nombre );
#	else
#		error "Bad preprocessing directive"
#	endif
			
				if ( Amount < 0 )
					ERRd();
					
				Tampon = (char *)Tampon + Amount;
				Nombre -= Amount;
			}
#elif defined( FLM__IOSTREAM )
			if ( Stream_.seekp( Position ).fail() )
				ERRd();

			if ( Stream_.write( (char *)Tampon, Nombre ).fail() )
				ERRd();
#endif
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
				
#ifdef FLM__UNIX_LIKE
				if ( ( lseek( FD_, Capacite - (capacite__)1, SEEK_SET ) == -1 )
				      || ( write( FD_, &Capacite, 1 ) != 1 ) )
#elif defined( FLM__MS_LOWLEVEL_IO )
				if ( ( _lseek( FD_, Capacite - (capacite__)1, SEEK_SET ) == -1 )
				      || ( _write( FD_, &Capacite, 1 ) != 1 ) )
#elif defined( FLM__IOSTREAM )
				if ( Stream_.seekp( ( Capacite - (capacite__)1 ) ).fail()
					 || Stream_.put( (char)0 ).fail() )
#endif
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
		struct s
		{
			// Standardization.
		};
		memoire_fichier_base_( s &S )
		{
			// Standardization.
		}
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
		memoire_fichier_base_ &operator =( const memoire_fichier_base_ &MFB )
		{
			ERRu();	// The '=' operator has no sens for this object. It is defined only because it is neede by some macros.

			return *this;
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
#ifdef FLM__UNIX_LIKE
				close( FD_ );
#elif defined( FLM__MS_LOWLEVEL_IO )
				_close( FD_ );
#elif defined( FLM__IOSTREAM )
				Stream_.close();
#endif

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
	class file_memory_driver_
	: public E_MEMORY_DRIVER_,
	  public memoire_fichier_base_
	{
	protected:
		virtual void MDRRead(
			row__ Position,
			bsize__ Amount,
			data__ *Buffer )
		{
			memoire_fichier_base_::Lire( (position__)Position, Amount, Buffer );
		}
		// lit à partir de 'Position' et place dans 'Tampon' 'Nombre' octets
		virtual void MDRWrite(
			const data__ *Buffer,
			bsize__ Amount,
			row__ Position )
		{
			memoire_fichier_base_::Ecrire( Buffer, Amount, (position__)Position );
		}
		// écrit 'Nombre' octets à la position 'Position'
		virtual void MDRAllocate( size__ Size )
		{
			memoire_fichier_base_::Allouer( (capacite__)Size );
		}
		// alloue 'Taille' octets
	public:
		struct s
		: public E_MEMORY_DRIVER_::s,
		  memoire_fichier_base_::s
		{};
		file_memory_driver_( s &S )
		: E_MEMORY_DRIVER_( S ),
		  memoire_fichier_base_( S )
		{}
		void reset( bool P = true )
		{
			memoire_fichier_base_::reset( P );
			E_MEMORY_DRIVER_::reset( P );
		}
		//f Return the mode.
		mdr::mode Mode( void )
		{
			return memoire_fichier_base_::Mode();
		}
		//f 'Mode' becomes the mode.
		void Mode( mdr::mode Mode )
		{
			memoire_fichier_base_::Mode( Mode );
		}
		file_memory_driver_ &operator =( const file_memory_driver_ &FMD )
		{
			memoire_fichier_base_::operator =( FMD );
			E_MEMORY_DRIVER_::operator =( FMD );

			return *this;
		}
		//f Initialize using 'Filename' as file, open it in mode 'Mode'.
		void Init(
			const char *FileName = NULL,
			mdr::mode Mode = mdr::mReadWrite,
			flm::creation Creation = flm::cFirstUse )
		{
			memoire_fichier_base_::Init( FileName, Mode, Creation );
			E_MEMORY_DRIVER_::Init();
		}
	};

	AUTO( file_memory_driver )
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
