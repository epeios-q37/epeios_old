/* $Id$ */

#include "egeckocom.h"

NS_IMPL_ISUPPORTS1(eshared, ieshared)

eshared::eshared()
{
  /* member initializers and constructor code */
}

eshared::~eshared()
{
  /* destructor code */
}


NS_IMPL_ISUPPORTS1(eprivate, ieprivate)

eprivate::eprivate()
{
  /* member initializers and constructor code */
}

eprivate::~eprivate()
{
  /* destructor code */
}

/* Gecko required part. */

#include "nsIGenericFactory.h"

NS_GENERIC_FACTORY_CONSTRUCTOR(eshared)
NS_GENERIC_FACTORY_CONSTRUCTOR(eprivate)

static nsModuleComponentInfo components[] =
{
    {
       ESHARED_CLASSNAME, 
       ESHARED_CID,
       ESHARED_CONTRACTID,
       esharedConstructor,
    },
    {
       EPRIVATE_CLASSNAME, 
       EPRIVATE_CID,
       EPRIVATE_CONTRACTID,
       eprivateConstructor,
    },
};

NS_IMPL_NSGETMODULE("EpeiosModule", components) 
