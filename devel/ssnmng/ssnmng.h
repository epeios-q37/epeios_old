/*
  Header for the 'ssnmng' library by Claude L. Simon (simon@epeios.org)
  Copyright (C) 2000 Claude L. SIMON (simon@epeios.org) 

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

#ifndef SSNMNG__INC
#define SSNMNG__INC

#define SSNMNG_NAME		"SSNMNG"

#define	SSNMNG_VERSION	"$Revision$"	

#define SSNMNG_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &SSNMNGTutor;

#if defined( XXX_DBG ) && !defined( SSNMNG_NODBG )
#define SSNMNG_DBG 
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

#include "err.h"
#include "flw.h"
#include "lst.h"
#include "idxbtq.h"
#include "str.h"

#ifndef SSNMNG_SIZE
//d Size of the session id.
#define SSNMNG_SIZE	5
#endif

namespace ssnmng {

	using lst::E_LIST_;
	using que::E_MQUEUE_;

	//c A session id.
	class session_id__
	{
		char Raw_[SSNMNG_SIZE+1];
	public:
		session_id__( void )
		{
			Raw_[0] = 0;
		}
		//f Create a new session id.
		void New( void );
		//f Return the value of the session id.
		const char *Value( void ) const
		{
			return Raw_;
		}
		//f Dump.
		void Dump( void )
		{
			Raw_[0] = 0;
		}
		//f Return true if empty.
		bso__bool IsEmpty( void ) const
		{
			return Raw_[0] == 0;
		}
	};


	struct chrono__ {
		time_t Relative;
		time_t Absolute;
	};

	//c A session manager.
	class sessions_manager_
	: public E_LIST_,
	  public E_MQUEUE_
	{
	protected:
		virtual void LSTAllocate( tym::size__ Size )
		{
			Table.Allocate( Size );
			Index.Allocate( Size );
			Chronos.Allocate( Size );
			SSNMNGAllocate( Size );
			E_MQUEUE_::Allocate( Size );
			
		}
		//v Permit to make an allocation with a affected structure.
		virtual void SSNMNGAllocate( tym::size__ Size ){}
	public:
		struct s
		: public E_LIST_::s,
		public E_MQUEUE_::s
		{
			bch::E_BUNCH_( session_id__ )::s Table;
			idxbtq::tree_queue_index_::s Index;
			bch::E_BUNCH_( chrono__ )::s Chronos;
			bso__ushort Absolute;
			bso__ushort Relative;
		} &S_;
		//o The table of session ids.
		bch::E_BUNCH_( session_id__ ) Table;
		//o The index.
		idxbtq::tree_queue_index_ Index;
		//o The timing.
		bch::E_BUNCH_( chrono__ ) Chronos;
		sessions_manager_( s &S )
		: S_( S ),
		  E_LIST_( S ),
		  Table( S.Table ),
		  Index( S.Index ),
		  Chronos( S.Chronos ),
		  E_MQUEUE_( S )
		{}
		void reset( bool P = true )
		{
			S_.Absolute = S_.Relative = 0;

			E_LIST_::reset( P );
			Table.reset( P );
			Index.reset( P );
			Chronos.reset( P );
			E_MQUEUE_::reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			E_LIST_::plug( M );
			Table.plug( M );
			Index.plug( M );
			Chronos.plug( M );
			E_MQUEUE_::plug( M );
		}
		sessions_manager_ &operator =( const sessions_manager_ &S )
		{
			E_LIST_::operator =( S );
			Table = S.Table;
			Index = S.Index;
			Chronos = S.Chronos;
			E_MQUEUE_::operator =( S );

			S_.Relative = S.S_.Relative;
			S_.Absolute = S.S_.Absolute;

			return *this;
		}
		//f Initialization with 'Relative' and 'Absolute' amonut of second.
		void Init(
			bso__ushort Relative = BSO_USHORT_MAX,
			bso__ushort Absolute = BSO_USHORT_MAX )
		{
			E_LIST_::Init();
			Table.Init();
			Index.Init();
			Chronos.Init();
			E_MQUEUE_::Init();

			S_.Relative = Relative;
			S_.Absolute = Absolute;
		}
		//f Return the position of a mandatory new session.
		tym::row__ Open( void );
		//f Remove the session id at position 'Position'.
		void Close( tym::row__ Position )
		{
			Index.Remove( Position );
			E_LIST_::Remove( Position );
			E_MQUEUE_::Remove( Position );
		}
		//f Return the position of 'SessionID' or NONE if non-existent.
		tym::row__ Position( const session_id__ &SessionID ) const
		{
			return Position( SessionID.Value() );
		}
		//f Return the position of 'SessionID' or NONE if non-existent.
		tym::row__ Position( const char *SessionID ) const;
		//f Return the position of 'SessionID' or NONE if non-existent.
		tym::row__ Position( const str::string_ &SessionID ) const;
		//f Return the session id. corresponding to 'Position'.
		session_id__ SessionID( tym::row__ Position )
		{
			return Table( Position );
		}
		//f Touche the session corresponding at position 'P'.
		void Touch( tym::row__ P )
		{
			chrono__ C = Chronos.Read( P );

			if ( time( &C.Relative ) == -1 )
				ERRs();

			Chronos.Write( C, P );

	#ifdef SSNMNG_DBG
			if ( E_MQUEUE_::Amount() == 0 )
				ERRu();
	#endif

			if ( ( E_MQUEUE_::Amount() != 1 ) && ( E_MQUEUE_::Tail() != P ) ) {
				E_MQUEUE_::Remove( P );
				E_MQUEUE_::InsertItemAfterNode( P, E_MQUEUE_::Tail() );
			}
		}
		//f Return true if session corresponding to 'P' is valid.
		bso__bool IsValid( tym::row__ P ) const
		{
			chrono__ C = Chronos.Read( P );

			return ( difftime( time( NULL ), C.Absolute ) < S_.Absolute )
				   && ( difftime( time( NULL ), C.Relative ) < S_.Relative );
		}
		//f Balance the index. 
		void Balance( void )
		{
			Index.Balance();
		}
	};

	AUTO( sessions_manager )
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
