/*
	Header for the 'msg' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) 2004 Claude SIMON (csimon@epeios.org).

	This file is part of the Epeios (http://epeios.org/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#ifndef MSG__INC
#define MSG__INC

#define MSG_NAME		"MSG"

#define	MSG_VERSION	"$Revision$"

#define MSG_OWNER		"Claude SIMON (csimon@epeios.org)"

#include "ttr.h"

extern class ttr_tutor &MSGTutor;

#if defined( XXX_DBG ) && !defined( MSG_NODBG )
#define MSG_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon@epeios.org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D MeSsaGes 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

#include "err.h"
#include "flw.h"
#include "lgg.h"
#include "str.h"
#include "ctn.h"
#include "xtf.h"

#define MSG_MESSAGE_LENGTH_MAX	100

#define MSG_CASE( m, p )	\
	case m:\
		Message = #m + p;\
		break

#ifdef CPE__USE_VC_WORKAROUND
#	undef GetMessage
#endif

namespace msg {

	struct buffer__
	{
		char Data[MSG_MESSAGE_LENGTH_MAX+1];
	};

	typedef str::string_	message_;
	typedef str::string		message;

	typedef ctn::E_XMCONTAINER_( str::string_ ) messages_;
	E_AUTO( messages )

	//c Internationalized messages. (Will be generalized).
	class i18_messages_
	{
	protected:
		virtual const char *MSGGetRawMessage( int MessageId) const = 0;
	private:
		const char *_GetRawMessage( int MessageId ) const
		{
			return MSGGetRawMessage( MessageId );
		}
		xtf::location__ LoadMessages_(
			xtf::extended_text_iflow__ &Flow,
			const messages_ &RawMessages );
		const messages_ &_GetMessages( lgg::language__ Language ) const;
		const char *_GetMessage(
			int MessageId,
			lgg::language__ Language,
			buffer__ &Buffer ) const;
	public:
		struct s {
			messages_::s French, English, German;
			bso::ubyte__ Amount;
		} &S_;
		messages_ French;
		messages_ English;
		messages_ German;
		i18_messages_( s &S )
		: S_( S ),
		  French( S.French ),
		  English( S.English ),
		  German( S.German )
		{}
		void reset( bso::bool__ P = true )
		{
			French.reset( P );
			English.reset( P );
			German.reset( P );
			S_.Amount = 0;
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			French.plug( MM );
			English.plug( MM );
			German.plug( MM );
		}
		i18_messages_ &operator =( const i18_messages_ &IM )
		{
			S_.Amount = IM.S_.Amount;

			French = IM.French;
			English = IM.English;
			German = IM.German;

			return *this;
		}
		//f Initialization.
		void Init( bso::ubyte__ Amount )
		{
			S_.Amount = Amount;

			French.Init();
			English.Init();
			German.Init();
		}
		xtf::location__ LoadMessages( xtf::extended_text_iflow__ &Flow );
		const char *GetMessage(
			int MessageId,
			lgg::language__ Language,
			buffer__ &Buffer ) const;
		void DumpRawMessages( txf::text_oflow__ &Flow ) const;
		void DumpRawMessages( messages_ &Messages ) const;
	};

	E_AUTO( i18_messages )
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
