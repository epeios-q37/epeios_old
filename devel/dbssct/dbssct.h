/*
	Header for the 'dbssct' library by Claude SIMON (csimon@epeios.org)
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

	typedef dbsbsc::file_features_	_file_features_;

	class static_content_
	: public _file_features_,
	  public _list_
	{
	protected:
		virtual void DBSBSCCompleteInitialization( void )
		{
			// Rien à faire.
		}
		virtual void DBSBSCDrop(  void )
		{
			reset();
		}
		virtual void LSTAllocate( epeios::size__ Amount )
		{
			Storage.Allocate( Amount * S_.Size );
		}
		void _Touch( void )
		{
			S_.ModificationTimeStamp = tol::Clock( false );
		}
	public:
		storage_ Storage;
		struct s
		: public _list_::s,
		  public _file_features_::s
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
		  _file_features_( S ),
		  Storage( S.Storage )
		{}
		void reset( bso::bool__ P = true )
		{
			_list_::reset( P );
			Storage.reset( P );

			S_.Size = 0;
			S_.ModificationTimeStamp = 0;
			_file_features_::reset( P );
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

			_file_features_::Init( Partial );
		}
		void Erase( rrow__ Row )
		{
			_list_::Delete( Row );

			_Touch();
		}
		void Store(
			const datum_ &Datum,
			rrow__ Row )
		{
			if ( Datum.Amount() != S_.Size )
				ERRu();

			Storage.Store( Datum, S_.Size, 0, *Row * S_.Size );

			_Touch();
		}
		rrow__ Store( const datum_ &Datum )
		{
			rrow__ Row = _list_::New();

			Store( Datum, Row );

			return Row;
		}
		// Retourne 'true' si l'enregistrement existe, faux sinon.
		void Retrieve(
			rrow__ Row,
			datum_ &Datum ) const
		{
			Datum.Append( Storage, S_.Size, *Row * S_.Size );
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
		void Drop( void )	// Efface tout, y compris la table sous-jacente.
		{
			_file_features_::Drop();
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
		virtual void DBSBSCDrop( void )
		{
			static_content_::DBSBSCDrop();
			_Drop();
		}
	private:
		time_t _GetUnderlyingFilesLastModificationTime( void ) const
		{
			return tol::GetFileLastModificationTime( S_.FileManager.FileName() );
		}
		void _SaveLocations( void ) const;
		bso::bool__ _ConnectToFiles( void );
		void _Drop( void );
	public:
		str::string_ RootFileName;
		struct s
		: public static_content_::s
		{
			tym::memory_file_manager___ FileManager;
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

			static_content_::reset( P );
			S_.FileManager.reset( P );
			S_.Mode = mdr::m_Undefined;
			RootFileName.reset( P );
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
			bso::bool__ Partial );
		void WriteLocationsFile( void )	// Met à jour les fichiers.
		{
			_SaveLocations();
		}
		void CloseFiles( void )	// Pour libèrer les 'file handlers'.
		{
			S_.FileManager.ReleaseFile();
		}
		void SwitchMode( mdr::mode__ Mode )
		{
			if ( Mode != S_.Mode ) {
				S_.FileManager.Mode( Mode );

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
