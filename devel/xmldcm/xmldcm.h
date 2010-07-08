/*
	Header for the 'xmldcm' library by Claude SIMON (csimon at zeusw dot org)
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

#ifndef XMLDCM__INC
#define XMLDCM__INC

#define XMLDCM_NAME		"XMLDCM"

#define	XMLDCM_VERSION	"$Revision$"

#define XMLDCM_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &XMLDCMTutor;

#if defined( XXX_DBG ) && !defined( XMLDCM_NODBG )
#define XMLDCM_DBG
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
//D XML DoCuMent 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

/* Addendum to the automatic documentation generation part. */
//D Hierarchically Organized Structures DoCuMent
/* End addendum to automatic documentation generation part. */

#error "Obsolete. Use 'XMLDOC' instead !"

#include "err.h"
#include "flw.h"
#include "xmlcor.h"

namespace xmldcm {
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
			core_file_memory_drivers::Init( Name, Directory, "xmldcmt", "xmldcmv" );
		}
	};


	class document_
	: public core_
	{
	public:
		struct s
		: core_::s
		{};
		document_( s &S )
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
		document_ &operator =( const document_ &XD )
		{
			core_::operator =( XD );

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			core_::Init();
		}
		//f Put into 'Value' value at row 'Row' (merged value).
		void GetValue(
			value_row__ Row,
			value_ &Value ) const;

	};

	E_AUTO( document )

	//f Write 'XMLD', from root 'Root', in XML format into 'Flow'.
	void WriteXML(
		const document_ &XMLD,
		txf::text_oflow___ &Flow,
		value_row__ Root = NONE );

		//c A XML document filler.
	class document_filler__
	: public core_filler__
	{
	private:
		void HandleValuesForPushTag_( void )
		{
			if ( ValueRow_ == NONE ) {
				ValueRow_ = XMLC_->GetRoot();
				XMLC_->Get( ValueRow_ ).Value.Init();
				XMLC_->Flush();
				ValueRow_ = XMLC_->CreateFirst( ValueRow_, NONE );
			} else {
				XMLC_->Get( ValueRow_ ).TagRow( TagRow_ );
				XMLC_->Flush();

				ValueRow_ = XMLC_->CreateFirst( ValueRow_, NONE );
			}
		}
	public:
		//f Initialize with 'XMLD'.
		void Init( xmldcm::document_ &XMLD )
		{
			core_filler__::Init( XMLD );
		}
		//f Return the XML document affected to this filler.
		document_ &GetDocument( void )
		{
			return *(document_ *)XMLC_;
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
		/*f Tag with name 'Name' becomes current tag and is created if needed.
		Return the reference of this tag. */
		tag_row__ PushTag( const char *Name )
		{
			return PushTag( name( Name ) );
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
		tag_row__ PopTag( void )
		{
			core_filler__::PopTag_();

			ValueRow_ = XMLC_->CreateNext( XMLC_->Parent( ValueRow_ ), NONE );

			return core_filler__::TagRow_;
		}
		//f 'Value' becomes the value of the current value, which reference is returned.
		value_row__ PutValue( const value_ &Value  )
		{
			XMLC_->Get( ValueRow_ ).Value.Append( Value );
			XMLC_->Flush();

			return ValueRow_;
		}
		//f 'Value' becomes the value of the current value, which reference is returned.
		value_row__ PutValue( const char *Value  )
		{
			return PutValue( value( Value ) );
		}
		/*f Put 'Value' with tag 'TagRow'. Do not use this function if you are not
		absolutely sure of what you do. Return its value row.*/
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

			tag_row__ TagRow = PushTag( TagName );
			ValueRow = PutValue( Value );
			PopTag();

			return ValueRow;
		}
		/*f If 'TagRow' != NONE, 'Value' is put associated with 'TagRow'.
		If 'TagRow' == NONE, 'Value' is associated with 'TagName', and the
		row corresponding to 'TagName' is put into 'TagRow'. Return its value row.*/
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
			const value_ &Value,
			const char *TagName )
		{
			return PutValue( Value, name( TagName ) );
		}
		/*f If 'TagRow' != NONE, 'Value' is put associated with 'TagRow'.
		If 'TagRow' == NONE, 'Value' is associated with 'TagName', and the
		row corresponding to 'TagName' is put into 'TagRow'.*/
		tag_row__ PutValue(
			const value_ &Value,
			const char *TagName,
			tag_row__ &TagRow )
		{
			PutValue( Value, name( TagName ), TagRow );
		}
		/*f Put attribute of tag row 'TagRow' and value 'Value'. REturn its value row.
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
			ValueRow = XMLC_->BecomeNext( TaggedValue, ValueRow_ );
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
		into 'TagRow'. Return its value row. */
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
		//f Push an entire document 'Document'..
		void Put( const document_ &Document );
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
