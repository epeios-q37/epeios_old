/*
	'scldalvik' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'scldalvik' header file ('scldalvik.h').
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

#define SCLDALVIK__COMPILATION

#include "scldalvik.h"

class scldalviktutor
: public ttr_tutor
{
public:
	scldalviktutor( void )
	: ttr_tutor( SCLDALVIK_NAME )
	{
#ifdef SCLDALVIK_DBG
		Version = SCLDALVIK_VERSION "\b\bD $";
#else
		Version = SCLDALVIK_VERSION;
#endif
		Owner = SCLDALVIK_OWNER;
		Date = "$Date$";
	}
	virtual ~scldalviktutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/
#include "str.h"
#include "cio.h"

#include "jvastf.h"

#include "dvkbse.h"
#include "sclmisc.h"

#include "dvkbse.h"
#include "dvkfev.h"

using namespace scldalvik;

STR_BUFFER___ TargetName_;

typedef fdr::oflow_driver___<> _oflow___;

class log_oflow_driver___
: public _oflow___
{
private:
	str::string _Buffer;
protected:
	virtual fdr::size__ FDRWrite(
		const fdr::datum__ *Buffer,
		fdr::size__ Maximum )
	{
		_Buffer.Append( (bso::char__ *)Buffer, Maximum );
		return Maximum;
	}
	virtual void FDRCommit( void )
	{
		if ( ::TargetName_ == NULL )
			ERRc();

		dvkbse::Log( ANDROID_LOG_ERROR, ::TargetName_, _Buffer );
		_Buffer.Init();
	}
public:
	void reset( bso::bool__ P = true )
	{
		_oflow___::reset( P );
		_Buffer.reset( P );
	}
	E_CVDTOR( log_oflow_driver___ );
	void Init( void )
	{
		_oflow___::Init( fdr::ts_Default );
		_Buffer.Init();
	}
};

log_oflow_driver___ OFlowDriver;

static void InitializeCIO_( void )
{
	::OFlowDriver.Init();

	cio::COutDriver.Init( OFlowDriver, fdr::ts_Default );
	cio::CErrDriver.Init( OFlowDriver, fdr::ts_Default );
	cio::CInDriver.Init( flx::VoidIFlowDriver, fdr::ts_Default );

	cio::Initialize( cio::tUser );
}

/**/
static void DoDalvikRelatedStuff_(
		JNIEnv *Env,
		jobject Activity,
		jobject Bundle,
		jstring PackageName ) 
{
	Env->CallNonvirtualVoidMethod(
		Activity, Env->FindClass( "android/app/Activity" ),
		jvabse::GetMethodID( Env, Activity, "onCreate", "(Landroid/os/Bundle;)V" ),
		Bundle );

	Env->CallVoidMethod(
		Activity,
		jvabse::GetMethodID( Env, Activity, "setContentView", "(I)V" ),
		jvabse::GetStaticIntField(  Env, dvkbse::GetRlayoutClass( PackageName, Env ), "main" ) );
}

static void InitializeFlow_(
	const char *Name,
	JNIEnv *Env,
	jobject Activity,
	jclass Raw,
	jvastf::input_stream_iflow___ &IFlow )
{
	jvastf::jni_param__ Param;

	Param.Env = Env;
	Param.Object = dvkbse::GetRawResourceInputStream( Name, Raw, Env, Activity );

	IFlow.Init( Param );
}

static void Initialize_(
	const char *Target,
	JNIEnv *Env,
	jobject Activity,
	jstring PackageName )
{
ERRProlog
	jvastf::input_stream_iflow___ Configuration, Locale;
	jclass Raw;
ERRBegin
	InitializeCIO_();

	Raw = dvkbse::GetRrawClass( PackageName, Env );

	InitializeFlow_( "locale", Env, Activity, Raw, Locale );
	InitializeFlow_( "configuration", Env, Activity, Raw, Configuration );

	sclmisc::Initialize( Locale, Configuration, Target, "", "" );
ERRErr
ERREnd
ERREpilog
}

static dalvik::steering_callback__ &GetSteering_(
	JNIEnv *Env,
	jobject Activity )
{
	dalvik::steering_callback__ &Steering = *(dalvik::steering_callback__ *)jvabse::GetLongField( Env, Activity, "steering" );

	if( &Steering == NULL )
		ERRc();

	return Steering;
}

static void Main_( 
	JNIEnv *Env,
	jobject Activity,
	jobject Bundle,
	jstring PackageName )
{
	ERRFProlog

	ERRFBegin
		dalvik::steering_callback__ *Steering = CreateSteering( PackageName );

		if ( Steering == NULL )
			ERRc();

		Steering->SetPackageName( PackageName );

		jvabse::SetLongField( Env, Activity, "steering", (jlong)Steering );

		GetSteering_( Env, Activity ).OnCreate( Env, Activity, Bundle );
	ERRFErr
	ERRFEnd
	ERRFEpilog
}

extern "C" {
	JNIEXPORT void JNICALL Java_org_zeusw_dalvik_EpeiosOnClickListener_onClick
	  (JNIEnv *Env, jobject Listener, jobject View)
	  {
		jobject Activity = jvabse::GetObjectField( Env, Listener, "activity", "Landroid/app/Activity;" );

		if ( Activity == NULL )
			return;

		dvkfev::listener_callback__ &Callback = *(dvkfev::listener_callback__ *)jvabse::GetLongField( Env, Listener, "callback" );

		if ( &Callback == NULL )
			ERRc();

		Callback.OnClick( Env, Activity, View );
	  }
}


extern "C" {
	JNIEXPORT void JNICALL Java_org_zeusw_dalvik_EpeiosActivity_launch(
		JNIEnv *Env,
		jobject Activity,
		jstring TargetName,
		jstring PackageName,
		jobject Bundle ) 
	{
		jvabse::Convert( TargetName, Env, ::TargetName_ );

		DoDalvikRelatedStuff_( Env, Activity, Bundle, PackageName );

		Initialize_( ::TargetName_, Env, Activity, PackageName );

		Main_( Env, Activity, Bundle, PackageName );
	}
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class scldalvikpersonnalization
: public scldalviktutor
{
public:
	scldalvikpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~scldalvikpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static scldalvikpersonnalization Tutor;

ttr_tutor &SCLDALVIKTutor = Tutor;
