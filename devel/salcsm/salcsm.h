/*
  Header for the 'salcsm' library by Claude L. Simon (simon@epeios.org)
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

#ifndef SALCSM__INC
#define SALCSM__INC

#define SALCSM_NAME		"SALCSM"

#define	SALCSM_VERSION	"$Revision$"	

#define SALCSM_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &SALCSMTutor;

#if defined( XXX_DBG ) && !defined( SALCSM_NODBG )
#define SALCSM_DBG 
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

//D Stand ALone CSM. Same as 'csm' library, but monothreaded.

#include "err.h"
#include "flw.h"
#include "bso.h"
#include "bch.h"

#define SALCSM_BUFFER_SIZE 1000

namespace salcsm {
	//e Behavior.
	enum behavior {
		//i Unknow behavior.
		bUnknow,
		//i Continue.
		bContinue,
		//i Stop.
		bStop
	};

	//c User defined functions.
	class functions__
	{
	protected:
		//v Client process function.
		virtual behavior SALCSMCP(
			flw::ioflow___ &Client,
			flw::ioflow___ &Server,
			void *UP )
		{
			return bStop;
		}
		//v Client initialization function.
		virtual void *SALCSMCI(
			flw::ioflow___ &Client,
			flw::ioflow___ &Server )
		{
			return NULL;
		}
		//v Client ending functions.
		virtual void SALCSMCE( void *UP )
		{}
		//v Server process function.
		virtual behavior SALCSMSP(
			flw::ioflow___ &Client,
			void *UP )
		{
			return bStop;
		}
		//v Server initialization function.
		virtual void *SALCSMSI( void )
		{
			return NULL;
		}
		//v Server ending function.
		virtual void SALCSMSE( void *UP )
		{}
	public:
		behavior CP(
			flw::ioflow___ &Client,
			flw::ioflow___ &Server, void *UP )
		{
			return SALCSMCP( Client, Server, UP );
		}
		void *CI(
			flw::ioflow___ &Client,
			flw::ioflow___ &Server )
		{
			return SALCSMCI( Client, Server );
		}
		void CE( void *UP )
		{
			SALCSMCE( UP );
		}
		behavior SP(
			flw::ioflow___ &Client,
			void *UP )
		{
			return SALCSMSP( Client, UP );
		}
		void *SI( void )
		{
			return SALCSMSI();
		}
		void SE( void *UP )
		{
			SALCSMSE( UP );
		}
	};
	
	class tampon
	: public bch::E_BUNCH( flw::data__ )
	{
	private:
		// Position de la prochaine lecture.
		bch::row_t__ Lecture_;
		// Position de la prochaine �criture.
		bch::row_t__ Ecriture_;
	public:
		tampon( void )
		{
			Lecture_ = Ecriture_ = 0;
		}
		bso::bsize__ Ecrire(
			const flw::data__ *Tampon,
			bso::bsize__ Taille );
		bso::bsize__ Lire(
			bso::bsize__ Minimum,
			flw::data__ *Tampon,
			bso::bsize__ Desire );
	};

	class flot
	: public flw::ioflow___
	{
	private:
		tampon *Lecture_;
		tampon *Ecriture_;
		flw::data__ Cache_[SALCSM_BUFFER_SIZE];
	protected:
		virtual flw::amount__ FLWGet(
			flw::amount__ Minimum,
			flw::data__ *Buffer,
			flw::amount__ Wanted );
		virtual flw::amount__ FLWPut(
			const flw::data__ *Buffer,
			flw::amount__ Wanted,
			flw::amount__ Minimum,
			bool Synchronization );
	public:
		void reset( bool P = true )
		{
			iflow___::reset( P );
			Ecriture_ = Lecture_ = NULL;
		}		
		flot( void )
		{
			reset( false );
		}
		~flot( void )
		{
			reset( true );
		}
		void Init(
			tampon &Ecriture,
			tampon &Lecture )
		{
			reset();
		
			Ecriture_ = &Ecriture;
			Lecture_ = &Lecture;
			ioflow___::Init( Cache_, sizeof( Cache_ ) );
		}
	};

	class flot_serveur_actif
	: public flot
	{
	private:
		functions__ *Functions_;
		flot *Flot_;
		void *UP_;
	protected:
		virtual flw::amount__ FLWPut(
			const flw::data__ *Buffer,
			flw::amount__ Wanted,
			flw::amount__ Minimum,
			bool Synchronization );
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( Functions_ != NULL )
					Functions_->SE( UP_ );
			}
		
			flot::reset( P );
		
			Functions_ = NULL;
			Flot_ = NULL;
			UP_ = NULL;
		}
		flot_serveur_actif( void )
		{
			reset( false );
		}
		~flot_serveur_actif( void )
		{
			reset( true );
		}
		void Init(
			tampon &Ecriture,
			tampon &Lecture,
			functions__ *Functions,
			flot &Flot,
			void *UP )
		{
			reset();
		
			flot::Init( Ecriture, Lecture );
			Functions_ = Functions;
			Flot_ = &Flot;
			UP_ = UP;
		}
	};


	class flot_client_actif
	: public flot
	{
	private:
		functions__ *Functions_;
		flot *Client_;
		flot_serveur_actif *Serveur_;
		void *UP_;
	protected:
		virtual flw::amount__ FLWPut(
			const flw::data__ *Buffer,
			flw::amount__ Wanted,
			flw::amount__ Minimum,
			bool Synchronization );
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( Functions_ != NULL )
					Functions_->CE( UP_ );
			}
		
			flot::reset( P );
		
			Functions_ = NULL;
			Client_ = NULL;
			Serveur_ = NULL;
			UP_ = NULL;
		}
		flot_client_actif( void )
		{
			reset( false );
		}
		~flot_client_actif( void )
		{
			reset( true );
		}
		void Init(
			tampon &Ecriture,
			tampon &Lecture,
			functions__ *Functions,
			flot_serveur_actif &Serveur,
			flot &Client,
			void *PU )
		{
			reset();
		
			flot::Init( Ecriture, Lecture );
			Functions_ = Functions;
			Serveur_ = &Serveur;
			Client_ = &Client;
			UP_ = PU;
		}
	};
	
	//c To handle client/manager operations, as in the 'CSM' library, but without multitasking.
	class manager___
	: public functions__
	{
	private:
		tampon TamponServeurActif_;
		tampon TamponServeurPassif_;
		tampon TamponClientActif_;
		tampon TamponClientPassif_;

		flot ServeurPassif_, ClientPassif_;
		flot_serveur_actif ServeurActif_;
		flot_client_actif ClientActif_;

	public:
		//f Initialization.
		void Init( void )
		{
			// For standardization.
		}
		//f Process request written in returned flow.
		flw::ioflow___ &Process( void );
	};
};
	
/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
