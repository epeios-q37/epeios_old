// $Id$

#ifndef ETEST__INC
#define ETEST__INC
 
#include "ietest.h"  // Ce fichier est génèré à partir du '.idl' correspondant.
 
#define ETEST_CONTRACTID "@epeios.org/etest;1"
#define ETEST_CLASSNAME "etest"
// {52EAC971-157D-482e-9130-C7D50BE076C7}
#define ETEST_CID  { 0x52eac971, 0x157d, 0x482e, { 0x91, 0x30, 0xc7, 0xd5, 0xb, 0xe0, 0x76, 0xc7 } }

/* Header file */
class etest
: public ietest
{
private:
public:
	NS_DECL_ISUPPORTS
	NS_DECL_IETEST
 
	etest();
	virtual ~etest();
};
 
#endif