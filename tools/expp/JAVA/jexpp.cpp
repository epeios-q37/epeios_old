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

#include "org_zeusw_epeios_FlowInputStream.h"
#include "org_zeusw_XPPInputStream.h"

using namespace jvabse;

#define BUFFER_SIZE	JVASTF_BUFFER_SIZE

struct data__ {
	xpp::preprocessing_iflow___ *XPPFlow;
	jvabse::jni_param__ Param;
	data__( void )
	{
		XPPFlow = NULL;
	}
};


struct work___ {
	data__ Data;
	jvastf::input_stream_iflow___ Flow;
	xtf::extended_text_iflow__ XFlow;
	xpp::preprocessing_iflow___ XPP;
	~work___( void )
	{
		XPP.reset();
		XFlow.reset();
		Flow.reset();
	}
};


JNIEXPORT void JNICALL Java_org_zeusw_epeios_FlowInputStream_constructor(
	JNIEnv *Env,
	jobject Object,
	jlong core )
{
ERRJProlog
ERRJBegin
	SetJField( Env, Object, "core", core );
ERRJErr
ERRJEnd
ERRJEpilog
}

JNIEXPORT void JNICALL Java_org_zeusw_epeios_FlowInputStream_destructor(
	JNIEnv *Env,
	jobject Object )
{
ERRJProlog
ERRJBegin
	SetJField<jlong>( Env, Object, "core", 0 );
ERRJErr
ERRJEnd
ERRJEpilog
}

static flw::iflow__ *GetFlow_(
	JNIEnv *Env,
	jobject Object )
{
	flw::iflow__ *Flow = NULL;
ERRProlog
	xpp::context___ Context;
	lcl::locale Locale;
	lcl::rack__ Rack;
	str::string Translation;
	STR_BUFFER___ Buffer;
ERRBegin
	data__ &Data = ((work___ *)GetJField<jlong>( Env, Object, "core" ))->Data;

	Data.Param.Env = Env;
	Data.Param.Object = Object;

	if ( !Data.XPPFlow->EndOfFlow() )
		Flow = Data.XPPFlow;
	else 
		switch ( Data.XPPFlow->GetContext( Context ).Status )
		{
		case xml::sOK:
			break;
		default:
			Locale.Init();
			Rack.Init( Locale, "" );

			Translation.Init();
			xpp::GetTranslation( Context, Rack, Translation );

			Env->ThrowNew( Env->FindClass( "java/lang/Exception" ), Translation.Convert( Buffer ) );
			break;
		}
ERRErr
ERREnd
ERREpilog
	return Flow;
}

JNIEXPORT jint JNICALL Java_org_zeusw_epeios_FlowInputStream_available(
	JNIEnv *Env,
	jobject Object )
{
	jint Amount = 0;
ERRJProlog
	flw::iflow__ *Flow = NULL;
ERRJBegin
	Flow = GetFlow_( Env, Object );

	if ( Flow != NULL )
		Amount = Flow->Available();
ERRJErr
ERRJEnd
ERRJEpilog
	return Amount;
}

JNIEXPORT jint JNICALL Java_org_zeusw_epeios_FlowInputStream_read__(
	JNIEnv *Env,
	jobject Object )
{
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
	return C;
}

int Read(
	JNIEnv *Env,
	jobject Object,
	jbyteArray b,
	jint off,
	jint len )
{
	jsize Amount = len - off;
ERRJProlog
	flw::iflow__ *Flow = NULL;
	fdr::datum__ Buffer[BUFFER_SIZE];
ERRJBegin
	if ( Amount > BUFFER_SIZE )
		Amount = BUFFER_SIZE;

	Flow = GetFlow_( Env, Object );

	if ( Flow != NULL ) {
		Amount = Flow->ReadUpTo( Amount, Buffer );

		if ( Amount == 0 )
			Amount = -1;
		else
			Env->SetByteArrayRegion( b, off, Amount, (const jbyte *)Buffer );
	} else
		Amount = -1;
ERRJErr
ERRJEnd
ERRJEpilog
	return Amount;
}

int Read(
	JNIEnv *Env,
	jobject Object,
	jbyteArray b )
{
	return Read( Env, Object, b, 0, Env->GetArrayLength( b ) );
}

JNIEXPORT jint JNICALL Java_org_zeusw_epeios_FlowInputStream_read___3B(
	JNIEnv *Env,
	jobject Object,
	jbyteArray b )
{
	return Read( Env, Object, b );
}

JNIEXPORT jint JNICALL Java_org_zeusw_epeios_FlowInputStream_read___3BII(
	JNIEnv *Env,
	jobject Object,
	jbyteArray b,
	jint off,
	jint len )
{
	return Read( Env, Object, b, off, len );
}

static void Print_(
	JNIEnv *Env,
	const char *Text )
{
	jclass System = Env->FindClass( "java/lang/System" );

	jobject Out = GetJField<jobject>( Env, System, "out", "Ljava/io/PrintStream;" );

	jcharArray Array = Env->NewCharArray( 1000 );
	

	Env->CallNonvirtualVoidMethod( Out, GetJClass( Env, Out ), GetJMethodID( Env, Out, "println", "(Ljava/lang/String;)V" ), Env->NewStringUTF( Text ) );

	Env->CallNonvirtualVoidMethod( Out, GetJClass( Env, Out ), GetJMethodID( Env, Out, "flush", "()V" ), Text );
}


JNIEXPORT void JNICALL Java_org_zeusw_XPPInputStream_contructor(
  JNIEnv *Env,
  jobject Object )
{
ERRJProlog
	work___ *Core = NULL;
	xpp::criterions___ C;
	str::string D, K;
ERRJBegin
	Print_( Env, __LOC__ );

	Core = new work___;

	Print_( Env, __LOC__ );

	if ( Core == NULL )
		ERRa();

	Print_( Env, __LOC__ );

	Core->Data.Param.Env = Env;
	Print_( Env, __LOC__ );

	Core->Data.Param.Object = Object;

	Print_( Env, __LOC__ );

	Core->Flow.Init( Core->Data.Param );
	Print_( Env, __LOC__ );

	Core->XFlow.Init( Core->Flow );
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
	Core->XPP.Init( Core->XFlow, C );
	Print_( Env, __LOC__ );

	Core->Data.XPPFlow = &Core->XPP;
//	Core->Data.Flow = &Core->Flow;

	Print_( Env, __LOC__ );

	SetJField<jlong>( Env, Object, "core", (jlong)Core );

	Print_( Env, __LOC__ );

	Env->CallNonvirtualVoidMethod( Object, GetJClass( Env, Object ), GetJMethodID( Env, Object, "constructor", "(J)V" ), (jlong)&Core->Data );
	Print_( Env, __LOC__ );

ERRJErr
ERRJEnd
ERRJEpilog
}


JNIEXPORT void JNICALL Java_org_zeusw_XPPInputStream_destructor(
	JNIEnv *Env,
	jobject Object )
{
ERRJProlog
ERRJBegin
	delete (work___ *)GetJField<jlong>( Env, Object, "core" );
ERRJErr
ERRJEnd
ERRJEpilog
}


