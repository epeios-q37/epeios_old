/*
	'$xxx$' library by $AUTHOR_NAME$ ($AUTHOR_EMAIL$)
	Requires the '$xxx$' header file ('$xxx$.h').
	Copyright (C) $COPYRIGHT_DATES$ $COPYRIGHT_OWNER$.

$NOTICE$
*/

//	$$Id$$

#define $XXX$__COMPILATION

#include "$xxx$.h"

class $xxx$tutor
: public ttr_tutor
{
public:
	$xxx$tutor( void )
	: ttr_tutor( $XXX$_NAME )
	{
#ifdef $XXX$_DBG
		Version = $XXX$_VERSION "\b\bD $$";
#else
		Version = $XXX$_VERSION;
#endif
		Owner = $XXX$_OWNER;
		Date = "$$Date$$";
	}
	virtual ~$xxx$tutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$$BEGIN$$*/

using namespace $xxx$;

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class $xxx$personnalization
: public $xxx$tutor
{
public:
	$xxx$personnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~$xxx$personnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$$END$$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static $xxx$personnalization Tutor;

ttr_tutor &$XXX$Tutor = Tutor;
