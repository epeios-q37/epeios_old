#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(
	int argc,
	const char *argv[] )
{
	int Retour = EXIT_SUCCESS;
	size_t i, Limite;

	switch( argc ) {
	case 2:
		Limite = strlen( argv[1] );

		for( i = 0; i < Limite; i++ )
			printf( "%c", toupper( argv[1][i] ) );
		break;
	default:
		printf( "Conversion en majuscule.\nNombre d'arguments incorrect.\n\t: Usage: toupper <chaine>\n" );
		Retour = EXIT_FAILURE;
		break;
	}
}


