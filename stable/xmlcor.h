/*
  Header for the 'xmlcor' library by Claude L. Simon (csimon@webmails.com)
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

#ifndef XMLCOR__INC
#define XMLCOR__INC

#define XMLCOR_NAME		"XMLCOR"

#define	XMLCOR_VERSION	"$Revision$"	

#define XMLCOR_OWNER		"the Epeios project (http://epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &XMLCORTutor;

#if defined( XXX_DBG ) && !defined( XMLCOR_NODBG )
#define XMLCOR_DBG 
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
//D eXtended Markup Language CORe
/* End addendum to automatic documentation generation part. */


#include "err.h"
#include "flw.h"
#include "xmltag.h"
#include "xmlval.h"

namespace xmlcor {
	using namespace xmltag;
	using namespace xmlval;
	
	//c Core file drivers 
	class core_file_memory_drivers
	{
	public:
		xmlbsc::file_memory_drivers Values, Tags;
		//f Initialization with name 'Name' and directory 'Directory'.
		void Init( 
			const char *Name,
			const char *Directory,
			const char *TagsSuffix,
			const char *ValuesSuffix )
		{
			Values.Init( Name, Directory, TagsSuffix );
			Tags.Init( Name, Directory, ValuesSuffix );
		}
	};



	//c The core for storing XML data.
	class xml_core_
	: public tagged_values_
	{
	public:
		tags_ Tags;
		struct s
		: tagged_values_::s
		{
			tags_::s Tags;
		};
		xml_core_( s &S )
		: tagged_values_( S ),
		  Tags( S.Tags )
		{}
		void reset( bso::bool__ P = true )
		{
			tagged_values_::reset( P );
			Tags.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			tagged_values_::plug( M );
			Tags.plug( M );
		}
		//f 'Drivers' aer used to store data.
		void Plug( core_file_memory_drivers &Drivers )
		{
			tagged_values_::Plug( Drivers.Values );
			Tags.Plug( Drivers.Tags );
		}
		xml_core_ &operator =( const xml_core_ &XC )	
		{
			tagged_values_::operator =( XC );
			Tags = XC.Tags;
			
			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			tagged_values_::Init( Tags.Init() );
			Tags.Init();
		}
		/*f Returns the position of the value tagged with 'TagRow'
		and which is brother of 'ValueRow', or 'NONE' if none. */
		value_row__ Next(
			value_row__ ValueRow,
			tag_row__ TagRow ) const;
		//f Returns row of value next to value at 'Row'
		value_row__ Next( value_row__ Row ) const
		{
			return tagged_values_::Next( Row );
		}
		/*f Returns the position of the value tagged with 'TagRow'
		and which is the first child of 'ValueRow', or 'NONE' if none. */
		value_row__ First(
			value_row__ ValueRow,
			tag_row__ TagRow ) const;
		//f Returns row of first value of value at 'Row'
		value_row__ First( value_row__ Row ) const
		{
			return tagged_values_::First( Row );
		}
	};

	//c A XML core filler.
	class xml_core_filler__
	{
	protected:
		// Corresponding core.
		xml_core_ *XMLC_;
		// Current tag.
		tag_row__ TagRow_;
		// Current value.
		value_row__ ValueRow_;
		tag_row__ GetOrCreateTag_(
			const name_ &Name,
			type Type )
		{
			tag_row__ TagRow;

			if ( ( TagRow = XMLC_->Tags.Position( Name, TagRow_, Type ) ) == NONE ) {
			ERRProlog
				tag Tag;
			ERRBegin
				Tag.Init( Name, Type );
				TagRow = XMLC_->Tags.BecomeLast( Tag, TagRow_ );
			ERRErr
			ERREnd
			ERREpilog
			}

			return TagRow;
		}
		void PushTag_( tag_row__ TagRow )
		{
#ifdef XMLCOR_DBG
			if ( TagRow_ == NONE ) {
				if ( TagRow != XMLC_->Tags.GetRoot() )
					ERRu();
			} else {
				if ( XMLC_->Tags.Parent( TagRow ) != TagRow_ )
					ERRu();
			}
#endif
			TagRow_ = TagRow;
		}
		void PushTag_( const name_ &Name )
		{
#ifdef XMLCOR_DBG
			if ( Name.Amount() == 0 )
				ERRu();
#endif
			if ( TagRow_ == NONE ) {
				PushTag_( XMLC_->Tags.GetRoot() );
				XMLC_->Tags( TagRow_ ).Name = Name;
				XMLC_->Tags.Sync();
			} else {
				PushTag_( GetOrCreateTag_( Name, tField ) );
			}
		}
		tag_row__ PopTag_( void )
		{
			return TagRow_ = XMLC_->Tags.Parent( TagRow_ );
		}

	public:
		void reset( bso::bool__ P = true )
		{
			XMLC_ = NULL;
			TagRow_ = NONE;
			ValueRow_ = NONE;
		}
		xml_core_filler__( void )
		{
			reset( false );
		}
		//f Initialize with 'XMLC'.
		void Init( xml_core_ &XMLC )
		{
			XMLC_ = &XMLC;
			TagRow_ = NONE;
			ValueRow_ = NONE;
		}
	};

	
	/*f Add 'Source' to 'Dest' using 'TagMap' as table of
	correspondances between 'Source' and 'Dest'.
	NOTA: The value of the root is ignored. */
	void Add(
		const tagged_values_ &Source,
		const tag_map_ &TagMap,
		tagged_values_ &Dest );
	
	/*f Add 'Source' into 'Dest' using 'TagMap' as table of
	correspondances between 'Source' and 'Dest'.
	NOTA: The value of the root is ignored. */
	inline void Add(
		const xml_core_ &Source,
		xml_core_ &Dest )
	{
	ERRProlog
		tag_map TagMap;
	ERRBegin
		TagMap.Init();
	
		Merge( Source.Tags, Dest.Tags, TagMap );
	
		Add( Source, TagMap, Dest );
	ERRErr
	ERREnd
	ERREpilog
	}
	


}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
