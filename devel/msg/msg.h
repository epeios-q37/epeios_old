/*
	Header for the 'msg' library by Claude SIMON (csimon@epeios.org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON (csimon@epeios.org).
$_RAW_$
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

#error "Obsolete ! Use 'LCL' library instead !".

#include "err.h"
#include "flw.h"
#include "lgg.h"
#include "str.h"
#include "ctn.h"
#include "xtf.h"
#include "cpe.h"

#define MSG_MESSAGE_LENGTH_MAX	100

#define MSG_CASE( m, p )	\
	case m:\
		Message = #m + p;\
		break

#ifdef CPE__USE_WORKAROUNDS
#	ifdef CPE__C_VC
#		undef GetMessage
#	endif
#	ifdef CPE__P_CYGWIN
#		undef GetMessage
#	endif
#endif

namespace msg {

	struct buffer__
	{
		char Data[MSG_MESSAGE_LENGTH_MAX+1];
		buffer__( void )
		{
			Data[0] = 0;
		}
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
			messages_::s French, English, German, Spanish;
			bso::ubyte__ Amount;
		} &S_;
		messages_ French;
		messages_ English;
		messages_ German;
		messages_ Spanish;
		i18_messages_( s &S )
		: S_( S ),
		  French( S.French ),
		  English( S.English ),
		  German( S.German ),
		  Spanish( S.Spanish )
		{}
		void reset( bso::bool__ P = true )
		{
			French.reset( P );
			English.reset( P );
			German.reset( P );
			Spanish.reset( P );

			S_.Amount = 0;
		}
		void plug( mmm::E_MULTIMEMORY_ &MM )
		{
			French.plug( MM );
			English.plug( MM );
			German.plug( MM );
			Spanish.plug( MM );
		}
		i18_messages_ &operator =( const i18_messages_ &IM )
		{
			S_.Amount = IM.S_.Amount;

			French = IM.French;
			English = IM.English;
			German = IM.German;
			Spanish = IM.Spanish;

			return *this;
		}
		//f Initialization.
		void Init( bso::ubyte__ Amount )
		{
			S_.Amount = Amount;

			French.Init();
			English.Init();
			German.Init();
			Spanish.Init();

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
