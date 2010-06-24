/*
	Header for the 'rpktbl' module by Claude SIMON (http://zeusw.org/epeios/contact.html)
	Copyright (C) 2010 Claude SIMON.
*/

// $Id$

#ifndef RPKTBL__INC
#define RPKTBL__INC

#include "rpkals.h"
#include "rpkrcd.h"

namespace rpktbl {

	using namespace rpkals;
	using namespace rpkrcd;

	class table_ {
	public:
		struct s {
			str::string_::s Label;
			records_::s Records;
	//		aliases_::s Aliases;
		};
		str::string_ Label;
		records_ Records;
	//	aliases_ Aliases;
		table_( s &S )
		: Label( S.Label ),
		  Records( S.Records )/*,
		  Aliases( S.Aliases ) */
		{};
		void reset( bso::bool__ P = true )
		{
			Label.reset( P );
			Records.reset( P );
	//		Aliases.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Label.plug( MM );
			Records.plug( MM );
	//		Aliases.plug( MM );
		}
		table_ &operator =(const table_ &T )
		{
			Label = T.Label;
			Records = T.Records;
	//		Aliases = T.Aliases;

			return *this;
		}
		void Init( void )
		{
			reset();

			Label.Init();
			Records.Init();
	//		Aliases.Init();
		}
	};

	E_AUTO( table )

	typedef ctn::E_XCONTAINERt_( table_, trow__ ) tables_;
	E_AUTO( tables );

	inline trow__ SearchTable(
		const str::string_ &Label,
		const tables_ &Tables )
	{
		return SearchInMulti<tables_, table_, trow__>( Label, Tables );
	}

	inline rrow__ SearchRecord(
		const str::string_ &Label,
		trow__ TableRow,
		const tables_ &Tables )
	{
		ctn::E_CITEMt( table_, trow__ ) Table;

		Table.Init( Tables );

		return SearchRecord( Label, Table( TableRow ).Records );
	}

	inline void Insert(
		rrow__ RecordRow,
		trow__ TableRow,
		const tables_ &Tables,
		record_ &Record )
	{
		ctn::E_CITEMt( table_, trow__ ) Table;

		Table.Init( Tables );

		Insert( RecordRow, Table( TableRow ).Records, Record );
	}

	inline void Insert(
		epeios::row__ AliasRow,
		const record_aliases_ &Aliases,
		const tables_ &Tables,
		record_ &Record )
	{
		ctn::E_CMITEM( record_alias_ ) Alias;

		Alias.Init( Aliases );

		Insert( Alias( AliasRow ).RecordRow(), Alias( AliasRow ).TableRow(), Tables, Record );
	}
};

#endif
