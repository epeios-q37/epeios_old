/*
	'mbdeng.h' by Claude SIMON (http://zeusw.org/).

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

// eMoBDa ENGine

#ifndef MBDENG__INC
#define MBDENG__INC

#include "mbdidx.h"
#include "mbdfld.h"

#include "ndbtbl.h"


#define MBDENG_ENGINE_VERSION	"0.1.0"	// Doit être modifié à chaque fois que le moteur change (format des fichiers utilisé pour stocker la bse).

namespace mbdeng {

	typedef ndbtbl::table_	_table_;
	typedef ndbdct::dynamic_content_	_content_;

	struct spreaded_file_manager___ {
	public:
		ndbtbl::table_spreaded_file_manager___ Table;
		ndbidx::index_spreaded_file_manager___
			TableRecordFieldIndex,
			TableFieldDatumIndex;
		void reset( bso::bool__ P = true )
		{
			TableRecordFieldIndex.reset( P );
			TableFieldDatumIndex.reset( P );
			Table.reset( P );
		}
	};

	class engine_
	: public _table_
	{
	public:
		struct s
		: public _table_::s
		{
			flm::id__ FilesgroupID;
			mbdidx::table_record_field_index_::s TableRecordFieldIndex;
			mbdidx::table_field_datum_index_::s TableFieldDatumIndex;
			spreaded_file_manager___ SpreadedFileManager;
		} &S_;
		mbdidx::table_record_field_index_ TableRecordFieldIndex;
		mbdidx::table_field_datum_index_ TableFieldDatumIndex;
		engine_( s &S )
		: S_( S ),
		  _table_( S ),
		  TableRecordFieldIndex( S.TableRecordFieldIndex ),
		  TableFieldDatumIndex( S.TableFieldDatumIndex )
		{}
		void reset( bso::bool__ P = true )
		{
			S_.SpreadedFileManager.reset( P );
			_table_::reset( P );
			TableRecordFieldIndex.reset( P );
			TableFieldDatumIndex.reset( P );

			if ( P ) {
				if ( S_.FilesgroupID != FLM_UNDEFINED_ID ) {
					flm::ReleaseFiles( S_.FilesgroupID );
					flm::ReleaseId( S_.FilesgroupID );
				}
			}

			S_.FilesgroupID = FLM_UNDEFINED_ID;
//			S_.TableRecordFieldIndexRow = S_.TableFieldDatumIndexRow = NONE;
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			_table_::plug( MM );
			Content.plug( MM );
			TableRecordFieldIndex.plug( MM );
			TableFieldDatumIndex.plug( MM );
		}
		engine_ &operator =( const engine_ &T )
		{
			_table_::operator=( *this );
			TableRecordFieldIndex = T.TableRecordFieldIndex;
			TableFieldDatumIndex = T.TableFieldDatumIndex;

//			S_.TableRecordFieldIndexRow = T.S_.TableRecordFieldIndexRow;
//			S_.TableFieldDatumIndexRow = T.S_.TableFieldDatumIndexRow;

			return *this;
		}
		void Init(
			const str::string_ &Location,
			const str::string_ &BaseName,
			ndbtbl::mode__ Mode,
			bso::bool__ EraseIndexes,
			bso::bool__ Partial );
		epeios::size__ GetTableRecordsAmount( mbdbsc::table_id__ TableId ) const;
//		E_RODISCLOSE_( ndbtbl::irow__, TableRecordFieldIndexRow );
//		E_RODISCLOSE_( ndbtbl::irow__, TableFieldDatumIndexRow );
	};


}


#endif
