/*
	'lcl' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'lcl' header file ('lcl.h').
	Copyright (C) $COPYRIGHT_DATES$Claude SIMON.
$_RAW_$
	This file is part of the Epeios (http://zeusw.org/epeios/) project.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
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

#define LCL__COMPILATION

#include "lcl.h"

class lcltutor
: public ttr_tutor
{
public:
	lcltutor( void )
	: ttr_tutor( LCL_NAME )
	{
#ifdef LCL_DBG
		Version = LCL_VERSION "\b\bD $";
#else
		Version = LCL_VERSION;
#endif
		Owner = LCL_OWNER;
		Date = "$Date$";
	}
	virtual ~lcltutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

using namespace lcl;

void lcl::locales_::_GetCorrespondingLabels(
	const strings_ &Labels,
	strings_ &Wordings ) const
{
ERRProlog
	ctn::E_CMITEM( str::string_ ) Label;
	str::string Wording;
	epeios::row__ Row = NONE;
	str::string Path;
ERRBegin
	Label.Init( Labels );

	Row = Labels.First();

	while ( Row != NONE ) {
		Path.Init( "Languages/Language[label=\"" );
		Path.Append( Label( Row ) );
		Path.Append( "\"]/@Wording" );

		Wording.Init();
		if ( Registry.GetValue( Path, S_.Root, Wording ) )
			Wordings.Append( Wording );
		else
			Wordings.Append( Label( Row ) );

		Row = Labels.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

bso::bool__ lcl::locales_::_GetTranslationFollowingLanguageThenMessage(
	const str::string_ &RawMessage,
	const str::string_ &Language,
	str::string_ &Translation ) const
{
	bso::bool__ Found = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init( "Translations[language=\"" );
	Path.Append( Language );
	Path.Append( "\"]/Translation[message=\"" );
	Path.Append( RawMessage );
	Path.Append( "\"]" );

	Found = Registry.GetValue( Path, S_.Root, Translation );
ERRErr
ERREnd
ERREpilog
	return Found;
}

bso::bool__ lcl::locales_::_GetTranslationFollowingMessageThenLanguage(
	const str::string_ &RawMessage,
	const str::string_ &Language,
	str::string_ &Translation ) const
{
	bso::bool__ Found = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init( "Translations[message=\"" );
	Path.Append( RawMessage );
	Path.Append( "\"]/Translation[language=\"" );
	Path.Append( Language );
	Path.Append( "\"]" );

	Found = Registry.GetValue( Path, S_.Root, Translation );
ERRErr
ERREnd
ERREpilog
	return Found;
}

void lcl::locales_::GetLanguages(
	strings_ &Labels,
	strings_ &Wordings ) const
{
	if ( S_.Root != NONE ) {
		epeios::row__ PathErrorRow = NONE;

		Registry.GetValues( str::string( "Languages/Language/@label" ), S_.Root, Labels, &PathErrorRow );

		_GetCorrespondingLabels( Labels, Wordings );

		if ( PathErrorRow != NONE )
			ERRc();
	}
}

bso::bool__ lcl::locales_::GetTranslation(
	const str::string_ &RawMessage,
	const str::string_ &Language,
	str::string_ &Translation ) const
{
	bso::bool__ Found = false;

	if ( S_.Root != NONE )
		if ( !( Found = _GetTranslationFollowingLanguageThenMessage( RawMessage, Language, Translation ) ) )
			Found = _GetTranslationFollowingMessageThenLanguage( RawMessage, Language, Translation );

	if ( !Found )
		Translation = RawMessage;

	return Found;
}

const char *lcl::locales_::GetTranslation(
	const str::string_ &RawMessage,
	const str::string_ &Language,
	STR_BUFFER___ &Buffer ) const
{
ERRProlog
	str::string Translation;
ERRBegin
	Translation.Init();

	GetTranslation( RawMessage, Language, Translation );

	Translation.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class lclpersonnalization
: public lcltutor
{
public:
	lclpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~lclpersonnalization( void )
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

static lclpersonnalization Tutor;

ttr_tutor &LCLTutor = Tutor;
