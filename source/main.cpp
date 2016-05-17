#include "Header.h"

int main( int argc, char *argv[] ) {
	srand( time( NULL ) );
	if ( argc != 5 ) {
		fprintf( stderr, "[APPS_FILE] [USERS_FILE] [EDGES_FILE] [SERVERS_FILE]\n" );
		return EXIT_FAILURE;
	}
	Graph G;
	G.readAll( argv[1], argv[2], argv[3], argv[4] );
	// G.readApps(argv[1]);
	// G.showApps();
	// G.readUsers(argv[2]);
	// G.showUsers();
	// G.readEdges(argv[3]);
	// G.showEdges();
	// G.readServers(argv[4]);
	// G.showServers();
	// for ( int t=0; t<10; t++ ) {
	// 	G.usersAction();
	// 	G.showUsers();
	// }
	G.showGraph();
	// G.test();
	// G.showDistribution();
	return EXIT_SUCCESS;
}
