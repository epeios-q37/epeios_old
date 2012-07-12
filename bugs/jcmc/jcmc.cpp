#include "jcmc.h"

#include <stdlib.h>

extern "C" {
	JNIEXPORT void JNICALL Java_jcmc_bug( JNIEnv *,	jclass )
	{
		malloc( 10 );
	}
}
