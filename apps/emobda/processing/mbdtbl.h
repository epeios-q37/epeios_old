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

#ifndef MDBTBL__INC
#define MDBTBL__INC

#include "dbstbl.h"
#include "mbdidx.h"

namespace mbdtbl {

	typedef dbstbl::E_DBTABLE_	_table_;
	typedef dbsdct::file_dynamic_content_	_content_;

	class table_
	: public _table_
	{
	public:
		struct s
		: public _table_
		{
			flm::id__ FilesgroupID;
			_content_::s Content;
			dbsctt::content__ UniversalContent;
			mbdidx::record_row_field_row_index_::s RecordRowFieldRowIndex;
			mbdidx::field_row_datum_index_::s FieldRowDatumIndex;
			dbstbl::irow__ RecordRowFieldRowIndexRow, FieldRowDatumIndexRow;
		} &S_;
		_content_ Content;
		mbdidx::record_row_field_row_index_ RecordRowFieldRowIndex;
		mbdidx::field_row_datum_index_ FieldRowDatumIndex;
		table_( s &S )
		: S_( S ),
		  _table_( S ),
		  Content( S.Content ),
		  RecordRowFieldRowIndex( S.RecordRowFieldRowIndex ),
		  FieldRowDatumIndex( S.FieldRowDatumIndex )
		{}
		void reset( bso::bool__ P = true )
		{
			_table_::reset( P );
			S_.UniversalContent.reset( P );
			Content.reset( P );
			RecordRowFieldRowIndex.reset( P );
			FieldRowDatumIndex.reset( P );

			if ( P ) {
				if ( S_.FilesgroupID != FLM_UNDEFINED_ID ) {
					flm::ReleaseFiles( S_.FilesgroupID );
					flm::ReleaseId( S_.FilesgroupID );
				}
			}

			S_.FilesgroupID = FLM_UNDEFINED_ID;
			S_.RecordRowFieldRowIndexRow = S_.FieldRowDatumIndexRow = NONE;
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			_table_::plug( MM );
			Content.plug( MM );
			RecordRowFieldRowIndex.plug( MM );
			FieldRowDatumIndex.plug( MM );
		}
		table_ &operator =( const table_ &T )
		{
			S_.UniversalContent = T.S_.UniversalContent;

			_table_::operator=( *this );
			Content = T.Content;
			RecordRowFieldRowIndex = T.RecordRowFieldRowIndex;
			FieldRowDatumIndex = T.FieldRowDatumIndex;

			S_.RecordRowFieldRowIndexRow = T.S_.RecordRowFieldRowIndexRow;
			S_.FieldRowDatumIndexRow = T.S_.FieldRowDatumIndexRow;

			return *this;
		}
		void Init(
			const str::string_ &Location,
			dbstbl::mode__ Mode,
			bso::bool__ Erase,
			bso::bool__ Partial );
	};


}


#endif