#include <stdio.h>
#include <algorithm>
#include "Application.h"

App::App( int i, int c, int s, int b, int num_s, int num_r, bool reps[], int max_r, int time_window ) {
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
	num_requests.resize(num_servers);
	for(int i = 0;i < num_servers;++i){
		for(int j = 0;j < time_window;++j){
			num_requests[i].push_back(max_r);
		}
	}
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
int App::getNumRequests(int server, int time_slot) {
	if(server > num_servers){
		fprintf( stderr, "[ERROR] Invalid server index %d on getNumRequests\n", server );
		return -1;
	}
	if(time_slot >= num_requests[server].size()){
		fprintf( stderr, "[ERROR] Invalid time_slot index %d on getNumRequests\n", time_slot );
		return -1;
	}
	return num_requests[server][time_slot];
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
void App::setNumRequests( int server, int time_slot, int r ) {
	if(server > num_servers || server < 0){
		fprintf( stderr, "[ERROR] Invalid server index %d on setNumRequests\n", server );
		return;
	}
	if(time_slot >= num_requests[server].size() || time_slot < 0){
		fprintf( stderr, "[ERROR] Invalid time_slot index %d on setNumRequests\n", time_slot );
		return;
	}
	num_requests[server][time_slot] = r;
}