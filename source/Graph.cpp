#include <stdio.h>
#include "Graph.h"

Graph::Graph( int window, int thres, int total_s, Server s[], int total_e, Edge e[], int total_u, User u[], int total_a, App a[] ) {
	time_window = window;
	power_threshold = thres;
	
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
	distribution.resize( time_window+1, std::vector< std::vector<int> >( total_servers, std::vector<int>( total_apps, 0 ) ) );
}

int Graph::getTimeWindow() {
	return time_window;
}

int Graph::getPowerThreshold() {
	return power_threshold;
}

double Graph::getPower() {
	return power;
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

void Graph::setTimeWindow( int w ) {
	time_window = w;
}

void Graph::setPowerThreshold( int thres ) {
	power_threshold = thres;
}

void Graph::setPower( double p ) {
	power = p;
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
	
	distribution.resize( time_window+1, std::vector< std::vector<int> >( total_servers, std::vector<int>( total_apps, 0 ) ) );
}
void Graph::readApps( const char *name ) {
	int i, time_window, num_server, num_link, num_user, num_apps, comp, stor, bandwidth, num_replicas, n, max_requests;
	FILE *fptr = fopen( name, "r" );
	if ( fptr ) {
		fscanf( fptr, "%d %d %d %d %d", &time_window, &num_server, &num_link, &num_user, &num_apps );
		setTotalApps( num_apps );
		total_apps = num_apps;
		for (i = 0;i < num_apps;i++) {
			bool reps[num_server];
			for(int j = 0;j < num_server;++j){
				reps[j] = false;
			}
			fscanf( fptr, "%d %d %d %d", &comp, &stor, &bandwidth, &num_replicas );
			char temp;
			fscanf(fptr, "%c", &temp);
			for(int j = 0;j < num_replicas;++j){
				fscanf(fptr, "%d", &n);
				reps[n] = true;
			}
			fscanf(fptr, "%c", &temp);
			fscanf( fptr, "%d", &max_requests );

			App app( i, comp, stor, bandwidth, num_server, num_replicas, reps, max_requests);
			setApp( i, app );
		}
		fclose( fptr ); 
	}
	else fprintf( stderr, "[ERROR] Can not open application file: %s\n", name );
}
void Graph::readUsers( const char *name ) {
	int i, j, time_window, num_server, num_link, num_user, num_apps, location, n, x;
	FILE *fptr = fopen( name, "r" );
	if ( fptr ) {
		fscanf( fptr, "%d %d %d %d %d", &time_window, &num_server, &num_link, &num_user, &num_apps ); 
		setTotalUsers( num_user );
		total_users = num_user;
		for (i = 0;i < num_user;i++) {
			fscanf( fptr, "%d %d", &location, &n );
			bool app[num_apps];
			for (j = 0;j < num_apps;j++) app[j] = false;
			for (j = 0;j < n;j++) {
				fscanf( fptr, "%d", &x );
				app[x] = true;
			}
			User user( i, location, num_apps, app );
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
	int i, time_window, num_server, num_link, num_user, num_apps, bandwidth, nodei, nodej; // nodei, nodej are the two nodes connected by the link (edge)
	FILE *fptr = fopen(name, "r");
	if (fptr){
		fscanf(fptr, "%d %d %d %d %d", &time_window, &num_server, &num_link, &num_user, &num_apps); 
		setTimeWindow( time_window );
		setTotalEdges(num_link);
		total_edges = num_link;
		for(i = 0;i < num_link;i++){
			fscanf(fptr, "%d %d %d", &bandwidth, &nodei, &nodej);
			Edge edge( i, time_window, bandwidth, nodei, nodej);
			setEdge(i, edge);
		}
		fclose(fptr);
	}
	else fprintf( stderr, "[ERROR] Can not open edge file: %s\n", name );
}
void Graph::readServers( const char *name ) {
	int i, j, time_window, num_server, num_link, num_user, num_apps, comp, stor, users, app, x;
	FILE *fptr = fopen( name, "r" );
	if ( fptr ) {
		fscanf( fptr, "%d %d %d %d %d", &time_window, &num_server, &num_link, &num_user, &num_apps );
		setTotalServers( num_server );
		total_servers = num_server;
		fprintf( stderr, "%d %d %d %d\n", num_server, num_link, num_user, num_apps );
		setPowerThreshold( num_server * 100 ); // setting
		
		for (i = 0;i < num_server;i++) {
			fscanf( fptr, "%d %d %d %d", &comp, &stor, &app, &users);
			bool conn[num_server], edge[num_link], user[num_user], reps[num_apps];
			for (j = 0;j < num_server;j++) conn[j] = false;
			for (j = 0;j < num_link;j++) edge[j] = false;
			for (j = 0;j < num_user;j++) user[j] = false;
			for (j = 0;j < num_apps;j++) reps[j] = false;
			
			// connections and edges
			int num_conn = 0;
			for (j = 0;j < num_link;j++) {
				if(edges[j].getSrcIndex() == i){
					edge[j] = true;
					conn[edges[j].getDstIndex()] = true;
					num_conn++;
				}
				else if(edges[j].getDstIndex() == i){
					edge[j] = true;
					conn[edges[j].getSrcIndex()] = true;
					num_conn++;
				}
			}
			for (j = 0;j < app;j++) {
				fscanf( fptr, "%d", &x );
				reps[x] = true;
			}
			
			for (j = 0;j < users;j++) {
				fscanf( fptr, "%d", &x );
				user[x] = true;
			}
			// serving
			int serving[ num_apps ] = {0};
			for ( j=0; j<num_apps; j++ ) fscanf( fptr, "%d", &serving[j] );
			
			int idle = 70, peak = 100, apps = 0;
			Server server( i, comp, stor, idle, peak, num_server, num_conn, conn, num_link, num_conn, edge, num_user, users, user, num_apps, apps, reps, serving );
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
	int i, j;
	fprintf( stdout, "<========== Servers: %d ==========>\n", total_servers );
	for ( i=0; i<total_servers; i++ ) {
		fprintf( stdout, "[SERVER %d] (comp, memo)=(%d, %d)\n", i, servers[i].getComp(), servers[i].getStor() );
		// connections and edges
		fprintf( stdout, "total_servers=%d, num_connections=%d\n", servers[i].getTotalServers(), servers[i].getNumConnections() );
		fprintf( stdout, "total_edges=%d, num_edges=%d\n", servers[i].getTotalEdges(), servers[i].getNumEdges() );
		for ( j=0; j<servers[i].getTotalEdges(); j++ )
			if( servers[i].getEdge(j) )
				fprintf( stdout, "	[EDGE %d]	[SERVER %d]\n", j, edges[j].getConnection(i) );
			
		// users
		fprintf( stdout, "total_users=%d, num_users=%d", servers[i].getTotalUsers(), servers[i].getNumUsers() );
		for ( j=0; j<servers[i].getTotalUsers(); j++ )
			if( servers[i].getUser(j) )
				fprintf( stdout, "	[USER %d]", j );
		// apps
		fprintf( stdout, "\ntotal_apps=%d, num_replicas=%d\n", servers[i].getTotalApps(), servers[i].getNumApps() );
		for ( j=0; j<servers[i].getTotalApps(); j++ ) {
			fprintf( stdout, "	[APP %d] served at [SERVER %d]", j, servers[i].getServing(j) );
			if( servers[i].getApp(j) ) fprintf( stdout, "	[REPLICA %d]", j );
			fprintf( stdout, "\n" );
			
		}
		fprintf( stdout, "\n" );
	}
}
void Graph::showEdges() {
	fprintf( stdout, "<========== Edges: %d ==========>\n", total_edges );
	for ( int i=0; i<total_edges; i++ ) {
		fprintf( stdout, "[EDGE %d] (time_window, bandwidth, src_index, dst_index)=(%d, %d, %d, %d)\n", i, edges[i].getTimeWindow(), edges[i].getBandwidth(), edges[i].getSrcIndex(), edges[i].getDstIndex() );
		for ( int j=0; j<=edges[i].getTimeWindow(); j++ ) fprintf( stdout, "	[TIME %d] %d", j, edges[i].getRemainBand(j) );
		fprintf( stdout, "\n" );
	}
}
void Graph::showUsers() {
	fprintf( stdout, "<========== Users: %d ==========>\n", total_users );
	for ( int i=0; i<total_users; i++ ) {
		fprintf( stdout, "[USER %d] location=%d, num_apps=%d\n", i, users[i].getLocation(), users[i].getNumApps() );
		/* FIX HERE */
		fprintf( stdout, "Applications: " );
		for ( int j=0; j<users[i].getNumApps(); j++ ){
			if(users[i].getApplication(j))
				fprintf( stdout, "	[APP %d]", j );
		}
		fprintf( stdout, "\nRequests: " );
		for ( int j=0; j<users[i].getNumApps(); j++ ){
			if(users[i].getRequest(j))
				fprintf( stdout, "	[APP %d]", j );
		}
		fprintf( stdout, "\n" );
	}
}
void Graph::showApps() {
	fprintf( stdout, "<========== Applications: %d ==========>\n", total_apps );
	for ( int i=0; i<total_apps; i++ ) {
		fprintf( stdout, "[APP %d] (comp_req, stor_req, band_req, max_requests)=(%d, %d, %d, %d)\n", i, apps[i].getComp(), apps[i].getStor(), apps[i].getBand(), apps[i].getMaxRequests());
		fprintf( stdout, "num_servers=%d, num_replicas=%d\n", apps[i].getNumServers(), apps[i].getNumReplicas() );
		for ( int j=0; j<apps[i].getNumServers(); j++ )
			if(apps[i].getReplica(j))
				fprintf( stdout, "[SERVER %d]", j );
		fprintf( stdout, "\n" );
	}
}

void Graph::userMovement( int id ) {
	User *u = getUser( id );
	Server *old_s = getServer( u->getLocation() );
	int index = rand() % ( old_s->getNumConnections() + 1 ), j = 0, c = 0;
	if ( index != old_s->getNumConnections() ) {
		for ( j=0; j<old_s->getTotalServers() && c<=index; j++ ) c += old_s->getConnection(j);
		Server *new_s = getServer( j-1 );
		u->setLocation( new_s->getIndex() );
		old_s->setUser( u->getIndex(), false ); old_s->setNumUsers( old_s->getNumUsers()-1 );
		new_s->setUser( u->getIndex(), true ); new_s->setNumUsers( new_s->getNumUsers()+1 );
	}
}

void Graph::genRequests( int t ) {
	if ( t < time_window ) {
		for ( int u=0; u<total_users; u++ ) {
			getUser(u)->launchRequests();
			for ( int a=0; a<total_apps; a++ ) {
				distribution[ t ][ getUser(u)->getLocation() ][ a ] += getUser(u)->getRequest(a);
				distribution[ time_window ][ getUser(u)->getLocation() ][ a ] += getUser(u)->getRequest(a);
			}
			userMovement( u );
		}
		// simulate network flow and calculate network cost
		// http://www.csie.ntnu.edu.tw/~u91029/Flow2.html
		// multi-commodity flow problem: https://en.wikipedia.org/wiki/Multi-commodity_flow_problem
	}
	else fprintf( stderr, "[ERROR] Invalid time slot %d\n", t );
}

void Graph::cleanRequests( int t ) {
	if ( t <= time_window )
		for ( int s=0; s<total_servers; s++ )
			for ( int a=0; a<total_apps; a++ )
				distribution[t][s][a] = 0;
	else fprintf( stderr, "[ERROR] Invalid time slot %d\n", t );
}

void Graph::genDistribution() {
	for ( int t=0; t<time_window; t++ ) genRequests( t );
}

void Graph::showDistribution( int t ) {
	int slot = ( t ? t : time_window );
	fprintf( stdout, "<========== Distribution of time slot: %d ==========>\n", slot );
	for ( int s=0; s<total_servers; s++ ) {
		for ( int a=0; a<total_apps; a++ ) fprintf( stdout, "%d ", distribution[slot][s][a] );
		fprintf( stdout, "\n" );
	}
}

void Graph::cleanDistribution() {
	for ( int t=0; t<=time_window; t++ ) cleanRequests( t );
}

void Graph::reboot() {
	// power
	setPower();
	// servers
	for ( int i=0; i<total_servers; i++ ) {
		servers[i].setUsedComp();
		servers[i].setUsedStor();
		servers[i].setUtilization();
		servers[i].setPower();
	}
	// edges
	for ( int i=0; i<total_edges; i++ )
		for ( int j=0; j<=time_window; j++ )
			edges[i].setRemainBand( j );
}

void Graph::algorithm() {
	reboot();
	
	// requests
	std::vector< std::tuple< int, int, int, int > > requests;
	for ( int s=0; s<total_servers; s++ ) {
		for ( int a=0; a<total_apps; a++ ) {
			std::tuple<int, int, int, int> temp = std::make_tuple( time_window, s, a, distribution[ time_window ][s][a] * apps[a].getBand() );
			requests.push_back( temp );
		}
	}
	std::sort( rbegin( requests ), rend( requests ), 
		[]( std::tuple<int, int, int, int> const &t1, std::tuple<int, int, int, int> const &t2 )
		{ return std::get<3>( t1 ) < std::get<3>( t2 ); } );
		
	// BFS
	for ( unsigned int i=0; i<requests.size() && std::get<3>( requests[i] ) > 0; i++ ) {
		// root
		int root, app, size;
		std::tie( std::ignore, root, app, size ) = requests[i];
		// vertex
		vertex v[ total_servers ];
		for ( int j=0; j<total_servers; j++ ) {
			v[j].index = j;
			v[j].distance = INT_MAX;
			v[j].parent = -1;
			v[j].cost = INT_MAX;
		}
		
		v[ root ].distance = 0;
		std::queue< vertex > Q;
		Q.push( v[ root ] );
		
		int sol_flag = 0, sol = servers[ root ].getServing( app );
		int pre_dist = v[ root ].distance, pre_cost = ( feasibility( app, size / apps[app].getBand(), root, sol )? costCal() : INT_MAX );
		while ( Q.empty() == false ) {
			vertex cur = Q.front();
			Q.pop();
			
			fprintf( stderr, "[ROOT %d] [APP %d] [SIZE %d] [CUR %d] [DIST %d]\n", root, app, size, cur.index, cur.distance );
			// check
			if ( sol_flag == true && pre_dist < cur.distance ) break;
			else pre_dist = cur.distance;
			
			// cost
			int cur_cost = ( feasibility( app, size / apps[app].getBand(), root, cur.index )? costCal() + costCalReplication() : INT_MAX );
			
			fprintf( stderr, "[COST] (cur=%d) (pre=%d) [SOL %d]\n", cur_cost, pre_cost, sol );
			if ( cur_cost <= pre_cost ) {
				pre_cost = cur_cost;
				sol = cur.index;
				sol_flag = true;
			}
			fprintf( stderr, "[COST] (cur=%d) (pre=%d) [SOL %d]\n", cur_cost, pre_cost, sol );
			
			// queue
			for ( int j=0; j<total_servers; j++ ) {
				if ( servers[ cur.index ].getConnection( j ) == true && v[j].distance == INT_MAX ) {
					v[j].distance = cur.distance + 1;
					v[j].parent = cur.index;
					Q.push( v[j] );
				}
			}
		}
		// server edge power
	}
}

bool Graph::feasibility( int app, int num, int root, int cur ) {
	// check for server capacity, power threshold and (max_requests)
	if ( servers[cur].getUsedComp() + apps[ app ].getComp() <= servers[cur].getComp() ) {
		double u = (double)( servers[cur].getUsedComp() + apps[ app ].getComp() ) / servers[cur].getComp();
		double p = servers[cur].getPowerIdle() + ( servers[cur].getPowerPeak() - servers[cur].getPowerIdle() ) * u - servers[cur].getPower();
		if ( power + p <= power_threshold ) return true;
	}
	return false;
}

int Graph::costCal() {
	// http://www.csie.ntnu.edu.tw/~u91029/Flow2.html
	// feasibility: edge capacity
	// minmum cost flow problem: https://en.wikipedia.org/wiki/Minimum-cost_flow_problem
	return INT_MAX;
}

int Graph::costCalReplication() {
	return 0;
}