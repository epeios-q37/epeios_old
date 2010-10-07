/*
	Header for the 'lstbch' library by Claude SIMON (http://zeusw.org/intl/contact.html)
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

#ifndef LSTBCH__INC
#define LSTBCH__INC

#define LSTBCH_NAME		"LSTBCH"

#define	LSTBCH_VERSION	"$Revision$"

#define LSTBCH_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &LSTBCHTutor;

#if defined( XXX_DBG ) && !defined( LSTBCH_NODBG )
#define LSTBCH_DBG
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
//D LiST BunCH 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "bch.h"
#include "lst.h"

namespace lstbch {

	using lst::list_;
	using bch::bunch_;

	//c Bunch associated to a list.
	template <typename type, typename row, typename row_t> class list_bunch_
	: public list_<row, row_t>,
	  public bunch_<type, row>
	{
	protected:
		virtual void LSTAllocate(
			epeios::size__ Size,
			aem::mode__ Mode )
		{
			bunch_<type, row>::Allocate( Size, Mode );
		}
	public:
		struct s
		: public list_<row, row_t>::s,
		  public bunch_<type, row>::s
		{};
		list_bunch_( s &S )
		: list_<row, row_t>( S ),
		  bunch_<type, row>( S )
		{}
		void reset( bso::bool__ P = true )
		{
			list_<row, row_t>::reset( P );
			bunch_<type, row>::reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			list_<row, row_t>::plug( MM );
			bunch_<type, row>::plug( MM );
		}
		list_bunch_ &operator =( const list_bunch_ &LB )
		{
			list_<row, row_t>::operator =( LB );
			bunch_<type, row>::operator =( LB );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			list_<row, row_t>::Init();
			bunch_<type, row>::Init();
		}
		E_NAVt( list_<E_COVER2(row,row_t)>::, row )
		//f Add 'Object' and return its row.
		row Add( const type &Object )
		{
			row Row = list_<row, row_t>::New();

			bunch_<type, row>::Store( Object, Row );

			return Row;
		}
		//f Delete entry 'Row'.
		void Delete( row Row )
		{
			list_<row, row_t>::Delete( Row );
		}
		//f Create new entry and return its row.
		row New( void )
		{
			return list_<row, row_t>::New();
		}
		//f Create new entry with row 'Row'.
		row New( row Row )
		{
			return list_<row, row_t>::New( Row );
		}
		// To avoid the use of herited 'Append' methods.
		void Append( void ) const
		{
			ERRu();
		}
		// To avoid the use of herited 'Remove' methods.
		void Remove( void ) const
		{
			ERRu();
		}
		bunch_<type, row> &Bunch( void )
		{
			return *this;
		}
		const bunch_<type, row> &Bunch( void ) const
		{
			return *this;
		}
		list_<row, row_t> &List( void )
		{
			return *this;
		}
		const list_<row, row_t> &List( void ) const
		{
			return *this;
		}
	};

	E_AUTO3( list_bunch )

#ifndef FLM__COMPILATION
	typedef bch::bunch_file_manager___ _bunch_file_manager___;

	class list_bunch_file_manager___
	: public _bunch_file_manager___
	{
	private:
		lst::store_ *_ListStore;
		tol::E_FPOINTER___( bso::char__ ) _ListFileName;
	public:
		void reset( bso::bool__ P = true )
		{
			_bunch_file_manager___::ReleaseFile();

			if ( P ) {
				if ( ( _ListStore != NULL )
					 && _bunch_file_manager___::IsPersistent()
					 && _bunch_file_manager___::Exists()
					 && ( !fil::FileExists( _ListFileName )
					      || ( _bunch_file_manager___::TimeStamp()
						       >= fil::GetFileLastModificationTime( _ListFileName ) ) ) )
					lst::WriteToFile( *_ListStore, _ListFileName, _bunch_file_manager___::TimeStamp() );
			}

			_bunch_file_manager___::reset( P );
			_ListFileName.reset( P );

			_ListStore = NULL;
		}
		list_bunch_file_manager___( void )
		{
			reset( false );
		}
		~list_bunch_file_manager___( void )
		{
			reset();
		}
		void Init(
			const char *BunchFileName,
			const char *ListFileName,
			fil::mode__ Mode,
			bso::bool__ Persistent,
			flm::id__ ID )
		{
			reset();

			_bunch_file_manager___::Init( BunchFileName, Mode, Persistent, ID );

			if ( ( _ListFileName = malloc( strlen( ListFileName ) + 1 ) ) == NULL )
				ERRa();

			strcpy( _ListFileName, ListFileName );
		}
		void Drop( void )
		{
			if ( ( _ListStore == NULL ) || ( _ListFileName == NULL ) )
				ERRu();

			_bunch_file_manager___::Drop();

			if ( fil::FileExists( _ListFileName ) )
				if ( remove( _ListFileName ) != 0 )
					ERRu();
		}
		const char *ListFileName( void ) const
		{
			return _ListFileName;
		}
		void Set( lst::store_ &Store )
		{
			if ( _ListStore != NULL )
				ERRu();

			_ListStore = &Store;
		}
		bso::bool__ Exists( void ) const
		{
			bso::bool__ Exists = _bunch_file_manager___::Exists();

			if ( Exists != fil::FileExists( _ListFileName ) )
				ERRc();

			return Exists;
		}
#ifdef CPE__C_VC
#	undef CreateFile
#endif
		bso::bool__ CreateFiles( err::handling__ ErrorHandling = err::h_Default )
		{
			bso::bool__ Success = _bunch_file_manager___::CreateFile( ErrorHandling );

			if ( !Success )
				return false;

			if ( fil::FileExists( _ListFileName ) )
				if ( ErrorHandling == err::hThrowException )
					ERRf();
				else
					return false;

			Success = fil::CreateFile( _ListFileName, ErrorHandling );

			return Success;
		}
		void CreateFile( int )	// Pour éviter un appel à la fonction correspondante de '_bunch_file_manager___'.
		{
			ERRu();
		}
	};


	template <typename list_bunch> bso::bool__ Connect(
		list_bunch &ListBunch,
		list_bunch_file_manager___ &FileManager )
	{
		bso::bool__ Exists = bch::Connect( ListBunch.Bunch(), FileManager );

		FileManager.Set( ListBunch.Locations );

		if ( Exists )
			if ( !lst::ReadFromFile( FileManager.ListFileName(), FileManager.FileSize() / ListBunch.GetItemSize(), FileManager.TimeStamp(), ListBunch.Locations ) )
				ERRu();

		return Exists;
	}
#endif

	#define E_LBUNCHtx_( type, row, row_t )		list_bunch_<type, row, row_t>
	#define E_LBUNCHtx( type, row, row_t )		list_bunch<type, row, row_t>

	#define E_LBUNCHt_( type, row )	E_LBUNCHtx_( type, row, epeios::row_t__)
	#define E_LBUNCHt( type, row )	E_LBUNCHtx( type, row, epeios::row_t__)

	#define  E_LBUNCH_( type )		E_LBUNCHt_( type, epeios::row__ )
	#define  E_LBUNCH( type )		E_LBUNCHt( type, epeios::row__ )
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
