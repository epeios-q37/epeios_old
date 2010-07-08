/*
	Header for the 'xmldbs' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef XMLDBS__INC
#define XMLDBS__INC

#define XMLDBS_NAME		"XMLDBS"

#define	XMLDBS_VERSION	"$Revision$"

#define XMLDBS_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XMLDBSTutor;

#if defined( XXX_DBG ) && !defined( XMLDBS_NODBG )
#define XMLDBS_DBG
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
//D XML DataBaSe 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D Hierarchically Organized Structures DataBaSe
/* End addendum to automatic documentation generation part. */

#error "Obsolete. Use 'XMLDOC' instead."

#include "err.h"
#include "flw.h"
#include "xmlcor.h"

namespace xmldbs {
	using namespace xmlcor;

	//c File drivers
	class file_memory_drivers
	: public core_file_memory_drivers
	{
	public:
		//f Initialization with name 'Name' and directory 'Directory'.
		void Init(
			const char *Name,
			const char *Directory = NULL )
		{
			core_file_memory_drivers::Init( Name, Directory, "xmldbsv", "xmldbst" );
		}
	};

	//c An XML database.
	class database_
	: public core_
	{
	public:
		struct s
		: public core_::s
		{};
		database_( s &S )
		: core_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			core_::reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			core_::plug( M );
		}
		database_ &operator =( const database_ &XD )
		{
			core_::operator =( XD );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			core_::Init();
		}
		/*f Return the position of the tagged value which have a value of
		'Value' and which is brother of tagged value at position 'ValueRow'. */
		value_row__ Next(
			value_row__ ValueRow,
			const value_ &Value ) const;
		/*f Return the position of tagged value which have a value of 'Value',
		tagged by 'TagRow' and which is brother next to tagged value
		at position 'Position'. */
		value_row__ Next(
			value_row__ ValueRow,
			tag_row__ TagRow,
			const value_ &Value ) const;
		/*f Return the position of the tagged value which have a value of
		'Value' and which is first child of tagged value at position 'ValueRow'. */
		value_row__ First(
			value_row__ ValueRow,
			const value_ &Value ) const;
		/*f Return the position of tagged value which have a value of 'Value',
		tagged by 'TagRow' and which is first child of tagged value
		at position 'Position'. */
		value_row__ First(
			value_row__ ValueRow,
			tag_row__ TagRow,
			const value_ &Value ) const;
		/*f Returns the position of the value tagged with 'TagRow'
		and which is brother of 'ValueRow', or 'NONE' if none. */
		value_row__ Next(
			value_row__ ValueRow,
			tag_row__ TagRow ) const
		{
			return core_::Next( ValueRow, TagRow );
		}
		//f Returns row of value next to value at 'Row'
		value_row__ Next( value_row__ Row ) const
		{
			return core_::Next( Row );
		}
		/*f Returns the position of the value tagged with 'TagRow'
		and which is the first child of 'ValueRow', or 'NONE' if none. */
		value_row__ First(
			value_row__ ValueRow,
			tag_row__ TagRow ) const
		{
			return core_::First( ValueRow, TagRow );
		}
		//f Returns row of first value of value at 'Row'
		value_row__ First( value_row__ Row ) const
		{
			return core_::First( Row );
		}
		//f Put into 'Value' value at row 'Row'.
		void GetValue(
			value_row__ Row,
			value_ &Value ) const
		{
			ctn::E_CMITEMt( tagged_value_, value_row__ ) TaggedValue;

			TaggedValue.Init( *this );

			Value = TaggedValue( Row ).Value;
		}
	};

	E_AUTO( database )

	//f Write 'XMLD' in XML format into 'Flow'.
	void WriteXML(
		const database_ &XMLD,
		txf::text_oflow__ &Flow );

	//c A XML database filler.
	class database_filler__
	: public core_filler__
	{
	private:
		void HandleValuesForPushTag_( void )
		{
			if ( ValueRow_ == NONE )
				ValueRow_ = XMLC_->GetRoot();
			else
				ValueRow_ = XMLC_->CreateLast( ValueRow_, TagRow_ );
		}
	public:
		//f Initialize with 'HDB'.
		void Init( xmldbs::database_ &XMLD )
		{
			core_filler__::Init( XMLD );
		}
		//f Return the XML database affected to this filler.
		database_ &GetDatabase( void )
		{
			return *(database_ *)XMLC_;
		}
		//f Jump directly to 'TagRow'.
		void JumpToTag( tag_row__ TagRow )
		{
			JumpToTag_( TagRow );
		}
		/*f 'TagRow' becomes current tag. Don't use this function if you are
		not absolutely sure of what you do. */
		void PushTag( tag_row__ TagRow )
		{
			core_filler__::PushTag_( TagRow );

			HandleValuesForPushTag_();
		}
		/*f Tag with name 'Name' becomes current tag and is created if needed.
		Return the reference of this tag. */
		tag_row__ PushTag( const name_ &Name )
		{
			core_filler__::PushTag_( Name );

			HandleValuesForPushTag_();

			return core_filler__::TagRow_;
		}
		/*f If 'TagRow'!= NONE, 'TagRow' becomes the current tag. If 'TagRow' == NONE,
		'Name' becomes current tag and is created if needed. The reference of this
		tag is returned and put in 'TagRow'. */
		tag_row__ PushTag(
			const name_ &Name,
			tag_row__ &TagRow )
		{
			if ( TagRow == NONE )
				TagRow = PushTag( Name );
			else
				PushTag( TagRow );

			return TagRow;
		}
		/*f Tag with name 'TagName' becomes current tag and is created if needed.
		Return the reference of this tag. */
		tag_row__ PushTag( const char *TagName )
		{
			return PushTag( name( TagName ) );
		}
		/*f If 'TagRow'!= NONE, 'TagRow' becomes the current tag. If 'TagRow' == NONE,
		'Name' becomes current tag and is created if needed. The reference of this
		tag is returned and put in 'TagRow'. */
		tag_row__ PushTag(
			const char *Name,
			tag_row__ &TagRow )
		{
			if ( TagRow == NONE )
				TagRow = PushTag( Name );
			else
				PushTag( TagRow );

			return TagRow;
		}
		//f Parent tag becomes current tag, which reference is returned.
		xmldbs::tag_row__ PopTag( void )
		{
			core_filler__::PopTag_();

			ValueRow_ = XMLC_->Parent( ValueRow_ );

			return core_filler__::TagRow_;
		}
		//f 'Value' becomes the value of the current value, which reference is returned.
		xmldbs::value_row__ PutValue( const xmldbs::value_ &Value  )
		{
			XMLC_->Get( ValueRow_ ).Value.Append( Value );
			XMLC_->Flush();

			return ValueRow_;
		}
		//f 'Value' becomes the value of the current value, which reference is returned.
		value_row__ PutValue( const char *Value  )
		{
			return PutValue( xmldbs::value( Value ) );
		}
		/*f Put 'Value' with tag 'TagRow'. Return corresponding value row.
		Do not use this function if you are not absolutely sure of what you do. */
		value_row__ PutValue(
			const value_ &Value,
			const tag_row__ TagRow )
		{
			value_row__ ValueRow;

			PushTag( TagRow );
			ValueRow = PutValue( Value );
			PopTag();

			return ValueRow;
		}
		//f Put 'Value' with tag name 'TagName'. Return its value row.
		value_row__ PutValue(
			const value_ &Value,
			const name_ &TagName )
		{
			value_row__ ValueRow;

			PushTag( TagName );
			ValueRow = PutValue( Value );
			PopTag();

			return ValueRow;
		}
		//f Put 'Value' with tag name 'TagName'. Return its value row.
		value_row__ PutValue(
			const value_ &Value,
			const char *TagName )
		{
			return PutValue( Value, name( TagName ) );
		}
		/*f If 'TagRow' != NONE, 'Value' is put associated with 'TagRow'.
		REturn its value row.
		If 'TagRow' == NONE, 'Value' is associated with 'TagName', and the
		row corresponding to 'TagName' is put into 'TagRow'.*/
		value_row__ PutValue(
			const value_ &Value,
			const name_ &TagName,
			tag_row__ &TagRow )
		{
			value_row__ ValueRow;

			if ( TagRow == NONE ) {
				TagRow = PushTag( TagName );
				ValueRow = PutValue( Value );
				PopTag();
			} else
				ValueRow = PutValue( Value, TagRow );

			return ValueRow;
		}
		//f Put 'Value' with tag name 'TagName'. Return its value row.
		value_row__ PutValue(
			const char *Value,
			const char *TagName )
		{
			return PutValue( value( Value ), name( TagName ) );
		}
		/*f IF 'TagRow' != NONE, 'Value' is put associated with 'TagRow'.
		Return corresponding value row.
		If 'TagRow' == NONE, 'Value' is associated with 'TagName', and the
		row corresponding to 'TagName' is put into 'TagRow'.*/
		value_row__ PutValue(
			const value_ &Value,
			const char *TagName,
			tag_row__ &TagRow )
		{
			return PutValue( Value, name( TagName ), TagRow );
		}
		/*f Put attribute of tag row 'TagRow' and value 'Value'.
		Return its value row.
		Don't use this function if you are not sure of what you are doing. */
		value_row__ PutAttribute(
			const tag_row__ TagRow,
			const value_ &Value )
		{
			value_row__ ValueRow;
		ERRProlog
			tagged_value TaggedValue;
		ERRBegin
			TaggedValue.Init( Value, TagRow );
			ValueRow = XMLC_->BecomeLast( TaggedValue, ValueRow_ );
		ERRErr
		ERREnd
		ERREpilog
			return ValueRow;
		}
		//f Put attribute of name 'Name' and value 'Value'. Return its value row.
		value_row__ PutAttribute(
			const name_ &Name,
			const value_ &Value )
		{
			return PutAttribute( GetOrCreateTag_( Name, tAttribute ), Value );
		}
		/*f Put attribute of name 'Name' if 'TagRow' == NONE or use
		'TagRow', and value 'Value'. The tag row of the attribute is put
		into 'TagRow'. Return its value row.*/
		value_row__ PutAttribute(
			const name_ &Name,
			const value_ &Value,
			tag_row__ &TagRow)
		{
			if ( TagRow != NONE )
				return PutAttribute( TagRow, Value );
			else
				return PutAttribute( TagRow = GetOrCreateTag_( Name, tAttribute ), Value );
		}
		//f Put attribute of name 'Name' and value 'Value'. Return its value row.
		value_row__ PutAttribute(
			const char *Name,
			const value_ &Value )
		{
			return PutAttribute( name( Name), Value );
		}
		//f Put attribute of name 'Name' and value 'Value'. Return its value row.
		value_row__ PutAttribute(
			const char *Name,
			const char *Value )
		{
			return PutAttribute( name( Name), value( Value ) );
		}
		/*f Put attribute of name 'Name' if 'TagRow' == NONE or use
		'TagRow', and value 'Value'. The tag row of the attribute is put
		into 'TagRow'. Return its value row.*/
		value_row__ PutAttribute(
			const char *Name,
			const value_ &Value,
			tag_row__ &TagRow)
		{
			if ( TagRow != NONE )
				return PutAttribute( TagRow, Value );
			else
				return PutAttribute( TagRow = GetOrCreateTag_( name( Name ), tAttribute ), Value );
		}
		/*f Put attribute of name 'Name' if 'TagRow' == NONE or use
		'TagRow', and value 'Value'. The tag row of the attribute is put
		into 'TagRow'. Return its value row.*/
		value_row__ PutAttribute(
			const char *Name,
			const char *Value,
			tag_row__ &TagRow)
		{
			return PutAttribute( Name, xmldbs::value( Value ), TagRow );
		}
		//f Push an entire XML database 'Database'.
		void Put( const database_ &Database );
	};


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
