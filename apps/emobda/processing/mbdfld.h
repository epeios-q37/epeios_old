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

	using mbdbsc::table_id__;
	using mbdbsc::field_id__;

	class field_ {
	public:
		struct s {
			str::string_::s Name;
			table_id__ TableId;
			field_id__ FieldId;
		} &S_;
		str::string_ Name;
		field_( s &S )
		: S_( S ),
		  Name( S.Name )
		{}
		void reset( bso::bool__ P = true )
		{
			Name.reset( P );
			S_.TableId = MBDBSC_UNDEFINED_TABLE_ID;
			S_.FieldId = MBDBSC_UNDEFINED_FIELD_ID;
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Name.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Name.plug( MM );
		}
		field_ &operator =( const field_ &F )
		{
			Name = F.Name;

			S_.TableId = F.S_.TableId;
			S_.FieldId = F.S_.FieldId;

			return *this;
		}
		void Init( void )
		{
			reset();

			Name.Init();
		}
		void Init(
			const str::string_ &Name,
			table_id__ TableId = MBDBSC_UNDEFINED_TABLE_ID,
			field_id__ FieldId = MBDBSC_UNDEFINED_FIELD_ID )
		{
			Init();

			this->Name = Name;

			S_.TableId = TableId;
			S_.FieldId = FieldId;

		}
		E_RWDISCLOSE_( table_id__, TableId );
		E_RWDISCLOSE_( field_id__, FieldId );
	};

	E_AUTO( field );

	typedef lstctn::E_LXMCONTAINERt_( field_, field_row__ ) fields_;
	E_AUTO( fields );

	typedef field_ field_description_;
	E_AUTO( field_description );

	typedef ctn::E_XMCONTAINER_( field_description_ ) field_descriptions_;
	E_AUTO( field_descriptions );

}

#endif