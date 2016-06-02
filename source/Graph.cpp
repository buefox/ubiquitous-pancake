#include <stdio.h>
#include <cmath>
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
	// printf("XD\n");
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

			App app( i, comp, stor, bandwidth, num_server, num_replicas, reps, max_requests, time_window);
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
		setPowerThreshold( num_server * 70 ); // setting
		
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
			int num_serv[ num_apps ], serving[ num_apps ];
			for ( j=0; j<num_apps; j++ ) {
				num_serv[j] = (int)std::ceil( (double)total_users / apps[j].getMaxRequests() );
				fscanf( fptr, "%d", &serving[j] );
			}
			int idle = 70, peak = 100, apps = 0;
			Server server( i, comp, stor, idle, peak, num_server, num_conn, conn, num_link, num_conn, edge, num_user, users, user, num_apps, apps, reps, num_serv, serving );
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
			for ( unsigned int k=0; k<servers[i].getServing(j).size(); k++ ) {
				if ( servers[i].getServing( j, k ) != -1 )
					fprintf( stdout, "	[APP %d] served at [SERVER %d]", j, servers[i].getServing(j, k) );
			}
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
		fprintf( stdout, "[APP %d]\n", i );
		fprintf( stdout, "(comp_req=%d, stor_req=%d, band_req=%d, max_requests=%d)\n", apps[i].getComp(), apps[i].getStor(), apps[i].getBand(), apps[i].getMaxRequests() );
		
		fprintf( stdout, "(num_servers=%d, num_replicas=%d)", apps[i].getNumServers(), apps[i].getNumReplicas() );
		for ( int j=0; j<apps[i].getNumServers(); j++ )
			if ( servers[j].getApp(i) )
				fprintf( stdout, " [SERVER %d]", j );
		fprintf( stdout, "\nServing\n" );
		for ( int j=0; j<apps[i].getNumServers(); j++ )
			fprintf( stdout, "	AT [SERVER %d] GOTO [SERVER %d]\n", j, servers[j].getServing( i, 0 ) );
	}
}

void Graph::userMovement( int id ) {
	int old_s = users[id].getLocation();
	int index = rand() % ( servers[old_s].getNumConnections() + 1), j = 0, c = 0;
	if( index != servers[old_s].getNumConnections() ){
		for(j = 0;j < servers[old_s].getTotalServers() && c <= index;++j)
			if(servers[old_s].getConnection(j))
				c++;
		int new_s = j-1;
		users[id].setLocation(new_s);
		servers[old_s].setUser(id, false); servers[old_s].setNumUsers(servers[old_s].getNumUsers() - 1);	
		servers[new_s].setUser(id,  true); servers[new_s].setNumUsers(servers[new_s].getNumUsers() + 1);
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
	for(int i = 0;i < total_apps;++i){
		for(int j = 0;j < total_servers;++j){
			for(int k = 0;k < time_window;++k){
				apps[i].setNumRequests(j, k, apps[i].getMaxRequests());
			}
		}
	}
}

void Graph::algorithm(int iter) {
	reboot();
	// tables storing some necessary data
	std::vector< std::vector<int> > pre_band(total_edges);
	std::vector< std::vector<int> > cur_band(total_edges);
	std::vector< std::vector<bool> > app_dis(total_servers);
	// std::vectro< std::vector< std::vector<int> > > pre_req(total_apps);
	// std::vectro< std::vector< std::vector<int> > > cur_req(total_apps);
	std::vector<int> pre_req(time_window);
	std::vector<int> cur_req(time_window);
	int cur_source = INT_MAX;
	for(int i = 0;i < total_edges;++i){
		pre_band[i].resize(time_window+1);
		cur_band[i].resize(time_window+1);
	}
	for(int i = 0;i < total_servers;++i){
		for(int j = 0;j < total_apps;++j){
			app_dis[i].push_back(servers[i].getApp(j));
			servers[i].setApp(j, false);
			apps[j].setReplica(i, false);
		}
		servers[i].setNumApps(0);
	}
	// for(int j = 0;j < total_servers;++j){
	// 	for(int k = 0;k < total_apps;++k){
	// 		if(app_dis[j][k])
	// 			printf("1 ");
	// 		else
	// 			printf("0 ");
	// 	}
	// 	printf("\n");
	// }
	// int z;
	// scanf("%d", &z);
	// 6/1 add table(s) for req quota
	
	// requests
	std::vector< std::tuple< int, int, int, int > > requests;
	int max_requests[ total_apps ], max_index[ total_apps ];
	for(int i = 0;i < total_apps;++i) {
		apps[i].setNumReplicas(0);
		max_requests[i] = 0;
		max_index[i] = -1;
	}

	for ( int s=0; s<total_servers; s++ ) {
		for ( int a=0; a<total_apps; a++ ) {
			std::tuple<int, int, int, int> temp = std::make_tuple( time_window, s, a, distribution[ time_window ][s][a] * apps[a].getBand() );
			if ( max_requests[a] < distribution[ time_window ][s][a] * apps[a].getBand() ) {
				max_requests[a] = distribution[ time_window ][s][a] * apps[a].getBand();
				max_index[a] = requests.size();
				// fprintf( stderr, "%d %d %d %d\n", s, a, distribution[ time_window ][s][a] * apps[a].getBand(), max_index[a] );
			}
			requests.push_back( temp );
		}
	}
	for ( int a=0; a<total_apps; a++ ) std::swap( requests[ a ], requests[ max_index[a] ] );
	std::sort( requests.rbegin() + total_servers*total_apps - total_apps, requests.rend(), 
		[]( std::tuple<int, int, int, int> const &t1, std::tuple<int, int, int, int> const &t2 )
		{ return std::get<3>( t1 ) < std::get<3>( t2 ); } );
	
	std::sort( requests.rbegin(), requests.rend() - total_apps, 
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
		
		for(int i = 0;i < total_edges;++i){

			for(int j = 0;j <= time_window;++j){
				pre_band[i][j] = edges[i].getRemainBand(j);
				cur_band[i][j] = edges[i].getRemainBand(j);
			}
		}
		cur_source = INT_MAX;
		int f_value, c = INT_MAX, r = INT_MAX;

		int sol_flag = 0, sol = servers[ root ].getServing( app, 0 );
		for(int i = 0;i < time_window;++i){
			pre_req[i] = apps[app].getNumRequests(sol, i);
			cur_req[i] = apps[app].getNumRequests(sol, i);
		}

		int pre_dist = v[ root ].distance, pre_cost = ( feasibility( app, size / apps[app].getBand(), root, sol )? costCal(root, sol, app, apps[app].getBand(), pre_band, pre_req) : INT_MAX );
		// band update
		for(int i = 0;i < total_edges;++i){
			for(int j = 0;j <= time_window;++j){
				cur_band[i][j] = pre_band[i][j];
			}
		}
		// req quota update
		for(int i = 0;i < time_window;++i){
			cur_req[i] = pre_req[i];
			pre_req[i] = apps[app].getNumRequests(sol, i);
		}

		while ( Q.empty() == false ) {
			vertex cur = Q.front();
			Q.pop();
			
			fprintf( stderr, "[ROOT %d] [APP %d] [SIZE %d] [CUR %d] [DIST %d]\n", root, app, size, cur.index, cur.distance );
			// check
			if ( sol_flag == true && pre_dist < cur.distance ) break;
			else pre_dist = cur.distance;
			
			// cost
			int cur_cost; 
			// avoiding overflow, change the condition
			for(int i = 0;i < time_window;++i){
				pre_req[i] = apps[app].getMaxRequests();
			}
			f_value = feasibility( app, size / apps[app].getBand(), root, cur.index );
			if(  f_value ){
				c = costCal(root, cur.index, app, apps[app].getBand(), pre_band, pre_req);
				r = costCalReplication(cur.index, app, apps[app].getStor(), app_dis, pre_band, cur_source);
				
				if(cur_source == INT_MAX){
					printf("!\n");
					printf("(iter=%d) (root=%d) (cur=%d)\n", iter, root, cur.index);
					for(int j = 0;j < total_servers;++j){
						for(int k = 0;k < total_apps;++k){
							if(app_dis[j][k])
								printf("1 ");
							else
								printf("0 ");
						}
						printf("\n");
					}
					int z;
					scanf("%d", &z);
				
				}
				// printf("%d %d\n", c, r);
				// printf("!");
				// int z;
				// scanf("%d", &z);
				if(c == INT_MAX || r == INT_MAX){
					cur_cost = INT_MAX;
				}
				else{
					cur_cost = c + r;
				}
			}
			else{
				cur_cost = INT_MAX;
			}

			// fprintf( stderr, "[COST] (cur=%d) (pre=%d) [SOL %d]\n", cur_cost, pre_cost, sol );
			if ( cur_cost <= pre_cost && cur_cost < INT_MAX) {
				pre_cost = cur_cost;
				sol = cur.index;
				sol_flag = true;
				// band update
				for(int i = 0;i < total_edges;++i){
					for(int j = 0;j <= time_window;++j){
						cur_band[i][j] = pre_band[i][j];
					}
				}
				for(int i = 0;i < time_window;++i){
					cur_req[i] = pre_req[i];
					pre_req[i] = apps[app].getNumRequests(sol, i);
				}
				// for(int i = 0;i < time_window;++i)
				// 	printf("%d ", cur_req[i]);
				// printf("\n");
			}
			// fprintf( stderr, "[COST] (cur=%d) (pre=%d) [SOL %d]\n", cur_cost, pre_cost, sol );
			
			// queue
			for ( int j=0; j<total_servers; j++ ) {
				if ( servers[ cur.index ].getConnection( j ) == true && v[j].distance == INT_MAX ) {
					v[j].distance = cur.distance + 1;
					v[j].parent = cur.index;
					Q.push( v[j] );
				}
			}
		}

		if(sol_flag){
			// there is a feasible solution
			// update server
			servers[root].setServing(app, 0, sol);
			if(!servers[sol].getApp(app)){
				servers[sol].setApp(app, true);
				
				// update power
				double pre_power = servers[sol].getPower();
				servers[sol].setUsedComp(servers[sol].getUsedComp() + apps[app].getComp());
				servers[sol].setUsedStor(servers[sol].getUsedStor() + apps[app].getStor());
				servers[sol].setUtilization();
				servers[sol].setPower();
				servers[sol].setNumApps(servers[sol].getNumApps() + 1);
				power += servers[sol].getPower() - pre_power;
				// printf("server:%d usedcomp:%d usedstor:%d power:%f\n", sol, servers[sol].getUsedComp(), servers[sol].getUsedStor(), power);			
				// update apps
				apps[app].setReplica(sol, true);
				apps[app].setNumReplicas(apps[app].getNumReplicas() + 1);
				// if(cur_source == INT_MAX){
				// 	printf("!\n");
				// 	printf("(root=%d) (cur=%d)\n", root, sol);
				// 	int z;
				// 	scanf("%d", &z);
				// }

				if(cur_source != sol)
					printf("Replicate (app=%d) from (source=%d) to (sol=%d)\n", app, cur_source, sol);
			}
			// update edge remaining bandwidth
			// fprintf( stderr, "[COST] (root=%d) (cur=%d) [SOL %d]\n",root, pre_cost, sol );
			for(int i = 0;i < total_edges;++i){
				for(int j = 0;j <= time_window;++j){
					edges[i].setRemainBand(j, cur_band[i][j]);
					// printf("%d ", edges[i].getRemainBand(j));
				}
				// printf("\n");
			}

			for(int i = 0;i < time_window;++i){
				apps[app].setNumRequests(sol, i, cur_req[i]);
				// printf("%d ", cur_req[i]);
			}
			// printf("\n");
			// int z;
			// scanf("%d", &z);
		}
		else {
			fprintf(stderr, "[No feasible solution] Because of:\n");
			if ( f_value == false ) fprintf( stderr, "	Server Capacity" );
			if ( r == INT_MAX ) fprintf( stderr, "	Replication Cost" );
			if ( c == INT_MAX ) fprintf( stderr, "	Transmission Cost" );
			fprintf( stderr, "\n" );
			fprintf( stderr, "[ROOT=%d] [APP=%d] [COST=%d] [SOL %d]\n", root, app, pre_cost, sol );
			// showEdges();
			// showServers();
			// for(int j = 0;j < total_servers;++j){
			// 	for(int k = 0;k < total_apps;++k){
			// 		if(app_dis[j][k])
			// 			printf("1 ");
			// 		else
			// 			printf("0 ");
			// 	}
			// 	printf("\n");
			// }

			int z;
			scanf("%d", &z);
		}
		fprintf( stderr, "[ROOT=%d] [APP=%d] [COST=%d] [SOL %d]\n", root, app, pre_cost, sol );
		fprintf(stderr, "(power=%f) (thres=%d)\n", power, power_threshold);
		fprintf(stderr, "--\n");
	}
	// clean up
	for(int i = 0;i < total_edges;++i){
		pre_band[i].clear();
		cur_band[i].clear();
	}
	for(int i = 0;i < total_servers;++i){
		app_dis[i].clear();
	}

	pre_req.clear();
	cur_req.clear();
	for(int i = 0;i < total_servers;++i){
		for(int j = 0;j < total_apps;++j){
			if(!servers[i].getApp(j)){
				for(int l = 0;l < time_window;++l){
					apps[j].setNumRequests(i, l, -1);
				}
			}
		}
	}
	// Fill up the candidates
	for(int i = 0;i < total_servers;++i){
		for(int j = 0;j < total_apps;++j){
			int len = servers[i].getServing(j).size();
			// printf("len:%d\n", len);
			int n = 1;
			bool visited[total_servers];
			for(int k = 0;k < total_servers;++k)
				visited[k] = false;
			std::queue<int> q;
			q.push(i);
			visited[i] = true;
			// BFS
			while(!q.empty() && n < len){
				int cur = q.front();
				q.pop();
				bool in_serving = false;
				for(int k = 0;k < n;++k){
					if(servers[i].getServing(j)[k] == cur){
						in_serving = true;
						break;
					}
				}
				if(!in_serving && servers[cur].getApp(j)){
					// fill in
					servers[i].setServing(j, n, cur);
					n++;
				}
				for(int k = 0;k < total_servers;++k){
					if (servers[cur].getConnection(k) && !visited[k]){
						visited[k] = true;
						q.push(k);
					}
				}
			}
			for(int k = n;k < len;++k){
				servers[i].setServing(j, k, -1);
			}
		}
	}
	// showDistribution();
	// showApps();
	showServers();

	// for(int i = 0;i < total_servers;++i){
	// 	printf("[SERVER %d]:\n", i);
	// 	for(int j = 0;j < total_apps;++j){
	// 		int l = servers[i].getServing(j).size();
	// 		for(int k = 0;k < l;++k){
	// 			printf("%d ", servers[i].getServing(j, k));
	// 		}
	// 		printf("\n");
	// 	}
	// 	printf("\n");
	// }

	// for(int i = 0;i < total_apps;++i){
	// 	printf("app[%d]: ", i);
	// 	for(int j = 0;j < total_servers;++j){
	// 		for(int k = 0;k < time_window;++k){
	// 			printf("%d ", apps[i].getNumRequests(j, k));
	// 		}
	// 		printf("\n");
	// 	}
	// 	printf("\n");
	// }
}

bool Graph::feasibility( int app, int num, int root, int cur ) {
	// check for server capacity, power threshold and (max_requests)
	if(servers[cur].getApp(app)){ // if current server is already exists replica 
		                          // then no need to replicate one to replicate one
		int c = 0;
		for(int i = 0;i < time_window;++i){
			// printf("%d ", apps[app].getNumRequests(cur, i));
			if(apps[app].getNumRequests(cur, i) > 0)
				c++;
		}
		// 6/1 add the condition flow on remaining quota on VM
		if(c >= time_window / 2)
			return true;
		else{
			fprintf(stderr, "not enough quota (root=%d) (cur=%d)\n", root, cur);
			int z;
			scanf("%d", &z);
			return false;
		}
	}
	if ( (servers[cur].getUsedComp() + apps[ app ].getComp() <= servers[cur].getComp()) &&
		 (servers[cur].getUsedStor() + apps[ app ].getStor() <= servers[cur].getStor())) {
		double u = (double)( servers[cur].getUsedComp() + apps[ app ].getComp() ) / servers[cur].getComp();
		double p = servers[cur].getPowerIdle() + ( servers[cur].getPowerPeak() - servers[cur].getPowerIdle() ) * u - servers[cur].getPower();
		if ( power + p <= power_threshold ) return true;
	}
	return false;
}

void CleanUp(_edges& E) {
	// helper function of costCal
	// cleaning up, releasing mem
	for (size_t i = 0; i < E.size(); ++i) {
		for (size_t j = 0; j < E[i].size(); ++j) {
			delete E[i][j];
		}
	}
}

void addDirectedEdge(_edges& E, int a, int b, int cap, int cost){
	// helper function for mcmf (minimum cost maximum flow)
	// building up the edges (and its reverse edges)
	E[a].push_back(new _edge(b, cap, cost));
	E[b].push_back(new _edge(a, 0, -cost));
	E[a].back()->reverse_edge = E[b].back();
	E[b].back()->reverse_edge = E[a].back();
}

Ans Graph::mcmf(_edges& E, int s, int t, std::vector< std::vector<int> >& pre_band, int cur_time){
	// algo implementation of minimum cost maximum(or to say, fixed amount) flow
	std::vector<nodeinfo> nf;
	Ans a(0,0);
	while(true){
		// using SPFA https://en.wikipedia.org/wiki/Shortest_Path_Faster_Algorithm
		// a method similar to dijkstra, like BFS
		std::vector<int> route(0);
		std::vector<int> cap(0);
		nf.assign(E.size(), nodeinfo(INT_MAX, 0, false, NULL));

		nf[s].cost = 0;
		nf[s].cap = INT_MAX;

		std::queue<int> q;
		q.push(s);
		nf[s].inq = true;

		while(!q.empty()){ // implementation of SPAF
			int cur = q.front();
			q.pop();
			nf[cur].inq = false;

			for(size_t i = 0;i < E[cur].size(); ++i){
				_edge* cur_e = E[cur][i];
				if(cur_e->cap == 0)
					continue;
				if(nf[cur].cost + cur_e->cost < nf[cur_e->to].cost){
					nf[cur_e->to].cost = nf[cur].cost + cur_e->cost;
					nf[cur_e->to].cap = std::min(nf[cur].cap, cur_e->cap);
					nf[cur_e->to].income_edge = cur_e;

					if(!nf[cur_e->to].inq && cur_e->to != t){
						q.push(cur_e->to);
						nf[cur_e->to].inq = true;
					}
				}
			}
		}
		if(nf[t].cap == 0) // means that all the flow available goes to terminal
			break;
		// update total cost and capacity
		a.cap += nf[t].cap;
		a.cost += nf[t].cap * nf[t].cost;

		for(_edge* i = nf[t].income_edge;i != NULL;i = nf[i->reverse_edge->to].income_edge){
			// update current path and flow
			i->cap -= nf[t].cap;
			i->reverse_edge->cap += nf[t].cap;

			// printf("!s:%d t:%d %d %d\n", s, t, i->reverse_edge->to, i->cap);

			// trace back current path
			if(i->reverse_edge->to != s)
				route.push_back(i->reverse_edge->to);
			if(i != nf[t].income_edge && i->reverse_edge->to != s)
				cap.push_back(i->cap);
		}
		// record the bandwidth usage for now
		for(size_t i = 0;i < route.size()-1;++i){
			for(int j = 0;j < total_edges;++j){
				if(((edges[j].getSrcIndex() == route[i]) && (edges[j].getDstIndex() == route[i+1])) ||
				   ((edges[j].getDstIndex() == route[i]) && (edges[j].getSrcIndex() == route[i+1]))){
					pre_band[j][cur_time] = cap[i];
					// if(cur_time == time_window)
					// 	printf("%d %d %d\n", route[i], route[i+1], cap[i]);
				}
			}
		}
		route.clear();
		cap.clear();
		// if(cur_time == time_window)
		// 	printf("--\n");
	}
	// printf("==%d\n", _c);
	return a;

}
int Graph::costCal(int s, int t, int app, int size, std::vector< std::vector<int> >& pre_band, std::vector<int>& pre_req) {
	// http://www.csie.ntnu.edu.tw/~u91029/Flow2.html
	// feasibility: edge capacity
	// minmum cost flow problem: https://en.wikipedia.org/wiki/Minimum-cost_flow_problem
	// UVa 10806 - Dijkstra, Dijkstra.
	// 5/29: about bandwidth load-balancing, we need to consider more like splitting the flow
	int total_cost = 0;
	// for each time slot, find the minimum cost flow path
	
	if(s == t){
		for(int i = 0;i < total_edges;++i){
			for(int j = 0;j < time_window;++j)
				pre_band[i][j] = edges[i].getRemainBand(j);
		}
		// printf("---");
		for(int i = 0; i < time_window;++i){
			// printf("%d ", distribution[i][s][app]);
			if(pre_req[i] <= distribution[i][s][app])
				pre_req[i] = 0; // 6/1
			else
				pre_req[i] -= distribution[i][s][app];
		}

		return 0;
	}

	// printf("s:%d t:%d\n", s, t);
	for(int i = 0;i < time_window;++i){
		_edges E(total_servers + 2);
		// printf("%d ", distribution[i][s][app]);
		for(int j = 0;j < total_edges;++j){
			addDirectedEdge(E, edges[j].getSrcIndex(), edges[j].getDstIndex(), edges[j].getRemainBand(i), 1);
			addDirectedEdge(E, edges[j].getDstIndex(), edges[j].getSrcIndex(), edges[j].getRemainBand(i), 1);
			// printf("src:%d dst:%d band:%d\n", edges[i].getSrcIndex(), edges[i].getDstIndex(), edges[i].getRemainBand(0));
		}
		addDirectedEdge(E, total_servers, s, distribution[i][s][app] * size, 0);
		addDirectedEdge(E, t, total_servers + 1, distribution[i][s][app] * size, 0);
		Ans ans = mcmf(E, total_servers, total_servers + 1, pre_band, i);
		if(ans.cap != distribution[i][s][app] * size){
			// printf("s:%d t:%d size:%d cap:%d\n", s, t, distribution[i][s][app] * size, ans.cap);
			CleanUp(E);
			return INT_MAX;
		}
		if(pre_req[i] <= ans.cap / size)
			pre_req[i] = 0; // 6/1
		else
			pre_req[i] -= ans.cap / size;
		total_cost += ans.cost;
		CleanUp(E);
	}
	// printf("\n");
	return total_cost;
}

int Graph::costCalReplication(int t, int app, int size, std::vector< std::vector<bool> >& app_dis, std::vector< std::vector<int> >& pre_band, int& cur_source) {
	int lowest_cost = INT_MAX;
	int cur = INT_MAX;
 	std::vector<int> low_band;
 	for(int i = 0;i < total_edges;++i)
 		low_band.push_back(pre_band[i][time_window]);
	for(int i = 0;i < total_servers;++i){
		if(app_dis[i][app] || servers[i].getApp(app)){
			if(i == t){ // replica exists on the root
				// printf("XDD\n");
				cur_source = i;
				return 0;
			}
			_edges E(total_servers + 2);
			for(int i = 0;i < total_edges;++i){
				addDirectedEdge(E, edges[i].getSrcIndex(), edges[i].getDstIndex(), edges[i].getRemainBand(time_window), 1);
				addDirectedEdge(E, edges[i].getDstIndex(), edges[i].getSrcIndex(), edges[i].getRemainBand(time_window), 1);
			}
			addDirectedEdge(E, total_servers, i, size, 0);
			addDirectedEdge(E, t, total_servers + 1, size, 0);
			Ans a = mcmf(E, total_servers, total_servers + 1, pre_band, time_window);
			CleanUp(E);
			if(a.cost < lowest_cost && a.cap == size){
				lowest_cost = a.cost;
				for(int j = 0;j < total_edges;++j)
					low_band[j] = pre_band[j][time_window];
				// printf("a.cost: %d\n", a.cost);
				cur = i;
			}
		}
	}
	for(int i = 0;i < total_edges;++i)
		pre_band[i][time_window] = low_band[i];
	cur_source = cur;
	if(cur == INT_MAX){
		fprintf(stderr, "app %d replication source not found\n", app);
		return INT_MAX;
	}
	return lowest_cost;
}