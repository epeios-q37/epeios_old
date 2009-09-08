/*
	'mdbfld.h' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'emobda' software.

    'emobda' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'emobda' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'emobda'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

// eMoBDa FieLDs

#ifndef MBDFLD__INC
#define MBDFLD__INC

#include "mbdbsc.h"

#include "str.h"
#include "lstctn.h"

namespace mbdfld {

	using mbdbsc::field_row__;
	using mbdbsc::table_row__;

	using mbdbsc::field_type__;
	using mbdbsc::table_id__;
	using mbdbsc::field_id__;

	MBDBSC_USING_FT;

	class field_core_ {
	public:
		struct s {
			field_type__ Type;
			str::string_::s Name, Comment;
			field_id__ Id;
			table_row__ TableRow;
		} &S_;
		str::string_ Name, Comment;
		field_core_( s &S )
		: S_( S ),
		  Name( S.Name ),
		  Comment( S.Comment )
		{}
		void reset( bso::bool__ P = true )
		{
			Name.reset( P );
			Comment.reset( P );

			S_.Type = ft_Undefined;
			S_.Id = MBDBSC_UNDEFINED_FIELD_ID;
			S_.TableRow = NONE;
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Name.plug( MM );
			Comment.plug( MM );
		}
		field_core_ &operator =( const field_core_ &FC )
		{
			Name = FC.Name;
			Comment = FC.Comment;

			S_.Type = FC.S_.Type;
			S_.Id = FC.S_.Id;
			S_.TableRow = FC.S_.TableRow;

			return *this;
		}
		void Init( void )
		{
			reset();

			Name.Init();
			Comment.Init();
		}
		void Init(
			field_type__ Type,
			const str::string_ &Name,
			const str::string_ &Comment,
			field_id__ Id = MBDBSC_UNDEFINED_FIELD_ID,
			table_row__ TableRow = NONE )
		{
			Init();

			this->Name = Name;
			this->Comment = Comment;

			S_.Id = Id;
			S_.TableRow = TableRow;

		}
		void Set(
			const str::string_ &Name,
			const str::string_ &Comment )
		{
			if ( S_.Type == ft_Undefined )
				ERRu();

			if ( S_.TableRow == NONE )
				ERRu();

			if ( S_.Id == MBDBSC_UNDEFINED_FIELD_ID )
				ERRu();

			this->Name = Name;
			this->Comment = Comment;
		}
		E_RWDISCLOSE_( field_type__, Type );
		E_RWDISCLOSE_( field_id__, Id );
		E_RWDISCLOSE_( table_row__, TableRow );
	};

	typedef field_core_	field_;
	E_AUTO( field )

	typedef ctn::E_XCONTAINERt_( field_, field_row__ ) fields_;
	E_AUTO( fields );

	typedef field_ field_description_;
	E_AUTO( field_description );

	typedef ctn::E_XCONTAINER_( field_description_ ) field_descriptions_;
	E_AUTO( field_descriptions );
}

#endif
