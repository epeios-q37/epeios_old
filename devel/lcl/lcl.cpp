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

void lcl::locale_::_GetCorrespondingLabels(
	const strings_ &Labels,
	strings_ &Wordings ) const
{
ERRProlog
	ctn::E_CMITEM( str::string_ ) Label;
	str::string Wording;
	mdr::row__ Row = NONE;
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

bso::bool__ lcl::locale_::_GetTranslationFollowingLanguageThenMessage(
	const char *Text,
	const char *Language,
	const char *Prefix,
	str::string_ &Translation ) const
{
	bso::bool__ Found = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init( "Translations[language=\"" );
	Path.Append( Language );
	Path.Append( "\"]/Translation[message=\"" );
	Path.Append( Prefix );
	Path.Append( Text );
	Path.Append( "\"]" );

	Found = Registry.GetValue( Path, S_.Root, Translation );
ERRErr
ERREnd
ERREpilog
	return Found;
}

bso::bool__ lcl::locale_::_GetTranslationFollowingMessageThenLanguage(
	const char *Text,
	const char *Language,
	const char *Prefix,
	str::string_ &Translation ) const
{
	bso::bool__ Found = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init( "Translations[message=\"" );
	Path.Append( Prefix );
	Path.Append( Text );
	Path.Append( "\"]/Translation[language=\"" );
	Path.Append( Language );
	Path.Append( "\"]" );

	Found = Registry.GetValue( Path, S_.Root, Translation );
ERRErr
ERREnd
ERREpilog
	return Found;
}

bso::bool__ lcl::locale_::_GetTranslationFollowingLanguageThenText(
	const char *Text,
	const char *Language,
	const char *Prefix,
	str::string_ &Translation ) const
{
	bso::bool__ Found = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init( "Translations[language=\"" );
	Path.Append( Language );
	Path.Append( "\"]/Translation[text=\"" );
	Path.Append( Prefix );
	Path.Append( Text );
	Path.Append( "\"]" );

	Found = Registry.GetValue( Path, S_.Root, Translation );
ERRErr
ERREnd
ERREpilog
	return Found;
}

bso::bool__ lcl::locale_::_GetTranslationFollowingTextThenLanguage(
	const char *Text,
	const char *Language,
	const char *Prefix,
	str::string_ &Translation ) const
{
	bso::bool__ Found = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init( "Translations[text=\"" );
	Path.Append( Prefix );
	Path.Append( Text );
	Path.Append( "\"]/Translation[language=\"" );
	Path.Append( Language );
	Path.Append( "\"]" );

	Found = Registry.GetValue( Path, S_.Root, Translation );
ERRErr
ERREnd
ERREpilog
	return Found;
}

void lcl::locale_::GetLanguages(
	strings_ &Labels,
	strings_ &Wordings ) const
{
	if ( S_.Root != NONE ) {
		mdr::row__ PathErrorRow = NONE;

		Registry.GetValues( str::string( "Languages/Language/@label" ), S_.Root, Labels, &PathErrorRow );

		_GetCorrespondingLabels( Labels, Wordings );

		if ( PathErrorRow != NONE )
			ERRc();
	}
}

bso::bool__ lcl::locale_::GetTranslation(
	const char *Text,
	const char *Language,
	const char *Prefix,
	str::string_ &Translation ) const
{
	bso::bool__ Found = false;

	if ( Prefix == NULL )
		Prefix = "";

	if ( S_.Root != NONE )
		if ( !( Found = _GetTranslationFollowingLanguageThenText( Text, Language, Prefix, Translation ) ) )
			if ( !( Found = _GetTranslationFollowingTextThenLanguage( Text, Language, Prefix, Translation ) ) )
				if ( !( Found = _GetTranslationFollowingLanguageThenMessage( Text, Language, Prefix, Translation ) ) )	// Pour des raisons de compatibilité ascendante.
					Found = _GetTranslationFollowingMessageThenLanguage( Text, Language, Prefix, Translation );	// Pour des raisons de compatibilité ascendante.

	if ( !Found ) {
		Translation.Append( Prefix );
		Translation.Append( Text );
		Translation.Append( LCL_TAG_MARKER_S "0" );	// Lorsque pas de traduction trouvée, on rajoute le 'marker' remplacé par l'ensemble des paramètres.
	}

	return Found;
}

const char *lcl::locale_::GetTranslation(
	const char *Text,
	const char *Language,
	const char *Prefix,
	STR_BUFFER___ &Buffer ) const
{
ERRProlog
	str::string Translation;
ERRBegin
	Translation.Init();

	GetTranslation( Text, Language, Prefix, Translation );

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
