/*
  Header for the 'xmlval' library by Claude L. Simon (csimon@webmails.com)
  Copyright (C) 2000,2001 Claude L. SIMON (csimon@webmails.com) 

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

#ifndef XMLVAL__INC
#define XMLVAL__INC

#define XMLVAL_NAME		"XMLVAL"

#define	XMLVAL_VERSION	"$Revision$"	

#define XMLVAL_OWNER		"the Epeios project (http://epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &XMLVALTutor;

#if defined( XXX_DBG ) && !defined( XMLVAL_NODBG )
#define XMLVAL_DBG 
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude L. SIMON (csimon@webmails.com)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D eXtended Markup Langage VALue.
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "xmlbsc.h"
#include "xmltag.h"

namespace xmlval {

	using xmltag::tag_row__;

	//t A value.
	typedef str::string_ 	value_;
	typedef str::string		value;

	//t A reference to a value.
	TYPEDEF( epeios::row_t__, value_row__ );

	//c A tagged value.
	class tagged_value_
	{
	public:
		//o The value.
		value_ Value;
		struct s 
		{
			value_::s Value;
			tag_row__ TagRow;
		} &S_;
		tagged_value_( s &S )
		: S_( S ),
		  Value( S.Value )
		{}
		void reset( bool P = true )
		{
			Value.reset( P );
			S_.TagRow = NONE;
		}
		void plug( mmm::multimemory_ &M )
		{
			Value.plug( M );
		}
		void plug( mdr::E_MEMORY_DRIVER_ &MD )
		{
			Value.plug( MD );
		}
		tagged_value_ &operator =( const tagged_value_ &RV )
		{
			Value = RV.Value;
			S_.TagRow = RV.S_.TagRow;
			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Value.Init();
			S_.TagRow = NONE;
		}
		/*f Initialization with value 'Value' and tag row 'TagRow'. */
		void Init(
			const value_ &Value,
			tag_row__ TagRow )
		{
			this->Value.Init();
			this->Value = Value;
			S_.TagRow = TagRow;
		}
		//f 'TagRow' becomes the new tag row.
		void TagRow( tag_row__ TagRow )
		{
			S_.TagRow = TagRow;
		}
		//f Return the current tag row.
		tag_row__ TagRow( void ) const
		{
			return S_.TagRow;
		}
	};

	AUTO( tagged_value )

	//t The basic manager without templates.
	typedef xmlbsc::basic_< tagged_value_, value_row__ > basic_;


	/*c A tagged value manager. NOTA: the tag row affected to a value
	is NOT its tag, but the tag corresponding to its childs, hence the
	'Raw' prefix for some functions. */
	class tagged_values_
	: public basic_
	{
	private:
		void CreateTaggedValue_(
			tag_row__ TagRow,
			value_row__ ValueRow )
		{
			basic_::Read( ValueRow ).Init();
			basic_::Read( ValueRow ).TagRow( TagRow );
			basic_::Sync();
		}
	public:
		struct s
		: basic_::s
		{};
		tagged_values_( s &S )
		: basic_( S )
		{}
		void reset( bool P = true )
		{
			basic_::reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			basic_::plug( M );
		}
		tagged_values_ &operator =( const tagged_values_ &VM )
		{
			basic_::operator =( VM );

			return *this;
		}
		//f Initialization with tag root row 'RootTag'.
		value_row__ Init( tag_row__ RootTag );
		/*f Return position of a new empty item which is first to 'ValueRow'
		and tagged with 'TagRow'. */
		value_row__ CreateFirst(
			value_row__ ValueRow,
			tag_row__ TagRow )
		{
			value_row__ P = basic_::Create();

			basic_::BecomeFirst( P, ValueRow );

			CreateTaggedValue_( TagRow, P );

			return P;
		}
		/*f Return position of a new empty item which is last to 'ValueRow.
		and tagged with 'TagRow'. */
		value_row__ CreateLast(
			value_row__ ValueRow,
			tag_row__ TagRow )
		{
			value_row__ P = basic_::Create();

			basic_::BecomeLast( P, ValueRow );

			CreateTaggedValue_( TagRow, P );

			return P;
		}
		/*f Return position of a new empty item which is next to 'ValueRow'
		and tagged with 'TagRow'. */
		value_row__ CreateNext(
			value_row__ ValueRow,
			tag_row__ TagRow )
		{
			value_row__ P = basic_::Create();

			basic_::BecomeNext( P, ValueRow );

			CreateTaggedValue_( TagRow, P );

			return P;
		}
		/*f Return position of a new empty item which is Previous to 'ValueRow'
		and tagged with 'TagRow'. */
		value_row__ CreatePrevious(
			value_row__ ValueRow,
			tag_row__ TagRow )
		{
			value_row__ P = basic_::Create();

			basic_::BecomePrevious( P, ValueRow );

			CreateTaggedValue_( TagRow, P );

			return P;
		}
		//f Returns the first child of 'ValueRow', or 'NONE' if none.
		value_row__ First( value_row__ ValueRow ) const
		{
			return basic_::First( ValueRow );
		}
		//f Returns the value next of 'ValueRow', or 'NONE' if none.
		value_row__ Next( value_row__ ValueRow ) const
		{
			return basic_::Next( ValueRow );
		}
	};

	AUTO( tagged_values )


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
