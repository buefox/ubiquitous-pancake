#include "Header.h"

int main( int argc, char *argv[] ) {
	srand( time( NULL ) );
	if ( argc != 6 ) {
		fprintf( stderr, "[ITERATION] [APPS_FILE] [USERS_FILE] [EDGES_FILE] [SERVERS_FILE]\n" );
		return EXIT_FAILURE;
	}
	Graph G;
	G.readAll( argv[2], argv[3], argv[4], argv[5] );
	// G.showGraph();
	// G.showServers();
	
	for ( int iter=0; iter<atoi( argv[1] ); iter++ ) { // iterations
		G.genDistribution();
		// G.showDistribution();
		G.algorithm();
	}
	return EXIT_SUCCESS;
}