/*
  Header for the 'xmldcm' library by Claude L. Simon (csimon@webmails.com)
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

#ifndef XMLDCM__INC
#define XMLDCM__INC

#define XMLDCM_NAME		"XMLDCM"

#define	XMLDCM_VERSION	"$Revision$"	

#define XMLDCM_OWNER		"the Epeios project (http://epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &XMLDCMTutor;

#if defined( XXX_DBG ) && !defined( XMLDCM_NODBG )
#define XMLDCM_DBG 
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
//D eXtended Markup Language DoCuMent
/* End addendum to automatic documentation generation part. */


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


	class xml_document_
	: public xml_core_
	{
	public:
		struct s
		: xml_core_::s
		{};
		xml_document_( s &S )
		: xml_core_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			xml_core_::reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			xml_core_::plug( M );
		}
		xml_document_ &operator =( const xml_document_ &XD )	
		{
			xml_core_::operator =( XD );
			
			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			xml_core_::Init();
		}
		//f Put into 'Value' value at row 'Row' (merged value).
		void GetValue(
			value_row__ Row,
			value_ &Value ) const;

	};
	
	AUTO( xml_document )
	
	//f Write 'XMLD', from root 'Root', in XML format into 'Flow'.
	void WriteXML(
		const xml_document_ &XMLD,
		txf::text_oflow___ &Flow,
		value_row__ Root = NONE );
		
		//c A XML document filler.
	class xml_document_filler__
	: public xml_core_filler__
	{
	private:
		void HandleValuesForPushTag_( void )
		{
			if ( ValueRow_ == NONE ) {
				ValueRow_ = XMLC_->GetRoot();
				XMLC_->Read( ValueRow_ ).Value.Init();
				XMLC_->Sync();
				ValueRow_ = XMLC_->CreateFirst( ValueRow_, NONE );
			} else {
				XMLC_->Read( ValueRow_ ).TagRow( TagRow_ );
				XMLC_->Sync();
				
				ValueRow_ = XMLC_->CreateFirst( ValueRow_, NONE );
			}
		}
	public:
		//f Initialize with 'XMLD'.
		void Init( xmldcm::xml_document_ &XMLD )
		{
			xml_core_filler__::Init( XMLD );
		}
		/*f 'TagRow' becomes current tag. Don't use this function if you are
		not absolutely sure of what you do. */
		void PushTag( tag_row__ TagRow )
		{
			xml_core_filler__::PushTag_( TagRow );

			HandleValuesForPushTag_();
		}
		/*f Tag with name 'Name' becomes current tag and is created if needed.
		Return the reference of this tag. */
		tag_row__ PushTag( const name_ &Name )
		{
			xml_core_filler__::PushTag_( Name );
			
			HandleValuesForPushTag_();

			return xml_core_filler__::TagRow_;
		}
		/*f Tag with name 'TagName' becomes current tag and is created if needed.
		Return the reference of this tag. */
		tag_row__ PushTag( const char *TagName )
		{
			return PushTag( name( TagName ) );
		}
		//f Parent tag becomes current tag, which reference is returned.
		tag_row__ PopTag( void )
		{
			xml_core_filler__::PopTag_();
			
			ValueRow_ = XMLC_->CreateNext( XMLC_->Parent( ValueRow_ ), NONE );

			return xml_core_filler__::TagRow_;
		}
		//f 'Value' becomes the value of the current value, which reference is returned.
		value_row__ PutValue( const value_ &Value  )
		{
			XMLC_->Read( ValueRow_ ).Value.Add( Value );
			XMLC_->Sync();
			
			return ValueRow_;
		}
		//f 'Value' becomes the value of the current value, which reference is returned.
		value_row__ PutValue( const char *Value  )
		{
			return PutValue( value( Value ) );
		}
		/*f Put 'Value' with tag 'TagRow'. Do not use this function if you are not
		absolutely sure of what you do. */
		void PutValue(
			const value_ &Value,
			const tag_row__ TagRow )
		{
			PushTag( TagRow );
			PutValue( Value );
			PopTag();
		}
		//f Put 'Value' with tag name 'TagName'. Return corresponding tag row.
		tag_row__ PutValue(
			const value_ &Value,
			const name_ &TagName )
		{
			tag_row__ TagRow = PushTag( TagName );
			PutValue( Value );
			PopTag();
			
			return TagRow;
		}
		//f Put 'Value' with tag name 'TagName'.
		void PutValue(
			const value_ &Value,
			const char *TagName )
		{
			PutValue( Value, name( TagName ) );
		}
		/*f Put attribute of tag row 'TagRow' and value 'Value'.
		Don't use this function if you are not sure of what you are doing. */
		void PutAttribute(
			const tag_row__ TagRow,
			const value_ &Value )
		{
		ERRProlog
			tagged_value TaggedValue;
		ERRBegin
			TaggedValue.Init( Value, TagRow );
			XMLC_->BecomeNext( TaggedValue, ValueRow_ );
		ERRErr
		ERREnd
		ERREpilog
		}
		//f Put attribute of name 'Name' and value 'Value'.
		void PutAttribute(
			const name_ &Name,
			const value_ &Value )
		{
			PutAttribute( GetOrCreateTag_( Name, tAttribute ), Value );
		}
		//f Put attribute of name 'Name' and value 'Value'.
		void PutAttribute(
			const char *Name,
			const value_ &Value )
		{
			PutAttribute( name( Name), Value );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
