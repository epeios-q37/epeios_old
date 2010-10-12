/*
	Header for the 'ndbsct' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 2004 Claude SIMON.

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

#ifndef NDBSCT__INC
#define NDBSCT__INC

#define NDBSCT_NAME		"NDBSCT"

#define	NDBSCT_VERSION	"$Revision$"

#define NDBSCT_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &NDBSCTTutor;

#if defined( XXX_DBG ) && !defined( NDBSCT_NODBG )
#define NDBSCT_DBG
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
//D New DataBase Static Content (will be moved on its own project in the future) 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "ndbbsc.h"
#include "lst.h"

namespace ndbsct {
	using ndbbsc::rrow__;
	using ndbbsc::datum_;
	using ndbbsc::datum;

	typedef tym::E_MEMORY_( ndbbsc::atom__ )	storage_;

	typedef lst::E_LISTt_( rrow__ ) _list_;

	class static_content_
	: public _list_
	{
	protected:
		virtual void LSTAllocate(
			epeios::size__ Amount,
			aem::mode__ Mode )
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
		: public _list_::s
		{
			storage_::s Storage;
			// Taille de l'�l�ment.
			epeios::size__ Size;
			time_t ModificationTimeStamp;
		} &S_;
		static_content_( s &S )
		: S_( S ),
		  _list_( S ),
		  Storage( S.Storage )
		{}
		void reset( bso::bool__ P = true )
		{
			_list_::reset( P );
			Storage.reset( P );

			S_.Size = 0;
			S_.ModificationTimeStamp = 0;
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			_list_::plug( MM );
			Storage.plug( MM );
		}
		static_content_ &operator =( const static_content_ &SC )
		{
			if ( ( Amount() != 0 ) || ( SC.Amount() != 0 ) ) {
				_list_::operator =( SC );
				Storage.Allocate( SC.Extent() * S_.Size );
				Storage.Store( SC.Storage, SC.Extent() * S_.Size );
				//S_.ModificationTimeStamp = SC.S_.ModificationTimeStamp;	// Ecras� par le '_Touch()' ci-dessous.

				_Touch();
			}

			S_.Size = SC.S_.Size;

			return *this;
		}
		void Init( epeios::size__ Size )
		{
			_list_::Init();
			Storage.Init();

			S_.Size = Size;
			S_.ModificationTimeStamp = 0;
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

			Storage.Store( Datum, S_.Size, *Row * S_.Size );

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
			ndbbsc::cache_ &Cache ) const
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
		E_RODISCLOSE_( time_t, ModificationTimeStamp );
	};

	E_AUTO( static_content )

	class static_content_atomized_file_manager___
	{
	private:
		static_content_ *_Content;
		str::string _BaseFileName;
		tym::memory_file_manager___ _FileManager;
		fil::mode__ _Mode;
		time_t _GetUnderlyingFilesLastModificationTime( void ) const
		{
			return _FileManager.TimeStamp();
		}
		void _SaveLocations( void ) const;
		void _ErasePhysically( void );
	public:
		void reset( bso::bool__ P = true )
		{
			_FileManager.ReleaseFile();	// Pour que les 'TimeStamp' des fichiers soient mis � jour.

			if ( P ) {
				if ( ( _Content != NULL ) && ( _BaseFileName.Amount() != 0 ) && ( _Content->ModificationTimeStamp() != 0 ) )
					_SaveLocations();
			}

			_FileManager.reset( P );
			_Mode = fil::m_Undefined;
			_BaseFileName.reset( P );
			_Content = NULL;
		}
		static_content_atomized_file_manager___( void )
		{
			reset( false );
		}
		~static_content_atomized_file_manager___( void )
		{
			reset();
		}
		void Init(
			static_content_ &Content,
			const str::string_ &BaseFileName,
			fil::mode__ Mode,
			flm::id__ ID );
		void WriteLocationsFile( void )	// Met � jour les fichiers.
		{
			_SaveLocations();
		}
		void CloseFiles( void )	// Pour lib�rer les 'file handlers'.
		{
			_FileManager.ReleaseFile();
		}
		void SwitchMode( fil::mode__ Mode )
		{
			if ( Mode != _Mode ) {
				_FileManager.Mode( Mode );

				_Mode = Mode;
			}
		}
		const str::string_ &BaseFileName( void ) const
		{
			return _BaseFileName;
		}
		uym::status__ ConnectToFiles( uym::purpose__ Purpose );
	};


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
