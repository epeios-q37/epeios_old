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

	inline version__ Convert( const char *Label )
	{
		int V = v_amount;

		while ( V-- && strcmp( Label, GetLabel( (version__)V ) ) );

		if ( V == -1 )
			V = v_Undefined;

		return (version__)V;

	}

	enum item__ {
		iMiscellaneousTag,
		iEngineTag,
		iEngineVersionAttribute,
		iStructureTag,
		iStructureNameTag,
		iStructureCommentTag,
		iTablesTag,
		iTablesAmountAttribute,
		iTableTag,
		iTableIdAttribute,
		iTableNameTag,
		iTableCommentTag,
		iFieldsTag,
		iFieldsAmountAttribute,
		iFieldTag,
		iFieldIdAttribute,
		iFieldNameTag,
		iFieldCommentTag,
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
		case iStructureCommentTag:
			Label = "Comment";
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
			Label = "Id";
			break;
		case iTableNameTag:
			Label = "Name";
			break;
		case iTableCommentTag:
			Label = "Comment";
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
			Label="Id";
			break;
		case iFieldNameTag:
			Label="Name";
			break;
		case iFieldCommentTag:
			Label="Comment";
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
		const mbdstr::structure_ &Structure,
		xml::writer_ &Writer,
		bso::bool__ AsOrphan );

	typedef mbdstr::structure_description_ description_;
	E_AUTO( description );


	bso::bool__ Import(
		xtf::extended_text_iflow__ &Flow,
		description_ &Description );
}

#endif
