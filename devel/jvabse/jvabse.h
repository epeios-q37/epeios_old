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

# ifndef CPE__T_JAVA
#  error "Use of JNI, but without defining the target as a Java component (define 'CPE_JAVA')."
# endif

# ifndef CPE__T_LIBRARY
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
		if ( ERRMajor != err::itn ) {
			err::buffer__ Buffer;

			const char *Message = err::Message( Buffer );

			if ( ERRExitValue == EXIT_SUCCESS )
				ERRExitValue = EXIT_FAILURE;

			ERRRst();	// To avoid relaunching of current error by objects of the 'FLW' library.

			Env->ThrowNew( Env->FindClass( "java/lang/Exception" ), Message );	}
		else
			ERRRst();
	}


	struct jni_param__ {
		JNIEnv *Env;
		jobject Object;
		jni_param__( void )
		{
			Env = NULL;
			Object = NULL;
		}
	};

	inline jclass GetJClass(
		JNIEnv *Env,
		jobject Object )
	{
		jclass Class = Env->GetObjectClass( Object );

		if ( Class == NULL )
			ERRx();

		return Class;
	}

	inline jmethodID GetJMethodID(
		JNIEnv *Env,
		jobject Object,
		const char *Name,
		const char *Signature )
	{
		return Env->GetMethodID( GetJClass( Env, Object ), Name, Signature );
	}

	inline jmethodID GetJStaticMethodID(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		const char *Signature )
	{
		return Env->GetStaticMethodID( Class, Name, Signature );
	}

	inline jmethodID GetJStaticMethodID(
		JNIEnv *Env,
		jobject Object,
		const char *Name,
		const char *Signature )
	{
		return GetJStaticMethodID( Env, GetJClass( Env, Object ), Name, Signature );
	}

	inline jfieldID GetJFieldID(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		jint &)
	{
		return Env->GetFieldID( Class, Name, "I" );
	}

	inline jfieldID GetJStaticFieldID(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		jint &)
	{
		return Env->GetStaticFieldID( Class, Name, "I" );
	}

	inline jfieldID GetJFieldID(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		jlong &)
	{
		return Env->GetFieldID( Class, Name, "J" );
	}

	inline jfieldID GetJStaticFieldID(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		jlong &)
	{
		return Env->GetStaticFieldID( Class, Name, "J" );
	}

	template <typename jtype> inline jfieldID GetJFieldID(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		const char *Signature,
		jtype &Value )
	{
		if ( Signature != NULL )
			ERRc();

		return GetJFieldID( Env, Class, Name, Value );
	}

	template <typename jtype> inline jfieldID GetJStaticFieldID(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		const char *Signature,
		jtype &Value )
	{
		if ( Signature != NULL )
			ERRc();

		return GetJStaticFieldID( Env, Class, Name, Value );
	}

	inline jfieldID GetJFieldID(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		const char *Signature,
		jobject &)
	{
		if ( ( Signature == NULL ) || ( *Signature != 'L' ) || ( Signature[strlen( Signature ) - 1] != ';' ) )
			ERRc();

		return Env->GetFieldID( Class, Name, Signature );
	}

	inline jfieldID GetJStaticFieldID(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		const char *Signature,
		jobject &)
	{
		if ( ( Signature == NULL ) || ( *Signature != 'L' ) || ( Signature[strlen( Signature ) - 1] != ';' ) )
			ERRc();

		return Env->GetStaticFieldID( Class, Name, Signature );
	}

	inline jint GetJField(
		JNIEnv *Env,
		jclass Class,
		jfieldID FieldID,
		jint &)
	{
		return Env->GetStaticIntField( Class, FieldID );
	}

	inline jint GetJField(
		JNIEnv *Env,
		jobject Object,
		jfieldID FieldID,
		jint &)
	{
		return Env->GetIntField( Object, FieldID );
	}

	inline jlong GetJField(
		JNIEnv *Env,
		jclass Class,
		jfieldID FieldID,
		jlong &)
	{
		return Env->GetStaticLongField( Class, FieldID );
	}

	inline jlong GetJField(
		JNIEnv *Env,
		jobject Object,
		jfieldID FieldID,
		jlong &)
	{
		return Env->GetLongField( Object, FieldID );
	}

	inline jobject GetJField(
		JNIEnv *Env,
		jclass Class,
		jfieldID FieldID,
		jobject &)
	{
		return Env->GetStaticObjectField( Class, FieldID );
	}

	inline jobject GetJField(
		JNIEnv *Env,
		jobject Object,
		jfieldID FieldID,
		jobject &)
	{
		return Env->GetObjectField( Object, FieldID );
	}

	template <typename jtype> jtype GetJField(
		JNIEnv *Env,
		jclass Class,
		const char *Name,
		const char *ObjectSignature = NULL )	// Obligatoire pour 'jtype' == 'jobjet'. Doit être == 'NULL' pour les austre types.
	{
		jtype Value;	// Ne sert qu'à didcrimier entre les différents types des différents 'template's.

		return GetJField( Env, Class, GetJStaticFieldID( Env, Class, Name, ObjectSignature, Value ), Value );
	}

	template <typename jtype> jtype GetJField(
		JNIEnv *Env,
		jobject Object,
		const char *Name,
		const char *ObjectSignature = NULL )	// Obligatoire pour 'jtype' == 'jobjet'. Doit être == 'NULL' pour les austre types.
	{
		jtype Value;	// Ne sert qu'à didcrimier entre les différents types des différents 'template's.

		return GetJField( Env, Object, GetJFieldID( Env, GetJClass( Env, Object ), Name, ObjectSignature, Value ), Value );
	}

	inline void SetJField(
		JNIEnv *Env,
		jobject Object,
		jfieldID FieldID,
		jlong Value )
	{
		Env->SetLongField( Object, FieldID, Value );
	}

	inline void SetJField(
		JNIEnv *Env,
		jobject Object,
		jfieldID FieldID,
		jint Value )
	{
		Env->SetIntField( Object, FieldID, Value );
	}

	inline void SetJField(
		JNIEnv *Env,
		jobject Object,
		jfieldID FieldID,
		jobject Value )
	{
		Env->SetObjectField( Object, FieldID, Value );
	}

	template <typename jtype> void SetJField(
		JNIEnv *Env,
		jobject Object,
		const char *Name,
		jtype Value,
		const char *ObjectSignature = NULL )	// Obligatoire pour 'jtype' == 'jobjet'. Doit être == 'NULL' pour les austre types.
	{
		SetJField( Env, Object, GetJFieldID( Env, GetJClass( Env, Object ), Name, ObjectSignature, Value ), Value );
	}

}

/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
