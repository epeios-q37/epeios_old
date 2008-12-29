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

#ifndef MDBDSC__INC
#define MDBDSC__INC

#include "mbdstr.h"
#include "mbdeng.h"

#include "xml.h"

#define MBDDSC_NAMESPACE_URI				"http://zeusw.org/emobda/"
#define MBDDSC_DESCRIPTION_TAG_NAME			"Description"
#define MBDDSC_NAMESPACE_ATTRIBUTE_NAME		"xmlns"
#define	MBDDSC_VERSION_ATTRIBUTE_NAME		"version"

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

	const char *GetLabel( version__ Version )
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
		iStructureTagName,
		iEngineTagName,
		iEngineVersionAttributeName,
		iFieldsTagName,
		iFieldsAmountAttributeName,
		iFieldTagName,
		iFieldIdAttributeName,
		iFieldNameTagName,
		i_amount,
		i_Undefined
	};

	inline const char *GetLabel(
		item__ Item,
		version__ Version )
	{
		const char *Label = NULL;

		switch ( Item ) {
		case iStructureTagName:
			Label="Structure";
			break;
		case iEngineTagName:
			Label="Engine";
			break;
		case iEngineVersionAttributeName:
			Label="Version";
			break;
		case iFieldsTagName:
			Label = "Fields";
			break;
		case iFieldsAmountAttributeName:
			Label="Amount";
			break;
		case iFieldTagName:
			Label="Field";
			break;
		case iFieldIdAttributeName:
			Label="id";
			break;
		case iFieldNameTagName:
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
		bso::bool__ WithInternals,
		bso::bool__ AsOrphan );
}

#endif