/*
	'mdbdsc.h' by Claude SIMON (http://zeusw.org/).

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

// eMoBDa DeSCription

#ifndef MBDDSC__INC
#define MBDDSC__INC

#include "mbdstr.h"
#include "mbdeng.h"

#include "xml.h"

#define MBDDSC_NAMESPACE_URI			"http://zeusw.org/emobda/"
#define MBDDSC_DESCRIPTION_TAG			"Description"
#define MBDDSC_NAMESPACE_ATTRIBUTE		"xmlns"
#define	MBDDSC_VERSION_ATTRIBUTE		"version"

#define MBDDSC_ENGINE_VERSION			MBDENG_ENGINE_VERSION

namespace mbddsc {
	using namespace mbdeng;
	using namespace mbdstr;

	enum version__ {
		v0_1_0,
		vDefault = v0_1_0,
		v_amount,
		v_Undefined
	};

	inline const char *GetLabel( version__ Version )
	{
		const char *Label = NULL;

		switch ( Version ) {
		case v0_1_0:
			Label = "0.1.0";
			break;
		case v_Undefined:
			ERRu();
			break;
		default:
			ERRu();
			break;
		}

		return Label;
	}

	enum item__ {
		iMiscellaneousTag,
		iEngineTag,
		iEngineVersionAttribute,
		iStructureTag,
		iStructureNameTag,
		iTablesTag,
		iTablesAmountAttribute,
		iTableTag,
		iTableIdAttribute,
		iTableNameTag,
		iFieldsTag,
		iFieldsAmountAttribute,
		iFieldTag,
		iFieldIdAttribute,
		iFieldNameTag,
		i_amount,
		i_Undefined
	};

	inline const char *GetLabel(
		item__ Item,
		version__ Version )
	{
		const char *Label = NULL;

		switch ( Item ) {
		case iMiscellaneousTag:
			Label = "Miscellaneous";
			break;
		case iEngineTag:
			Label="Engine";
			break;
		case iEngineVersionAttribute:
			Label="Version";
			break;
		case iStructureTag:
			Label="Structure";
			break;
		case iStructureNameTag:
			Label = "Name";
			break;
		case iTablesTag:
			Label = "Tables";
			break;
		case iTablesAmountAttribute:
			Label = "Amount";
			break;
		case iTableTag:
			Label = "Table";
			break;
		case iTableIdAttribute:
			Label = "id";
			break;
		case iTableNameTag:
			Label = "Name";
			break;
		case iFieldsTag:
			Label = "Fields";
			break;
		case iFieldsAmountAttribute:
			Label="Amount";
			break;
		case iFieldTag:
			Label="Field";
			break;
		case iFieldIdAttribute:
			Label="id";
			break;
		case iFieldNameTag:
			Label="Name";
			break;
		case i_Undefined:
			ERRu();
			break;
		default:
			ERRu();
			break;
		}

		return Label;
	}

	void Export(
		const mbdstr::structure_ &Description,
		xml::writer_ &Writer,
		bso::bool__ AsOrphan );

	class field_description_ {
	public:
		struct s {
			str::string_::s Name;
			mbdbsc::field_row__ FieldRow;
		} &S_;
		str::string_ Name;
		field_description_( s &S )
		: S_( S ),
		  Name( S.Name )
		{}
		void reset( bso::bool__ P = true )
		{
			Name.reset( P );
			S_.FieldRow = NONE;
		}
		void plug( mdr::E_MEMORY_DRIVER__ &MD )
		{
			Name.plug( MD );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Name.plug( MM );
		}
		field_description_ &operator =( const field_description_ &FD )
		{
			Name = FD.Name;
			S_.FieldRow = FD.S_.FieldRow;

			return *this;
		}
		void Init( void )
		{
			reset();

			Name.Init();
		}
		E_RWDISCLOSE_( field_row__, FieldRow );
	};

	E_AUTO( field_description );

	typedef ctn::E_XMCONTAINER_( field_description_ ) field_descriptions_;
	E_AUTO( field_descriptions )


	class description_
	{
	public:
		struct s
		{
			str::string_::s Name;
			field_descriptions_::s Fields;
		};
		str::string_ Name;
		field_descriptions_ Fields;
		description_( s &S )
		: Name( S.Name ),
		  Fields( S.Fields )
		{}
		void reset( bso::bool__ P = true )
		{
			Name.reset( P );
			Fields.reset( P );
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			Name.plug( MM );
			Fields.plug( MM );
		}
		description_ &operator =( const description_ &D )
		{
			Name = D.Name;
			Fields = D.Fields;

			return *this;
		}
		void Init( void )
		{
			reset();

			Name.Init();
			Fields.Init();
		}
	};

	E_AUTO( description );

	void Import(
		xtf::extended_text_iflow__ &Flow,
		description_ &Description );
}

#endif