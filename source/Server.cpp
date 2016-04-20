#include <stdio.h>
#include <algorithm>
#include "Server.h"

Server::Server( int i, int c, int s, int total_s, int num_c, bool conns[], int total_e, int num_e, bool e[], int total_u, int num_u, bool u[], int total_a, int num_r ) {
	index = i;
	comp = c;
	stor = s;

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
	// for( int j=0; j<total_apps; j++ ) {}
}

int Server::getIndex() {
	return index;
}
int Server::getComp() {
	return comp;
}
int Server::getStor() {
	return stor;
}

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

int Server::getTotalApps() {
	return total_apps;
}
int Server::getNumApps() {
	return num_apps;
}
/*
Replica getReplica( int r ) {
	if ( r < total_apps ) return replicas[r];
	else fprintf( stderr, "[ERROR] Invalid application index %d\n", r );
	return NULL;
}
*/

void Server::setAll( int i, int c, int m, int total_s, int num_c, bool conns[], int total_e, int num_e, bool e[], int total_u, int num_u, bool u[], int total_a, int num_r ) {
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
	// replicas.resize( total_apps, NULL );
	// for( int j=0; j<total_apps; j++ ) {}
}

void Server::setIndex( int i ) {
	index = i;
}
void Server::setComp( int c ) {
	comp = c;
}
void Server::setStor( int m ) {
	stor = m;
}

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

void Server::setTotalApps( int a ) {
	total_apps = a;
	// replicas.resize( total_apps, NULL );
}
void Server::setNumApps( int a ) {
	num_apps = a;
}
// void Server::setReplica( int a, Replica r ) {}
