/*
	Header for the 'rpkals' module by Claude SIMON (http://zeusw.org/epeios/contact.html)
	Copyright (C) 2010 Claude SIMON.
*/

// $Id$

#ifndef RPKALS__INC
#define RPKALS__INC

#include "str.h"

#include "rpkbsc.h"

namespace rpkals {

	using namespace rpkbsc;

	class record_alias_
	{
	public:
		struct s {
			str::string_::s Label;
			trow__ TableRow;
			rrow__ RecordRow;
		}&S_;
		str::string_ Label;
		record_alias_( s &S )
		: S_( S ),
		  Label( S.Label )
		{}
		void reset( bso::bool__ P = true )
		{
			Label.reset( P );

			S_.TableRow = NONE;
			S_.RecordRow = NONE;
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Label.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Label.plug( MM );
		}
		record_alias_ &operator =( const record_alias_ &RA )
		{
			Label = RA.Label;

			S_.TableRow = RA.S_.TableRow;
			S_.RecordRow = RA.S_.RecordRow;

			return *this;
		}
		void Init( void )
		{
			reset();

			Label.Init();
		}
		void Init(
			const str::string_ &Label,
			trow__ TableRow,
			rrow__ RecordRow )
		{
			reset();

			this->Label.Init( Label );

			S_.TableRow = TableRow;
			S_.RecordRow = RecordRow;
		}
		E_RODISCLOSE_( trow__, TableRow );
		E_RODISCLOSE_( rrow__, RecordRow );
	};

	E_AUTO( record_alias );

	typedef ctn::E_XMCONTAINER_( record_alias_ ) record_aliases_;
	E_AUTO( record_aliases )

	class table_alias_
	{
	public:
		struct s {
			str::string_::s Label;
			trow__ TableRow;
		}&S_;
		str::string_ Label;
		table_alias_( s &S )
		: S_( S ),
		  Label( S.Label )
		{}
		void reset( bso::bool__ P = true )
		{
			Label.reset( P );

			S_.TableRow = NONE;
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Label.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Label.plug( MM );
		}
		table_alias_ &operator =( const table_alias_ &TA )
		{
			Label = TA.Label;
			S_.TableRow = TA.S_.TableRow;

			return *this;
		}
		void Init( void )
		{
			reset();

			Label.Init();
		}
		void Init(
			const str::string_ &Label,
			trow__ TableRow )
		{
			reset();

			this->Label.Init( Label );
			S_.TableRow = TableRow;
		}
		E_RODISCLOSE_( trow__, TableRow );
	};

	E_AUTO( table_alias );


	typedef ctn::E_XMCONTAINER_( table_alias_ ) table_aliases_;
	E_AUTO( table_aliases )

	class aliases_ {
	public:
		struct s {
			record_aliases_::s Records;
			table_aliases_::s Tables;
		};
		record_aliases_ Records;
		table_aliases_ Tables;
		aliases_ ( s &S )
		: Records( S.Records ),
		  Tables( S.Tables )
		{}
		void reset( bso::bool__ P = true )
		{
			Records.reset( P );
			Tables.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Records.plug( MM );
			Tables.plug( MM );
		}
		aliases_ &operator =( const aliases_ &A )
		{
			Records = A.Records;
			Tables = A.Tables;

			return *this;
		}
		void Init( void )
		{
			reset();

			Records.Init();
			Tables.Init();
		}
	};

	E_AUTO( aliases )

	inline epeios::row__ FindRecordAlias_(
		const str::string_ &Label,
		const record_aliases_ &Aliases )
	{
		return SearchInMono<record_aliases_, record_alias_, epeios::row__>( Label, Aliases );
	}

	inline epeios::row__ FindTableAlias_(
		const str::string_ &Label,
		const table_aliases_ &Aliases )
	{
		return SearchInMono<table_aliases_, table_alias_, epeios::row__>( Label, Aliases );
	}

	trow__ SearchTable(
		const str::string_ &Label,
		const table_aliases_ &Aliases );
};

#endif
