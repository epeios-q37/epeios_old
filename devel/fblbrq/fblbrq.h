/*
	Header for the 'fblbrq' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
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

#ifndef FBLBRQ__INC
#define FBLBRQ__INC

#define FBLBRQ_NAME		"FBLBRQ"

#define	FBLBRQ_VERSION	"$Revision$"

#define FBLBRQ_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &FBLBRQTutor;

#if defined( XXX_DBG ) && !defined( FBLBRQ_NODBG )
#define FBLBRQ_DBG
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
//D Frontend/Backend Layout Backend ReQuest 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"

# include "fbltyp.h"
# include "fblbrd.h"
# include "fblovl.h"

# ifdef CPE__VC
#  undef GetObject
# endif

# define FBLBRQ_INVALID_COMMAND		((unsigned long)BSO_USHORT_MAX)
# define FBLBRQ_COMMAND_MAX			( FBLBRQ_INVALID_COMMAND - 1UL )

# define FBLBRQ_INVALID_TYPE			((unsigned long)BSO_USHORT_MAX)
# define FBLBRQ_TYPE_MAX				( FBLBRQ_INVALID_TYPE - 1UL )

# define FBLBRQ_INVALID_OBJECT		((unsigned long)BSO_USHORT_MAX)
# define FBLBRQ_OBJECT_MAX			( FBLBRQ_INVALID_OBJECT - 1UL )

namespace fblbrq {
	using namespace fblcst;

# define FBLBRQ_M( name, type )\
	const fbltyp::type &name##In( void )\
	{\
		TestInput_( c##name );\
		return *(const fbltyp::type *)_Get( Position_, c##name );\
	}\
	fbltyp::type &name##Out( void )\
	{\
		TestOutput_( c##name );\
		return *(fbltyp::type *)_Put( Position_, c##name );\
	}\

	struct functions__
	{
	protected:
		virtual const void *FBLBRQGet(
			epeios::row__ Row,
			cast__ Cast ) = 0;
		virtual void *FBLBRQPut(
			epeios::row__ Row,
			cast__ Cast ) = 0;
		virtual void FBLBRQPop(
			flw::iflow__ &Flow,
			const casts_ &Casts ) = 0;
		virtual void FBLBRQPush(
			flw::oflow__ &Flow,
			const casts_ &Casts ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			// Standardisation.
		}
		functions__( void )
		{
			reset( false );
		}
		~functions__( void )
		{
			reset();
		}
		void Init( void )
		{
			// Standardisation.
		}
		const void *Get(
			epeios::row__ Row,
			cast__ Cast )
		{
			return FBLBRQGet( Row, Cast );
		}
		void *Put(
			epeios::row__ Row,
			cast__ Cast )
		{
			return FBLBRQPut( Row, Cast );
		}
		void Pop(
			flw::iflow__ &Flow,
			const casts_ &Casts )
		{
			FBLBRQPop( Flow, Casts );
		}
		void Push(
			flw::oflow__ &Flow,
			const casts_ &Casts )
		{
			 FBLBRQPush( Flow, Casts );
		}
	};

	//c A request manager.
	class request__
	{
	private:
		functions__ *_Functions;
		casts Casts_;
		// Position in the Description_;
		epeios::row__ Position_;
		// At true if all the answer be sent.
		bso::bool__ Closed_;
		// Cast buffer.
		cast Cast_;
		// At true if the request parsed.
		bso::bool__ Parsed_;
		// The input/output channel for the request.
		flw::ioflow__ *Channel_;
		functions__ &_F( void )
		{
# ifdef FBLBRQ_DBG
			if ( _Functions == NULL )
				ERRu();
# endif

			return *_Functions;
		}
		const void *_Get(
			epeios::row__ Row,
			cast__ Cast )
		{
			return _F().Get( Row, Cast );
		}
		const void *_Put(
			epeios::row__ Row,
			cast__ Cast )
		{
			return _F().Put( Row, Cast );
		}
		void _Pop(
			flw::iflow__ &Flow,
			const casts_ &Casts )
		{
				_F().Pop( Flow, Casts );
		}
		void _Push(
			flw::oflow__ &Flow,
			const casts_ &Casts )
		{
			_F().Push( Flow, Casts );
		}
		void Test_( cast Cast )
		{
			if ( Position_ == NONE )
				Position_ = Casts_.First();
			else
				Position_ = Casts_.Next( Position_ );

			if ( Casts_.Get( Position_ ) != Cast )
				ERRu();
		}
		void TestInput_( cast Cast )
		{
			Test_( Cast );
		}
		void TestOutput_( cast Cast )
		{
			if ( !Parsed_ ) {
				Test_( cEnd );
				
				Parsed_ = true;

				Channel_->Dismiss();
			}

			Test_( Cast );
		}
	public:
		void reset( bool P = true )
		{
			_Functions = NULL;
			Casts_.reset( P );

			Position_ = NONE;
			Closed_ = true;
			Cast_ = c_Unknow;
			Parsed_ = false;

			Channel_ = NULL;
		}
		request__( void )
		{
			reset( false );
		}
		virtual ~request__( void )
		{
			reset( true );
		}
		//f Initialization with 'Channel' to parse/answer the request.
		void Init(
			functions__ &Functions,
			flw::ioflow__ &Channel )
		{
			reset();
			_Functions = &Functions;
			Channel_ = &Channel;
			Closed_ = false;
			Casts_.Init();
		}
		//f Initialization with 'Channel' to parse/answer the request.
		void Prepare( const casts_ &Casts )
		{
			Position_ = NONE;
			Closed_ = false;
			Cast_ = c_Unknow;
			Parsed_ = false;
			Casts_ = Casts;

			_Pop( *Channel_, Casts );
		}
		FBLBRQ_M( Object, object__)
		FBLBRQ_M( Boolean, boolean__ )
		FBLBRQ_M( Booleans, booleans_ )
		FBLBRQ_M( SLong, slong__ )
		FBLBRQ_M( SLongs, slongs_ )
		FBLBRQ_M( ULong, ulong__ )
		FBLBRQ_M( ULongs, ulongs_ )
		FBLBRQ_M( Id8, id8__ )
		FBLBRQ_M( Ids8, ids8_ )
		FBLBRQ_M( XIds8, xids8_ )
		FBLBRQ_M( Id16, id16__ )
		FBLBRQ_M( Ids16, ids16 )
		FBLBRQ_M( XIds16, xids16_ )
		FBLBRQ_M( Id32, id32__ )
		FBLBRQ_M( Ids32, ids32_ )
		FBLBRQ_M( XIds32, xids32_ )
		FBLBRQ_M( Char, char__ )
		FBLBRQ_M( String, string_ )
		FBLBRQ_M( Strings, strings_ )
		FBLBRQ_M( XStrings, xstrings_ )
		FBLBRQ_M( Byte, byte__ )
		FBLBRQ_M( Binary, binary_ )
		FBLBRQ_M( Binaries, binaries_ )
		FBLBRQ_M( Items8, items8_ )
		FBLBRQ_M( Items16, items16_ )
		FBLBRQ_M( Items32, items32_ )
		FBLBRQ_M( XItems8, xitems8_ )
		FBLBRQ_M( XItems16, xitems16_ )
		FBLBRQ_M( XItems32, xitems32_ )
		FBLBRQ_M( CommandsDetails, commands_details_ )
		FBLBRQ_M( ObjectsReferences, objects_references_ )
		//f Tell that the request is complete (parsed and answered).
		void Complete( void )
		{
			if ( Closed_ )
				return;

			Channel_->Dismiss();
				
			if ( Casts_.Amount() != 0 ) /* If == 0, it means that the request was handled
								   by handling DIRECTLY the underlying flows. */
			{
				if ( !Parsed_ )
					Test_( cEnd );
					
				Channel_->Put( 0 );	// Empty explanation message.

				_Push( *Channel_, Casts_ );

				if ( Casts_.Last() != Position_  )
					ERRu();
			}

			fbltyp::PutId8( cEnd, *Channel_ );

			Closed_ = true;

			Parsed_ = false;

			Channel_->Commit();
		}
		//f Send a message that explain the reason of no treatment.
		void Report(
			fblovl::reply__ Reply,
			const char *Message )
		{
			flw::Put( (flw::datum__)Reply, *Channel_ );

			if ( Reply == fblovl::rOK )
					ERRu();

			if ( ( Message == NULL ) || ( Message[0] == 0 ) )
				ERRu();

			flw::PutString( Message, *Channel_ );
			Channel_->Put( 0 );

			Closed_ = true;

			Channel_->Commit();
		}
		void ReportError( const char *Message )	// 'User error'.
		{
			Report( fblovl::rError, Message );
		}
		void Notify( const char *Message )
		{
			Report( fblovl::rNotification, Message );
		}
		const casts_ &GetCasts( void ) const
		{
			return Casts_;
		}
		//f Return the channel used to handle the request as input flow.
		flw::iflow__ &Input( void )
		{
			return *Channel_;
		}
		//f Return the channel used to handle the request as ouput flow.
		flw::oflow__ &Output( void )
		{
			return *Channel_;
		}
	};

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
