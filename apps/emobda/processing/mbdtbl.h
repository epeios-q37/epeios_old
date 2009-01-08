/*
	'mdbtbl.h' by Claude SIMON (http://zeusw.org/).

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

// eMoBDa TaBLe

#ifndef MBDTBL__INC
#define MBDTBL__INC

#include "mbdbsc.h"
#include "mbdfld.h"

#include "ids.h"

namespace mbdtbl {
	using mbdbsc::table_row__;
	using mbdbsc::table_id__;
	using mbdbsc::field_id__;

	using mbdbsc::field_row__;
	using mbdbsc::field_rows_;

	typedef ids::E_IDS_STORE_( field_id__ ) field_ids_store_;

	class table_
	{
	public:
		struct s
		{
			table_id__ TableId;
			str::string_::s Name;
			field_rows_::s Fields;
			field_ids_store_::s FieldIdsStore;
		} &S_;
		str::string_ Name;
		field_rows_ Fields;
		field_ids_store_ FieldIdsStore;
		table_( s &S )
		: S_( S ),
		  Name( S.Name ),
		  Fields( S.Fields ),
		  FieldIdsStore( S.FieldIdsStore )
		{}
		void reset( bso::bool__ P = true )
		{
			Name.reset( P );
			Fields.reset( P );
			FieldIdsStore.reset( P );

			S_.TableId = MBDBSC_UNDEFINED_TABLE_ID;
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Name.plug( MM );
			Fields.plug( MM );
			FieldIdsStore.plug( MM );
		}
		table_ &operator =( const table_ &T )
		{
			Name = T.Name;
			Fields = T.Fields;
			FieldIdsStore = T.FieldIdsStore;

			S_.TableId = T.S_.TableId;

			return *this;
		}
		void Init( void )
		{
			reset();

			Name.Init();
			Fields.Init();
			FieldIdsStore.Init();
		}
		void Init(
			const str::string_ &Name,
			table_id__ TableId )
		{
			Init();

			this->Name = Name;
			S_.TableId = TableId;
		}
		field_id__ GetNewFieldId( field_id__ FieldId = MBDBSC_UNDEFINED_FIELD_ID )
		{
			if ( FieldId == MBDBSC_UNDEFINED_FIELD_ID )
				FieldId = FieldIdsStore.New();
			else
				FieldIdsStore.New( FieldId );

			return FieldId;
		}
		void Append( field_row__ FieldRow )
		{
			Fields.Append( FieldRow );
		}
		E_RODISCLOSE_( table_id__, TableId );
	};

	E_AUTO( table );

	typedef lstctn::E_LXCONTAINERt_( table_, table_row__ ) tables_;
	E_AUTO( tables );


	class table_description_
	{
	public:
		struct s
		{
			table_id__ TableId;
			str::string_::s Name;
			mbdfld::field_descriptions_::s Fields;
		} &S_;
		str::string_ Name;
		mbdfld::field_descriptions_ Fields;
		table_description_( s &S )
		: S_( S ),
		  Name( S.Name ),
		  Fields( S.Fields )
		{}
		void reset( bso::bool__ P = true )
		{
			Name.reset( P );
			Fields.reset( P );

			S_.TableId = MBDBSC_UNDEFINED_TABLE_ID;
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Name.plug( MM );
			Fields.plug( MM );
		}
		table_description_ &operator =( const table_description_ &TD )
		{
			Name = TD.Name;
			Fields = TD.Fields;

			S_.TableId = TD.S_.TableId;

			return *this;
		}
		void Init( void )
		{
			reset();

			Name.Init();
			Fields.Init();
		}
		void Init(
			const str::string_ &Name,
			table_id__ TableId )
		{
			Init();

			this->Name = Name;
			S_.TableId = TableId;
		}
		E_RODISCLOSE_( table_id__, TableId );
	};

	E_AUTO( table_description );

	typedef ctn::E_XCONTAINER_( table_description_ ) table_descriptions_;
	E_AUTO( table_descriptions );

}


#endif