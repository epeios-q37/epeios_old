#include "jcmc.h"

#include <stdlib.h>
  /*
 * Class:     jcmc
 * Method:    main
 * Signature: ([Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_jcmc_bug
  (JNIEnv *, jclass )
  {
	malloc( 1 );
  }

