/*
	'jexpp' by Claude SIMON (claude.simon@zeusw.org)
	XML Preprocessor library for 'XPPInputStream' JAVA component.
	Copyright (C) 2007-2011 Claude SIMON

	This file is part of the Epeios project (http://zeusw.org/epeios/).

    'jexpp' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'jexpp' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'jexpp'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "iof.h"
#include "xpp.h"
#include "lcl.h"

#include "jvastf.h"

using namespace jvabse;

#include "XMLPreprocessorDemo.h"
#include "org_zeusw_XPPInputStream.h"

#define WORKAROUND	1

static void Print_(
	JNIEnv *Env,
	const char *Text )
{
	jclass System = Env->FindClass( "java/lang/System" );

	jobject Out = GetStaticObjectField( Env, System, "out", "Ljava/io/PrintStream;" );

	jcharArray Array = Env->NewCharArray( 1000 );
	

	Env->CallNonvirtualVoidMethod( Out, GetClass( Env, Out ), GetMethodID( Env, Out, "println", "(Ljava/lang/String;)V" ), Env->NewStringUTF( Text ) );

	Env->CallNonvirtualVoidMethod( Out, GetClass( Env, Out ), GetMethodID( Env, Out, "flush", "()V" ), Text );
}

#ifdef WORKAROUND
#endif

#if 0
static void *malloc_( int Size )
{
#ifdef WORKAROUND
	return new char;
#else
	return calloc( Size, 1 );
#endif

}

static void free_( void *P )
{
#ifdef WORKAROUND
	delete P;
#else
	free( P );
#endif

}
#endif
JNIEXPORT void JNICALL Java_XMLPreprocessorDemo_mallocCygwinBug
  (JNIEnv *Env, jclass)
{
#if 0
ERRJProlog
	void *Test = NULL;
ERRJBegin
	Test = malloc_( 10 );

	if( Test == NULL )
		ERRa();
ERRJErr
ERRJEnd
	if ( Test != NULL )
		free_( Test );
ERRJEpilog
#endif
}

#define BUFFER_SIZE	JVASTF_BUFFER_SIZE

struct data___ {
	xpp::preprocessing_iflow___ *XPPFlow;
	jvabse::jni_param__ Param;
	jvastf::input_stream_iflow___ Flow;
	xtf::extended_text_iflow__ XFlow;
	xpp::preprocessing_iflow___ XPP;
	~data___( void )
	{
		XPPFlow = NULL;
		XPP.reset();
		XFlow.reset();
		Flow.reset();
	}
};

static flw::iflow__ *GetFlow_(
	JNIEnv *Env,
	jobject Object )
{
	Print_( Env, __LOC__ );
	flw::iflow__ *Flow = NULL;
ERRProlog
	xpp::context___ Context;
	lcl::locale Locale;
	lcl::meaning ErrorMeaning;
	str::string Translation;
	STR_BUFFER___ Buffer;
ERRBegin
	Print_( Env, __LOC__ );
	data___ &Data = *(data___ *)GetLongField( Env, Object, "core" );
	Print_( Env, __LOC__ );

	Data.Param.Env = Env;
	Data.Param.Object = GetObjectField( Env, Object, "in", "Ljava/io/InputStream;" );

	Print_( Env, __LOC__ );

	if ( !Data.XPPFlow->EndOfFlow() ) {
		Print_( Env, __LOC__ );
		Flow = Data.XPPFlow;
	Print_( Env, __LOC__ );
	} else 
	Print_( Env, __LOC__ );
		switch ( Data.XPPFlow->GetContext( Context ).Status )
		{
		case xml::sOK:
			Print_( Env, __LOC__ );
			break;
		default:
			Print_( Env, __LOC__ );
			Locale.Init();
	Print_( Env, __LOC__ );

			ErrorMeaning.Init();
			xpp::GetMeaning( Context, ErrorMeaning );

			Translation.Init();
			Locale.GetTranslation( ErrorMeaning, "", Translation );
	Print_( Env, __LOC__ );

			Env->ThrowNew( Env->FindClass( "java/lang/Exception" ), Translation.Convert( Buffer ) );
	Print_( Env, __LOC__ );
			break;
		}
ERRErr
ERREnd
ERREpilog
	Print_( Env, __LOC__ );
	return Flow;
}

JNIEXPORT jint JNICALL Java_org_zeusw_XPPInputStream_available(
	JNIEnv *Env,
	jobject Object )
{
	Print_( Env, __LOC__ );
	bso::size__ Amount = 0;
ERRJProlog
	flw::iflow__ *Flow = NULL;
ERRJBegin
	Flow = GetFlow_( Env, Object );

	if ( Flow != NULL )
		Amount = Flow->Available();

	if ( Amount < 0 )
		ERRFwk();
	else if ( Amount > ULONG_MAX )
		Amount = ULONG_MAX;

ERRJErr
ERRJEnd
ERRJEpilog
	Print_( Env, __LOC__ );
	return (jint)Amount;
}

JNIEXPORT jint JNICALL Java_org_zeusw_XPPInputStream_read__(
	JNIEnv *Env,
	jobject Object )
{
	Print_( Env, __LOC__ );
	jint C = -1;
ERRJProlog
	flw::iflow__ *Flow = NULL;
ERRJBegin
	Flow = GetFlow_( Env, Object );

	if ( Flow != NULL )
		C = Flow->Get();
ERRJErr
ERRJEnd
ERRJEpilog
	Print_( Env, __LOC__ );
	return C;
}

int Read(
	JNIEnv *Env,
	jobject Object,
	jbyteArray b,
	jint off,
	jint len )
{
	Print_( Env, __LOC__ );
	jsize Amount = len - off;
ERRJProlog
	flw::iflow__ *Flow = NULL;
	fdr::datum__ Buffer[BUFFER_SIZE];
ERRJBegin
	if ( Amount > BUFFER_SIZE )
		Amount = BUFFER_SIZE;

	Flow = GetFlow_( Env, Object );

	if ( Flow != NULL ) {
		Amount = (jsize)Flow->ReadUpTo( Amount, Buffer );

		if ( Amount == 0 )
			Amount = -1;
		else
			Env->SetByteArrayRegion( b, off, Amount, (const jbyte *)Buffer );
	} else
		Amount = -1;
ERRJErr
ERRJEnd
ERRJEpilog
	Print_( Env, __LOC__ );
	return Amount;
}

int Read(
	JNIEnv *Env,
	jobject Object,
	jbyteArray b )
{
	return Read( Env, Object, b, 0, Env->GetArrayLength( b ) );
}

JNIEXPORT jint JNICALL Java_org_zeusw_XPPInputStream_read___3B(
	JNIEnv *Env,
	jobject Object,
	jbyteArray b )
{
	return Read( Env, Object, b );
}

JNIEXPORT jint JNICALL Java_org_zeusw_XPPInputStream_read___3BII(
	JNIEnv *Env,
	jobject Object,
	jbyteArray b,
	jint off,
	jint len )
{
	return Read( Env, Object, b, off, len );
}



JNIEXPORT void JNICALL Java_org_zeusw_XPPInputStream_constructor(
  JNIEnv *Env,
  jobject Object )
{
ERRJProlog
	data___ *Data = NULL;
	xpp::criterions___ C;
	str::string D, K;
ERRJBegin
	//Print_( Env, __LOC__ );

	Data = new data___;

	Print_( Env, __LOC__ );

	if ( Data == NULL )
		ERRAlc();

	Print_( Env, __LOC__ );

	Data->Param.Env = Env;
	Print_( Env, __LOC__ );

	Data->Param.Object = GetObjectField( Env, Object, "in", "Ljava/io/InputStream;" );

	Print_( Env, __LOC__ );

	Data->Flow.Init( Data->Param );
	Print_( Env, __LOC__ );

	Data->XFlow.Init( Data->Flow );
	Print_( Env, __LOC__ );

#if 0
	D.Init( ".." );
#else
	D.Init();
	Print_( Env, __LOC__ );
	D.Append( '.' );
	Print_( Env, __LOC__ );
	D.Append( '.' );

#endif

	Print_( Env, __LOC__ );
	K.Init();
	Print_( Env, __LOC__ );

	C.Init( D, K, K );

	Print_( Env, __LOC__ );
	Data->XPP.Init( Data->XFlow, C );
	Print_( Env, __LOC__ );

	Data->XPPFlow = &Data->XPP;
//	Core->Data.Flow = &Core->Flow;

	Print_( Env, __LOC__ );

	SetLongField( Env, Object, "core", (jlong)Data );

	Print_( Env, __LOC__ );
ERRJErr
ERRJEnd
ERRJEpilog
	Print_( Env, __LOC__ );
}


JNIEXPORT void JNICALL Java_org_zeusw_XPPInputStream_destructor(
	JNIEnv *Env,
	jobject Object )
{
	Print_( Env, __LOC__ );
ERRJProlog
ERRJBegin
	delete (data___ *)GetLongField( Env, Object, "core" );
ERRJErr
ERRJEnd
ERRJEpilog
	Print_( Env, __LOC__ );
}


