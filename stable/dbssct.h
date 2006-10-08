/*
	Header for the 'dbssct' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

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

#ifndef DBSSCT__INC
#define DBSSCT__INC

#define DBSSCT_NAME		"DBSSCT"

#define	DBSSCT_VERSION	"$Revision$"

#define DBSSCT_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &DBSSCTTutor;

#if defined( XXX_DBG ) && !defined( DBSSCT_NODBG )
#define DBSSCT_DBG
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
//D DataBaSe Dynamic Static ConTent 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "tym.h"
#include "lst.h"
#include "dbsbsc.h"
#include "flm.h"

namespace dbssct {

	using dbsbsc::rrow__;
	using dbsbsc::_cache_;
	using dbsbsc::datum_;
	using dbsbsc::atom__;

	typedef tym::E_MEMORY_( atom__)	storage_;

	typedef lst::E_LISTt_( rrow__ ) _list_;

	typedef dbsbsc::delayed_initialization_	_delayed_initialization_;

	class static_content_
	: public _delayed_initialization_,
	  public _list_
	{
	protected:
		virtual void DBSBSCCompleteInitialization( void )
		{
			// Rien à faire.
		}
		virtual void LSTAllocate( epeios::size__ Amount )
		{
			Storage.Allocate( Amount * S_.Size );
		}
	public:
		storage_ Storage;
		struct s
		: public _list_::s,
		  public _delayed_initialization_::s
		{
			storage_::s Storage;
			_cache_::s Cache;
			// Taille de l'élément.
			size_t Size;
			time_t ModificationTimeStamp;
		} &S_;
		static_content_( s &S )
		: S_( S ),
		  _list_( S ),
		  _delayed_initialization_( S ),
		  Storage( S.Storage )
		{}
		void reset( bso::bool__ P = true )
		{
			_list_::reset( P );
			Storage.reset( P );

			S_.Size = 0;
			S_.ModificationTimeStamp = 0;
			_delayed_initialization_::reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			_list_::plug( MM );
			Storage.plug( MM );
		}
		static_content_ &operator =( const static_content_ &SC )
		{
			_list_::operator =( SC );
			Storage.Allocate( SC.Amount() * S_.Size );
			Storage.Store( SC.Storage, SC.Amount() * S_.Size );
			S_.Size = SC.S_.Size;
			S_.ModificationTimeStamp = SC.S_.ModificationTimeStamp;

			return *this;
		}
		void Init(
			epeios::size__ Size,
			bso::bool__ Partial = false )
		{
			_list_::Init();
			Storage.Init();

			S_.Size = Size;
			S_.ModificationTimeStamp = 0;

			_delayed_initialization_::Init( Partial );
		}
		rrow__ Store( const datum_ &Data )
		{
			rrow__ Row = _list_::New();

			if ( Data.Amount() != S_.Size )
				ERRu();

			Storage.Store( Data, 0, S_.Size );

			S_.ModificationTimeStamp = tol::Clock();

			return Row;
		}
		void Erase( rrow__ Row )
		{
			_list_::Delete( Row );

			S_.ModificationTimeStamp = tol::Clock();
		}
		void Store(
			const datum_ &Data,
			rrow__ Row )
		{
			if ( Data.Amount() != S_.Size )
				ERRu();

			Storage.Store( Data, 0, S_.Size );

			S_.ModificationTimeStamp = tol::Clock();
		}
		// Retourne 'true' si l'enregistrement existe, faux sinon.
		void Retrieve(
			rrow__ Row,
			datum_ &Datum ) const
		{
			Datum.Store( Storage, *Row * S_.Size, S_.Size );
		}
		// Retourne 'true' si l'enregistrement existe, faux sinon.
		void Retrieve(
			rrow__ Row,
			datum_ &Datum,
			_cache_ &Cache ) const
		{
			if ( ( &Cache == NULL ) || !Cache.Retrieve( Row, Datum ) ) {

				Retrieve( Row, Datum );

				if ( &Cache != NULL )
					Cache.Store( Datum, Row );
			}
		}
		// Reconstruction de la liste des items disponibles dans 'Entries' (sous-objet 'list_').
		void RebuildLocations( void )
		{
			ERRl();
		}
		// Reconstruit la liste des portions inoccupés dans 'Storage'.
		void RebuildAvailables( void )
		{
			ERRl();
		}
		E_RODISCLOSE_( time_t, ModificationTimeStamp );
	};

	E_AUTO( static_content )

	// Content stocké dans des fichiers.
	class file_static_content_
	: public static_content_
	{
	protected:
		virtual void DBSBSCCompleteInitialization( void )
		{
			_ConnectToFiles();
		}
	private:
		void _SaveLocations( void ) const;
		bso::bool__ _ConnectToFiles( void );
	public:
		str::string_ RootFileName;
		struct s
		: public static_content_::s
		{
			struct memory_driver__ {
				flm::E_FILE_MEMORY_DRIVER___
					Storage,
					List;
			} MemoryDriver;
			str::string_::s RootFileName;
			mdr::mode__ Mode;
		} &S_;
		file_static_content_( s &S )
		: S_( S ), 
		  static_content_( S ),
		  RootFileName( S.RootFileName )
		{}
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				if ( ( RootFileName.Amount() != 0 ) && ( ModificationTimeStamp() != 0 ) )
					_SaveLocations();
			}

			S_.MemoryDriver.Storage.reset( P );
			S_.MemoryDriver.List.reset( P );
			S_.Mode = mdr::m_Undefined;
			RootFileName.reset( P );
			static_content_::reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ & )
		{
			ERRu();	// Cette méthode n'a pas de sens dans ce contexte.
		}
		file_static_content_ &operator =( const file_static_content_ &FSC )
		{
			static_content_::operator =( FSC );

			return *this;
		}
		void Init(
			epeios::size__ Size,
			const str::string_ &RootFileName,
			mdr::mode__ Mode,
			bso::bool__ Partial )
		{
			reset();

			this->RootFileName.Init( RootFileName );
			S_.Mode = Mode;

			static_content_::Init( Size, Partial );
		}
		void WriteLocationsAndAvailablesFiles( void )	// Met à jour les fichiers.
		{
			_SaveLocations();
		}
		void CloseFiles( void )	// Pour libèrer les 'file handlers'.
		{
			S_.MemoryDriver.Storage.Liberer();
			S_.MemoryDriver.List.Liberer();
		}
		void SwitchMode( mdr::mode__ Mode )
		{
			if ( Mode != S_.Mode ) {
				S_.MemoryDriver.Storage.Mode( Mode );
				S_.MemoryDriver.List.Mode( Mode );

				S_.Mode = Mode;
			}
		}
	};

	E_AUTO( file_static_content )


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
