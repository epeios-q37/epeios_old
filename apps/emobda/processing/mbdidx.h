/*
	'mdbidx.h' by Claude SIMON (http://zeusw.org/).

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

// eMoBDa InDeX

#ifndef MDBIDX__INC
#define MDBIDX__INC

#include "dbsidx.h"
#include "mbdbsc.h"

namespace mbdidx {
	typedef dbsidx::sort_function__ _sort_function__;
	typedef dbsidx::file_index_ _index_;

	class record_row_field_row_sort_function__
	: public _sort_function__
	{
	protected:
		virtual bso::sign__ DBSIDXCompare(
			const dbsidx::datum_ &RawDatum1,
			const dbsidx::datum_ &RawDatum2,
			dbsidx::skip_level__ SkipLevel )
		{
			return mbdbsc::RecordRowFieldRowCompare( RawDatum1, RawDatum2, SkipLevel );
		}
	public:
		void reset( bso::bool__ = true )
		{}
		void Init( void )
		{
			reset();
		}
	};

	class field_row_datum_sort_function__
	: public _sort_function__
	{
	protected:
		virtual bso::sign__ DBSIDXCompare(
			const dbsidx::datum_ &RawDatum1,
			const dbsidx::datum_ &RawDatum2,
			dbsidx::skip_level__ SkipLevel )
		{
			return mbdbsc::FieldRowDatumCompare( RawDatum1, RawDatum2, SkipLevel );
		}
	public:
		void reset( bso::bool__ = true )
		{}
		void Init( void )
		{
			reset();
		}
	};

	template <typename sort_function> class index_
	: public _index_
	{
	public:
		struct s
		: public _index_::s
		{
			sort_function SortFunction;
		} &S_;
		index_( s &S )
		: S_( S ),
		_index_( S )
		{}
		void reset( bso::bool__ P = true )
		{
			_index_::reset( P );
			S_.SortFunction.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			_index_::plug( MM );
		}
		index_ operator =( const index_ &I )
		{
			S_.SortFunction = I.S_.SortFunction;

			_index_::operator =( I );

			return *this;
		}
		void Init(
			const str::string_ &RootFileName,
			const dbsctt::content__ &Content,
			mdr::mode__ Mode,
			bso::bool__ Erase,
			bso::bool__ Partial,
			flm::id__ ID )
		{
			reset();

			S_.SortFunction.Init();
			_index_::Init( RootFileName, Content, S_.SortFunction, Mode, Erase, Partial, ID );
		}
	};

	typedef index_<record_row_field_row_sort_function__> record_row_field_row_index_;
	E_AUTO( record_row_field_row_index );

	typedef index_<field_row_datum_sort_function__> field_row_datum_index_;
	E_AUTO( field_row_datum_index );

}
	

#endif
