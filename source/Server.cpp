#include <stdio.h>
#include <algorithm>
#include "Server.h"

Server::Server( int i, int c, int s, int idle, int peak, 
	int total_s, int num_c, bool conns[], int total_e, int num_e, bool e[], 
	int total_u, int num_u, bool u[], int total_a, int num_a, bool a[], int num_serv[], int serv[] ) {
	
	index = i;
	comp = c;
	stor = s;
	
	power_idle = idle;
	power_peak = peak;
	// connections
	total_servers = total_s;
	num_connections = num_c;
	connections.resize( total_servers, false );
	for ( int j=0; j<total_servers; j++ ) connections[j] = conns[j];
	// edges
	total_edges = total_e;
	num_edges = num_e;
	edges.resize( total_edges, false );
	for ( int j=0; j<total_edges; j++ ) edges[j] = e[j];
	// users
	total_users = total_u;
	num_users = num_u;
	users.resize( total_users, false );
	for ( int j=0; j<total_users; j++ )	users[j] = u[j];
	// apps
	total_apps = total_a;
	num_apps = num_a;
	apps.resize( total_apps, false );
	serving.resize( total_apps );
	for ( int j=0; j<total_apps; j++ ) {
		apps[j] = a[j];
		serving[j].resize( num_serv[j], -1 );
		serving[j][0] = serv[j]; // intital: one candidate
		num_apps += ( apps[j]? 1:0 ); 
	}
}
// basic
int Server::getIndex() {
	return index;
}
int Server::getComp() {
	return comp;
}
int Server::getStor() {
	return stor;
}
int Server::getPowerIdle() {
	return power_idle;
}
int Server::getPowerPeak() {
	return power_peak;
}
// connections
int Server::getTotalServers() {
	return total_servers;
}
int Server::getNumConnections() {
	return num_connections;
}
bool Server::getConnection( int s ) {
	if ( s < total_servers ) return connections[s];
	else fprintf( stderr, "[ERROR] Invalid server index %d\n", s );
	return false;
}
// edges
int Server::getTotalEdges() {
	return total_edges;
}
int Server::getNumEdges() {
	return num_edges;
}
bool Server::getEdge( int e ) {
	if ( e < total_edges ) return edges[e];
	else fprintf( stderr, "[ERROR] Invalid edge index %d\n", e );
	return false;
}
// users
int Server::getTotalUsers() {
	return total_users;
}
int Server::getNumUsers() {
	return num_users;
}
bool Server::getUser( int u ) {
	if ( u < total_users ) return users[u];
	else fprintf( stderr, "[ERROR] Invalid user index %d\n", u );
	return false;
}
// apps and serving
int Server::getTotalApps() {
	return total_apps;
}
int Server::getNumApps() {
	return num_apps;
}
bool Server::getApp( int a ) {
	if ( a < total_apps ) return apps[a];
	else fprintf( stderr, "[ERROR] Invalid application index %d\n", a );
	return false;
}
std::vector<int> Server::getServing( int a ) {
	if ( a < total_apps ) return serving[a];
	else fprintf( stderr, "[ERROR] Invalid application index %d for serving\n", a );
	return {-1};
}
int Server::getServing( int a, int c ) {
	if ( a < total_apps ) return serving[a][c];
	else fprintf( stderr, "[ERROR] Invalid application index %d for serving\n", a );
	return -1;
}

// utilization
int Server::getUsedComp() {
	return used_comp;
}
int Server::getUsedStor() {
	return used_stor;
}
double Server::getUtilization() {
	return utilization;
}
double Server::getPower() {
	return power;
}

void Server::setAll( int i, int c, int m, int total_s, int num_c, bool conns[], int total_e, int num_e, bool e[], int total_u, int num_u, bool u[], int total_a, int num_r, bool a[], int num_s[], int s[] ) {
	index = i;
	comp = c;
	stor = m;

	total_servers = total_s;
	num_connections = num_c;
	connections.resize( total_servers, false );
	for ( int j=0; j<total_servers; j++ ) connections[j] = conns[j];

	total_edges = total_e;
	num_edges = num_e;
	edges.resize( total_edges, false );
	for ( int j=0; j<total_edges; j++ ) edges[j] = e[j];

	total_users = total_u;
	num_users = num_u;
	users.resize( total_users, false );
	for ( int j=0; j<total_users; j++ ) users[j] = u[j];

	total_apps = total_a;
	num_apps = num_r;
	for ( int j=0; j<total_apps; j++ ) {
		apps[j] = a[j];
		for ( int k=0; k<num_s[j]; k++ )
			serving[j][k] = s[k];
	}
}
// basic
void Server::setIndex( int i ) {
	index = i;
}
void Server::setComp( int c ) {
	comp = c;
}
void Server::setStor( int m ) {
	stor = m;
}
// connections
void Server::setTotalServers( int s ) {
	total_servers = s;
	connections.resize( total_servers, false );
}
void Server::setNumConnections( int s ) {
	num_connections = s;
}
void Server::setConnection( int s, bool b ) {
	if ( s < total_servers ) connections[s] = b;
	else fprintf( stderr, "[ERROR] Invalid server index %d\n", s );
}
// edges
void Server::setTotalEdges( int e ) {
	total_edges = e;
	edges.resize( total_edges, false );
}
void Server::setNumEdges( int e ) {
	num_edges = e;
}
void Server::setEdge( int e, bool b ) {
	if ( e < total_edges ) edges[e] = b;
	else fprintf( stderr, "[ERROR] Invalid edge index %d\n", e );
}
// users
void Server::setTotalUsers( int u ){
	total_users = u;
	users.resize( total_users, false );
}
void Server::setNumUsers( int u ) {
	num_users = u;
}
void Server::setUser( int u, bool b ) {
	if ( u < total_users ) users[u] = b;
	else fprintf( stderr, "[ERROR] Invalid user index %d\n", u );
}
// apps
void Server::setTotalApps( int a ) {
	total_apps = a;
}
void Server::setNumApps( int a ) {
	num_apps = a;
}
void Server::setApp( int a, bool b ) {
	if ( a < total_apps ) apps[a] = b;
	else fprintf( stderr, "[ERROR] Invalid application index %d\n", a );	
}
void Server::setServing( int a, std::vector<int> v ) {
	if ( a < total_apps ) serving[a] = v;
	else fprintf( stderr, "[ERROR] Invalid application index %d for serving\n", a );
}
void Server::setServing( int a, int c, int s ) {
	if ( a < total_apps ) serving[a][c] = s;
	else fprintf( stderr, "[ERROR] Invalid application index %d for serving\n", a );
}

// utilization
void Server::setUsedComp( int used ) {
	used_comp = used;
}
void Server::setUsedStor( int used ) {
	used_stor = used;
}
void Server::setUtilization( double u ) {
	utilization = ( u? u : (double)used_comp / comp );
}
void Server::setPower( double p ) {
	power = ( p? p : power_idle * ( utilization ? 1 : 0 ) + ( power_peak - power_idle ) * utilization );
}