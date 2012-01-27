/*
	'frdkernl.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eDemo' software.

    'eDemo' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eDemo' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eDemo'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#include "frdkernl.h"
#include "flf.h"
#include "flx.h"
#include "lcl.h"
#include "dmofbc.h"

#define MESSAGES_PREFIX	DMOINF_UC_AFFIX "FRD"


using namespace fbltyp;

using namespace frdkernl;
using xml::writer_;

#define CASE( message )	\
	case m##message:\
		Message = #message;\
		break

static const char *GetRawMessage_( frdkernl::message__ MessageId )
{
	const char *Message = NULL;

	switch ( MessageId ) {
	CASE( NotImplementedYet );
	default:
		ERRu();
		break;
	}

	return Message;
}

static const str::string_ &GetMessage_(
	frdkernl::message__ Message,
	const lcl::rack__ &LocaleRack,
	str::string_ &Translation )
{
	LocaleRack.GetTranslation( GetRawMessage_( Message ), MESSAGES_PREFIX, Translation );

	return Translation;
}

static const char *GetMessage_(
	frdkernl::message__ Message,
	const lcl::rack__ &LocaleRack,
	STR_BUFFER___ &Buffer )
{
ERRProlog
	str::string Translation;
ERRBegin
	Translation.Init();

	GetMessage_( Message, LocaleRack, Translation );

	Translation.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer();
}

const str::string_ &frdkernl::kernel___::GetTranslation(
	message__ MessageId,
	str::string_ &Message  )
{
	return GetMessage_( MessageId, LocaleRack(), Message );
}

const char *frdkernl::kernel___::GetTranslation(
	message__ MessageId,
	STR_BUFFER___ &Buffer  )
{
	return GetMessage_( MessageId, LocaleRack(), Buffer );
}
