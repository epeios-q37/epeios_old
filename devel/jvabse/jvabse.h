/*
	Header for the 'jvabse' library by Claude SIMON (csimon at zeusw dot org)
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
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

#ifndef JVABSE__INC
# define JVABSE__INC

# define JVABSE_NAME		"JVABSE"

# define	JVABSE_VERSION	"$Revision$"

# define JVABSE_OWNER		"Claude SIMON"

# include "ttr.h"

extern class ttr_tutor &JVABSETutor;

# if defined( XXX_DBG ) && !defined( JVABSE_NODBG )
#  define JVABSE_DBG
# endif

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
//D JaVA BaSE. 
/* End addendum to automatic documentation generation part. */

/*$BEGIN$*/

# include "err.h"
# include "flw.h"
# include "cpe.h"
# include "tol.h"
# include "str.h"

# ifndef CPE__JAVA
#  error "Use of JNI, but without defining the target as a Java component (define 'CPE_JAVA')."
# endif

# ifndef CPE__LIBRARY
#  error "A Java native component must be a library."
# endif

# include <jni.h>

# define ERRJProlog	ERRProlog
# define ERRJBegin	ERRBegin
# define ERRJErr	ERRErr
# define ERRJEnd	ERREnd
# define ERRJEpilog	ERRCommonEpilog ERRTestEpilog else jvabse::ERRFinal( Env );

namespace jvabse {

	inline void ERRFinal( JNIEnv *Env )
	{
		err::buffer__ Buffer;

		const char *Message = err::Message( Buffer );

		if ( ERRExitValue == EXIT_SUCCESS )
			ERRExitValue = EXIT_FAILURE;

		ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.

		Env->ThrowNew( Env->FindClass( "java/lang/Exception" ), Message );
	}

	const str::string_ &Convert(
		jstring JString,
		JNIEnv *Env,
		str::string_ &String );

	const char *Convert(
		jstring JString,
		JNIEnv *Env,
		STR_BUFFER___ &Buffer );

	struct jni_param__ {
		JNIEnv *Env;
		jobject Object;
		void reset( bso::bool__ = true )
		{
			Env = NULL;
			Object = NULL;
		}
		E_CDTOR( jni_param__ )
	};

	inline jclass GetClass(
		JNIEnv *Env,
		jobject Object )
	{
		jclass Class = Env->GetObjectClass( Object );

		if ( Class == NULL )
			ERRx();

		return Class;
	}

	inline jmethodID GetMethodID(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		const char *Signature )
	{
		jmethodID MethodID = Env->GetMethodID( Class, Name, Signature );

		if ( MethodID == NULL )
			ERRc();

		return MethodID;
	}

	inline jmethodID GetMethodID(
		JNIEnv *Env,
		jobject Object,
		const char *Name,
		const char *Signature )
	{
		return GetMethodID( Env, GetClass( Env, Object ), Name, Signature );
	}

	inline jmethodID GetStaticMethodID(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		const char *Signature )
	{
		jmethodID MethodID = Env->GetStaticMethodID( Class, Name, Signature );

		if ( MethodID == NULL )
			ERRc();

		return MethodID;
	}

	inline jmethodID GetStaticMethodID(
		JNIEnv *Env,
		jobject Object,
		const char *Name,
		const char *Signature )
	{
		return GetStaticMethodID( Env, GetClass( Env, Object ), Name, Signature );
	}

	inline jfieldID GetFieldID(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		const char *Signature )
	{
		jfieldID FieldID = Env->GetFieldID( Class, Name, Signature );

		if ( FieldID == NULL )
			ERRc();

		return FieldID;
	}

	inline jfieldID GetFieldID(
		JNIEnv *Env,
		jobject Object,
		const char *Name,
		const char *Signature )
	{
		return GetFieldID( Env, GetClass( Env, Object ), Name, Signature );
	}

	inline jfieldID GetStaticFieldID(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		const char *Signature )
	{
		jfieldID FieldID = Env->GetStaticFieldID( Class, Name, Signature );

		if ( FieldID == NULL )
			ERRc();

		return FieldID;
	}

	inline jint GetIntField(
		JNIEnv *Env,
		jobject Object,
		const char *Name )
	{
		return Env->GetIntField( Object, GetFieldID( Env, Object, Name, "I" ) );
	}

	inline jint GetStaticIntField(
		JNIEnv *Env,
		jclass Class,
		const char *Name )
	{
		return Env->GetStaticIntField( Class, GetStaticFieldID( Env, Class, Name, "I" ) );
	}

	inline jlong GetLongField(
		JNIEnv *Env,
		jobject Object,
		const char *Name )
	{
		return Env->GetLongField( Object, GetFieldID( Env, Object, Name, "J" ) );
	}

	inline jlong GetStaticLongField(
		JNIEnv *Env,
		jclass Class,
		const char *Name )
	{
		return Env->GetStaticLongField( Class, GetStaticFieldID( Env, Class, Name, "J" ) );
	}

	inline jobject GetObjectField(
		JNIEnv *Env,
		jobject Object,
		const char *Name,
		const char *Signature )
	{
		jobject Field = Env->GetObjectField( Object, GetFieldID( Env, Object, Name, Signature ) );

		if ( Field == NULL )
			ERRc();

		return Field;
	}

	inline jobject GetStaticObjectField(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		const char *Signature )
	{
		jobject Object = Env->GetStaticObjectField( Class, GetStaticFieldID( Env, Class, Name, Signature ) );;

		if ( Object == NULL )
			ERRc();

		return Object;
	}

	inline void SetIntField(
		JNIEnv *Env,
		jobject Object,
		const char *Name,
		jint Value )
	{
		Env->SetIntField( Object, GetFieldID( Env, Object, Name, "J" ), Value );
	}

	inline void SetLongField(
		JNIEnv *Env,
		jobject Object,
		const char *Name,
		jlong Value )
	{
		Env->SetLongField( Object, GetFieldID( Env, Object, Name, "J" ), Value );
	}

	inline void SetObjectField(
		JNIEnv *Env,
		jobject Object,
		const char *Name,
		const char *Signature,
		jobject Value )
	{
		if ( ( Signature == NULL )
			  || ( Signature[0] == 0 )
			  ||( Signature[strlen( Signature )-1] != ';' ) )
			ERRc();

		Env->SetObjectField( Object, GetFieldID( Env, Object, Name, Signature ), Value );
	}

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
