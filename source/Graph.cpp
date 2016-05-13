#include <stdio.h>
#include "Graph.h"

Graph::Graph( int total_s, Server s[], int total_e, Edge e[], int total_u, User u[], int total_a, App a[] ) {
	total_servers = total_s;
	servers.resize( total_servers );
	for ( int i=0; i<total_servers; i++ ) setServer( i, s[i] );
	total_edges = total_e;
	edges.resize( total_edges );
	for ( int i=0; i<total_edges; i++ ) setEdge( i, e[i] );
	total_users = total_u;
	users.resize( total_users );
	for ( int i=0; i<total_users; i++ ) setUser( i, u[i] );
	total_apps = total_a;
	apps.resize( total_apps );
	for ( int i=0; i<total_apps; i++ ) setApp( i, a[i] );
	distribution.resize( total_servers, std::vector<int>( total_apps, 0 ) );
}

int Graph::getTotalServers() {
	return total_servers;
}
Server *Graph::getServer( int s ) {
	if ( s < total_servers ) return &servers[s];
	else fprintf( stderr, "[ERROR] Invalid server index %d\n", s );
	return NULL;
}
int Graph::getTotalEdges() {
	return total_edges;
}
Edge *Graph::getEdge( int e ) {
	if ( e < total_edges ) return &edges[e];
	else fprintf( stderr, "[ERROR] Invalid edge index %d\n", e );
	return NULL;
}
int Graph::getTotalUsers() {
	return total_users;
}
User *Graph::getUser( int u ) {
	if ( u < total_users ) return &users[u];
	else fprintf( stderr, "[ERROR] Invalid user index %d\n", u );
	return NULL;
}
int Graph::getTotalApps() {
	return total_apps;
}
App *Graph::getApp( int a ) {
	if ( a < total_apps ) return &apps[a];
	else fprintf( stderr, "[ERROR] Invalid application index %d\n", a );
	return NULL;
}

void Graph::setTotalServers( int s ) {
	total_servers = s;
	servers.resize( total_servers );
}
void Graph::setServer( int i, Server s ) {
	if ( i < total_servers ) servers[i] = s;
	else fprintf( stderr, "[ERROR] Invalid server index %d\n", i );
}
void Graph::setTotalEdges( int e ) {
	total_edges = e;
	edges.resize( total_edges );
}
void Graph::setEdge( int i, Edge e ) {
	if ( i < total_edges ) edges[i] = e;
	else fprintf( stderr, "[ERROR] Invalid edge index %d\n", i );
}
void Graph::setTotalUsers( int u ) {
	total_users = u;
	users.resize( total_users );
}
void Graph::setUser( int i, User u ){
	if ( i < total_users ) users[i] = u;
	else fprintf( stderr, "[ERROR] Invalid user index %d\n", i );
}
void Graph::setTotalApps( int a ) {
	total_apps = a;
	apps.resize( total_apps );
}
void Graph::setApp( int i, App a ) {
	if ( i < total_apps ) apps[i] = a;
	else fprintf( stderr, "[ERROR] Invalid application index %d\n", i );
}

void Graph::readAll( const char *file_apps, const char *file_users, const char *file_edges, const char *file_servers ) {
	readApps( file_apps );
	readUsers( file_users );
	readGraph( file_edges, file_servers );
}
void Graph::readApps( const char *name ) {
	int i, s, e, u, a, c, m, b, r;
	FILE *fptr = fopen( name, "r" );
	if ( fptr ) {
		fscanf( fptr, "%d %d %d %d", &s, &e, &u, &a ); setTotalApps( a );
		for ( i=0; i<total_apps; i++ ) {
			bool reps[a] = {false};
			fscanf( fptr, "%d %d %d %d", &c, &m, &b, &r );
			for(int j = 0;j < r;++j){
				fscanf(fptr, "%d", &n);
				reps[n] = true
			}
			App app( i, c, m, b, s, r, reps);
			setApp( i, app );
		}
		fclose( fptr ); 
	}
	else fprintf( stderr, "[ERROR] Can not open application file: %s\n", name );
}
void Graph::readUsers( const char *name ) {
	int i, j, s, e, u, a, l, n, x;
	FILE *fptr = fopen( name, "r" );
	if ( fptr ) {
		fscanf( fptr, "%d %d %d %d", &s, &e, &u, &a ); setTotalUsers( u );
		for ( i=0; i<total_users; i++ ) {
			fscanf( fptr, "%d %d", &l, &n );
			bool app[a];
			for ( j=0; j<a; j++ ) app[j] = false;
			for ( j=0; j<n; j++ ) {
				fscanf( fptr, "%d", &x );
				app[x] = 1;
			}
			User user( i, l, a, app );
			setUser( i, user );
		}
	}
	else fprintf( stderr, "[ERROR] Can not open user file: %s\n", name );
}

void Graph::readGraph( const char *file_edges, const char *file_servers ) {
	readEdges( file_edges );
	readServers( file_servers );
}
void Graph::readEdges( const char *name ) {
	int i, s, e, u, a, b, r, d;
	FILE *fptr = fopen( name, "r" );
	if ( fptr ) {
		fscanf( fptr, "%d %d %d %d", &s, &e, &u, &a ); setTotalEdges( e );
		for ( i=0; i<total_edges; i++ ) {
			fscanf( fptr, "%d %d %d", &b, &r, &d );
			Edge edge( i, b, r, d );
			setEdge( i, edge );
		}
		fclose( fptr );
	}
	else fprintf( stderr, "[ERROR] Can not open edge file: %s\n", name );
}
void Graph::readServers( const char *name ) {
	int i, j, k, s, e, u, a, c, m, n, o, x;
	FILE *fptr = fopen( name, "r" );
	if ( fptr ) {
		fscanf( fptr, "%d %d %d %d", &s, &e, &u, &a ); setTotalServers( s );
		for ( i=0; i<total_servers; i++ ) {
			fscanf( fptr, "%d %d %d %d", &c, &m, &n, &o );
			bool conn[s], edge[e], user[u];
			for ( j=0; j<s; j++ ) conn[j] = false;
			for ( j=0; j<e; j++ ) edge[j] = false;
			for ( j=0; j<u; j++ ) user[j] = false;
			for ( j=0; j<n; j++ ) {
				fscanf( fptr, "%d", &x );
				conn[x] = true;
				for ( k=0; k<e; k++ )
					edges[k].getConnection(i)==x? edge[k]=true : 0;
			}
			for ( j=0; j<o; j++ ) {
				fscanf( fptr, "%d", &x );
				user[x] = true;
			}
			Server server( i, c, m, s, n, conn, e, n, edge, u, o, user, a, 0 );
			setServer( i, server );
		}
		fclose( fptr );
	}
	else fprintf( stderr, "[ERROR] Can not open server file: %s\n", name );
}

void Graph::showAll() {
	showGraph();
	showUsers();
	showApps();
}
void Graph::showGraph() {
	showServers();
	showEdges();
}
void Graph::showServers() {
	int i, j, c;
	fprintf( stdout, "Servers: %d\n", total_servers );
	for ( i=0; i<total_servers; i++ ) {
		fprintf( stdout, "[SERVER %d] (comp, memo)=(%d, %d)\n", i, servers[i].getComp(), servers[i].getStor() );
		fprintf( stdout, "total_servers=%d, num_connections=%d\n", servers[i].getTotalServers(), servers[i].getNumConnections() );
		for ( c=0, j=0; j<servers[i].getTotalServers(); j++ )
			servers[i].getConnection(j)? fprintf( stdout, "[CONN %d] to [SERVER %d]\n", c++, j ) : 0;
		fprintf( stdout, "total_edges=%d, num_edges=%d", servers[i].getTotalEdges(), servers[i].getNumEdges() );
		for ( j=0; j<servers[i].getTotalEdges(); j++ ) 
			servers[i].getEdge(j)? fprintf( stdout, "	[EDGE %d]", j ) : 0;
		fprintf( stdout, "\ntotal_users=%d, num_users=%d", servers[i].getTotalUsers(), servers[i].getNumUsers() );
		for ( j=0; j<servers[i].getTotalUsers(); j++ ) 
			servers[i].getUser(j)? fprintf( stdout, "	[USER %d]", j ) : 0;
		fprintf( stdout, "\ntotal_apps=%d, num_replicas=%d\n", servers[i].getTotalApps(), servers[i].getNumApps() );
		// for ( j=0; j<servers[i].getTotalApps(); j++ )
	}
}
void Graph::showEdges() {
	fprintf( stdout, "Edges: %d\n", total_edges );
	for ( int i=0; i<total_edges; i++ )
		fprintf( stdout, "[EDGE %d] (bandwidth, src_index, dst_index)=(%d, %d, %d)\n", i, edges[i].getBandwidth(), edges[i].getSrcIndex(), edges[i].getDstIndex() );
}
void Graph::showUsers() {
	fprintf( stdout, "Users: %d\n", total_users );
	for ( int i=0; i<total_users; i++ ) {
		fprintf( stdout, "[USER %d] location=%d, num_apps=%d\n", i, users[i].getLocation(), users[i].getNumApps() );
		/* FIX HERE */
		fprintf( stdout, "Applications: " );
		for ( int j=0; j<users[i].getNumApps(); j++ )
			users[i].getApplication(j)? fprintf( stdout, "	[APP %d]", j ) : 0;
		fprintf( stdout, "\nRequests: " );
		for ( int j=0; j<users[i].getNumApps(); j++ )
			users[i].getRequest(j)? fprintf( stdout, "	[APP %d]", j ) : 0;
		fprintf( stdout, "\n" );
	}
}
void Graph::showApps() {
	fprintf( stdout, "Applications: %d\n", total_apps );
	for ( int i=0; i<total_apps; i++ ) {
		fprintf( stdout, "[APP %d] (comp_req, stor_req, band_req)=(%d, %d, %d)\n", i, apps[i].getComp(), apps[i].getStor(), apps[i].getBand());
		fprintf( stdout, "num_servers=%d, num_replicas=%d\n", apps[i].getNumServers(), apps[i].getNumReplicas() );
		for ( int j=0; j<apps[i].getNumServers(); j++ )
			apps[i].getReplica(j)? fprintf( stdout, "	Replica on [SERVER %d]", j ) : 0;
		fprintf( stdout, "\n" );
	}
}

void Graph::usersAction() {
	for ( int i=0; i<getTotalUsers(); i++ ) {
		User *u = getUser(i);
		Server *old_s = getServer( u->getLocation() );
		int index = u->movement( old_s->getNumConnections()+1 ), c = 0;
		if ( index < old_s->getTotalServers() ) {
			for ( int j=0; j<old_s->getTotalServers() && c<=index; j++ ) c += old_s->getConnection(j);
			Server *new_s = getServer( c+1 );
			u->setLocation( new_s->getIndex() );
			old_s->setUser( u->getIndex(), false ); old_s->setNumUsers( old_s->getNumUsers()-1 );
			new_s->setUser( u->getIndex(), true ); new_s->setNumUsers( new_s->getNumUsers()+1 );
		}
		u->launchRequests();
	}
}
