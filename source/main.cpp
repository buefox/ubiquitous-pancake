#include "Header.h"

int main( int argc, char *argv[] ) {
	srand( time( NULL ) );
	if ( argc != 7 ) {
		fprintf( stderr, "[ITERATION] [APPS_FILE] [USERS_FILE] [EDGES_FILE] [SERVERS_FILE] [THRESHOLD]\n" );
		return EXIT_FAILURE;
	}
	Graph G;
	G.readAll( argv[2], argv[3], argv[4], argv[5], atoi(argv[6]) );
	// G.showGraph();
	// G.showServers();
	G.setIter(atoi( argv[1] ));
	for ( int iter=0; iter<atoi( argv[1] ); iter++ ) { // iterations
		G.genDistribution();
		// for(int i = 0;i <= G.getTimeWindow();++i)
		// 	G.showDistribution(i);
		// int z;
		// scanf("%d", &z);
		if(iter != 0)
			G.run(iter);
		// printf("%d\n", iter);
		G.algorithm(iter);
		G.cleanDistribution();
	}
	G.putcosts();
	return EXIT_SUCCESS;
}