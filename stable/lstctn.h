/*
	Header for the 'lstctn' library by Claude SIMON (http://zeusw.org/intl/contact.html)
	Copyright (C) 2004 Claude SIMON (http://zeusw.org/intl/contact.html).

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

#ifndef LSTCTN__INC
#define LSTCTN__INC

#define LSTCTN_NAME		"LSTCTN"

#define	LSTCTN_VERSION	"$Revision$"

#define LSTCTN_OWNER		"Claude SIMON (http://zeusw.org/intl/contact.html)"

#include "ttr.h"

extern class ttr_tutor &LSTCTNTutor;

#if defined( XXX_DBG ) && !defined( LSTCTN_NODBG )
#define LSTCTN_DBG
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
//D LiST ConTaiNer 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "lst.h"
#include "ctn.h"

namespace lstctn {
	using lst::list_;

	template <typename container, typename row, typename row_t> class list_container_
	: public list_<row, row_t>,
	  public container
	{
	protected:
		virtual void LSTAllocate(
			epeios::size__ Size,
			aem::mode__ Mode )
		{
			container::Allocate( Size, Mode );
		}
	public:
		struct s
		: public list_<row, row_t>::s,
		  public container::s
		{};
		list_container_( s &S )
		: list_<row, row_t>( S ), 
		  container( S )
		{}
		void reset( bso::bool__ P = true )
		{
			list_<row, row_t>::reset( P );
			container::reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			list_<row, row_t>::plug( MM );
			container::plug( MM );
		}
		list_container_ &operator =( const list_container_ &LC )
		{
			list_<row, row_t>::operator =( LC );
			container::operator =( LC );

			return *this;
		}
		E_NAVt( list_<E_COVER2(row,row_t)>::, row )
		E_NAVXt( list_<E_COVER2(row,row_t)>::, row )
		//f Initialization.
		void Init( void )
		{
			list_<row, row_t>::Init();
			container::Init();
		}
		//f RemoveDelete object at 'Row'.
		void Delete( row Row )
		{
			container::operator()( Row ).reset();
			container::Flush();
			list_<row, row_t>::Delete( Row );
		}
		//f Create new entry.
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
		// Pour �viter certains 'castage'.
		const container &Container( void ) const
		{
			return *this;
		}
		// Pour �viter certains 'castage'.
		container &Container( void )
		{
			return *this;
		}
	};

	E_AUTO3( list_container )

	template <typename container> E_TTYPEDEF___( ctn::container_file_manager___<container>, _container_file_manager___ );

	template <typename container> class list_container_file_manager___
	{
	private:
		_container_file_manager___<container> _ContainerFileManager;
		lst::list_file_manager___ _ListFileManager;
		time_t _ContainerTimeStamp( void ) const
		{
			return _ContainerFileManager.TimeStamp();
		}
		time_t _ListTimeStamp( void ) const
		{
			return _ListFileManager.TimeStamp();
		}
	public:
		void reset( bso::bool__ P = true )
		{
			_ContainerFileManager.ReleaseFiles();	// Sinon le 'Settle()'  ci'dessous ne fonctionne pas correctement.

			if ( P ) {
				Settle();	// Lanc� explicitement, car le 'reset(...)' de '_ListFileManager' ne peut lancer son propre 'Settle(...)'.
			}

			_ContainerFileManager.reset( P );
			_ListFileManager.reset( P );
		}
		list_container_file_manager___( void )
		{
			reset( false );
		}
		~list_container_file_manager___( void )
		{
			reset();
		}
		void Init(
			const char *ContainerStaticsFileName,
			const char *ContainerDynamicsFileName,
			const char *ContainerMultimemoryFileName,
			const char *ContainerMultimemoryFreeFragmentPositionsFileName,
			const char *ListFileName,
			fil::mode__ Mode,
			bso::bool__ Persistent,
			flm::id__ ID )
		{
			reset();

			_ContainerFileManager.Init( ContainerStaticsFileName, ContainerDynamicsFileName, ContainerMultimemoryFileName, ContainerMultimemoryFreeFragmentPositionsFileName, Mode, Persistent, ID );
			_ListFileManager.Init( ListFileName, Mode, Persistent );
		}
		fil::mode__ Mode( fil::mode__ Mode )
		{
			fil::mode__ ModeBuffer = _ContainerFileManager.Mode( Mode );

			if ( ModeBuffer != _ListFileManager.Mode( Mode ) )
				ERRc();

			return ModeBuffer;
		}
		fil::mode__ Mode( void ) const
		{
			fil::mode__ Mode = _ContainerFileManager.Mode();

			if ( Mode != _ListFileManager.Mode() )
				ERRc();

			return Mode;
		}
		uym::state__ State( void ) const
		{
			uym::state__ State = _ContainerFileManager.State();

			if ( !uym::IsError( State ) )
				if ( State != _ListFileManager.State() )
					State = uym::sInconsistent;

			return State;
		}
		uym::state__ Bind( void )
		{
			uym::state__ State = _ContainerFileManager.Bind();

			if ( uym::IsError( State ) )
				return State;

			if ( _ListFileManager.Bind( _ContainerFileManager.TimeStamp() ) != State )
				State = uym::sInconsistent;

			return State;
		}
		uym::state__ Settle( void )
		{
			uym::state__ State = _ContainerFileManager.Settle();

			if ( uym::IsError( State ) )
				return State;

			if ( _ContainerFileManager.IsPersistent() && _ContainerFileManager.Exists() )
				if ( _ListFileManager.Settle( _ContainerFileManager.TimeStamp() ) != State )
						State = uym::sInconsistent;

			return State;
		}
		void Drop( void )
		{
			 _ContainerFileManager.Drop();
			_ListFileManager.Drop();
		}
		const char *ListFileName( void ) const
		{
			return _ListFileName;
		}
		bso::bool__ CreateFiles( err::handling__ ErrorHandling = err::h_Default )
		{
			bso::bool__ Success = _ContainerFileManager.CreateFiles( ErrorHandling );

			if ( !Success )
				return false;

			if ( uym::IsError( Settle() ) ) {
				_ContainerFileManager.Drop();
				Success = false;
			}

			return Success;
		}
		void ReleaseFiles( void )
		{
			_ContainerFileManager.ReleaseFiles();
			_ListFileManager.ReleaseFiles();
		}
		uym::state__ Plug( container &ListContainer )
		{
			uym::state__ State = ctn::Plug( ListContainer.Container(), _ContainerFileManager );

			if ( uym::IsError( State ) ) {
				reset();
			} else {
				if ( lst::Plug( ListContainer, _ListFileManager, _ContainerFileManager.StaticsFileManager().FileSize() /  ListContainer.GetStaticsItemSize(), _ContainerFileManager.TimeStamp() ) != State ) {
					reset();
					State = uym::sInconsistent;
				}
			}

			return State;
		}
		time_t TimeStamp( void ) const
		{
			time_t ContainerTimeStamp = _ContainerTimeStamp();
			time_t ListTimeStamp = _ListTimeStamp();

			if ( ContainerTimeStamp > ListTimeStamp )
				return ContainerTimeStamp;
			else
				return ListTimeStamp;
		}
		bso::bool__ IsPersistent( void ) const
		{
			bso::bool__ Is = _ContainerFileManager.IsPersistent();

			if ( Is != _ListFileManager.IsPersistent() )
				ERRc();

			return Is;
		}
	};

	template <typename list_container> inline uym::state__ Plug(
		list_container &ListContainer,
		list_container_file_manager___<list_container> &FileManager )
	{
		return FileManager.Plug( ListContainer );
	}


	template <typename container, typename object, typename row, typename row_t> class list_xcontainer_
	: public list_container_<container, row, row_t>
	{
	private:
		row NewWithoutFlush_( void )
		{
			row Row = list_container_<container, row, row_t>::New();

			list_container_<container, row, row_t>::operator()( Row ).Init();

			return Row;
		}
		row NewWithoutFlush_( row Row )
		{
			Row = list_container_<container, row, row_t>::New( Row );

			list_container_<container, row, row_t>::operator()( Row ).Init();

			return Row;
		}
	public:
		struct s
		: public list_container_<container, row, row_t>::s
		{};
		list_xcontainer_( s &S )
		: list_container_<container, row, row_t>( S )
		{}
		row New( void )
		{
			row Row = NewWithoutFlush_();

			list_container_<container, row, row_t>::Flush();

			return Row;
		}
		row New( row Row )
		{
			Row = NewWithoutFlush_( Row );

			list_container_<container, row, row_t>::Flush();

			return Row;
		}
		row Add( const object &Object )
		{
			row Row = NewWithoutFlush_();

			list_container_<container, row, row_t>::operator()( Row ).operator =( Object );
			list_container_<container, row, row_t>::Flush();

			return Row;
		}
		// To avoid the use of herited 'Append' methods.
		void Append( void )
		{
			ERRu();
		}
	};

	E_AUTO4( list_xcontainer )

	#define E_LMCONTAINERtx_( type, row, row_t )	list_container_< ctn::E_MCONTAINERt_( type, row ), row, row_t >
	#define E_LMCONTAINERtx( type, row, row_t )		list_container< ctn::E_MCONTAINERt_( type, row ), row, row_t >

	#define E_LMCONTAINERt_( type, row )	E_LMCONTAINERtx_( type, row, epeios::row_t__ )
	#define E_LMCONTAINERt( type, row )		E_LMCONTAINERtx( type, row, epeios::row_t__ )

	#define E_LMCONTAINER_( type )			E_LMCONTAINERt_( type, epeios::row__ )
	#define E_LMCONTAINER( type )			E_LMCONTAINERt( type, epeios::row__ )

	#define E_LCONTAINERtx_( type, row, row_t )		list_container_< ctn::E_CONTAINERt_( type, row ), row, row_t >
	#define E_LCONTAINERtx( type, row, row_t )		list_container< ctn::E_CONTAINERt_( type, row ), row, row_t >

	#define E_LCONTAINERt_( type, row )		E_LCONTAINERtx_( type, row, epeios::row_t__ )
	#define E_LCONTAINERt( type, row )		E_LCONTAINERtx( type, row, epeios::row_t__ )

	#define E_LCONTAINER_( type )			E_LCONTAINERt_( type, epeios::row__ )
	#define E_LCONTAINER( type )			E_LCONTAINERt( type, epeios::row__ )

	#define E_LXMCONTAINERtx_( type, row, row_t )	list_xcontainer_< ctn::E_MCONTAINERt_( type, row ), type, row, row_t >
	#define E_LXMCONTAINERtx( type, row, row_t )	list_xcontainer< ctn::E_MCONTAINERt_( type, row ), type, row, row_t >

	#define E_LXMCONTAINERt_( type, row )	E_LXMCONTAINERtx_( type, row, epeios::row_t__ )
	#define E_LXMCONTAINERt( type, row )	E_LXMCONTAINERtx( type, row, epeios::row_t__ )

	#define E_LXMCONTAINER_( type )			E_LXMCONTAINERt_( type, epeios::row__ )
	#define E_LXMCONTAINER( type )			E_LXMCONTAINERt( type, epeios::row__ )

	#define E_LXCONTAINERtx_( type, row, row_t )	list_xcontainer_< ctn::E_CONTAINERt_( type, row ), type, row, row_t >
	#define E_LXCONTAINERtx( type, row, row_t )		list_xcontainer< ctn::E_CONTAINERt_( type, row ), type, row, row_t >

	#define E_LXCONTAINERt_( type, row )	E_LXCONTAINERtx_( type, row, epeios::row_t__ )
	#define E_LXCONTAINERt( type, row )		E_LXCONTAINERtx( type, row, epeios::row_t__ )

	#define E_LXCONTAINER_( type )			E_LXCONTAINERt_( type, epeios::row__ )
	#define E_LXCONTAINER( type )			E_LXCONTAINERt( type, epeios::row__ )

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
