/*
	'lcl' library by Claude SIMON (csimon at zeusw dot org)
	Requires the 'lcl' header file ('lcl.h').
	Copyright (C) 20092004 Claude SIMON.

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
	sdr::row__ Row = NONE;
	str::string Path;
ERRBegin
	Label.Init( Labels );

	Row = Labels.First();

	while ( Row != NONE ) {
		Path.Init( "Languages/Language[label=\"" );
		Path.Append( Label( Row ) );
		Path.Append( "\"]/@Wording" );

		Wording.Init();
		if ( Registry.GetValue( Path, Wording ) )
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
	const str::string_ &Text,
	const char *Language,
	str::string_ &Translation ) const
{
	bso::bool__ Found = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init( "Translations[language=\"" );
	Path.Append( Language );
	Path.Append( "\"]/Translation[message=\"" );
	Path.Append( Text );
	Path.Append( "\"]" );

	Found = Registry.GetValue( Path, Translation );
ERRErr
ERREnd
ERREpilog
	return Found;
}

bso::bool__ lcl::locale_::_GetTranslationFollowingMessageThenLanguage(
	const str::string_ &Text,
	const char *Language,
	str::string_ &Translation ) const
{
	bso::bool__ Found = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init( "Translations[message=\"" );
	Path.Append( Text );
	Path.Append( "\"]/Translation[language=\"" );
	Path.Append( Language );
	Path.Append( "\"]" );

	Found = Registry.GetValue( Path, Translation );
ERRErr
ERREnd
ERREpilog
	return Found;
}

bso::bool__ lcl::locale_::_GetTranslationFollowingLanguageThenText(
	const str::string_ &Text,
	const char *Language,
	str::string_ &Translation ) const
{
	bso::bool__ Found = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init( "Translations[language=\"" );
	Path.Append( Language );
	Path.Append( "\"]/Translation[text=\"" );
	Path.Append( Text );
	Path.Append( "\"]" );

	Found = Registry.GetValue( Path, Translation );
ERRErr
ERREnd
ERREpilog
	return Found;
}

bso::bool__ lcl::locale_::_GetTranslationFollowingTextThenLanguage(
	const str::string_ &Text,
	const char *Language,
	str::string_ &Translation ) const
{
	bso::bool__ Found = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init( "Translations[text=\"" );
	Path.Append( Text );
	Path.Append( "\"]/Translation[language=\"" );
	Path.Append( Language );
	Path.Append( "\"]" );

	Found = Registry.GetValue( Path, Translation );
ERRErr
ERREnd
ERREpilog
	return Found;
}

void lcl::locale_::_GetTranslation(
	const str::string_ &Text,
	const char *Language,
	str::string_ &Translation ) const
{
	bso::bool__ Found = false;

	if ( !( Found = _GetTranslationFollowingLanguageThenText( Text, Language, Translation ) ) )
		if ( !( Found = _GetTranslationFollowingTextThenLanguage( Text, Language, Translation ) ) )
			if ( !( Found = _GetTranslationFollowingLanguageThenMessage( Text, Language, Translation ) ) )	// Pour des raisons de compatibilité ascendante.
				Found = _GetTranslationFollowingMessageThenLanguage( Text, Language, Translation );	// Pour des raisons de compatibilité ascendante.

	if ( !Found ) {
		Translation.Append( Text );
		Translation.Append( LCL_TAG_MARKER_S "0" );	// Lorsque pas de traduction trouvée, on rajoute le 'marker' remplacé par l'ensemble des paramètres.
	}
}


void lcl::locale_::GetLanguages(
	strings_ &Labels,
	strings_ &Wordings ) const
{
	sdr::row__ PathErrorRow = NONE;

	Registry.GetValues( str::string( "Languages/Language/@label" ), Labels, &PathErrorRow );

	_GetCorrespondingLabels( Labels, Wordings );

	if ( PathErrorRow != NONE )
		ERRCcp();
}

void lcl::meaning_::AddTag( const meaning_ &Meaning )
{
	row__ Row = Meaning.Levels.First();
	ctn::E_CMITEMt( str::string_, row__ ) Value;
	_level__ Level = 0;

	Value.Init( Meaning.Values );

	while ( Row != NONE ) {
		_Push( Level = ( Meaning.Levels( Row ) + 1 ), Value( Row ) );

		Row = Meaning.Levels.Next( Row );
	}

	if ( Meaning.Levels.Amount() == 1 )
		_Push( Level + 1, str::string() );	// Lorsque pas de 'tag', on en ajoute un, pour simplifier l'algorithme de la traduction..
}

void lcl::locale_::_GetTranslation(
	_levels_ &Levels,
	_values_ &Values,
	const char *Language,
	str::string_ &Translation ) const
{
ERRProlog
	str::string Value;
	_level__ Level = 0;;
	str::strings Tags;
	str::string Buffer;
ERRBegin
	Tags.Init();

	if ( !Levels.IsEmpty() ) {
		Level = Levels.Pop();

		Value.Init();
		Values.Pop( Value );

		if ( Level == 0 ) {
			Buffer.Init();
			_GetTranslation( Value, Language, Translation );
		} else {
			Buffer.Init();
			Tags.Insert( Value, 0 );
		}
	}

	while ( !Levels.IsEmpty() ) {
		Value.Init();
		Values.Pop( Value );

		if ( Levels.Top() != Level ) {
			Level = Levels.Pop();

			Buffer.Init();
			_GetTranslation( Value, Language, Buffer );

			str::ReplaceTags( Buffer, Tags, LCL_TAG_MARKER_C );

			if ( Level == 0 )
				Translation.Append( Buffer );
			else {
				if ( Levels.Push( Level ) != Values.Push( Buffer ) )
					ERRCcp();
			}

			Tags.Init();
		} else {
//			Buffer.Init();
			//_GetTranslation( Value, Language, Buffer );
			Tags.Insert( Value, 0 );
			Levels.Pop();
		}

	}
ERRErr
ERREnd
ERREpilog
}

const str::string_  &lcl::locale_::GetTranslation(
	const meaning_ &Meaning,
	const char *Language,
	str::string_ &Translation ) const
{
ERRProlog
	_levels Levels;
	_values Values;
ERRBegin
	Levels.Init();
	Levels = Meaning.Levels;

	Values.Init();
	Values = Meaning.Values;

	_GetTranslation( Levels, Values, Language, Translation );
ERRErr
ERREnd
ERREpilog
	return Translation;
}



/*

const char *lcl::locale_::GetTranslation(
	const char *Text,
	const char *Language,
	STR_BUFFER___ &Buffer ) const
{
ERRProlog
	str::string Translation;
ERRBegin
	Translation.Init();

	GetTranslation( Text, Language, Translation );

	Translation.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}
*/
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
