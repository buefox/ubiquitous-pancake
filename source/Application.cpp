#include <stdio.h>
#include <algorithm>
#include "Application.h"

App::App( int i, int c, int s, int b, int num_s, int num_r, bool reps[], int max_r ) {
	index = i;
	comp = c;
	stor = s;
	band = b;
	num_servers = num_s;
	num_replicas = num_r;
	replicas.resize( num_servers, false );
	if ( reps )
		for ( int j=0; j<num_servers; j++ ) replicas[j] = reps[j];
	max_requests = max_r;
}

int App::getIndex() {
	return index;
}
int App::getComp() {
	return comp;
}
int App::getStor() {
	return stor;
}
int App::getBand() {
	return band;
}
int App::getNumServers() {
	return num_servers;
}
int App::getNumReplicas() {
	return num_replicas;
}
bool App::getReplica( int r ) {
	if ( r < num_servers ) return replicas[r];
	else fprintf( stderr, "[ERROR] Invalid replica index %d\n", r );
	return false;
}

int App::getMaxRequests() {
	return max_requests;
}
int App::getNumRequests() {
	return num_requests;
}

void App::setAll( int i, int c, int m, int b, int num_s, int num_r, bool reps[] ) {
	index = i;
	comp = c;
	stor = m;
	band = b;
	num_servers = num_s;
	num_replicas = num_r;
	replicas.resize( num_servers, false );
	for ( int j=0; j<num_servers; j++ ) replicas[j] = reps[j];
}
void App::setIndex( int i ) {
	index = i;
}
void App::setComp( int comp_ ) {
	comp = comp_;
}
void App::setStor( int stor_ ) {
	stor = stor_;
}
void App::setBand( int band_ ) {
	band = band_;
}
void App::setNumServers( int num ) {
	num_servers = num;
}
void App::setNumReplicas( int num ) {
	num_replicas = num;
}
void App::setReplica( int r, bool b ) {
	if ( r < num_servers ) replicas[r] = b;
	else fprintf( stderr, "[ERROR] Invalid replica index %d\n", r );
}

void App::setMaxRequests( int r ) {
	max_requests = r;
}
void App::setNumRequests( int r ) {
	num_requests = r;
}