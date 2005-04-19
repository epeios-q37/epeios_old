// $Id$

#include "nsIGenericFactory.h"
#include "etest.h"

NS_GENERIC_FACTORY_CONSTRUCTOR(etest)

static nsModuleComponentInfo components[] =
{
    {
       ETEST_CLASSNAME, 
       ETEST_CID,
       ETEST_CONTRACTID,
       etestConstructor,
    }
};

NS_IMPL_NSGETMODULE("IEtest", components) 