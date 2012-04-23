/*
	Header for the 'jvastf' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) 20122004 Claude SIMON.

	This file is part of the Epeios (http://zeusw.org/epeios/) project.

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

#ifndef JVASTF__INC
#define JVASTF__INC

#define JVASTF_NAME		"JVASTF"

#define	JVASTF_VERSION	"$Revision$"

#define JVASTF_OWNER		"Claude SIMON"

#include "ttr.h"

extern class ttr_tutor &JVASTFTutor;

#if defined( XXX_DBG ) && !defined( JVASTF_NODBG )
#define JVASTF_DBG
#endif

/* Begin of automatic documentation generation part. */

//V $Revision$
//C Claude SIMON (csimon at zeusw dot org)
//R $Date$

/* End of automatic documentation generation part. */

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

/* Addendum to the automatic documentation generation part. */
//D JaVA STream Flow. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "iof.h"
# include "tol.h"

# include "jvabse.h"

# include <jni.h>

# define JVASTF_BUFFER_SIZE	1000

namespace jvastf {
	using namespace jvabse;

	typedef fdr::iflow_driver___<> _iflow_driver___;

	class input_stream__iflow_driver___
	: public _iflow_driver___
	{
	private:
		jvabse::jni_param__ *_JNIParam;
		jbyteArray _JByteArray;
		JNIEnv *_JEnv( void )
		{
			if ( _JNIParam == NULL )
				ERRc();

			if ( _JNIParam->Env == NULL )
				ERRc();

			return _JNIParam->Env;
		}
		jobject _JObject( void )
		{
			if ( _JNIParam == NULL )
				ERRc();

			if ( _JNIParam->Object == NULL )
				ERRc();

			return _JNIParam->Object;
		}
		fdr::size__ _JRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
			jobject InputStream = GetJField<jobject>( _JEnv(), _JObject(), "in", "Ljava/io/InputStream;" );

			_JByteArray = _JEnv()->NewByteArray( JVASTF_BUFFER_SIZE );
			fdr::size__ Amount = _JEnv()->CallIntMethod( InputStream, GetJMethodID( _JEnv(),  InputStream, "read", "([BII)I" ), _JByteArray, (jint)0, (jint)( Maximum > JVASTF_BUFFER_SIZE ? JVASTF_BUFFER_SIZE : Maximum ) );

			if ( Amount == -1 )
				Amount = 0;
			else
				_JEnv()->GetByteArrayRegion( _JByteArray, 0, Amount, (jbyte *)Buffer );

			return Amount;
		}
	protected:
		virtual fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::datum__ *Buffer )
		{
			return _JRead( Maximum, Buffer );
		}
		virtual void FDRDismiss( void )
		{
			// Rien à faire.
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P && false )
				if( _JByteArray != NULL ) 
					_JEnv()->DeleteLocalRef( _JByteArray );

			_iflow_driver___::reset( P );
			_JNIParam = NULL;
			_JByteArray = NULL;

		}
		E_CVDTOR( input_stream__iflow_driver___ )
		void Init( jni_param__ &JNIParam )
		{
			reset();

			_iflow_driver___::Init( fdr::ts_Default );

			_JNIParam = &JNIParam;
		}
	};

	class input_stream_iflow___
	: public flw::iflow__
	{
	private:
		input_stream__iflow_driver___ _Driver;
	public:
		void reset( bso::bool__ P = true )
		{
			_Driver.reset( P );
			iflow__::reset( P );
		}
		E_CVDTOR( input_stream_iflow___ )
		void Init(
			jni_param__ &JNIParam,
			bso::size__ AmountMax = FLW_SIZE_MAX )
		{
			_Driver.Init( JNIParam );
			iflow__::Init( _Driver, AmountMax );
		}
	};
}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
