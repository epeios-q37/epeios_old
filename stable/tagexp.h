/*
  Header for the 'tagexp' library by Claude L. Simon (simon@epeios.org)
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org) 

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

#ifndef TAGEXP__INC
#define TAGEXP__INC

#define TAGEXP_NAME		"TAGEXP"

#define	TAGEXP_VERSION	"$Revision$"	

#define TAGEXP_OWNER		"the Epeios project (http://www.epeios.org/)"

#include "ttr.h"

extern class ttr_tutor &TAGEXPTutor;

#if defined( XXX_DBG ) && !defined( TAGEXP_NODBG )
#define TAGEXP_DBG 
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

//D TAG EXPander. Expand tag with text or file content.

#include "err.h"
#include "flw.h"
#include "tagdtc.h"
#include "bitbch.h"
#include "ctn.h"
#include "xtf.h"
#include "fil.h"

namespace tagexp
{
	using namespace tagdtc;
	//e Nature of the tag reference.
	enum nature {
		//i Unknow type.
		nUnknow,
		//i A text.
		nText,
		//i A file.
		nFile,
		//i User-defined tag ; the expander gives the hand.
		nUserDefined
	};
	//e Status of the expander.
	enum status {
		//i No error
		sOK,
		//i Unable to handle given file.
		sBadFile,
		//i Unknow tag,
		sUnknowTag,
		/*i Value above this one is the value of a user-defined tag id,
		this value is never returned, but the value of the tag instead. */
		sUserDefined
	};
	//e Reserved tag ids.
	enum tag_id {
		//i Print to the output
		tPrint,
		//i Don't print to the output until encountering the 'Print' or 'Raw' tag.
		tSkip,
		//i Print the rest of the flow to the output as is, without expanding tags.
		tRaw,
		//i The rest of the flow is ignored.
		tEnd,
		// Amount of reserved tag,
		t_amount,
		//i value of the first unused tag.
		t_unused = t_amount
	};

	//t Value of a tag referrnce.
	typedef str::string_ value_;

	typedef str::string value;


	//f Return the label of the reserved tag of id 'Id'.
	const char *TagLabel( tagexp::tag_id Id );

	//c Natures of the tag references.
	class natures_ 
		: public bitbch::bibit_bunch_
	{
	public:
		struct s
		: public bitbch::bibit_bunch::s {};
		natures_( s &S )
		: bibit_bunch_( S )
		{}
		//f Return the value at 'Position'.
		nature operator()( tym::row__ Position ) const
		{
			return (tagexp::nature)bibit_bunch_::Read( Position );
		}
	};


	//c The tag references
	class references_
	{
	public:
		//o The value of the tag references
		ctn::E_XMCONTAINER_( value_ ) Values;
		//o The nature of the tag reference
		natures_ Natures;
		struct s
		{
			ctn::E_XMCONTAINER_( value_ )::s Values;
			natures_::s Natures;
		};
		references_( s &S )
		: Values( S.Values ),
		  Natures( S.Natures )
		{}
		void reset( bool P = true )
		{
			Values.reset( P );
			Natures.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			Values.plug( M );
			Natures.plug( M );
		}
		references_ operator =( const references_ &R )
		{
			Values = R.Values;
			Natures = R.Natures;

			return *this;
		}
		//f Initialization.
		void Init( void )
		{
			Values.Init();
			Natures.Init();
		}
		//f Return the position of a new tag reference of nature 'Nature', which should be completed later.
		id__ Create( void )
		{
			tym::row__ P = Values.Create();

			if ( Natures.Add( tagexp::nUnknow ) != P )
					ERRc();

			if ( ( P + tagexp::t_amount ) > TAGDTC_MAX_ID )
				ERRl();

			return (id__)P;
		}
		//f Write 'Value' and 'Nature' at 'Position'.
		void Write(
			const value_ &Value,
			nature Nature,
			id__ Position )
		{
			Natures.Write( Nature, Position );
			Values.Write( Value, Position );
		}
		//f Add 'Value' of nature 'Nature'. Return the position where added.
		id__ Add(
			const value_ &Value,
			nature Nature )
		{
			id__ P = Create();

			Write( Value, Nature, P );

			return P;
		}
	};



	//c The tag expander.
	class tag_expander_
	{
	public:
		//o The tag references.
		references_ References;
		//o The tag detector.
		tag_detector_ Detector;
		struct s
		{
			references_::s References;
			tag_detector_::s Detector;
		};
		tag_expander_( s &S )
		: References( S.References ),
		  Detector( S.Detector )
		{}
		void reset( bso__bool P = true )
		{
			References.reset( P );
			Detector.reset( P );
		}
		void plug( mmm::multimemory_ &M )
		{
			References.plug( M );
			Detector.plug( M );
		}
		tag_expander_ &operator =( const tag_expander_ &E )
		{
			References = E.References;
			Detector = E.Detector;

			return *this;
		}
		//f Initialization.
		void Init( void );
		//f Create tag 'Tag' which should be completed later. Return its id.
		id__ Create( const str::string_ &Tag )
		{
			id__ P;

			P = References.Create();

			Detector.Add( Tag, P );

			return P;
		}
		/*f Add tag 'Tag' with reference 'Reference' of nature 'Nature'.
		Return its id. */
		id__ Add(
			const str::string_ &Value,
			tagexp::nature Nature,
			const str::string_ &Tag )
		{
			id__ P;

			P = References.Add( Value, Nature );

			Detector.Add( Tag, P );

			return P;
		}
		//f Change to value 'Value' of nature 'Nature' the reference of the tag of id 'Id'.
		void Assign(
			const str::string_ &Reference,
			nature Nature,
			id__ Id )
		{
			References.Write( Reference, Nature, Id );
		}
		/*f Write 'IFlow' to 'OFlow' with expanding tags delimited by 'Delimiter'.
		If 'Action' = 'aSkip' then the contents isn't write to oflow until
		encoutering the 'tPrint' or the 'tRaw' tag. If retruned value > 'sUserDefined',
		then it's the value of an user-defined tag. */
		tagexp::status Expand(
			xtf::extended_text_iflow___ &IFlow,
			txf::text_oflow___ &OFlow,
			bso__char Demlimiter,
			str::string_ &File,
			tagexp::action Action = tagexp::aPrint,
			err::handle ErrHandle = err::hUsual ) const;
	};


	AUTO( tag_expander )
};

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
